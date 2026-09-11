#version 330 core
// -----------------------------------------------------------------------------
// portal.frag -- black-hole "magical matter" framing a window into another arcade
// -----------------------------------------------------------------------------
// Read docs/Portal.md for the visual design. In short, three layers composite here:
//
//   1. MATTER   a raymarched, gravitationally lensed accretion body. Supplies the
//               silhouette (the video's diagonal "eye") and its moving turbulent
//               texture. Its body mostly REFRACTS the scene behind the portal, so
//               it blends into the room instead of announcing itself.
//   2. WINDOW   the opponent's board, painted onto the matter as a rectangle. It
//               covers most of the matter, leaving two diagonal lobes exposed.
//   3. ARCS     where the window's border falls OUTSIDE the matter silhouette --
//               the two opposing corners the ellipse cannot reach -- a thin
//               electric filament stands in for the missing edge.
//
// Every tuning value is a `const` in this file rather than a uniform, so the look
// can be retuned by editing this file and rerunning: shaders are read from disk at
// startup and need no C++ rebuild.

in vec3 vLocal;
in vec2 vUV;

out vec4 FragColor;

uniform vec3  camLocal;        // camera position in portal-local space
uniform vec2  windowHalf;      // half-extents of the painted board rectangle
uniform float matterRadius;    // outer reach of the matter, world units
uniform float swirlPhase;      // three mutually irrational phase accumulators;
uniform float driftPhase;      // combined they never repeat, and each is wrapped
uniform float flickerPhase;    // CPU-side so float32 never loses the fine detail
uniform float materialisation; // 0 = absent, 1 = fully present
uniform vec2  viewportSize;

// Maps portal-local space to the content target. The target holds the grid PLUS a
// margin of the opponent's screen background, and this says what fraction of it the
// grid occupies -- so the window rectangle lands exactly on the grid and sampling
// beyond the rectangle walks out into the background instead of running off the edge.
uniform vec2  contentScale;

uniform sampler2D contentTex;    // the opponent's screen (render-to-texture)
uniform sampler2D backgroundTex; // the already-drawn scene, for refraction

// ---------------------------------------------------------------- tuning knobs

// Proportions, not sizes: absolute scale comes from the matterRadius uniform,
// which is the slab's SEMI-MAJOR axis, so the whole body grows coherently from one
// number on the C++ side.
//
// The slab lies very nearly IN the window's plane, and that is the whole point.
// It used to sit ~67 degrees from it -- near perpendicular -- which reproduced the
// reference video's edge-on lens head-on, but the video has a fixed camera and no
// window. Glued to one, that geometry meant walking to the side foreshortened the
// window to a line while simultaneously turning you to face down the disk's axis,
// so the matter opened into a full circle exactly as the window vanished. Both
// silhouettes were individually correct and the pairing was nonsense: a picture
// frame with a wheel through it edge-first, which reads as two unrelated effects
// rather than one object. Lying the slab down makes the two foreshorten together.
const float diskTiltDeg  = 11.0;  // how far the slab leans out of the window plane
// The slab is an ELLIPSE, not a circle. A circular disk lying in the window plane
// would project as a circle head-on; the diagonal silhouette that leaves two
// opposing corners exposed has to come from the body's own shape now that it no
// longer comes from foreshortening.
const float diskMinor    = 0.53;  // semi-minor / semi-major
// Real thickness, not a sheet. It is what the slab presents when seen edge-on, and
// it is what a grazing ray travels lengthwise through -- which is what finally
// hides the window at oblique angles, with no view-dependent term anywhere.
const float diskThick    = 0.20;  // half-thickness / semi-major
const float diskInner    = 0.34;  // inner hole, in normalised elliptical radius
const float horizonR     = 0.17;  // event horizon / semi-major
// Thin material filling the hole. The window sits at the centre of that hole, and
// a hole is unobstructed from every direction, so without this no amount of slab
// geometry could ever occlude the window.
const float innerHaze    = 0.30;
const float gravityK     = 0.55;  // ray deflection, in units of the horizon radius
const int   STEPS        = 40;    // march steps; cost scales linearly with this

const float noiseScale   = 3.4;   // spatial frequency of the turbulence
const float keplerRate   = 0.055; // differential rotation -- the never-looping part

const vec3  glowColor   = vec3(0.16, 0.94, 0.88);  // teal/cyan magic
const float glowGain    = 1.55;
const float rimGain     = 1.00;
const float concThresh  = 0.62;   // above this, turbulence reads as "concentrated"
const float refractGain = 0.085;  // how hard the matter bends the scene behind it

const float boardWarp   = 0.020;  // lens warp applied to the painted board
const float boardBleed  = 0.38;   // how much glow leaks over the board
const float arcWidth    = 0.0032;
const float arcJitter   = 0.0040;
// How far past the matter's edge the arcs take to reach full brightness. The glow
// must build with distance from the matter, not switch on the instant the corner
// clears it.
// Measured against how far the corners actually reach: the window's corner sits
// about 0.10 world units outside the silhouette, so a longer ramp than that means
// the arc never brightens over its whole visible length.
const float arcRampDist = 0.070;
const float arcFlowRate = 2.4;    // speed of the pulses travelling out along the arc

// Absorption of the matter, in Beer-Lambert terms. Using exp() rather than a clamp
// means a long path through the matter goes smoothly opaque instead of flat-topping
// into a grey wash, which is what grazing views used to do.
const float absorbK     = 9.0;
const vec3  matterTint  = vec3(0.030, 0.062, 0.068);
// How much of the scene behind the portal the matter reproduces. High on purpose:
// "most of the matter should be trying to mimic what is behind it".
const float matterMimic = 0.80;
// Matter between the viewer and the window barely glows: at oblique angles it is
// what hides the board, and a glowing occluder would shout.
const float frontGlow   = 0.16;

// ------------------------------------------------------------------ noise field

float hash13(vec3 p)
{
    p = fract(p * 0.1031);
    p += dot(p, p.zyx + 31.32);
    return fract((p.x + p.y) * p.z);
}

float vnoise(vec3 x)
{
    vec3 i = floor(x);
    vec3 f = fract(x);
    f = f * f * (3.0 - 2.0 * f);

    float n000 = hash13(i + vec3(0.0, 0.0, 0.0));
    float n100 = hash13(i + vec3(1.0, 0.0, 0.0));
    float n010 = hash13(i + vec3(0.0, 1.0, 0.0));
    float n110 = hash13(i + vec3(1.0, 1.0, 0.0));
    float n001 = hash13(i + vec3(0.0, 0.0, 1.0));
    float n101 = hash13(i + vec3(1.0, 0.0, 1.0));
    float n011 = hash13(i + vec3(0.0, 1.0, 1.0));
    float n111 = hash13(i + vec3(1.0, 1.0, 1.0));

    return mix(mix(mix(n000, n100, f.x), mix(n010, n110, f.x), f.y),
               mix(mix(n001, n101, f.x), mix(n011, n111, f.x), f.y), f.z);
}

float fbm(vec3 p)
{
    float sum = 0.0;
    float amp = 0.5;
    for (int i = 0; i < 4; i++)
    {
        sum += amp * vnoise(p);
        p = p * 2.03 + vec3(17.1, 9.7, 23.3);
        amp *= 0.5;
    }
    return sum;
}

// ------------------------------------------------------------------- 2D helpers

// Signed distance to an axis-aligned rectangle, negative inside.
float sdRect(vec2 p, vec2 halfSize)
{
    vec2 d = abs(p) - halfSize;
    return length(max(d, 0.0)) + min(max(d.x, d.y), 0.0);
}

mat2 rot2(float a)
{
    float c = cos(a), s = sin(a);
    return mat2(c, -s, s, c);
}

// --------------------------------------------------------------- matter marcher

struct Matter
{
    // The march composites front-to-back rather than returning bulk densities, and
    // the board is composited INSIDE it at the depth the ray actually reaches the
    // window. That ordering is the whole reason the effect survives oblique angles:
    // matter the ray meets first ends up over the board and hides it.
    //
    // The obvious cheaper test -- classify each sample by the sign of its local z --
    // is degenerate here and measured exactly zero back-side density. The disk is
    // centred ON the window plane and its projected major axis LIES IN that plane,
    // so the side a sample falls on flips arbitrarily precisely along the axis,
    // which is where the visible lobes are. Depth along the ray has no such
    // degeneracy.
    vec3  colour;   // accumulated, premultiplied by coverage
    float trans;    // remaining transmittance: 1 = nothing was hit
    float opacity;  // 1 - trans, kept for readability at the call site
    float body;     // silhouette coverage: did the ray pass through the disk AT ALL
    vec2  bend;     // screen-space refraction offset the body imposes
    vec2  axis;     // projected major axis of the disk, for the arc distance ramp
};

Matter marchMatter(vec3 ro, vec3 rd, float tilt, vec3 boardColour, float boardAlpha, float tWindow, vec3 sceneBehind)
{
    Matter m;
    m.colour = vec3(0.0);
    m.trans = 1.0;
    m.opacity = 0.0;
    m.body = 0.0;
    m.bend = vec2(0.0);
    m.axis = vec2(1.0, 0.0);

    // Basis for the slab. diskU is its major axis and lies IN the window plane,
    // along the rectangle's diagonal -- that alignment is what leaves two opposing
    // corners covered and two exposed. diskN starts as the window's own normal and
    // leans away from it by only a few degrees, so the slab lies down in the
    // window's plane rather than standing perpendicular to it.
    float tf = radians(diskTiltDeg);
    vec3 diskU = vec3(cos(-tilt), sin(-tilt), 0.0);
    vec3 zAxis = vec3(0.0, 0.0, 1.0);
    vec3 diskN = normalize(zAxis * cos(tf) + cross(diskU, zAxis) * sin(tf));
    vec3 diskV = normalize(cross(diskN, diskU));

    // diskU is in the window plane by construction, so it IS the projected major
    // axis. Deriving the arcs' ellipse from it rather than re-deriving the angle
    // by hand keeps the two from drifting apart when the slab is retuned.
    m.axis = diskU.xy;

    float aDisk = matterRadius;
    float bDisk = matterRadius * diskMinor;
    float hMax  = matterRadius * diskThick;
    float Rh    = matterRadius * horizonR;
    float grav  = gravityK * Rh;

    // Bound the march to the sphere that contains the matter; outside it there is
    // nothing to integrate and the ray is straight anyway.
    float bound = matterRadius * 1.05;
    float b = dot(ro, rd);
    float c = dot(ro, ro) - bound * bound;
    float disc = b * b - c;
    if (disc < 0.0)
    {
        // The ray misses the matter entirely, so the board is all there is.
        m.colour = boardColour * boardAlpha;
        m.trans = 1.0 - boardAlpha;
        m.opacity = boardAlpha;
        return m;
    }

    float sq = sqrt(disc);
    float t0 = max(-b - sq, 0.0);
    float t1 = -b + sq;
    if (t1 <= t0) return m;

    float dt = (t1 - t0) / float(STEPS);
    vec3 p = ro + rd * t0;
    vec3 v = rd;

    // Depth along the ray, so the board can be dropped in at the right moment.
    float t = t0;
    bool boardPlaced = false;

    for (int i = 0; i < STEPS; i++)
    {
        // The board sits where the ray reaches the window. Everything composited
        // before this point ends up in front of it.
        if (!boardPlaced && t >= tWindow)
        {
            boardPlaced = true;
            m.colour += m.trans * boardColour * boardAlpha;
            m.trans *= (1.0 - boardAlpha);
        }
        t += dt;

        float r2 = max(dot(p, p), 1e-8);
        float r  = sqrt(r2);

        // Newtonian deflection. Not a geodesic, but it produces the same visual
        // signature: the far side of the band bends up over the horizon and down
        // under it, which is the shape that reads as "black hole" in the video.
        v = normalize(v - grav * p / (r2 * r) * dt);
        p += v * dt;

        // Elliptical normalised radius: 1.0 is the slab's rim in every direction,
        // so the whole profile below is written once and the body's elongation
        // comes from the geometry rather than from a special case per axis.
        float h = dot(p, diskN);
        float q = length(vec2(dot(p, diskU) / aDisk, dot(p, diskV) / bDisk));

        // Matter in front of the board is quiet, so an occluded window stays
        // understated rather than shouting -- but only where there IS a board to
        // occlude. Damping it everywhere also dimmed the exposed lobes, where
        // nothing is being hidden and the matter should read at full strength.
        float emitScale = boardPlaced ? 1.0 : mix(1.0, frontGlow, boardAlpha);

        // Photon-ring proximity: brightest right where rays graze the horizon. It
        // is lensed light from BEHIND the hole, so it only counts past the board.
        if (boardPlaced)
        {
            float ring = exp(-pow((r - Rh * 1.45) / (Rh * 0.16), 2.0)) * dt;
            m.colour += m.trans * glowColor * ring * rimGain * 2.4;
        }

        // Silhouette coverage, kept separate from density on purpose. The matter
        // is an ANNULUS, so its density falls away towards the middle -- and the
        // window's side edges run right through that inner hole. Deriving "is the
        // window's border exposed here" from density therefore reported bare
        // border along the whole left and right sides, and the electric arcs
        // wrapped the entire rectangle instead of just the two corners. Coverage
        // ignores the inner cut and the turbulence: it asks only whether the ray
        // passed through the disk's outline.
        if (q < 1.0 && abs(h) < hMax)
        {
            m.body = max(m.body, (1.0 - smoothstep(0.82, 1.0, q))
                                * (1.0 - smoothstep(hMax * 0.5, hMax, abs(h))));
        }

        if (q < 1.0 && abs(h) < hMax)
        {
            // Differential (Keplerian-ish) rotation: inner matter laps outer
            // matter forever, so the field is quasi-periodic and never returns to
            // an earlier state. This is what kills the "loop becomes noticeable
            // after a while" problem, without needing a long baked animation.
            float ang = atan(dot(p, diskV) / bDisk, dot(p, diskU) / aDisk);
            ang += swirlPhase * keplerRate / pow(max(q, diskInner), 1.5);

            vec3 qv = vec3(cos(ang) * q, sin(ang) * q, h / max(hMax, 1e-4) * 0.35) * noiseScale;
            float n = fbm(qv + vec3(driftPhase * 0.07, 0.0, driftPhase * 0.04));

            // Radial and vertical falloff so the body has soft edges rather than a
            // cut-out annulus.
            float radial = smoothstep(diskInner, diskInner + (1.0 - diskInner) * 0.30, q)
                         * (1.0 - smoothstep(0.82, 1.0, q));
            // The hole is filled, faintly. See innerHaze.
            radial = max(radial, innerHaze * (1.0 - smoothstep(diskInner * 0.5, 0.60, q)));
            float vert = 1.0 - smoothstep(0.0, hMax, abs(h));

            float d = radial * vert * n;

            // "Magic concentrated on some spaces". Two scales, deliberately: the
            // ridges of the turbulence say WHICH filaments could emit, and a much
            // coarser, slower field says WHERE the magic happens to be gathered.
            // Driving the glow from the turbulence alone made every filament glow
            // equally, which reads as a uniform wash rather than as concentration.
            float patch = fbm(qv * 0.32 + vec3(0.0, driftPhase * 0.05, 11.0));
            float conc = smoothstep(concThresh, concThresh + 0.22, n)
                       * smoothstep(0.44, 0.70, patch);

            // Beer-Lambert per step. A grazing ray travels much further through the
            // same matter and goes smoothly opaque, rather than a clamp flat-topping
            // it into the uniform grey wash that oblique views used to show.
            float aStep = 1.0 - exp(-absorbK * d * dt);

            // The matter mostly mimics whatever is behind the portal -- that is what
            // makes it a chameleon rather than an object announcing itself. Its own
            // substance is only a faint tint on top, scaled by density so the
            // turbulence stays legible instead of flattening into one tinted blob.
            float dn = clamp(d * 12.0, 0.0, 1.0);
            vec3 shade = sceneBehind * matterMimic + matterTint * (0.5 + 3.0 * dn);

            m.colour += m.trans * shade * aStep;
            m.colour += m.trans * glowColor * conc * d * dt * 34.0 * glowGain * emitScale;
            m.trans *= (1.0 - aStep);

            m.bend += normalize(p.xy + 1e-6) * d * dt;
        }

        if (r < Rh) { m.body = 1.0; break; }
    }

    // The ray finished without ever reaching the window (it fell into the horizon,
    // or the window is behind the marched span). The board still belongs in the
    // result, behind everything the ray did meet.
    if (!boardPlaced)
    {
        m.colour += m.trans * boardColour * boardAlpha;
        m.trans *= (1.0 - boardAlpha);
    }

    m.opacity = 1.0 - m.trans;
    return m;
}

// ------------------------------------------------------------------------- main

void main()
{
    vec2 p = vLocal.xy;

    // Roll the matter onto the window rectangle's diagonal.
    float tilt = atan(windowHalf.y, windowHalf.x);

    vec3 ro = camLocal;
    vec3 rd = normalize(vLocal - camLocal);

    // ---- the painted view, built BEFORE the march ---------------------------
    // The march composites it at the depth the ray reaches the window, so it has
    // to exist first. The lens warp is driven by distance from the centre rather
    // than by matter density -- density is only known after the march, and making
    // the warp depend on it would be circular.
    float rNorm = length(p) / matterRadius;
    vec2 warp = normalize(p + 1e-6) * boardWarp * smoothstep(0.30, 1.05, rNorm);

    // The content target holds the opponent's grid with a margin of their screen
    // around it. contentScale lands the grid exactly on the window rectangle, so
    // sampling beyond the rectangle walks out into that background rather than off
    // the edge of the image.
    //
    // A rectangle inscribed in an ellipse leaves the ellipse spare room at the
    // rectangle's sides and falls short of two opposing corners. The background
    // fills that spare room so the ellipse is never showing nothing; the corners
    // still spill past the matter and get the arcs instead.
    vec2 contentUV = 0.5 + ((p - warp) / windowHalf) * contentScale * 0.5;
    vec3 paint = texture(contentTex, clamp(contentUV, 0.0, 1.0)).rgb;

    // Only enough feather to antialias. The old edge dissolved with matter density
    // up to 0.055 world units -- 26% of the window's half-width against 13% of its
    // half-height -- which ate the outer columns far harder than the outer rows and
    // was the main reason edge cells looked blunt. The board's own content is no
    // longer dissolved at all; matter in front of it is what occludes it now.
    float inRect = 1.0 - smoothstep(-0.0022, 0.0022, sdRect(p, windowHalf));

    // Fade the background out before the content target actually runs out, so the
    // clamped edge texels never smear into view.
    float contentEdge = max(abs(contentUV.x - 0.5), abs(contentUV.y - 0.5)) * 2.0;
    float bgFade = 1.0 - smoothstep(0.72, 0.97, contentEdge);
    float paintAlpha = max(inRect, bgFade);

    // The fragment lies on the window plane, so the distance to it IS the depth at
    // which the ray meets the board.
    float tWindow = length(vLocal - camLocal);

    // Sampled before the march because the matter is shaded with it. The refracted
    // version below reuses the same texture with the march's accumulated bend.
    vec2 screenUV = gl_FragCoord.xy / viewportSize;
    vec3 sceneBehind = texture(backgroundTex, screenUV).rgb;

    Matter m = marchMatter(ro, rd, tilt, paint, paintAlpha, tWindow, sceneBehind);

    // Near-binary silhouette. Answers "is there matter here", not "how much".
    float cover = smoothstep(0.04, 0.42, m.body);

    // ---- what is behind the portal, refracted -------------------------------
    vec2 refracted = screenUV + m.bend * refractGain;
    vec3 behind = texture(backgroundTex, refracted).rgb;

    // ---- electric arcs on the exposed corners --------------------------------
    // Distance outside the matter's projected silhouette, from an ellipse built on
    // the disk's own axis so the two cannot drift apart. This is what makes the
    // glow build with distance instead of switching to full brightness the instant
    // a corner clears the matter.
    // The slab's projected silhouette. Its major axis lies in the window plane so
    // it projects at full length; the minor extent picks up the slab's thickness
    // through the small lean out of the plane.
    float tfS = radians(diskTiltDeg);
    float aSil = matterRadius;
    float bSil = matterRadius * diskMinor * cos(tfS) + matterRadius * diskThick * sin(tfS);
    vec2 axisPerp = vec2(-m.axis.y, m.axis.x);
    vec2 e = vec2(dot(p, m.axis), dot(p, axisPerp)) / vec2(aSil, bSil);
    float distOutside = (length(e) - 1.0) * min(aSil, bSil);

    float arcRamp = smoothstep(0.0, arcRampDist, distOutside);

    // Pulses travelling outwards along the filament: energy flowing, not a lit
    // line. The travel coordinate is distance from the matter, so the pulses move
    // away from the portal.
    float pulse = fbm(vec3(distOutside * 30.0 - flickerPhase * arcFlowRate, p * 11.0));
    float flow = 0.32 + 1.35 * pulse;

    float borderDist = abs(sdRect(p, windowHalf));
    float jitter = (fbm(vec3(p * 90.0, flickerPhase * 2.1)) - 0.5) * arcJitter;
    float arcDist = abs(borderDist + jitter);
    float width = arcWidth * (0.70 + 0.65 * pulse);
    float arcCore = 1.0 - smoothstep(0.0, width, arcDist);
    float arcHalo = 1.0 - smoothstep(0.0, width * 5.5, arcDist);

    float arcGate = (1.0 - cover) * arcRamp * flow;
    arcCore *= arcGate;
    arcHalo *= arcGate;
    float arc = arcCore + arcHalo * 0.30;

    // ---- composite -----------------------------------------------------------
    // The march already ordered matter and board correctly and accumulated
    // PREMULTIPLIED, so putting the refracted room behind the residual
    // transmittance finishes an opaque colour. The portal is drawn with
    // premultiplied blending (GL_ONE, GL_ONE_MINUS_SRC_ALPHA); with ordinary
    // GL_SRC_ALPHA blending this same value got multiplied by alpha a second time
    // and everything the march accumulated came out roughly half as bright.
    vec3 col = m.colour + m.trans * behind;

    vec3 arcColor = mix(glowColor, vec3(1.0), 0.45);
    // Kept below saturation on purpose. At 1.15 the core clipped to pure white
    // along most of its length and the travelling pulses became invisible -- the
    // arc animated, but nothing about it could be seen to move.
    col += arcColor * arcCore * 0.72 + glowColor * arcHalo * 0.38;

    // Coverage, not opacity. Inside the silhouette the fragment already contains
    // the refracted room, so claiming full coverage there is what lets the lens
    // distortion replace the framebuffer and actually be visible.
    float coverage = max(max(m.opacity, paintAlpha), max(cover, arc));

    // Materialisation: the portal does not fade in uniformly, it condenses out of
    // nothing in patches. The threshold is compared against a noise field, so the
    // reveal front is ragged the way the hand-drawn spawn frames are, but it costs
    // no atlas and never has to line up with the window's aspect.
    float revealNoise = fbm(vec3(p * 7.5, 3.7));
    float reveal = smoothstep(revealNoise - 0.42, revealNoise + 0.02, materialisation);
    // The leading edge of the reveal burns: magic doing work.
    float revealEdge = reveal * (1.0 - reveal) * 4.0;
    col += glowColor * revealEdge * 0.9;

    float alpha = clamp(coverage * reveal, 0.0, 1.0);

    if (alpha < 0.004) discard;

    // No flat ambient add here. Lifting every fragment by a constant raised the
    // opponent's playfield blacks to grey and the board read as a grey card; the
    // matter carries its own tint instead, so the painted view keeps its blacks.
    FragColor = vec4(col * alpha, alpha);
}
