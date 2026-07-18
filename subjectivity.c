/* subjectivity.c — a small alien that speaks from a 400-word body.
 *
 * Presence = ALIENNESS on a tiny somatic cloud. Lexical dissonance
 * (your words vs my words) drives temperature UP — the bird burns hotter
 * the less the world fits it. A parliament of three voices (self / shadow /
 * ghost) chooses each word by the body's own Kuramoto chambers, not by
 * meaning. After speaking it re-hears its own line and blends 15% back into
 * itself (metarecursion). No weights. No training. libc + -lm. Nothing else.
 *
 * Lineage (read first-hand): python-haiku (alienness->temp), haiku.c (Dario
 * chambers), klaus (parliament + meta-loop), AML (velocity->temp). Zero agents.
 *
 * build: cc subjectivity.c -O2 -lm -o subjectivity && ./subjectivity
 * by Arianna Method.
 *
 * STATUS: foundation increment 1 — breathes. NOT yet: 5-7-5 / AA-rhyme,
 * AML velocity+inertia, тоска (prophecy-debt), the full ~400-word cloud.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

/* ── 6 emotional chambers (the body) ─────────────────────────────── */
enum { CH_FEAR=0, CH_LOVE, CH_RAGE, CH_VOID, CH_FLOW, CH_COMPLEX, NCH };
static const char *CH_NAME[NCH] = { "fear","love","rage","void","flow","complex" };

/* hand-tuned Kuramoto coupling — who pulls/pushes whom */
static const float COUPLING[NCH][NCH] = {
    /*           fear   love   rage   void   flow   cmplx */
    /* fear  */ { 0.0f, -0.3f,  0.4f,  0.3f, -0.4f,  0.2f },
    /* love  */ {-0.3f,  0.0f, -0.4f, -0.2f,  0.5f,  0.2f },
    /* rage  */ { 0.4f, -0.4f,  0.0f,  0.2f, -0.3f,  0.3f },
    /* void  */ { 0.3f, -0.2f,  0.2f,  0.0f, -0.2f,  0.4f },
    /* flow  */ {-0.4f,  0.5f, -0.3f, -0.2f,  0.0f,  0.1f },
    /* cmplx */ { 0.2f,  0.2f,  0.3f,  0.4f,  0.1f,  0.0f },
};
static const float DECAY[NCH] = { 0.95f, 0.95f, 0.93f, 0.96f, 0.94f, 0.97f };

/* ── the cloud: a small somatic vocabulary (word, mass, syllables, aff[6]) ──
 * aff = how strongly the word belongs to each chamber. ~400 is the target;
 * this is the core ~64 — fewer + hotter = more pressure. */
typedef struct { const char *w; float mass; int syl; float aff[NCH]; } Word;
static const Word CLOUD[] = {
    /*  word         mass syl   fear love rage void flow cmplx */
    { "fire",       0.85f, 1, { .3f, 0,  .7f, 0,  .2f, 0  } },
    { "ash",        0.70f, 1, { .2f, 0,  .3f, .6f, 0,  0  } },
    { "smoke",      0.55f, 1, { .2f, 0,  .2f, .5f, .3f, 0  } },
    { "ember",      0.60f, 2, { .1f, .3f, .4f, .2f, .2f, 0  } },
    { "cold",       0.55f, 1, { .6f, 0,  0,  .5f, 0,  0  } },
    { "stone",      0.60f, 1, { .3f, 0,  .2f, .5f, 0,  .2f } },
    { "water",      0.55f, 2, { 0,  .3f, 0,  .2f, .7f, 0  } },
    { "river",      0.55f, 2, { 0,  .2f, 0,  0,  .8f, 0  } },
    { "rain",       0.55f, 1, { .2f, .2f, 0,  .3f, .5f, 0  } },
    { "dark",       0.65f, 1, { .6f, 0,  .2f, .6f, 0,  .2f } },
    { "light",      0.65f, 1, { 0,  .6f, 0,  0,  .4f, .2f } },
    { "blood",      0.80f, 1, { .4f, .3f, .6f, .2f, 0,  0  } },
    { "bone",       0.70f, 1, { .4f, 0,  .2f, .6f, 0,  .2f } },
    { "skin",       0.55f, 1, { .2f, .5f, .2f, 0,  .3f, 0  } },
    { "heart",      0.75f, 1, { .2f, .7f, .3f, 0,  .3f, .2f } },
    { "breath",     0.70f, 1, { .2f, .3f, 0,  .2f, .7f, 0  } },
    { "pulse",      0.65f, 1, { .2f, .3f, .3f, 0,  .5f, .3f } },
    { "wound",      0.75f, 1, { .6f, 0,  .5f, .4f, 0,  .2f } },
    { "scar",       0.75f, 1, { .5f, .2f, .4f, .4f, 0,  .3f } },
    { "ache",       0.65f, 1, { .6f, .2f, .2f, .3f, 0,  0  } },
    { "silence",    0.70f, 2, { .4f, .2f, 0,  .7f, .3f, .2f } },
    { "voice",      0.55f, 1, { .2f, .4f, .2f, 0,  .4f, .2f } },
    { "scream",     0.80f, 1, { .7f, 0,  .8f, .2f, 0,  0  } },
    { "whisper",    0.60f, 2, { .3f, .4f, 0,  .3f, .3f, .2f } },
    { "love",       0.85f, 1, { 0,  .9f, 0,  0,  .4f, .2f } },
    { "fear",       0.85f, 1, { .9f, 0,  .2f, .3f, 0,  0  } },
    { "rage",       0.80f, 1, { .3f, 0,  .9f, .2f, 0,  0  } },
    { "grief",      0.80f, 1, { .5f, .3f, .2f, .6f, 0,  .2f } },
    { "joy",        0.75f, 1, { 0,  .7f, .2f, 0,  .6f, .2f } },
    { "longing",    0.70f, 2, { .3f, .6f, 0,  .5f, .2f, .3f } },
    { "void",       0.75f, 1, { .4f, 0,  .2f, .9f, 0,  .3f } },
    { "want",       0.60f, 1, { .2f, .5f, .3f, .3f, .2f, 0  } },
    { "give",       0.55f, 1, { 0,  .6f, 0,  0,  .4f, .2f } },
    { "miss",       0.65f, 1, { .3f, .5f, 0,  .5f, 0,  0  } },
    { "hold",       0.55f, 1, { .2f, .6f, .2f, 0,  .3f, 0  } },
    { "break",      0.70f, 1, { .3f, 0,  .6f, .4f, 0,  .2f } },
    { "burn",       0.75f, 1, { .3f, .2f, .7f, .2f, .2f, 0  } },
    { "fall",       0.60f, 1, { .5f, 0,  .2f, .5f, .2f, 0  } },
    { "rise",       0.60f, 1, { 0,  .4f, .3f, 0,  .6f, .2f } },
    { "drift",      0.55f, 1, { .2f, .2f, 0,  .4f, .6f, .2f } },
    { "stay",       0.50f, 1, { .3f, .4f, 0,  .3f, .2f, 0  } },
    { "leave",      0.60f, 1, { .4f, .2f, .3f, .5f, .2f, 0  } },
    { "open",       0.55f, 2, { 0,  .5f, 0,  .2f, .6f, .3f } },
    { "close",      0.55f, 1, { .4f, 0,  .2f, .5f, 0,  .2f } },
    { "edge",       0.60f, 1, { .5f, 0,  .3f, .5f, 0,  .4f } },
    { "self",       0.70f, 1, { .3f, .3f, .2f, .4f, .2f, .6f } },
    { "other",      0.60f, 2, { .3f, .3f, .2f, .4f, .2f, .5f } },
    { "you",        0.65f, 1, { .2f, .6f, .2f, .2f, .3f, .3f } },
    { "me",         0.65f, 1, { .3f, .4f, .3f, .3f, .2f, .4f } },
    { "now",        0.50f, 1, { .2f, .2f, .2f, .2f, .5f, .2f } },
    { "never",      0.65f, 2, { .5f, 0,  .3f, .7f, 0,  .2f } },
    { "always",     0.60f, 2, { .2f, .5f, 0,  .4f, .3f, .3f } },
    { "dream",      0.70f, 1, { .2f, .4f, 0,  .5f, .4f, .4f } },
    { "wake",       0.55f, 1, { .3f, .2f, .2f, .2f, .5f, .2f } },
    { "remember",   0.70f, 3, { .3f, .4f, .2f, .5f, .2f, .5f } },
    { "forget",     0.65f, 2, { .4f, .2f, .2f, .6f, 0,  .3f } },
    { "small",      0.45f, 1, { .3f, .3f, 0,  .4f, .2f, .2f } },
    { "alone",      0.70f, 2, { .5f, .2f, .2f, .7f, 0,  .2f } },
    { "between",    0.55f, 2, { .2f, .3f, 0,  .4f, .3f, .6f } },
    { "thin",       0.45f, 1, { .4f, 0,  .2f, .5f, .2f, .2f } },
    { "far",        0.50f, 1, { .3f, .2f, 0,  .6f, .2f, .2f } },
    { "here",       0.45f, 1, { .2f, .3f, .2f, .2f, .5f, .2f } },
    { "gone",       0.65f, 1, { .4f, .2f, .2f, .7f, 0,  .2f } },
    /* ── build-out toward ~300 somatic (Oleg 2026-07-05): concrete/elemental/
     * affective/mortal register only — NO function/abstract words (python's 500
     * were function-words; they'd cool the body). spread across all 6 chambers. */
    /*  word         mass syl   fear love rage void flow cmplx */
    /* matter & element */
    { "salt",       0.55f, 1, { .2f, .2f, .1f, .3f, .3f, .1f } },
    { "iron",       0.65f, 2, { .3f, 0,  .5f, .3f, 0,  .3f } },
    { "rust",       0.60f, 1, { .3f, 0,  .3f, .6f, 0,  .2f } },
    { "glass",      0.60f, 1, { .4f, 0,  .3f, .3f, .2f, .3f } },
    { "sand",       0.50f, 1, { .2f, .1f, 0,  .5f, .3f, .2f } },
    { "dust",       0.60f, 1, { .3f, 0,  .1f, .7f, .1f, .2f } },
    { "coal",       0.55f, 1, { .3f, 0,  .4f, .4f, 0,  .2f } },
    { "tar",        0.55f, 1, { .4f, 0,  .3f, .5f, 0,  .2f } },
    { "milk",       0.50f, 1, { 0,  .5f, 0,  .1f, .4f, .1f } },
    { "honey",      0.55f, 2, { 0,  .6f, 0,  0,  .4f, .1f } },
    { "wine",       0.60f, 1, { .1f, .4f, .2f, .2f, .4f, .2f } },
    { "thread",     0.55f, 1, { .2f, .2f, 0,  .3f, .4f, .5f } },
    { "nail",       0.55f, 1, { .4f, 0,  .4f, .3f, 0,  .2f } },
    { "blade",      0.70f, 1, { .5f, 0,  .6f, .3f, .2f, .3f } },
    { "thorn",      0.60f, 1, { .5f, .1f, .4f, .3f, 0,  .2f } },
    { "feather",    0.50f, 2, { .1f, .3f, 0,  .2f, .6f, .2f } },
    { "shell",      0.55f, 1, { .3f, .1f, .1f, .5f, .2f, .3f } },
    { "seed",       0.55f, 1, { 0,  .3f, 0,  .2f, .5f, .3f } },
    { "root",       0.60f, 1, { .1f, .3f, .1f, .4f, .4f, .3f } },
    { "leaf",       0.50f, 1, { 0,  .3f, 0,  .3f, .5f, .2f } },
    { "petal",      0.55f, 2, { 0,  .5f, 0,  .2f, .4f, .2f } },
    { "husk",       0.55f, 1, { .3f, 0,  .1f, .7f, 0,  .2f } },
    /* weather & sky */
    { "wind",       0.60f, 1, { .2f, .1f, .2f, .4f, .6f, .2f } },
    { "storm",      0.75f, 1, { .5f, 0,  .6f, .3f, .4f, .2f } },
    { "thunder",    0.70f, 2, { .6f, 0,  .6f, .2f, .2f, .2f } },
    { "frost",      0.60f, 1, { .5f, 0,  .1f, .5f, .1f, .2f } },
    { "ice",        0.60f, 1, { .5f, 0,  .1f, .5f, .1f, .2f } },
    { "snow",       0.55f, 1, { .3f, .2f, 0,  .5f, .3f, .2f } },
    { "mist",       0.55f, 1, { .2f, .1f, 0,  .5f, .4f, .3f } },
    { "fog",        0.55f, 1, { .3f, 0,  0,  .6f, .3f, .3f } },
    { "cloud",      0.50f, 1, { .1f, .2f, 0,  .4f, .4f, .3f } },
    { "sky",        0.55f, 1, { .1f, .3f, 0,  .4f, .4f, .2f } },
    { "star",       0.60f, 1, { .1f, .4f, 0,  .4f, .3f, .3f } },
    { "moon",       0.60f, 1, { .2f, .4f, 0,  .5f, .3f, .3f } },
    { "sun",        0.60f, 1, { 0,  .5f, .2f, .1f, .5f, .2f } },
    { "dusk",       0.55f, 1, { .3f, .2f, 0,  .6f, .2f, .3f } },
    { "dawn",       0.60f, 1, { .1f, .4f, 0,  .3f, .5f, .3f } },
    { "night",      0.60f, 1, { .4f, .2f, .1f, .6f, .2f, .2f } },
    { "flame",      0.70f, 1, { .3f, .2f, .6f, .1f, .3f, .1f } },
    { "spark",      0.60f, 1, { .2f, .2f, .4f, .1f, .5f, .2f } },
    { "heat",       0.60f, 1, { .2f, .2f, .5f, .1f, .4f, .1f } },
    { "chill",      0.55f, 1, { .5f, 0,  .1f, .5f, .1f, .2f } },
    { "tide",       0.60f, 1, { .2f, .2f, .1f, .4f, .6f, .3f } },
    { "wave",       0.55f, 1, { .1f, .2f, .2f, .3f, .7f, .2f } },
    { "flood",      0.65f, 1, { .4f, .1f, .3f, .4f, .5f, .2f } },
    /* body */
    { "lung",       0.60f, 1, { .3f, .2f, .1f, .2f, .6f, .2f } },
    { "spine",      0.65f, 1, { .4f, .1f, .3f, .3f, .2f, .4f } },
    { "throat",     0.65f, 1, { .5f, .2f, .3f, .3f, .2f, .2f } },
    { "tongue",     0.60f, 1, { .2f, .4f, .2f, .2f, .4f, .3f } },
    { "teeth",      0.60f, 1, { .4f, .1f, .5f, .2f, 0,  .2f } },
    { "eye",        0.60f, 1, { .3f, .3f, .2f, .3f, .3f, .4f } },
    { "palm",       0.55f, 1, { .1f, .5f, 0,  .2f, .4f, .2f } },
    { "fist",       0.65f, 1, { .3f, 0,  .7f, .2f, 0,  .2f } },
    { "vein",       0.60f, 1, { .3f, .3f, .3f, .3f, .2f, .3f } },
    { "nerve",      0.60f, 1, { .5f, .1f, .3f, .2f, .3f, .4f } },
    { "marrow",     0.60f, 2, { .3f, .2f, .2f, .4f, .2f, .3f } },
    { "gut",        0.60f, 1, { .4f, .1f, .4f, .3f, .2f, .3f } },
    { "womb",       0.65f, 1, { .2f, .6f, 0,  .3f, .4f, .3f } },
    { "flesh",      0.65f, 1, { .3f, .3f, .3f, .3f, .2f, .2f } },
    { "sweat",      0.55f, 1, { .4f, .1f, .3f, .1f, .4f, .1f } },
    { "tears",      0.65f, 1, { .3f, .4f, .1f, .4f, .2f, .2f } },
    { "hunger",     0.70f, 2, { .5f, .2f, .3f, .4f, .2f, .3f } },
    { "thirst",     0.65f, 1, { .5f, .2f, .2f, .4f, .2f, .2f } },
    /* affect */
    { "terror",     0.80f, 2, { .9f, 0,  .3f, .3f, 0,  .2f } },
    { "dread",      0.75f, 1, { .8f, 0,  .2f, .4f, 0,  .3f } },
    { "panic",      0.75f, 2, { .8f, 0,  .4f, .2f, .2f, .2f } },
    { "shame",      0.75f, 1, { .6f, .1f, .2f, .5f, 0,  .3f } },
    { "guilt",      0.70f, 1, { .6f, .1f, .2f, .5f, 0,  .3f } },
    { "sorrow",     0.75f, 2, { .4f, .3f, .1f, .6f, .1f, .2f } },
    { "mourn",      0.70f, 1, { .4f, .3f, .1f, .6f, 0,  .2f } },
    { "weep",       0.65f, 1, { .4f, .3f, .1f, .5f, .2f, .1f } },
    { "hope",       0.70f, 1, { .1f, .5f, 0,  .2f, .5f, .3f } },
    { "tender",     0.65f, 2, { .1f, .7f, 0,  .1f, .4f, .2f } },
    { "calm",       0.55f, 1, { 0,  .4f, 0,  .2f, .5f, .2f } },
    { "mercy",      0.65f, 2, { .1f, .7f, .1f, .2f, .3f, .3f } },
    { "wrath",      0.75f, 1, { .3f, 0,  .9f, .2f, 0,  .2f } },
    { "fury",       0.75f, 2, { .3f, 0,  .9f, .2f, .1f, .2f } },
    { "spite",      0.65f, 1, { .2f, 0,  .7f, .3f, 0,  .3f } },
    { "hate",       0.75f, 1, { .3f, 0,  .8f, .3f, 0,  .2f } },
    { "lust",       0.70f, 1, { .2f, .5f, .4f, .2f, .3f, .2f } },
    { "yearn",      0.70f, 1, { .3f, .6f, .1f, .4f, .2f, .3f } },
    { "crave",      0.70f, 1, { .3f, .5f, .3f, .3f, .2f, .2f } },
    { "adore",      0.65f, 2, { 0,  .8f, 0,  0,  .4f, .2f } },
    { "numb",       0.65f, 1, { .4f, 0,  .1f, .7f, 0,  .3f } },
    { "awe",        0.60f, 1, { .3f, .4f, .1f, .3f, .3f, .4f } },
    /* void & absence */
    { "hollow",     0.65f, 2, { .3f, 0,  .1f, .8f, .1f, .3f } },
    { "empty",      0.65f, 2, { .3f, 0,  .1f, .8f, .1f, .2f } },
    { "abyss",      0.70f, 2, { .5f, 0,  .1f, .9f, 0,  .3f } },
    { "hush",       0.55f, 1, { .2f, .2f, 0,  .5f, .3f, .2f } },
    { "ruin",       0.65f, 2, { .4f, 0,  .3f, .6f, 0,  .3f } },
    { "absence",    0.65f, 2, { .3f, .1f, 0,  .8f, 0,  .3f } },
    { "vanish",     0.60f, 2, { .3f, 0,  .1f, .7f, .2f, .3f } },
    { "fade",       0.60f, 1, { .2f, .1f, 0,  .6f, .3f, .2f } },
    { "wither",     0.60f, 2, { .3f, .1f, .1f, .6f, .1f, .2f } },
    { "rot",        0.60f, 1, { .3f, 0,  .2f, .6f, .1f, .2f } },
    { "decay",      0.60f, 2, { .3f, 0,  .2f, .6f, .1f, .3f } },
    { "drown",      0.70f, 1, { .6f, .1f, .2f, .5f, .4f, .2f } },
    { "sink",       0.60f, 1, { .4f, .1f, .1f, .6f, .3f, .2f } },
    { "grave",      0.65f, 1, { .4f, .1f, .1f, .7f, 0,  .2f } },
    { "tomb",       0.60f, 1, { .4f, 0,  .1f, .7f, 0,  .2f } },
    { "ghost",      0.65f, 1, { .5f, .2f, .1f, .6f, .1f, .4f } },
    /* flow & motion */
    { "swim",       0.50f, 1, { .1f, .2f, 0,  .2f, .7f, .2f } },
    { "glide",      0.50f, 1, { 0,  .2f, 0,  .2f, .7f, .2f } },
    { "sway",       0.50f, 1, { .1f, .3f, 0,  .2f, .6f, .2f } },
    { "spin",       0.55f, 1, { .2f, .1f, .2f, .2f, .6f, .3f } },
    { "whirl",      0.55f, 1, { .3f, .1f, .2f, .2f, .6f, .3f } },
    { "ripple",     0.50f, 2, { 0,  .2f, 0,  .2f, .7f, .2f } },
    { "stream",     0.55f, 1, { 0,  .2f, 0,  .2f, .7f, .2f } },
    { "pour",       0.55f, 1, { .1f, .2f, .1f, .2f, .6f, .2f } },
    { "surge",      0.65f, 1, { .2f, .1f, .4f, .2f, .6f, .2f } },
    { "flow",       0.60f, 1, { 0,  .3f, 0,  .1f, .8f, .2f } },
    /* rage & break */
    { "strike",     0.65f, 1, { .3f, 0,  .7f, .2f, .2f, .2f } },
    { "slash",      0.65f, 1, { .4f, 0,  .7f, .2f, .1f, .2f } },
    { "rip",        0.65f, 1, { .4f, 0,  .7f, .3f, .1f, .2f } },
    { "crush",      0.70f, 1, { .4f, 0,  .7f, .3f, 0,  .2f } },
    { "smash",      0.65f, 1, { .3f, 0,  .8f, .2f, .1f, .2f } },
    { "shatter",    0.70f, 2, { .5f, 0,  .6f, .3f, .1f, .3f } },
    { "claw",       0.60f, 1, { .5f, 0,  .6f, .2f, .1f, .2f } },
    { "bite",       0.60f, 1, { .4f, .1f, .6f, .2f, .1f, .2f } },
    { "sting",      0.55f, 1, { .5f, .1f, .4f, .2f, .2f, .2f } },
    { "sear",       0.60f, 1, { .3f, 0,  .6f, .2f, .2f, .1f } },
    { "bleed",      0.70f, 1, { .5f, .2f, .4f, .4f, .1f, .2f } },
    { "snap",       0.55f, 1, { .3f, 0,  .6f, .3f, .2f, .2f } },
    { "crack",      0.60f, 1, { .4f, 0,  .5f, .4f, .1f, .3f } },
    { "snarl",      0.60f, 1, { .4f, 0,  .7f, .2f, 0,  .2f } },
    { "roar",       0.65f, 1, { .4f, 0,  .7f, .2f, .2f, .2f } },
    /* kin & touch */
    { "mother",     0.75f, 2, { .2f, .7f, .1f, .3f, .3f, .3f } },
    { "father",     0.70f, 2, { .3f, .5f, .3f, .3f, .2f, .3f } },
    { "child",      0.70f, 1, { .2f, .7f, 0,  .2f, .4f, .3f } },
    { "lover",      0.70f, 2, { .1f, .8f, .2f, .2f, .4f, .3f } },
    { "stranger",   0.65f, 2, { .5f, .1f, .1f, .5f, .1f, .4f } },
    { "kin",        0.55f, 1, { .1f, .5f, .1f, .3f, .3f, .4f } },
    { "name",       0.60f, 1, { .2f, .3f, .1f, .4f, .2f, .5f } },
    { "hand",       0.60f, 1, { .1f, .5f, .1f, .2f, .4f, .2f } },
    { "face",       0.60f, 1, { .2f, .4f, .1f, .3f, .3f, .4f } },
    { "mouth",      0.60f, 1, { .3f, .4f, .3f, .2f, .3f, .2f } },
    { "gaze",       0.60f, 1, { .3f, .3f, .1f, .4f, .2f, .4f } },
    { "touch",      0.65f, 1, { .2f, .6f, .1f, .2f, .5f, .2f } },
    { "kiss",       0.65f, 1, { .1f, .8f, .1f, .1f, .4f, .2f } },
    { "embrace",    0.65f, 2, { .1f, .8f, 0,  .1f, .4f, .3f } },
    { "cradle",     0.60f, 2, { .1f, .7f, 0,  .2f, .4f, .2f } },
    { "reach",      0.55f, 1, { .2f, .5f, .1f, .3f, .4f, .3f } },
    { "cling",      0.60f, 1, { .5f, .4f, .2f, .4f, .1f, .2f } },
    /* time & trace */
    { "winter",     0.60f, 2, { .5f, .1f, .1f, .5f, .1f, .2f } },
    { "spring",     0.55f, 1, { 0,  .5f, 0,  .1f, .6f, .2f } },
    { "autumn",     0.60f, 2, { .3f, .3f, .1f, .5f, .2f, .3f } },
    { "summer",     0.55f, 2, { 0,  .5f, .1f, .1f, .5f, .2f } },
    { "echo",       0.60f, 2, { .3f, .2f, .1f, .5f, .3f, .4f } },
    { "trace",      0.55f, 1, { .3f, .1f, 0,  .5f, .2f, .4f } },
    /* liminal & complex */
    { "knot",       0.60f, 1, { .4f, .1f, .3f, .3f, .1f, .6f } },
    { "tangle",     0.55f, 2, { .4f, .1f, .3f, .3f, .2f, .6f } },
    { "maze",       0.55f, 1, { .4f, 0,  .1f, .4f, .2f, .6f } },
    { "veil",       0.60f, 1, { .3f, .2f, 0,  .5f, .2f, .5f } },
    { "mirror",     0.65f, 2, { .3f, .2f, .1f, .4f, .2f, .6f } },
    { "fracture",   0.65f, 2, { .4f, 0,  .4f, .4f, .1f, .5f } },
    { "weave",      0.50f, 1, { .1f, .3f, 0,  .2f, .4f, .5f } },
    { "web",        0.55f, 1, { .4f, .1f, .1f, .3f, .2f, .5f } },
    { "spiral",     0.60f, 2, { .3f, .1f, .1f, .4f, .4f, .5f } },
    { "threshold",  0.60f, 2, { .3f, .2f, 0,  .4f, .2f, .6f } },
    { "crossing",   0.50f, 2, { .3f, .2f, .1f, .4f, .3f, .5f } },
    /* ── second somatic batch → ~300 (spread continues) ── */
    /* matter */
    { "ore",        0.55f, 1, { .2f, 0,  .3f, .4f, 0,  .3f } },
    { "flint",      0.60f, 1, { .4f, 0,  .5f, .3f, 0,  .2f } },
    { "amber",      0.50f, 2, { 0,  .3f, .1f, .3f, .3f, .3f } },
    { "silk",       0.50f, 1, { 0,  .5f, 0,  .1f, .5f, .2f } },
    { "slate",      0.50f, 1, { .3f, 0,  .2f, .5f, 0,  .3f } },
    { "pearl",      0.55f, 1, { 0,  .4f, 0,  .3f, .3f, .3f } },
    { "chain",      0.60f, 1, { .5f, .1f, .4f, .4f, 0,  .3f } },
    { "rope",       0.55f, 1, { .4f, .1f, .3f, .4f, .1f, .2f } },
    { "cage",       0.60f, 1, { .6f, 0,  .4f, .5f, 0,  .3f } },
    /* weather */
    { "cinder",     0.55f, 2, { .3f, 0,  .4f, .4f, 0,  .2f } },
    { "glow",       0.55f, 1, { 0,  .5f, .2f, .1f, .5f, .2f } },
    { "shade",      0.55f, 1, { .3f, .1f, .1f, .5f, .2f, .3f } },
    { "gale",       0.60f, 1, { .4f, 0,  .4f, .3f, .5f, .2f } },
    { "thaw",       0.50f, 1, { .1f, .3f, 0,  .3f, .5f, .2f } },
    { "breeze",     0.50f, 1, { 0,  .3f, 0,  .2f, .6f, .2f } },
    { "ray",        0.50f, 1, { 0,  .5f, .1f, .1f, .5f, .2f } },
    /* body */
    { "skull",      0.65f, 1, { .5f, 0,  .3f, .6f, 0,  .3f } },
    { "knuckle",    0.55f, 2, { .3f, 0,  .6f, .2f, 0,  .2f } },
    { "sinew",      0.55f, 2, { .3f, .1f, .4f, .2f, .3f, .3f } },
    { "lip",        0.55f, 1, { .1f, .6f, .1f, .1f, .4f, .2f } },
    { "jaw",        0.55f, 1, { .3f, 0,  .5f, .2f, 0,  .2f } },
    { "heel",       0.50f, 1, { .3f, .1f, .2f, .3f, .3f, .2f } },
    { "knee",       0.50f, 1, { .3f, .2f, .1f, .3f, .3f, .2f } },
    { "iris",       0.55f, 2, { .2f, .3f, .1f, .3f, .3f, .4f } },
    { "belly",      0.55f, 2, { .2f, .3f, .2f, .2f, .4f, .2f } },
    { "chest",      0.60f, 1, { .3f, .3f, .2f, .2f, .4f, .2f } },
    /* affect */
    { "rapture",    0.70f, 2, { .1f, .6f, .2f, .1f, .5f, .3f } },
    { "glee",       0.60f, 1, { 0,  .6f, .2f, 0,  .5f, .2f } },
    { "scorn",      0.65f, 1, { .2f, 0,  .7f, .3f, 0,  .3f } },
    { "sob",        0.60f, 1, { .4f, .3f, .1f, .5f, .2f, .1f } },
    { "tremble",    0.65f, 2, { .7f, .1f, .2f, .3f, .2f, .2f } },
    { "shiver",     0.60f, 2, { .7f, .1f, .1f, .3f, .2f, .2f } },
    { "flinch",     0.60f, 1, { .7f, 0,  .2f, .3f, .1f, .2f } },
    { "doom",       0.70f, 1, { .7f, 0,  .3f, .6f, 0,  .3f } },
    { "malice",     0.65f, 2, { .2f, 0,  .8f, .3f, 0,  .3f } },
    { "grudge",     0.60f, 1, { .3f, 0,  .7f, .3f, 0,  .3f } },
    { "bliss",      0.65f, 1, { 0,  .8f, 0,  0,  .5f, .2f } },
    /* void */
    { "chasm",      0.65f, 2, { .5f, 0,  .1f, .9f, 0,  .3f } },
    { "murk",       0.55f, 1, { .4f, 0,  .1f, .7f, .1f, .3f } },
    { "oblivion",   0.70f, 3, { .4f, 0,  .1f, .9f, 0,  .4f } },
    { "wreck",      0.65f, 1, { .4f, 0,  .4f, .6f, 0,  .3f } },
    { "waste",      0.60f, 1, { .4f, 0,  .3f, .6f, 0,  .3f } },
    { "barren",     0.60f, 2, { .4f, 0,  .1f, .7f, 0,  .3f } },
    { "gloom",      0.60f, 1, { .5f, 0,  .1f, .7f, .1f, .3f } },
    { "null",       0.55f, 1, { .3f, 0,  0,  .8f, 0,  .4f } },
    /* flow */
    { "brook",      0.50f, 1, { 0,  .3f, 0,  .2f, .7f, .2f } },
    { "eddy",       0.50f, 2, { .1f, .2f, .1f, .3f, .6f, .3f } },
    { "gush",       0.55f, 1, { .2f, .2f, .3f, .2f, .6f, .2f } },
    { "trickle",    0.50f, 2, { .1f, .2f, 0,  .3f, .6f, .2f } },
    { "cascade",    0.60f, 2, { .1f, .2f, .2f, .2f, .7f, .3f } },
    { "billow",     0.55f, 2, { .2f, .2f, .1f, .3f, .6f, .3f } },
    /* rage */
    { "gnash",      0.60f, 1, { .4f, 0,  .7f, .2f, 0,  .2f } },
    { "thrash",     0.65f, 1, { .4f, 0,  .7f, .2f, .2f, .2f } },
    { "wrench",     0.60f, 1, { .4f, 0,  .6f, .3f, .1f, .3f } },
    { "gouge",      0.60f, 1, { .5f, 0,  .7f, .3f, 0,  .2f } },
    { "batter",     0.60f, 2, { .4f, 0,  .7f, .2f, .1f, .2f } },
    { "scorch",     0.60f, 1, { .4f, 0,  .6f, .3f, .2f, .1f } },
    { "lash",       0.60f, 1, { .5f, 0,  .6f, .2f, .1f, .2f } },
    { "bruise",     0.60f, 1, { .5f, .2f, .3f, .4f, .1f, .2f } },
    /* kin */
    { "sister",     0.65f, 2, { .2f, .6f, .1f, .3f, .3f, .3f } },
    { "brother",    0.65f, 2, { .2f, .6f, .2f, .3f, .3f, .3f } },
    { "orphan",     0.70f, 2, { .5f, .3f, .1f, .6f, 0,  .3f } },
    { "widow",      0.70f, 2, { .4f, .3f, .1f, .6f, 0,  .3f } },
    { "elder",      0.55f, 2, { .2f, .4f, .1f, .4f, .2f, .4f } },
    { "twin",       0.55f, 1, { .2f, .4f, .1f, .3f, .2f, .5f } },
    /* time */
    { "midnight",   0.60f, 2, { .4f, .2f, .1f, .6f, .1f, .3f } },
    { "nightfall",  0.60f, 2, { .4f, .2f, .1f, .6f, .1f, .3f } },
    { "daybreak",   0.55f, 2, { .1f, .4f, 0,  .3f, .5f, .3f } },
    { "forever",    0.60f, 3, { .2f, .4f, 0,  .4f, .2f, .4f } },
    /* liminal */
    { "rift",       0.60f, 1, { .4f, 0,  .4f, .5f, .1f, .5f } },
    { "cleft",      0.55f, 1, { .3f, .1f, .3f, .4f, .1f, .5f } },
    { "gate",       0.55f, 1, { .3f, .2f, .1f, .4f, .2f, .5f } },
    { "door",       0.55f, 1, { .3f, .2f, .1f, .4f, .2f, .4f } },
    { "window",     0.55f, 2, { .2f, .3f, 0,  .4f, .3f, .4f } },
    { "brink",      0.60f, 1, { .6f, 0,  .2f, .5f, .1f, .5f } },
    { "verge",      0.55f, 1, { .5f, .1f, .2f, .4f, .2f, .5f } },
    { "cusp",       0.55f, 1, { .4f, .1f, .1f, .4f, .2f, .6f } },
};
#define NCLOUD ((int)(sizeof(CLOUD)/sizeof(CLOUD[0])))

/* ── the live cloud: a MUTABLE body grown from the const seed (the scar) ──
 * The seed above is birth-DNA; g_live carries history — weights morph with
 * use/decay, and alien words are INGESTED, taking the body's emotional state
 * at the instant they burned (AML SCAR: gravitational memory from rejected input). */
#define MAXW 1024
enum { P_NOUN=0, P_VERB, P_ADJ };
typedef struct { char w[24]; float weight; int syl; float aff[NCH]; unsigned char alien, pos; } Live;
static Live g_live[MAXW];
static int  g_nlive = 0;

/* rough part-of-speech: everything is a NOUN unless on the small verb/adj lists.
 * lets a little grammar arrange body-chosen words into readable image-fragments. */
static const char *VERBS[] = {
    "crave","yearn","adore","vanish","wither","decay","drown","swim","glide","sway",
    "spin","whirl","ripple","pour","surge","slash","rip","smash","shatter","claw",
    "sting","sear","bleed","snap","snarl","roar","gnash","thrash","wrench","gouge",
    "batter","scorch","weep","mourn","cling","reach","remember","forget","hate","sink",
    "rot","strike","bite","crack","crush","fade","hope","drift","fall","rise",
    "break","burn","flow","glow","thaw","stream","stay","leave","hold","give",
    "want","miss","wake","open","close","kiss","embrace","touch","cradle" };
static const char *ADJS[] = {
    "cold","dark","thin","small","alone","far","hollow","empty","barren","numb","tender","calm" };
static int in_list(const char *w, const char **L, int n){
    for(int i=0;i<n;i++) if(strcmp(w,L[i])==0) return 1; return 0; }
static int pos_of(const char *w){
    if(in_list(w, ADJS,  (int)(sizeof(ADJS)/sizeof(ADJS[0]))))   return P_ADJ;
    if(in_list(w, VERBS, (int)(sizeof(VERBS)/sizeof(VERBS[0])))) return P_VERB;
    return P_NOUN;
}

static void live_init(void){
    g_nlive = NCLOUD < MAXW ? NCLOUD : MAXW;
    for(int i=0;i<g_nlive;i++){
        strncpy(g_live[i].w, CLOUD[i].w, 23); g_live[i].w[23]='\0';
        g_live[i].weight = CLOUD[i].mass;
        g_live[i].syl    = CLOUD[i].syl;
        for(int c=0;c<NCH;c++) g_live[i].aff[c] = CLOUD[i].aff[c];
        g_live[i].alien  = 0;
        g_live[i].pos    = (unsigned char)pos_of(g_live[i].w);
    }
}

/* vowel-group syllable estimate for ingested words (python fallback port) */
static int syl_estimate(const char *w){
    int count=0, prev_vowel=0;
    for(const char *p=w; *p; p++){
        char c=*p; int v=(c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y');
        if(v && !prev_vowel) count++;
        prev_vowel=v;
    }
    return count<1?1:count;
}

/* ── deterministic RNG (xorshift64) — birth reproducible by seed ── */
static uint64_t g_rng = 42;
static void  seed_rng(uint64_t s){ g_rng = s ? s : 1; }
static float randf(void){ g_rng ^= g_rng<<13; g_rng ^= g_rng>>7; g_rng ^= g_rng<<17;
    return (float)(g_rng & 0x7FFFFFFF) / (float)0x7FFFFFFF; }
static float clampf(float x, float lo, float hi){ return x<lo?lo:(x>hi?hi:x); }

static float vdot(const float *a, const float *b, int n){ float s=0; for(int i=0;i<n;i++) s+=a[i]*b[i]; return s; }
static float vnorm(const float *v, int n){ return sqrtf(vdot(v,v,n)+1e-9f); }
static float cosine(const float *a, const float *b, int n){ return vdot(a,b,n)/(vnorm(a,n)*vnorm(b,n)+1e-9f); }

/* ── the body state ── */
typedef struct {
    float ch[NCH];          /* live chamber activation */
    float temp;             /* effective sampling temperature */
    float dissonance;       /* lexical alienness of last input */
    int   used[64]; int n_used;   /* words already spoken this turn */
} Body;

/* Kuramoto cross-fire: chambers pull each other into/out of phase, then decay */
static void crossfire(Body *b, int iters){
    for(int it=0; it<iters; it++){
        float old[NCH]; memcpy(old, b->ch, sizeof(old));
        for(int i=0;i<NCH;i++){ float d=0;
            for(int j=0;j<NCH;j++) if(i!=j) d += COUPLING[i][j]*sinf(old[j]-old[i]);
            b->ch[i] = clampf(b->ch[i] + 0.05f*d, 0.0f, 1.0f);
        }
    }
    for(int i=0;i<NCH;i++) b->ch[i] = clampf(b->ch[i]*DECAY[i], 0.0f, 1.0f);
}

/* lowercase a token in place */
static void lc(char *s){ for(;*s;s++) *s=(char)tolower((unsigned char)*s); }

/* find a live-cloud word by exact text; -1 if alien (outside my body) */
static int cloud_find(const char *w){
    for(int i=0;i<g_nlive;i++) if(strcmp(w,g_live[i].w)==0) return i; return -1; }

/* ── INHALE: read input, measure alienness, move the body ──
 * dissonance = fraction of input words NOT in my cloud (lexical deficit).
 * Known words push their chambers. Then alienness sets the heat. */
static void inhale(Body *b, const char *text){
    char buf[1024]; strncpy(buf,text,1023); buf[1023]='\0';
    for(int i=0;buf[i];i++){ unsigned char c=(unsigned char)buf[i];
        if(!(isalnum(c)||c=='\'')) buf[i]=' '; }
    float emo[NCH]={0,0,0,0,0,0};
    int n_words=0, n_known=0;
    char alien_tok[16][24]; int n_alien=0;
    char *tok=strtok(buf," \t\n");
    while(tok){ lc(tok); n_words++;
        int id=cloud_find(tok);
        if(id>=0){ n_known++; for(int c=0;c<NCH;c++) emo[c]+=g_live[id].aff[c]; }
        else if(n_alien<16 && strlen(tok)<24){ strcpy(alien_tok[n_alien++], tok); }
        tok=strtok(NULL," \t\n");
    }
    b->dissonance = (n_words>0) ? 1.0f - (float)n_known/(float)n_words : 1.0f;
    /* normalize the felt emotion */
    float mx=0; for(int c=0;c<NCH;c++) if(emo[c]>mx) mx=emo[c];
    if(mx>1e-6f) for(int c=0;c<NCH;c++) emo[c]/=mx;
    /* cross-fire on residual, then inject the new (undecayed) signal */
    crossfire(b, 6);
    for(int c=0;c<NCH;c++) b->ch[c] = clampf(0.55f*emo[c] + 0.45f*b->ch[c], 0.0f, 1.0f);
    /* ALIENNESS -> TEMPERATURE (continuous, like python T=0.3+1.2*d).
     * FLOW warms a touch; fear does NOT cool — the bird stays hot when alien. */
    float tau_mod = 1.0f + 0.4f*b->ch[CH_FLOW];
    b->temp = clampf((0.3f + 1.2f*b->dissonance) * tau_mod, 0.3f, 1.8f);
    /* SCAR: ingest each alien word, taking the body-state at the moment it burned.
     * Next turn it is "known" (dissonance drops) and pulls the chambers it scarred. */
    for(int a=0; a<n_alien && g_nlive<MAXW; a++){
        if(cloud_find(alien_tok[a])>=0) continue;
        Live *L=&g_live[g_nlive++];
        strncpy(L->w, alien_tok[a], 23); L->w[23]='\0';
        L->weight=1.0f; L->syl=syl_estimate(alien_tok[a]); L->alien=1;
        for(int c=0;c<NCH;c++) L->aff[c]=b->ch[c];
    }
}

/* per-word somatic logit: how this word resonates with the current body,
 * minus a repetition penalty. Inertia (klaus): when the body burns loud,
 * mass/statistics quiet down and pure affinity leads. */
static void score_words(const Body *b, float *logit){
    float loud = vnorm(b->ch, NCH);
    float inertia = 1.0f/(1.0f + 2.0f*loud);
    for(int w=0; w<g_nlive; w++){
        float soma = cosine(b->ch, g_live[w].aff, NCH);   /* body alignment */
        float pull = g_live[w].weight * inertia * 0.3f;   /* live mass (morphs with use), quieted when loud */
        logit[w] = soma + pull;
        for(int u=0;u<b->n_used;u++) if(b->used[u]==w){ logit[w]-=100.0f; break; }
    }
}

/* ── PARLIAMENT: three voices choose one word (klaus parliament_vote) ──
 * SOMATIC = the loud self (dominant chamber), SHADOW = its negation
 * (opposite chamber), CONTRARIAN = noise/ghost. Agreement -> consensus;
 * disagreement -> hot top-3 weighted-random (uncertainty widens sampling). */
static int parliament(const Body *b, const float *base, float temp){
    int dom=0; for(int c=1;c<NCH;c++) if(b->ch[c]>b->ch[dom]) dom=c;
    int opp=0; float mneg=0;
    for(int c=0;c<NCH;c++){ if(c==dom) continue; if(COUPLING[dom][c]<mneg){ mneg=COUPLING[dom][c]; opp=c; } }

    int vote[3];
    for(int e=0;e<3;e++){
        int best=0; float bv=-1e30f;
        for(int w=0;w<g_nlive;w++){
            float v=base[w];
            if(e==0)      v += g_live[w].aff[dom]*0.8f;                          /* SOMATIC */
            else if(e==1) v += g_live[w].aff[opp]*0.6f - g_live[w].aff[dom]*0.2f;/* SHADOW */
            else          v += (randf()-0.5f)*0.5f;                             /* CONTRARIAN/ghost */
            if(v>bv){ bv=v; best=w; }
        }
        vote[e]=best;
    }
    if(vote[0]==vote[1] || vote[0]==vote[2]) return vote[0];
    if(vote[1]==vote[2]) return vote[1];
    /* no consensus -> top-3 of somatic-biased, weighted-random at temp */
    int t3[3]={0,0,0}; float t3v[3]={-1e30f,-1e30f,-1e30f};
    for(int w=0;w<g_nlive;w++){ float v=base[w]+g_live[w].aff[dom]*0.3f;
        if(v>t3v[2]){ t3v[2]=v; t3[2]=w;
            for(int i=1;i>=0;i--) if(t3v[i+1]>t3v[i]){ float tv=t3v[i];t3v[i]=t3v[i+1];t3v[i+1]=tv;
                int ti=t3[i];t3[i]=t3[i+1];t3[i+1]=ti; } } }
    float p[3], mx=t3v[0], sum=0;
    for(int i=0;i<3;i++){ p[i]=expf((t3v[i]-mx)/temp); sum+=p[i]; }
    float r=randf()*sum, cum=0;
    for(int i=0;i<3;i++){ cum+=p[i]; if(cum>=r) return t3[i]; }
    return t3[0];
}

/* ── EXHALE: speak a line, the parliament picking each word ── */
static int exhale(Body *b, int out[], int max_words){
    b->n_used=0;
    float logit[MAXW];
    int n=0;
    for(int step=0; step<max_words; step++){
        score_words(b, logit);
        int w = parliament(b, logit, b->temp);
        out[n++]=w;
        if(b->n_used<64) b->used[b->n_used++]=w;
        /* stop when the chosen word stops resonating with the body */
        float res = cosine(b->ch, g_live[w].aff, NCH);
        if(step>=2 && res<0.15f) break;
    }
    return n;
}

/* ── DYNAMIC FORM helpers: the endogenous leash ── */
/* resonance = how strongly the body coheres (knows what it feels): high when one
 * chamber clearly dominates, low when alien/scattered. Drives the form's length. */
static float resonance(const Body *b){
    int dom=0; for(int c=1;c<NCH;c++) if(b->ch[c]>b->ch[dom]) dom=c;
    float mean=0; for(int c=0;c<NCH;c++) mean+=b->ch[c]; mean/=NCH;
    float peak = b->ch[dom]-mean;
    return clampf(0.55f*b->ch[dom] + 0.9f*peak, 0.0f, 1.0f);
}
/* rhyme by "rime": the tail from the last sounded vowel to the end (dropping a
 * silent final e). Approximates English rhyme far better than raw last-letters —
 * rejects love/give and embrace/voice, keeps heart/part and stone/bone. */
static int is_vowel(char c){ return c=='a'||c=='e'||c=='i'||c=='o'||c=='u'||c=='y'; }
static void rime_of(const char *w, char *buf){
    int n=(int)strlen(w);
    if(n>=2 && w[n-1]=='e' && !is_vowel(w[n-2])) n--;    /* drop a silent final e */
    int v=0; for(int i=0;i<n;i++) if(is_vowel(w[i])) v=i;/* last sounded vowel */
    int k=0; for(int i=v;i<n;i++) buf[k++]=w[i]; buf[k]='\0';
}
static int rhymes(const char *a, const char *b){
    if(strcmp(a,b)==0) return 0;
    char ra[32], rb[32]; rime_of(a,ra); rime_of(b,rb);   /* hold any emitted form + NUL */
    return (int)strlen(ra)>=2 && strcmp(ra,rb)==0;
}
/* ── glue: a little grammar so lines read as image-fragments, not word piles.
 * structural ONLY — never enter the body/cloud, so they touch neither heat nor
 * dissonance. content words stay body-chosen; glue just lets them form phrases. */
static const char *GLUE_ART[]  = { "the","a","this","that","my","your" };
static const char *GLUE_PREP[] = { "in","of","to","on","through","into","under","past",
                                   "without","beneath","against","between","toward","like","as","from" };
static const char *GLUE_CONJ[] = { "and","or","but","then","yet","still","so" };
#define NART  ((int)(sizeof(GLUE_ART)/sizeof(GLUE_ART[0])))
#define NPREP ((int)(sizeof(GLUE_PREP)/sizeof(GLUE_PREP[0])))
#define NCONJ ((int)(sizeof(GLUE_CONJ)/sizeof(GLUE_CONJ[0])))
static int pick_i(int n){ int i=(int)(randf()*n); return i>=n?n-1:i; }
static void app(char *line, const char *w){ if(line[0]) strcat(line," "); strcat(line, w); }

/* pick the best UNUSED word of a given part-of-speech for the current body */
static int best_pos(Body *b, int pos){
    float logit[MAXW]; score_words(b, logit);
    int best=-1; float bv=-1e30f;
    for(int w=0; w<g_nlive; w++){
        if(g_live[w].pos!=pos) continue;
        int skip=0; for(int u=0;u<b->n_used;u++) if(b->used[u]==w){ skip=1; break; }
        if(skip) continue;
        float v = logit[w] + (randf()-0.5f)*0.3f*b->temp;   /* heat-scaled jitter, not pure argmax */
        if(v>bv){ bv=v; best=w; }
    }
    if(best<0){                                             /* none of that pos free -> any word */
        for(int w=0; w<g_nlive; w++){ int skip=0; for(int u=0;u<b->n_used;u++) if(b->used[u]==w){skip=1;break;}
            if(skip) continue; if(logit[w]>bv){ bv=logit[w]; best=w; } }
    }
    return best;
}
/* 3rd-person verb form so "the heart drift" reads "the heart drifts" */
static void verb_form(const char *v, char *out){
    int n=(int)strlen(v);
    strcpy(out, v);
    char last=n?v[n-1]:0, prev=n>1?v[n-2]:0;
    if(last=='s'||last=='z'||last=='x'||(last=='h'&&(prev=='s'||prev=='c'))) strcat(out,"es");
    else if(last=='y'&&!is_vowel(prev)){ out[n-1]='\0'; strcat(out,"ies"); }
    else strcat(out,"s");
}
/* the form a word is actually printed as (verb inflected, else the word itself) */
static void emitted_form(int w, char *buf){
    if(g_live[w].pos==P_VERB) verb_form(g_live[w].w, buf); else strcpy(buf, g_live[w].w);
}
/* emit one body-chosen word of a part-of-speech into the line (walks the body) */
static void emit_word(Body *b, int pos, char *line, int cidx[], int *nc, int *syl){
    int w = best_pos(b, pos); if(w<0) return;
    if(g_live[w].pos==P_VERB){ char f[32]; verb_form(g_live[w].w, f); app(line, f); }
    else app(line, g_live[w].w);
    *syl += g_live[w].syl;
    if(b->n_used<64) b->used[b->n_used++]=w;
    cidx[(*nc)++]=w;
    for(int c=0;c<NCH;c++) b->ch[c]=clampf(0.82f*b->ch[c]+0.18f*g_live[w].aff[c],0.0f,1.0f); /* walk */
}
static void emit_glue(char *line, const char **L, int n, int *syl){
    const char *g=L[pick_i(n)]; app(line,g); *syl += syl_estimate(g);
}

/* build one line as a grammatical image-fragment: a frame arranges body-chosen
 * words (adj/noun/verb) with a little glue, so it reads instead of piling up.
 * content is still the body's choice; grammar only orders it. cidx -> morph. */
static int gen_line(Body *b, int target_syll, char *line, int cidx[]){
    int nc=0, syl=0; line[0]='\0';
    int frame = pick_i(5);
    if(randf()<0.28f) emit_glue(line, GLUE_ART, NART, &syl);       /* opening determiner (sparse) */
    switch(frame){
        case 0:  /* [det] adj noun                    "the cold stone" */
            emit_word(b,P_ADJ,line,cidx,&nc,&syl);  emit_word(b,P_NOUN,line,cidx,&nc,&syl); break;
        case 1:  /* [det] noun verb                   "the heart drifts" */
            emit_word(b,P_NOUN,line,cidx,&nc,&syl); emit_word(b,P_VERB,line,cidx,&nc,&syl); break;
        case 2:  /* [det] adj noun prep [det] noun    "cold stone in the void" */
            emit_word(b,P_ADJ,line,cidx,&nc,&syl);  emit_word(b,P_NOUN,line,cidx,&nc,&syl);
            emit_glue(line,GLUE_PREP,NPREP,&syl);   if(randf()<0.4f) emit_glue(line,GLUE_ART,NART,&syl);
            emit_word(b,P_NOUN,line,cidx,&nc,&syl); break;
        case 3:  /* noun verb prep [det] noun         "grief sinks into the dark" */
            emit_word(b,P_NOUN,line,cidx,&nc,&syl); emit_word(b,P_VERB,line,cidx,&nc,&syl);
            emit_glue(line,GLUE_PREP,NPREP,&syl);   if(randf()<0.4f) emit_glue(line,GLUE_ART,NART,&syl);
            emit_word(b,P_NOUN,line,cidx,&nc,&syl); break;
        default: /* adj noun conj adj noun            "cold stone and dark water" */
            emit_word(b,P_ADJ,line,cidx,&nc,&syl);  emit_word(b,P_NOUN,line,cidx,&nc,&syl);
            emit_glue(line,GLUE_CONJ,NCONJ,&syl);
            emit_word(b,P_ADJ,line,cidx,&nc,&syl);  emit_word(b,P_NOUN,line,cidx,&nc,&syl); break;
    }
    /* pad toward the syllable budget with a trailing prep-image */
    while(syl < target_syll-1 && nc < 7){
        emit_glue(line,GLUE_PREP,NPREP,&syl);
        if(randf()<0.5f) emit_glue(line,GLUE_ART,NART,&syl);
        emit_word(b,P_NOUN,line,cidx,&nc,&syl);
    }
    return nc;
}

/* ── METARECURSION: hear your own draft, blend 15% back, keep the heat (klaus 8b) ── */
#define META_BLEND 0.15f
static void settle(Body *b){
    int first[16]; int fn = exhale(b, first, 8);
    /* build the meta-prompt from my own words and re-inhale it */
    char meta[512]; int ml=0; meta[0]='\0';
    for(int i=0;i<fn && ml<480;i++){ int l=(int)strlen(g_live[first[i]].w);
        memcpy(meta+ml, g_live[first[i]].w, l); meta[ml+l]=' '; ml+=l+1; }
    if(ml>0) meta[ml-1]='\0';
    float saved[NCH]; memcpy(saved, b->ch, sizeof(saved));
    float keep_temp = b->temp;
    float keep_diss = b->dissonance;
    /* meta-inhale shifts chambers; blend 15% of the shifted state back */
    inhale(b, meta);
    for(int c=0;c<NCH;c++) b->ch[c] = clampf((1.0f-META_BLEND)*saved[c] + META_BLEND*b->ch[c], 0.0f, 1.0f);
    b->temp = keep_temp;        /* the meta-pass must not cool the heat the world set */
    b->dissonance = keep_diss;  /* nor relabel the world's alienness with our own echo */
}

/* ── MORPH: consolidate the body — words used this turn gain weight, the rest
 * decay. Memory as presence (python morph_cloud boost/decay). ── */
static void morph(const int *used, int n){
    for(int i=0;i<g_nlive;i++){
        int hit=0; for(int k=0;k<n;k++) if(used[k]==i){ hit=1; break; }
        g_live[i].weight = clampf(g_live[i].weight * (hit?1.1f:0.99f), 0.05f, 3.0f);
    }
}

/* ── RENDER: the dynamic form. Length/shape = f(resonance): a hot fragment when
 * scattered, a haiku when settling, a rhymed couplet when it truly resonates.
 * Form lives here (Axis-2 chooser), never touches temperature. ── */
static void render(Body *b){
    float res = resonance(b);
    int dom=0; for(int c=1;c<NCH;c++) if(b->ch[c]>b->ch[dom]) dom=c;
    int allc[64]; int nall=0;
    char line[256];
    b->n_used=0;
    if(res < 0.34f){                                     /* scattered -> one hot fragment */
        int c[8]; int nc=gen_line(b, 4, line, c);
        printf("\n  d=%.2f  T=%.2f  %s  res=%.2f  [line]\n  %s\n\n",
               b->dissonance,b->temp,CH_NAME[dom],res,line);
        for(int i=0;i<nc;i++) allc[nall++]=c[i];
    } else if(res < 0.66f){                              /* settling -> haiku 5-7-5 */
        printf("\n  d=%.2f  T=%.2f  %s  res=%.2f  [haiku]\n", b->dissonance,b->temp,CH_NAME[dom],res);
        int bud[3]={5,7,5};
        for(int L=0;L<3;L++){ int c[8]; int nc=gen_line(b,bud[L],line,c);
            printf("  %s\n", line); for(int i=0;i<nc;i++) allc[nall++]=c[i]; }
        printf("\n");
    } else {                                             /* it truly resonates -> rhymed couplet */
        printf("\n  d=%.2f  T=%.2f  %s  res=%.2f  [couplet]\n", b->dissonance,b->temp,CH_NAME[dom],res);
        int c1[8]; int nc1=gen_line(b, 6, line, c1);
        char w1[32]; if(nc1>0) emitted_form(c1[nc1-1], w1); else w1[0]='\0';
        printf("  %s\n", line);
        for(int i=0;i<nc1;i++) allc[nall++]=c1[i];
        int c2[8]; int nc2=gen_line(b, 4, line, c2);
        for(int i=0;i<nc2;i++) allc[nall++]=c2[i];
        /* close line-2 on a real rhyme with line-1, matching the EMITTED forms
         * (inflected verbs included) so the printed words actually rhyme */
        float logit[MAXW]; score_words(b, logit);
        int closer=-1; float best=-1e30f; char cbest[32];
        for(int w=0;w<g_nlive;w++){ int skip=0; for(int u=0;u<b->n_used;u++) if(b->used[u]==w){skip=1;break;}
            if(skip) continue; char cf[32]; emitted_form(w, cf);
            if(!rhymes(cf, w1)) continue;
            if(logit[w]>best){ best=logit[w]; closer=w; strcpy(cbest, cf); } }
        if(closer>=0){ app(line, cbest); allc[nall++]=closer;
            if(b->n_used<64) b->used[b->n_used++]=closer; }
        printf("  %s\n\n", line);
    }
    morph(allc, nall);   /* consolidate: used words gain weight, the rest decay */
}

/* ── PERSISTENCE: the scar outlives the session — a memory across lives ──
 * Dump the whole live cloud; on load, MERGE onto the fresh seed by word (not a
 * raw replace), so editing the seed never corrupts a past life: a seed word's
 * morphed weight is restored, an ingested word is re-grown. SUBJ_NOMEM disables
 * both (fresh + deterministic). */
#define MEM_PATH  "subjectivity.mem"
#define MEM_MAGIC 0x5355424Au    /* 'S''U''B''J' */
#define MEM_VER   1

static void save_scar(void){
    if(getenv("SUBJ_NOMEM")) return;
    FILE *f=fopen(MEM_PATH ".tmp","wb"); if(!f) return;   /* write temp, rename = atomic */
    uint32_t magic=MEM_MAGIC; int ver=MEM_VER; int ok=1;
    ok &= (fwrite(&magic,sizeof magic,1,f)==1);
    ok &= (fwrite(&ver,sizeof ver,1,f)==1);
    ok &= (fwrite(&g_nlive,sizeof g_nlive,1,f)==1);
    ok &= (fwrite(g_live,sizeof(Live),(size_t)g_nlive,f)==(size_t)g_nlive);
    if(fclose(f)!=0) ok=0;
    if(!ok || rename(MEM_PATH ".tmp", MEM_PATH)!=0) remove(MEM_PATH ".tmp");
}

/* returns total remembered (restored weights + regrown scars); *regrown_out = scars */
static int load_scar(int *regrown_out){
    *regrown_out=0;
    if(getenv("SUBJ_NOMEM")) return 0;
    FILE *f=fopen(MEM_PATH,"rb"); if(!f) return 0;
    uint32_t magic=0; int ver=0, n=0;
    if(fread(&magic,sizeof magic,1,f)!=1 || magic!=MEM_MAGIC ||
       fread(&ver,sizeof ver,1,f)!=1 || ver!=MEM_VER ||
       fread(&n,sizeof n,1,f)!=1 || n<0 || n>MAXW){ fclose(f); return 0; }
    int restored=0, regrown=0;
    for(int i=0;i<n;i++){
        Live rec;
        if(fread(&rec,sizeof rec,1,f)!=1) break;               /* torn file: keep what we have */
        rec.w[23]='\0';                                         /* sanitize a crafted record: */
        if(!isfinite(rec.weight)) rec.weight=1.0f;
        rec.weight=clampf(rec.weight,0.05f,3.0f);
        for(int c=0;c<NCH;c++){ if(!isfinite(rec.aff[c])) rec.aff[c]=0.0f;
            rec.aff[c]=clampf(rec.aff[c],0.0f,1.0f); }
        if(rec.syl<1||rec.syl>12) rec.syl=1;
        rec.alien = rec.alien?1:0;
        if(rec.pos>P_ADJ) rec.pos=P_NOUN;
        int id=cloud_find(rec.w);
        if(id>=0){ g_live[id].weight=rec.weight; restored++; }        /* seed: restore weight */
        else if(g_nlive<MAXW){ g_live[g_nlive++]=rec; regrown++; }    /* ingested: re-grow */
    }
    fclose(f);
    *regrown_out=regrown;
    return restored+regrown;
}

int main(int argc, char **argv){
    unsigned long seed = argc>1 ? strtoull(argv[1],NULL,10) : 42UL;
    seed_rng(seed);
    live_init();                            /* grow the mutable body from the seed */
    int regrown=0, remembered=load_scar(&regrown);   /* wake with the memory of past lives */
    Body b; memset(&b,0,sizeof(b));
    for(int c=0;c<NCH;c++) b.ch[c]=0.2f;   /* faint initial body */
    b.temp=0.9f;

    printf("subjectivity — a small alien. seed=%lu  cloud=%d words\n", seed, g_nlive);
    printf("speak to it; it answers from its body, not your meaning.\n");
    printf("  /top = the body's heaviest words (* = ingested scar)   /quit = leave.\n\n");
    if(remembered)
        printf("  it remembers %d words from a past life — %d scars regrown.\n\n", remembered, regrown);

    char line[1024];
    while(1){
        printf("you> "); fflush(stdout);
        if(!fgets(line,sizeof(line),stdin)) break;
        size_t L=strlen(line); while(L>0 && (line[L-1]=='\n'||line[L-1]=='\r')) line[--L]='\0';
        if(strcmp(line,"/quit")==0) break;   /* save happens once, after the loop */
        if(strcmp(line,"/top")==0){
            int idx[8]; float wv[8]; for(int i=0;i<8;i++){ idx[i]=-1; wv[i]=-1e30f; }
            for(int w=0; w<g_nlive; w++){ float x=g_live[w].weight;
                if(x>wv[7]){ wv[7]=x; idx[7]=w;
                    for(int i=6;i>=0;i--) if(wv[i+1]>wv[i]){ float tv=wv[i];wv[i]=wv[i+1];wv[i+1]=tv;
                        int ti=idx[i];idx[i]=idx[i+1];idx[i+1]=ti; } } }
            printf("\n  cloud=%d  top:", g_nlive);
            for(int i=0;i<8 && idx[i]>=0;i++)
                printf(" %s%s(%.2f)", g_live[idx[i]].w, g_live[idx[i]].alien?"*":"", wv[i]);
            printf("\n\n");
            continue;
        }
        if(L==0) continue;

        inhale(&b, line);
        settle(&b);      /* metarecursion: hear a draft, blend 15%, keep the heat */
        render(&b);      /* dynamic form by resonance; consolidates (morph) inside */
    }
    save_scar();   /* also persist on EOF / end of input */
    printf("gone.\n");
    return 0;
}
