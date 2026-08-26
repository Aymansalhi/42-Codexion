# 🧵 Codexion — Code Audit Report

> Internal review of outstanding issues before evaluation/defense.
> Findings are grouped by severity. Fix **Level 1** items first — these are
> the ones most likely to zero the project outright per the subject's rules.

---

## 🔴 Level 1 — Critical (must fix before evaluation) 

### 1. Required priority queue (heap) is not implemented        SOLVED YESSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
The subject mandates a **heap-based** priority queue for both `fifo` and `edf`
scheduling — no standard library priority queue allowed. The current
implementation uses linked structures and linear scans instead:

- FIFO linked queue → `fifo_queue.c:14`
- EDF linked list + scan → `edf_queue.c:57`

**Why it matters:** this isn't a style preference — it's a literal mandatory
requirement. An evaluator reading the source against the subject can flag
this regardless of whether scheduling *behaves* correctly at runtime.

**Fix:** replace both with a single binary min-heap (array-backed,
sift-up/sift-down), keyed by a swappable comparator — arrival timestamp for
`fifo`, deadline for `edf`. One structure, one lock, two comparators.

---

### 2. EDF selection logic is reversed for burnout priority  SOLVED YESSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS
Earliest deadline = **smallest** value of `last_compile_start + time_to_burnout`.
The current comparator picks the **larger** `last_compile_start` using `>`,
which selects the *later* deadline — the opposite of correct EDF behavior.

- `edf_queue.c:66`

**Impact:** this actively works against the liveness guarantee the subject
requires (*"no coder should be starved... under edf scheduling"*) — it
prioritizes coders who are furthest from burnout instead of closest.

**Fix:**
```c
int compare_edf(t_coder *a, t_coder *b)
{
    long deadline_a = a->last_compile_start + a->state->cfg->time_to_burnout;
    long deadline_b = b->last_compile_start + b->state->cfg->time_to_burnout;

    if (deadline_a != deadline_b)
        return (deadline_a < deadline_b);   // smaller deadline wins
    return (a->id < b->id);                 // deterministic tie-break
}
```

---

### 3. Single-coder behavior violates the resource rule
With one coder there is exactly one dongle. Current logic allows compiling
via a one-lock shortcut instead of the intended two-dongle acquisition path:

- one-lock shortcut when `left == right` → `coder_utils.c:31`
- only one take logged/set in that case → `dongel_managment.c:39`

**Impact:** a single coder can compile using only one dongle acquisition
event instead of the required two-dongle-hold semantics — likely judged
non-compliant at evaluation.

**Fix:** explicitly special-case `number_of_coders == 1` in the acquisition
function rather than letting the general two-dongle path silently collapse
into one lock/unlock.

---

### 4. Fair arbitration is not truly per-dongle scheduler control
The scheduler gate (deciding "whose turn is it") is separate from the actual
dongle acquisition. Threads pass the scheduler wait, then **busy-spin** until
both dongles happen to be available — which can break strict FIFO/EDF
ordering under contention.

- scheduler wait → `scheduler_utils.c:15`
- busy wait after scheduler → `coder_utils.c:58`

**Impact:** even with a correct comparator, a coder can be "granted" its
turn by the scheduler but then lose a dongle race to another coder who
wasn't next in line — arbitration isn't actually enforced at the resource
level.

**Fix:** the scheduler decision and the dongle grant need to be atomic —
i.e., popping from the priority queue should itself perform (or immediately
be followed under the same lock by) the dongle assignment, not a separate
spin-and-hope step.

---

## 🟠 Level 2 — Major (high risk / likely evaluation deductions)

### 1. Possible null dereference in EDF pop
If the target coder isn't found, `current` is `NULL` but gets dereferenced
anyway.
- `edf_queue.c:102`

**Fix:** guard with `if (!current) return (NULL);` before any dereference,
and make every caller handle a `NULL` return.

### 2. Cleanup destroys synchronization primitives without init guards
`pthread_mutex_destroy`/`pthread_cond_destroy` are called unconditionally in
the cleanup path — including parse-failure paths where those primitives were
never initialized.
- `exit_clean.c:67`
- `exit_clean.c:42`

**Fix:** track what was actually initialized (a counter or a set of boolean
flags in `t_state`) and only destroy what was successfully set up — same
pattern as the `dongels_initialized` fix applied earlier in this project.

### 3. Shared flags are unsynchronized (data-race risk)
`simulation_running` and `is_finished` are read/written across multiple
threads with no dedicated lock or atomic discipline:
- writes → `monitor.c:48`
- reads in worker loop → `coder.c:65`
- completion read in monitor → `monitor.c:52`

**Fix:** guard both flags with a mutex (or promote to `_Atomic` if your norm/
toolchain allows it), and make sure every read/write goes through the same
lock consistently — a race here can cause missed shutdown signals or coder
threads that never notice a burnout occurred.

### 4. Makefile flags mismatch mandatory expectation
Subject explicitly requires the build to include `-pthread`. Current flags
omit it.
- `Makefile:10`

**Fix:** add `-pthread` to your compile flags — without it, some libc/pthread
implementations link incorrectly or silently misbehave under threading.

### 5. README is not compliant with required structure/content
Missing the required first-line italic attribution format, and missing
required sections (**Blocking cases handled**, **Thread synchronization
mechanisms**). Current content is placeholder text.
- `README.md`

**Fix:** rebuild per the subject's explicit README requirements — see the
project subject's README Requirements chapter for the exact required
sections and first-line format.

---

## 🟡 Level 3 — Minor (quality / robustness gaps)

### 1. EDF tie-breaker for equal deadlines is not explicit
Subject requires a deterministic tie-break rule; no explicit policy exists
in the current EDF comparator.
- `edf_queue.c:57`

**Fix:** pick and document a rule (e.g. lower coder id wins) — see the fix
in Level 1 §2 above, which already includes this.

### 2. Typo/consistency issues in user-facing errors and naming
Not a functional blocker, but worth a cleanup pass for polish and
readability during defense.
- `codexion.h`

---

## ✅ What's already solid

- Argument count and scheduler validation implemented → `parsing.c:47`
- Cooldown timing check exists → `dongel_managment.c:22`
- Logging is mutex-serialized → `coder.c:24`
- Monitor thread exists and checks burnout regularly → `monitor.c:27`

---

## 📋 Suggested fix order

1. Heap-based priority queue (Level 1 §1) — structural, everything else
   downstream depends on this
2. EDF comparator direction (Level 1 §2) — trivial once the heap exists
3. Atomic scheduler-decision + dongle-grant (Level 1 §4)
4. Single-coder path (Level 1 §3)
5. Null-safety + init-guarded cleanup (Level 2 §1–2)
6. Shared-flag synchronization (Level 2 §3)
7. Makefile `-pthread` (Level 2 §4) — quick, do it anytime
8. README rewrite (Level 2 §5) — do last, once behavior is final so the
   "Blocking cases handled" section reflects what's actually implemented
9. Tie-breaker documentation + polish (Level 3)