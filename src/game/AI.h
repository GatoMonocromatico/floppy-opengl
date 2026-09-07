#pragma once
#include "game/gameObject.h"
#include "util/Timer.h"
#include <vector>

struct GameState; // forward-declared: GameState.h includes this header

// ──────────────────────── Evaluation weights ──────────────────────────────
// Positive = reward, negative = penalty.
// Default values are Dellacherie-inspired and work well out of the box.
struct AIWeights {
    float aggregateHeight = -0.510066f; // sum of all column heights
    float linesCleared    =  0.760666f; // lines cleared by this placement
    float holes           = -0.356630f; // empty cells covered by a filled cell above
    float bumpiness       = -0.184483f; // sum |h[c] – h[c+1]| across columns
    float maxHeight       = -0.500000f; // height of the tallest column
};

// ──────────────────────── Piece shape (one rotation) ──────────────────────
// Cell offsets are relative to the bounding-box top-left corner.
// cells[i] = { col_offset, row_offset }
struct AIPieceShape {
    int8_t cells[4][2];
    int8_t width;
    int8_t height;
};

// ──────────────────────── Bitboard grid simulation ────────────────────────
// Row 0 = top (spawn area), row 25 = floor.  Pieces fall toward higher rows.
// Only locked cells are represented; the active piece is excluded.
struct AISim {
    uint32_t rows[26]  = {}; // bit c = column c occupied at this row
    int8_t   colTop[10]= {}; // topmost occupied row per column (26 if empty)

    // Build from the real grid, excluding the currently-falling piece.
    static AISim fromGrid(const GridData& gd);

    bool isOccupied(int col, int row) const { return (rows[row] >> col) & 1; }

    // Row index at which the piece would land; –1 means the stack is too high.
    int calcLandingRow(const AIPieceShape& shape, int colOffset) const;

    // Drop the piece, clear full rows in-place, return lines cleared.
    // Returns –1 when the placement is invalid.
    int place(const AIPieceShape& shape, int colOffset);

    // Heuristic score; higher = better.
    float evaluate(const AIWeights& w, int linesCleared) const;
};

// ──────────────────────── Beam-search node ────────────────────────────────
struct AINode {
    AISim  sim;
    float  score    =  0.f;
    int8_t firstRot = -1; // rotation used at depth 0 (the actual move)
    int8_t firstCol = -1; // column  used at depth 0
};

// ──────────────────────── Incremental search state ────────────────────────
enum class AIPhase { Idle, Searching, WaitToPlay, Executing };

struct AISearch {
    AIPhase  phase  = AIPhase::Idle;
    uint32_t lastId = 0; // brickId of the piece currently being analysed

    Piece pieces[7] = {};
    int   numPieces = 1; // pieces[0] = current, pieces[1..] = look-ahead

    std::vector<AINode> beam;
    std::vector<AINode> nextBeam;

    // Resumable iteration counters
    int depth   = 0;
    int nodeIdx = 0;
    int rotIdx  = 0;
    int colIdx  = 0;

    // Quality knobs
    int maxDepth  = 2; // look-ahead pieces beyond the current one
    int beamWidth = 5; // candidates kept at each depth level

    int8_t bestRot = 0;
    int8_t bestCol = 0;
};

// ──────────────────────── Per-AI-player data ──────────────────────────────
struct AIPlayer {
    AISearch  search;
    AIWeights weights;

    // Max placements simulated per frame.  Higher = faster decision,
    // more CPU per frame.  Tune against target frame budget.
    int workBudgetPerFrame = 40;

    // Pause between finishing analysis and executing the move.
    // Larger = more human-like; 0 = instant.
    Timer playIntervalTimer;

    // Legacy-compatible constructor (GridData& is intentionally ignored;
    // the AI no longer keeps a heavyweight grid copy).
    AIPlayer(GridData&, Timer t, int8_t depth, int8_t beam)
        : playIntervalTimer(t)
    {
        search.maxDepth  = depth;
        search.beamWidth = beam;
    }

    AIPlayer() : playIntervalTimer(0.3f) {}
};

// ──────────────────────── Public API ──────────────────────────────────────

// Build the piece-shape lookup table once, before entering the game loop.
void initAIPieceShapes();

// Call every frame for each AI-controlled grid (gridIndex >= 1).
void AIUpdate(GameState& gs, Resources& res, size_t gridIndex, float deltaTime);
