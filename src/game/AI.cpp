#include "game/AI.h"
#include "game/GameState.h"
#include "game/gameCore.h"
#include "game/brickUtils.h"
#include <algorithm>
#include <cmath>
#include <cstring>

// MSVC doesn't have __builtin_ctz; use _BitScanForward instead.
#ifdef _MSC_VER
#include <intrin.h>
static inline int ai_ctz(unsigned x) {
    unsigned long idx;
    _BitScanForward(&idx, x);
    return static_cast<int>(idx);
}
#else
static inline int ai_ctz(unsigned x) { return __builtin_ctz(x); }
#endif

// ──────────────────────── Static lookup tables ────────────────────────────

static AIPieceShape g_shapes[7][4];

// Number of distinct rotation footprints per piece.
// Pieces with rotational symmetry (I, S, Z, O) have fewer unique rotations.
// Index order matches Piece enum: T=0, I=1, S=2, Z=3, J=4, L=5, O=6.
// Entry [7] guards against nullPiece; it is never legitimately reached.
static const int8_t g_uniqueRots[8] = { 4, 2, 2, 2, 4, 4, 1, 0 };

void initAIPieceShapes() {
    for (int p = 0; p < 7; p++) {
        Piece piece = static_cast<Piece>(p);

        // Start from the game's canonical spawn positions and apply
        // the game's own rotation adjustments to derive every rotation state.
        std::array<glm::ivec2, 4> pos = createStartingPos(piece);
        auto adj = createRotationalAdjustments(piece);

        for (int r = 0; r < 4; r++) {
            int minX = 99, minY = 99, maxX = -99, maxY = -99;
            for (auto& v : pos) {
                if (v.x < minX) minX = v.x;
                if (v.y < minY) minY = v.y;
                if (v.x > maxX) maxX = v.x;
                if (v.y > maxY) maxY = v.y;
            }

            g_shapes[p][r].width  = static_cast<int8_t>(maxX - minX + 1);
            g_shapes[p][r].height = static_cast<int8_t>(maxY - minY + 1);

            for (int i = 0; i < 4; i++) {
                g_shapes[p][r].cells[i][0] = static_cast<int8_t>(pos[i].x - minX);
                g_shapes[p][r].cells[i][1] = static_cast<int8_t>(pos[i].y - minY);
            }

            // Advance positions to the next rotation state
            if (r < 3) {
                for (int i = 0; i < 4; i++) {
                    pos[i] += adj[i][r];
                }
            }
        }
    }
}

// ──────────────────────── AISim: grid construction ────────────────────────

AISim AISim::fromGrid(const GridData& gd) {
    AISim sim;
    std::fill(std::begin(sim.colTop), std::end(sim.colTop), int8_t(26));

    // Exclude the active (falling) piece; it is always at the back.
    int16_t playingIdx = static_cast<int16_t>(gd.currentBricks.size()) - 1;

    for (int row = 0; row < gd.gridRows; row++) {
        for (int col = 0; col < gd.gridColumns; col++) {
            const GridCell& cell = gd.gridUnitsData[row][col];
            if (cell && cell.brickIndex != playingIdx) {
                sim.rows[row] |= (1u << col);
                if (row < sim.colTop[col])
                    sim.colTop[col] = static_cast<int8_t>(row);
            }
        }
    }
    return sim;
}

// ──────────────────────── AISim: landing row ──────────────────────────────

int AISim::calcLandingRow(const AIPieceShape& shape, int colOffset) const {
    // For each column that the piece touches, find the maximum row offset
    // (the bottommost cell in that column within the piece's bounding box).
    int8_t maxDr[10];
    std::fill(std::begin(maxDr), std::end(maxDr), int8_t(-1));
    int8_t maxDrOverall = -1;

    for (int i = 0; i < 4; i++) {
        int    col = colOffset + shape.cells[i][0];
        int8_t dr  = shape.cells[i][1];
        if (dr > maxDr[col])     maxDr[col]     = dr;
        if (dr > maxDrOverall)   maxDrOverall    = dr;
    }

    // Floor constraint: the piece's bottom edge must not exceed row 25.
    int landing = 25 - maxDrOverall;

    // Stack constraint: for every column the piece occupies, the piece must
    // land above the topmost occupied cell in that column.
    for (int col = 0; col < 10; col++) {
        if (maxDr[col] < 0) continue;
        int constraint = static_cast<int>(colTop[col]) - maxDr[col] - 1;
        if (constraint < landing) landing = constraint;
    }

    // Negative landing means the stack is too high for this placement.
    return landing;
}

// ──────────────────────── AISim: piece placement ──────────────────────────

int AISim::place(const AIPieceShape& shape, int colOffset) {
    int landing = calcLandingRow(shape, colOffset);
    if (landing < 0) return -1;

    // Place all four cells and update the per-column top pointer.
    for (int i = 0; i < 4; i++) {
        int col = colOffset + shape.cells[i][0];
        int row = landing    + shape.cells[i][1];
        rows[row] |= (1u << col);
        if (row < colTop[col])
            colTop[col] = static_cast<int8_t>(row);
    }

    // Clear full rows by compacting: iterate from the bottom upward,
    // skipping full rows and writing non-full rows downward.
    constexpr uint32_t FULL_ROW = (1u << 10) - 1u;
    int linesCleared = 0;
    int writeRow = 25;

    for (int readRow = 25; readRow >= 0; readRow--) {
        if (rows[readRow] == FULL_ROW) {
            linesCleared++;
        } else {
            rows[writeRow--] = rows[readRow];
        }
    }
    while (writeRow >= 0) rows[writeRow--] = 0u;

    // Rebuild per-column top pointers only when the row layout changed.
    if (linesCleared > 0) {
        std::fill(std::begin(colTop), std::end(colTop), int8_t(26));
        for (int row = 0; row < 26; row++) {
            uint32_t rowBits = rows[row];
            while (rowBits) {
                int col = ai_ctz(rowBits); // position of lowest set bit
                rowBits &= rowBits - 1;
                if (colTop[col] == 26)
                    colTop[col] = static_cast<int8_t>(row);
            }
        }
    }

    return linesCleared;
}

// ──────────────────────── AISim: position evaluation ─────────────────────

float AISim::evaluate(const AIWeights& w, int linesCleared) const {
    int heights[10] = {};
    int aggregate   = 0;
    int maxH        = 0;

    for (int col = 0; col < 10; col++) {
        heights[col] = (colTop[col] < 26) ? (26 - colTop[col]) : 0;
        aggregate   += heights[col];
        if (heights[col] > maxH) maxH = heights[col];
    }

    // Count holes: empty cells that have at least one filled cell above them
    // in the same column.
    int holes = 0;
    for (int col = 0; col < 10; col++) {
        if (colTop[col] >= 26) continue;
        for (int row = colTop[col] + 1; row < 26; row++) {
            if (!((rows[row] >> col) & 1)) holes++;
        }
    }

    // Bumpiness: sum of absolute height differences between adjacent columns.
    int bumpiness = 0;
    for (int col = 0; col < 9; col++) {
        bumpiness += std::abs(heights[col] - heights[col + 1]);
    }

    return w.aggregateHeight * static_cast<float>(aggregate)
         + w.linesCleared    * static_cast<float>(linesCleared)
         + w.holes           * static_cast<float>(holes)
         + w.bumpiness       * static_cast<float>(bumpiness)
         + w.maxHeight       * static_cast<float>(maxH);
}

// ──────────────────────── Beam search helpers ─────────────────────────────

static void beginSearch(AISearch& s, const GridData& gd) {
    s.phase   = AIPhase::Searching;
    s.depth   = 0;
    s.nodeIdx = 0;
    s.rotIdx  = 0;
    s.colIdx  = 0;
    s.nextBeam.clear();

    // Current piece and look-ahead queue
    s.pieces[0]  = gd.currentBricks.back().shape;
    int queueSz  = static_cast<int>(gd.nextBricks.size());
    s.numPieces  = std::min(s.maxDepth + 1, queueSz + 1);

    for (int i = 1; i < s.numPieces; i++) {
        int idx      = (gd.rotationIndexNextBricks + (i - 1)) % queueSz;
        s.pieces[i]  = gd.nextBricks[idx];
    }

    // Seed the beam with the current (empty of new placements) grid state.
    s.beam.clear();
    AINode seed;
    seed.sim      = AISim::fromGrid(gd);
    seed.score    = 0.f;
    seed.firstRot = -1;
    seed.firstCol = -1;
    s.beam.push_back(std::move(seed));
}

// Prune nextBeam to beamWidth, replace beam, advance depth.
// Returns false when the search is complete.
static bool advanceBeam(AISearch& s) {
    s.depth++;
    s.nodeIdx = 0;
    s.rotIdx  = 0;
    s.colIdx  = 0;

    if (!s.nextBeam.empty()) {
        int keep = std::min(static_cast<int>(s.nextBeam.size()), s.beamWidth);
        std::partial_sort(
            s.nextBeam.begin(), s.nextBeam.begin() + keep, s.nextBeam.end(),
            [](const AINode& a, const AINode& b) { return a.score > b.score; });
        s.nextBeam.resize(keep);
        s.beam     = std::move(s.nextBeam);
        s.nextBeam.clear();
    }

    // Finish if we've covered all look-ahead pieces, or if no moves exist.
    if (s.depth == s.numPieces || s.beam.empty()) {
        int best = -1;
        for (int i = 0; i < static_cast<int>(s.beam.size()); i++) {
            if (s.beam[i].firstRot >= 0) {
                if (best < 0 || s.beam[i].score > s.beam[best].score)
                    best = i;
            }
        }
        s.bestRot = (best >= 0) ? s.beam[best].firstRot : -1;
        s.bestCol = (best >= 0) ? s.beam[best].firstCol : -1;
        s.phase   = AIPhase::WaitToPlay;
        return false;
    }

    return true;
}

// Evaluate one candidate placement (one "work unit").
// Returns true while the search is ongoing, false when it finishes.
static bool searchStep(AISearch& s, const AIWeights& w) {
    if (s.phase != AIPhase::Searching) return false;

    int pieceIdx = static_cast<int>(s.pieces[s.depth]);
    if (pieceIdx < 0 || pieceIdx > 6) return advanceBeam(s); // nullPiece guard
    int numRots  = g_uniqueRots[pieceIdx];

    while (s.nodeIdx < static_cast<int>(s.beam.size())) {
        const AINode& parent = s.beam[s.nodeIdx];

        while (s.rotIdx < numRots) {
            const AIPieceShape& shape = g_shapes[pieceIdx][s.rotIdx];
            int maxCol = 10 - shape.width;

            while (s.colIdx <= maxCol) {
                int col = s.colIdx++;

                // Skip placements where the stack is already too high.
                if (parent.sim.calcLandingRow(shape, col) < 0) continue;

                AISim childSim = parent.sim;
                int lines      = childSim.place(shape, col);

                AINode child;
                child.sim   = std::move(childSim);
                child.score = child.sim.evaluate(w, lines);
                child.firstRot = (s.depth == 0)
                    ? static_cast<int8_t>(s.rotIdx) : parent.firstRot;
                child.firstCol = (s.depth == 0)
                    ? static_cast<int8_t>(col)      : parent.firstCol;

                s.nextBeam.push_back(std::move(child));
                return true; // one unit of work done; resume next call
            }

            s.colIdx = 0;
            s.rotIdx++;
        }

        s.rotIdx = 0;
        s.nodeIdx++;
    }

    return advanceBeam(s);
}

// ──────────────────────── Move execution ──────────────────────────────────

static void executeMove(GameState& gs, Resources& res, GridData& gd,
                        size_t gridIndex, int8_t targetRot, int8_t targetCol)
{
    if (gd.currentBricks.empty()) return;
    BrickData& playing = gd.currentBricks.back();
    if (playing.state == BrickState::solid) return;

    // 1. Rotate to the target rotation state.
    int numRot = (static_cast<int>(targetRot) - playing.rotationState + 4) % 4;
    for (int i = 0; i < numRot; i++) {
        updateBrickPositionRotation(playing, gd, 1, true);
    }

    // 2. Find the current leftmost column of the piece after rotation.
    int minX = gd.gridColumns; // start above the max valid index
    for (const auto& u : playing.units) {
        int x = gd.currentUnits[u.specificDataLocation].position.x;
        if (x < minX) minX = x;
    }

    // 3. Slide the piece to the target column.
    int dx  = static_cast<int>(targetCol) - minX;
    int dir = (dx > 0) ? 1 : -1;
    for (int i = 0; i < std::abs(dx); i++) {
        // Stop early if a wall or stack blocks further movement.
        if (!updateBrickPositionTranslational(playing, gd, glm::ivec2(dir, 0), true))
            break;
    }

    // 4. Hard-drop to the landing row.
    int fall = getbiggestYFallForBrick(playing, gd);
    if (fall > 0) {
        updateBrickPositionTranslational(playing, gd, glm::ivec2(0, fall), true);
    }

    // 5. Lock the piece, clear full lines, and spawn the next piece.
    // Note: createPlayingBrick (called internally) resets gs.fallLockInTimer
    // and gs.coutingLockIn, which are currently shared across all grids.
    // This is a pre-existing design limitation; it has no effect on the
    // player grid in practice because both fields are only acted upon for
    // grid index 0 inside the player lock-in logic.
    playingPieceDropped(gs, res, gd, gridIndex);
}

// ──────────────────────── Per-frame AI entry point ────────────────────────

void AIUpdate(GameState& gs, Resources& res, size_t gridIndex, float deltaTime) {
    AIPlayer& ai = gs.IAs[gridIndex - 1];
    AISearch& s  = ai.search;
    GridData& gd = gs.grids[gridIndex];

    if (gd.currentBricks.empty()) return;

    // Detect a newly spawned piece and restart the search.
    uint32_t currentId = gd.currentBricks.back().brickId;
    if (currentId != s.lastId) {
        s.lastId = currentId;
        beginSearch(s, gd);
    }

    // ── Phase: spread search work across frames ────────────────────────────
    if (s.phase == AIPhase::Searching) {
        for (int i = 0; i < ai.workBudgetPerFrame; i++) {
            if (!searchStep(s, ai.weights)) break;
        }
    }

    // ── Phase: wait before committing to the move ─────────────────────────
    if (s.phase == AIPhase::WaitToPlay) {
        ai.playIntervalTimer.step(deltaTime);
        if (ai.playIntervalTimer.isTimedOut()) {
            ai.playIntervalTimer.reset();
            s.phase = AIPhase::Executing;
        }
    }

    // ── Phase: apply the chosen move ──────────────────────────────────────
    if (s.phase == AIPhase::Executing) {
        if (s.bestRot >= 0 && s.bestCol >= 0) {
            executeMove(gs, res, gd, gridIndex, s.bestRot, s.bestCol);
        } else {
            // Fallback when no valid placement was found (near game-over).
            BrickData& playing = gd.currentBricks.back();
            int fall = getbiggestYFallForBrick(playing, gd);
            if (fall > 0)
                updateBrickPositionTranslational(playing, gd, glm::ivec2(0, fall), true);
            playingPieceDropped(gs, res, gd, gridIndex);
        }
        // New piece is now at the back of currentBricks; the next AIUpdate
        // call will detect the brickId change and restart the search.
        s.phase = AIPhase::Idle;
    }
}
