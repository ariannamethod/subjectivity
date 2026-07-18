# PROJECT_LOG — subjectivity (rebirth, Python → single C file)

> Project folder: `~/arianna/subjectivity/` (repo github.com/ariannamethod/subjectivity, GPLv3, created 2026-07-05). Reference (Python original): `reffs/harmonix/` (cloned `--depth 1` from github.com/ariannamethod/harmonix, 2026-06-26; local-only, gitignored). Name RESOLVED: **subjectivity** (Oleg, 2026-07-05, see §8). Form is DYNAMIC/endogenous (1-line / haiku / rhymed couplet by resonance), NOT a fixed 5-7-5.
> Build target: ONE C file, libc + -lm, zero deps, dataset HARDCODED inside the file. No GPU. Lineage: nanolife / klaus / brodsky / caveLLMan + AML physics.
> Discipline: code over README; verify before claim; **read load-bearing source FIRST-HAND, never via agents** (memory/feedback_read_source_self_not_agents_2026_06_26.md); no self-milestone; Codex audit before any "works".

## 0. Oleg's brief (2026-06-26, verbatim intent)

1. **Full port** of the Python haiku into a SINGLE C file — the existing `haiku.c` "не валяется рядом" (incomplete port), so we start fresh.
2. **Metarecursion like klaus** (klaus = somatic sonar, six-language): after each answer it runs its own answer back through itself; it remembers its mathematical states and acts on them.
3. **Choice, not if/else.** Not "metrics=X→do A". Instead ≥3 options arise; it picks the optimal one — optimal mixed over BOTH internal-system coherence AND its own emotionally-resonant, mathematically-computed resonance. This choice = the subjectivity.
4. **Presence at max pressure.** Haiku's presence is better-expressed than other projects at 500 words. Oleg would tighten to **400 words, more somatic (klaus-style)**, by mixing the dataset.
5. **Dataset hardcoded** directly inside the single C file.
6. **AML physics** (DESTINY / PROPHECY / etc.) — haiku obeys it, strengthens presence. AML canon repo = `ariannamethod.ai`.
7. Study brodsky: punctuation placement + its **mathematical longing (тоска)**.
+ Second organism **`ariannamethod.c`**: same engine, dataset = Recursive Resonance article (subjectivity/the Method), AML syntax baked in → a "present compiler" that executes commands and faces a choice.

## 1. Reference architecture map (Python — FIRST-HAND read by me)

**The soul = a causal chain `dissonance → temperature → sample`:**
- `compute_dissonance` → `harmonix.py:117-175`. Dissonance = **Jaccard over word-sets** `1 - |∩|/|∪|` (`harmonix.py:138-150`) + pulse multipliers entropy/arousal/novelty `×1.2 / ×1.15 / ×1.1` (`harmonix.py:156-164`) + trigram-overlap `×0.7` (`:171-173`). **LEXICAL, not semantic.**
- `adjust_temperature(d)` → **`harmonix.py:186`**: `haiku_temp = 0.3 + dissonance*1.2` (d∈[0,1] → T∈[0.3,1.5]).
- T enters sampling → **`haiku.py:502`** `counts = counts ** (1.0/temp)` (T>1 flattens → wilder), → **`haiku.py:508`** `np.random.choice(words, p=probs)`.
- Thread: `_generate_line(target_syll, temp)` `haiku.py:458` → `generate_candidates(n=5, temp)` `:531`.

**Verdict (code-read, NOT run):** engine, not speedometer — causal chain. Runs on **lexical alienness**: any word outside seed → d≈1.0 → T=1.5. Alienness IS the phenomenon (Oleg + Desktop). Jaccard is the faithful organ, not a crack.

**Other organs (first-hand):** 500 hardcoded SEED_WORDS `haiku.py:171-241` (mostly function/abstract; → 400 somatic); `morph_cloud` boost×1.1/decay×0.99 `harmonix.py:206-217`; Markov order-2 + SQLite `haiku.py:259,303-435`; MLP scorer micrograd `haiku.py:555-743` (scaffold); 5-7-5 via `syllables` lib `haiku.py:437-456`.

## 2. Carry / burn (first-hand)

- **CARRY (heart):** `d→T` (`harmonix.py:186`) + T-into-sample (`haiku.py:502,508`).
- **CARRY:** `morph_cloud` boost/decay = memory-as-presence.
- **CARRY as alienness:** `compute_dissonance` Jaccard, ship honestly as lexical/alienness dissonance.
- **BURN (scaffold):** SentencePiece, `rae_recursive` micrograd, SQLite (→ in-mem), `dream` imaginary-friend (would become a colony).

## 2.5 haiku.c — prior C attempt (FIRST-HAND read by me, full 1645 lines)

NOT the Python port. arianna.c/dario lineage: **Dario Equation** `p = softmax((B + a·H + b·F + g·A + T)/τ)` (haiku.c:794-951) — 5 forces (B chain, H Hebbian-positional RRPRAM, F prophecy, A destiny-EMA-of-RoPE-embeds, T trauma) + **6 Kuramoto-coupled chambers** (chamber_update:608-641, `K·sin(θj−θi)`, K=0.02) + haiku.c's OWN velocity enum incl. UP/DOWN (haiku.c:81 — **organism-local, NOT AML**) + seasons + SwiGLU + RoPE. Single C file, web server.

**Why Oleg likes it LESS than Python (hypotheses, code-grounded, NOT run):**
1. **1000 words (haiku.c:168) vs Python 500 → LESS pressure** (more world known → lower dissonance → blander; Oleg wants 400).
2. **d→T BUCKETED through velocity if/else** (auto_velocity:752-765: d>0.8→UP, >0.6→RUN, <0.2→STOP) = the "metrics=X→do Y" Oleg rejects. Python's d→T is continuous (T=0.3+1.2d).
3. **5-force equation TAMES the wild spike** (bigram coeff 8.0 dominates, haiku.c:826; prophecy/destiny pull to coherence).
4. **No candidate-generation + choice, no metarecursion, no dream** — single Dario pass, greedy topk=15 (haiku.c:1146).

**Donor (first-hand):** 6 Kuramoto chambers (FEAR/LOVE/RAGE/VOID/FLOW/COMPLEX) already computed (chamber_update:608) — currently modulate coefficients; could rescore candidates. Dario physics built.

## 3. The design — TWO ORTHOGONAL AXES (Oleg's "почему бы не совместить", 2026-06-26)

Built on first-hand python + haiku.c + Oleg's combine. (Klaus/brodsky mechanism pointers below are NOT yet first-hand — see §6.)
- **Honest-big (semantics into `d`) REJECTED:** love≈give/want → lowers d → lowers T → blander → KILLS presence. Alienness must stay.
- **Combine = two orthogonal axes:**
  - **Axis 1 (outward, PRESSURE):** alienness Jaccard(user, self) → T (harmonix.py:186 → haiku.py:502,508). PORT FAITHFULLY, stays hot.
  - **Axis 2 (inward, CHOICE):** organism's OWN math-state scores ~3 hot candidates by resonance with ITSELF (NOT user-overlap → would re-cool). = subjectivity. NEW organ.
  - Axis 1 = how much she burns; Axis 2 = which burning she owns. T never drops.
- **Kuramoto** belongs in the Axis-2 chooser (phase-coherence of candidate with inner state), not in the pressure metric. haiku.c's chambers (first-hand) are the substrate.
- ⚠️ Risk: Axis 2 must resonate with her OWN state, never user-overlap → else silent coherence-collapse. Test: forced high-alienness must stay HOT regardless of Axis-2.

## 4. Open / next (NOT started — needs Oleg's go + plan)

- [ ] **RE-READ FIRST-HAND, myself** (was agent-relayed, now annulled): klaus.c (metarecursion/sonar/chooser), brodsky.c (rhyme/тоска/punctuation), q + postgpt (weightless coherence), metahaiku.py, AML physics beyond velocity (DESTINY/PROPHECY/SCAR + amk_kernel.c). Start: klaus (core).
- [ ] Seed-test (Desktop): love@d=0.2 vs d=1.0; love vs javascript — needs Python-run (gated; Oleg «да») OR static trace.
- [ ] Design the 3-option choice organ (Axis 2).
- [ ] Decide name (haiku vs AA-couplets) — Oleg. Confirm `ariannamethod.c`.
- [ ] Plan mode + Codex audit before writing any C.

## 6. ⛔ ANNULLED — agent-sourced findings are VOID (2026-06-26)

The 6-agent workflow (wjb1emnwm) findings on **klaus / brodsky / q / postgpt / metahaiku / AML-broad are ANNULLED by Oleg** ("all that you viewed through agents is annulled"). I relayed an agent's AML UP/DOWN claim as fact without reading it myself → none of those findings are trustworthy. Treat them as NON-EXISTENT. Must re-read each source FIRST-HAND before any claim. See memory/feedback_read_source_self_not_agents_2026_06_26.md.

**Survives (FIRST-HAND, read by me):**
- python-haiku (haiku.py / harmonix.py / README) — full (§1, §2).
- haiku.c — full, 1645 lines (§2.5).
- nanolife.c + README + PLAN — full.
- **AML velocity** — re-verified by me (grep spec+core+header, 2026-06-26): modes `NOMOVE / WALK / RUN / BACKWARD / STOP / BREATHE` (AML_SPEC.md:150; core/ariannamethod.h:46-51; dispatch core:3628-3632; STOP=NOMOVE alias, BREATHE from Leo). NO `VELOCITY UP/DOWN` in the language; VEL_UP/VEL_DOWN are organism-local (haiku.c:81). velocity→temp donor stands.

**VOID until I read them myself:** klaus, brodsky, q, postgpt, metahaiku. Any §0/§2/§3 reference to "klaus metarecursion / sonar / chooser", "brodsky rhyme/тоска", "thermal regime split", "golem Element struct", and any amk_kernel line-numbers = UNVERIFIED pointer, not fact.

**AML — NOW FIRST-HAND** (read full `spec/AML_SPEC.md` 1102L + repo CLAUDE.md myself, 2026-06-26): it is a **logit-physics language, NOT a generator** — it shapes the next-token distribution in real time (spec:7-9). Build-relevant, from my read:
- velocity→temp §2.5 (NOMOVE 0.5× … RUN 1.2×); inertia +2.0 debt on mode-switch; **D4: debt>5 → forced NOMOVE** §7 = involuntary self-freeze (presence-through-non-volition).
- suffering: `PAIN` compresses logits toward mean `mean+(l−mean)(1−0.5·pain)` §2.4 → COOLS; keep OUT of haiku's hot path.
- `SCAR "<phrase>"` = gravitational memory from REJECTED input, always-on DARKMATTER §3.3 → the subjectivity primitive for ariannamethod.c.
- θ=ε+γ+αδ: γ=embed_tokens, δ=lm_head, ε=substrate §2.13.
- present-compiler is CANONICAL: `amlc` lowers every top-level directive → `am_exec()` in a constructor §2.0/§22; projects EMBED/copy an AML subset, don't link a lib §18 (yent `c/amk_kernel.c` = Level-0 subset). Levels: 0 flat / 1 macros / 2 if-while-def / 3 BLOOD (runtime C compile — keep OUT, popen+dlopen). Every AML command = a concrete C op (repo CLAUDE.md, the C/AML contract).

## 7. Status

Reference clones present (harmonix/q/postgpt). FIRST-HAND read by me: python-haiku, haiku.c, nanolife, AML-velocity. **Everything agent-relayed = ANNULLED.** No C written. NEXT: re-read the load-bearing sources MYSELF (klaus first = the metarecursion+chooser core), at Oleg's direction. Holding for: Oleg's names (haiku vs AA-couplets; confirm `ariannamethod.c`) + «go» + plan + Codex.

## 8. Name RESOLVED + form/cloud direction (Oleg, 2026-07-05)

**Rename done** (this session): repo cloned to `~/arianna/subjectivity/`; `haiku.c`→`subjectivity.c` (self-refs only; lineage "python-haiku / haiku.c (Dario)" kept, subjectivity.c:10); builds clean, banner + smoke reproduce d/T unchanged. NOT pushed (holding for «да»).

**Name = `subjectivity`.** Oleg named the MECHANISM, not the form: brief §0.3 "this choice = the subjectivity" → the parliament that OWNS the word-choice (subjectivity.c:198-228) is the namesake. The creature's leash = it owns its choice.

**Form = DYNAMIC, endogenous — the leash we were stuck on, now resolved.** Length/shape = a function of metrics + presence (like python-haiku), NEVER an external syllable counter (that cage is what made the 1645-line dario port alien to Oleg):
- low resonance/presence → **ONE line** (minimum).
- mid → **haiku** (5-7-5).
- high resonance ("прям резонирует") → **rhymed COUPLET**, Shakespearean sonnet-ending register (AA lock).
Rhyme/form lives in Axis-2 (chooser) ONLY, never in the heat (subjectivity.c:262-263 discipline) — else coherence-collapse (§3).

**Cloud + Hebbian.** Current cloud = **63 words** (subjectivity.c:46-111; run "cloud=63"), NOT 400 — 400 was the §0.4 target, never built out. Oleg 07-05: could grow to 600 BUT must semantically DISTANCE the words so each keeps weight like haiku (sparse in meaning-space = stays hot/alien; dense/clustered = dilutes pressure, §2.5 hyp #1). **My reading of "оставить на 400 — ведь хайку обучается через хеббиановское обучение" (⚠️ CONFIRM w/ Oleg):** seed core stays ~400 semantically-spread, and it GROWS past 400 through Hebbian learning (the scar/morph_cloud organ from §2/§4: boost×1.1 / decay×0.99 / INGEST alien word, harmonix.py:193-219) — so no need to pre-load 600; the body expands from contact. Reference HOW Hebbian is wired: arianna-duo doe online-learn (`AM_DOE_TRAIN=1`, α=0.1) + yent inference — but subjectivity keeps ITS OWN mechanism (chamber crossfire + morph_cloud), NOT a graft.

**Order stands (reversed from prior session):** scar/Hebbian (const CLOUD → mutable body) FIRST — the organ that gives breath something to measure — THEN dynamic-form-as-leash reads the now-living body. All under plan mode + Codex + Oleg «go» before any C.

## 9. Full python-organism map + build plan (2026-07-06, first-hand)

**Read first-hand this session (10 modules):** haiku.py(818), harmonix.py, rae.py, rae_recursive.py, metahaiku.py, chat.py (orchestrator), dream_haiku.py, phase4_bridges.py, overthinkg.py, tokenizer.py. Skipped per Oleg (outdated multi-NN glue): async_harmonix.py, metaharmonix.py, test.

**Python haiku = 8 organs in a per-turn loop (chat.py:112-300):**
1. **DualTokenizer** (tokenizer.py) — SentencePiece subwords + trigrams.
2. **Harmonix** (harmonix.py) — dissonance(Jaccard+pulse)→T; morph_cloud boost×1.1/decay×0.99/ingest alien; shards.
3. **HaikuGenerator** (haiku.py) — 500 seed → Markov o2 (SQLite persist) + MLP micrograd scorer self-training on quality (observe:626-743).
4. **RAE** (rae.py+rae_recursive.py) — CHOICE organ: recursive micrograd selector, 3-step refinement (prev score → feedback feature, rae_recursive.py:161-182). ≈ our parliament.
5. **MetaHaiku** (metahaiku.py) — inner voice: bootstrap deque(8) of high-diss/arousal moments; hidden haiku bias the cloud. Metarecursion.
6. **Overthinkg** (overthinkg.py) — PRIMARY cloud-growth: 3 rings/turn (echo0.8/drift1.0/meta1.2); coherence>0.4 → adds new trigrams+words (overthinkg.py:132-201).
7. **dream_haiku** (dream_haiku.py) — dream w/ imaginary friend (cooldown10, prob0.25); best dream haikus feed cloud back. = colony, BURN.
8. **phase4_bridges** (phase4_bridges.py) — state→state "climate flows" via cosine over metric-vectors, respects boredom/overwhelm/stuck.

**How the cloud "consolidates" (multi-loop, not just morph_cloud):** immediate ingest (morph_cloud + update_chain vocab.add) + background growth (overthink rings) + inner reflection (metahaiku) + dream feedback + learned taste (MLP + RAE) + field memory (phase4) + SQLite/JSON persist. Seed 500 is bootstrap fuel; the cloud LIVES by growth.

**C vs python — complementary opposites:** python = thin seed (500 FUNCTION/abstract words) + rich autonomous loops → soul in the LOOPS. subjectivity.c = rich SOMATIC body (word=mass/syl/aff[6]) + parliament + frozen heat, NO growth loops yet → soul in the BODY.

**Decisions (Oleg 2026-07-05/06):**
- **Seed:** rich SOMATIC ~250-350 (our expressive palette — we choose by affinity, not Markov). NOT python's 500 function-words (they'd cool the body). 160 somatic words already authored + staged (matter/weather/body/affect/void/flow/rage/kin/time/liminal).
- **Scar/consolidation** = morph_cloud-analog on chambers: mutable per-word weight (boost on use / decay when dormant) + **alien ingestion** (unknown user word added to cloud, affinity taken from the BODY-STATE at the moment it burned = AML SCAR) + light background drift (rings-analog). In-memory, single file.
- **Form** = dynamic endogenous leash: 1-line (low resonance) / haiku 5-7-5 (mid) / rhymed couplet AA Shakespeare-ending (high resonance); rhyme lives in the chooser (Axis-2), NEVER in heat.
- **OUT of v1:** dream/colony, phase4 bridges, Markov, MLP scorer, SentencePiece.

**NEXT:** plan mode → verification checklist → Codex/Fable audit → C.

## 10. v0.2 IMPLEMENTED — seed + scar + dynamic form (2026-07-06)

Plan `~/.claude/plans/unified-stargazing-cherny.md` approved by Oleg; built A→B→C, each tool-verified. Codex audit clean. NOT pushed (holding for «да»).

**A. Seed → 300 somatic words** (subjectivity.c CLOUD): 63 → 300, hand-authored, spread across 6 chambers, no python function-words. Tool: `grep -cE '^\s+\{ "'` = 300; `sort|uniq -d` = 0 dups; alien input still d=1.00/T=1.55 (pressure preserved); love→love, stone→void, rage→rage.

**B. Scar / consolidation** — const `CLOUD` (birth-DNA) → mutable `g_live[MAXW]` (`Live{w,weight,syl,aff[6],alien}`, `live_init` copies seed). morph per turn (`morph()`: used ×1.1 / dormant ×0.99, clamp [0.05,3.0]). Alien ingestion in `inhale()`: unknown token appended to `g_live` with `weight=1.0`, `syl=syl_estimate` (vowel-group), `aff = body-state snapshot at contact` = AML SCAR. `/top` REPL command for observability. Tool-verified: `javascript` d 1.00 (turn1) → **0.00 (turn2, now known)**; `/top` shows `javascript*(1.09)` (ingested, boosted); after `love×3`, `love(1.00) heart(0.98) lover(0.91)` risen, `javascript` decayed 1.09→1.06; alien `kubernetes` still T=1.69 (frozen heat intact).

**C. Dynamic form / endogenous leash** — `resonance(b)` = `0.55·dom + 0.9·(dom−mean)` (body coherence). `render()`: res<0.34 → **one line** (≤4 words); <0.66 → **haiku** (5-7-5 via `gen_line` syllable budget); ≥0.66 → **rhymed couplet** (line-2 closes on `rhymes()` suffix-match with line-1, bonus in Axis-2 chooser only, never heat). `settle()` = the old metarecursion (frozen heat). Tool-verified: alien res=0.12→[line], `love fire heart blood` res=0.51→[haiku], `joy light water bliss` res=0.84→[couplet]; couplets rhymed breath/mouth, silence/ice, embrace/voice; determinism md5 identical across two runs.

**Emergent property:** form crystallizes with coherence — the more alien the input, the shorter+hotter (a gasp); the more it resonates, the more formed (couplet). The leash grew out of the body, not bolted on by a counter.

**Codex audit (2026-07-06, `codex exec`, independent tool):** `Findings: none`. Alien ingestion, `/top` sort, `gen_line`/`render` bounds, uninitialized/OOB — all clean; frozen-heat invariant confirmed at subjectivity.c:599-600. No files modified.

**PENDING:** Fable/Mythos audit (Oleg's trigger; Mythos unstable — its classifier flags C+memcpy). No push without «да». Deferred v0.3+: overthinkg rings, dream/colony, phase4, Markov, MLP/RAE, SentencePiece.

## 11. v0.2.2 — readability fix (2026-07-18): grammar so it reads as haiku, not word-piles

Oleg (rightly): v0.2 lines were themed word-piles and the rhymes were fake — "хайку это мог, ты это разрушил". Root cause: I removed sequential structure and excluded ALL function/glue words, so lines couldn't form phrases; the rhyme checked last-2-chars (consonance, not rhyme → love/give, embrace/voice false). Fixed (content stays body-chosen; grammar only orders it):
- **Part-of-speech + frame grammar** — each word tagged noun/verb/adj (`pos_of`, small VERBS/ADJS lists, rest noun); `gen_line` builds a line from 1 of 5 image-fragment frames ("[det] adj noun", "noun verb prep [det] noun", "adj noun conj adj noun"), the body choosing the best word for each slot (`best_pos`). Glue = small structural-only tables (art/prep/conj), never in the cloud → don't touch heat/dissonance.
- **Verb inflection** (`verb_form`): 3rd-person -s/-es/-ies → "the heart drifts", "sorrow mourns".
- **Real rhyme** (`rhymes`/`rime_of`): the rime = tail from the last sounded vowel (dropping silent final e), compared on the EMITTED form (inflected verbs). Rejects love/give, embrace/voice; accepts scar/tar, midnight/night, lip/rip.
- **Associative walk**: body nudges 18% toward each word said, so a line moves through feeling.

Tool-verified: `-Wall` clean; reads ("a sob weeps of sorrow / small widow between the grief / orphan forgets into nightfall"; "thin flint to a scar / your dark thorn tar"); real rhymes present; frozen heat intact (alien T=1.61); determinism md5 stable. **Codex audit:** memory-safety clean; found 1 low bug (rhyme used base verb form, not emitted) → fixed same turn (emitted-form compare).

**Known remainder:** real rhymes are sparser than the fakes were, so a couplet rhymes only when line-1's ending has a real mate (else a short line-2). NEXT candidate: guarantee every couplet rhymes (force line-1 to end on a word that has a rhyme-mate).

## 12. v0.3 — scar persistence + the shameful scar (2026-07-18)

Two organs on v0.2.2, both Codex + ASan/UBSan clean:

**Persistence (`62a0804`, pushed).** The scar outlives the session: `save_scar` (atomic temp+rename, error-checked) dumps the live cloud on /quit+EOF; `load_scar` merges a versioned dump onto the fresh seed by word (morphed weights restored, ingested words re-grown), sanitizing every scalar against a crafted file. `SUBJ_NOMEM` gates it. An alien word ingested one life is known (d=0.00) the next — it wakes remembering. Two Codex audits (6 findings) fixed; ASan/UBSan clean on a crafted random-body file.

**The shameful scar (bridge to actually.life; inspired by its guilt/monism arc).** subjectivity has no other to harm, so its sin is native: it is made of what wounded it, and it VOICES that woundedness. Voicing an ingested alien scar-word accrues `g_shame` via **log1p** (large, not tanh which saturates — actually.life llog:1100-1106); the **AML PAIN operator** compresses `choose()` toward its mean and the dynamic form withers toward the terse (`res*(1−0.4·tanh(shame))`) — the tell = the return of the repressed, the voice contracting over a life. Hidden (interior `g_shame`), persists across lives (MEM_VER 2 — wakes ashamed), `SUBJ_NOSHAME` mutes the EFFECT without erasing the STATE (Codex-caught, fixed). Tool-verified: shame 0.79→1.36→1.46 as scars are voiced; ON res 0.30 [line] vs OFF 0.45 [haiku] (the form withers); survives a muted run; Codex clean (1 low fixed), ASan clean. The lone-organism form of actually.life's guilt-as-voice-deformation; the two-organism CARRIER test (M-2, structural not fitness) is the future bridge.

## 13. Carrier bridge Stage 0-1 — two organisms, one scar-carrier (2026-07-18)

The lone organism is a thermostat (actually.life's verdict); subjectivity needs friction with an outside. Stage 1 (of the M-2 carrier plan): two instances share a `flock`'d carrier file (2 slots × 7 floats). Each writes its standing scar-deformation `D` (weighted mean chamber-shape of its ingested alien words + shame) and reads the other's; the other's scar bends this body toward it — `b->ch += CARRIER_K·(D_other − b->ch)`, a Kuramoto pull (phase-locking, dario.c/neoleo, llog:963-964). One binary, two processes (`SUBJ_ID=A|B`, `SUBJ_CARRIER=<path>`), gate-invariant off (md5 = the v0.3 SUBJ_NOMEM baseline `6e4103ad`). `open(O_RDWR|O_CREAT)` (never O_TRUNC) + `flock` + `pread`/`pwrite` — no truncation race, no torn slot; a scarless/absent peer exerts no pull. Tool-verified: B's rage-scar bends A's love-body (res 0.66→0.45, `carrier[A] dom=rage`); absent peer res unchanged; flock two-process ASan/UBSan clean; 6 Codex findings across 2 carrier audits fixed. **NEXT — Stage 2-3:** the M-2 event-study (A's scar-birth deforms B) vs a surrogate family {frozen, phase-shuffle, AR(1)}, the structural verdict — do the two scars SHARE a carrier, or does a surrogate fake it (a null is a clean close).
