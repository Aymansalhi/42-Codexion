#!/bin/bash
###############################################################################
# test_codexion.sh
#
# Black-box validation suite for the Codexion 42 project. Every check below
# ties back to a specific rule in the subject (referenced inline). Run as:
#
#   ./test_codexion.sh ./codexion
#
# Logs for every run are kept in ./test_logs/<case>.log for manual review
# during a peer-evaluation defense.
###############################################################################

BIN="${1:-./codexion}"
LOGDIR="./test_logs"
mkdir -p "$LOGDIR"

RED='\033[0;31m'; GRN='\033[0;32m'; YEL='\033[1;33m'; NC='\033[0m'

PASS=0
FAIL=0
WARN=0

pass() { echo -e "${GRN}[PASS]${NC} $1"; PASS=$((PASS+1)); }
fail() { echo -e "${RED}[FAIL]${NC} $1"; FAIL=$((FAIL+1)); }
warn() { echo -e "${YEL}[WARN]${NC} $1"; WARN=$((WARN+1)); }

HAVE_PY=0
command -v python3 >/dev/null 2>&1 && HAVE_PY=1

###############################################################################
# SECTION 0 — Build requirements
# Subject: "Your Makefile must contain at least the rules $(NAME), all, clean,
# fclean and re." / "Your code must compile with -Wall -Wextra -Werror -pthread."
###############################################################################
echo "== Section 0: Build =="

if [ -f "Makefile" ]; then
    for rule in "NAME" "all" "clean" "fclean" "re"; do
        if grep -qE "^${rule}[[:space:]]*:" Makefile || grep -qE "^\\\$\\(${rule}\\)" Makefile; then
            pass "build: Makefile has a '$rule' rule"
        else
            if [ "$rule" = "NAME" ] && grep -qE "^NAME[[:space:]]*=" Makefile; then
                pass "build: Makefile defines NAME variable"
            else
                fail "build: Makefile missing '$rule' rule/target"
            fi
        fi
    done

    build_log="$LOGDIR/build_re.log"
    make re > "$build_log" 2>&1
    build_rc=$?
    if [ "$build_rc" -ne 0 ]; then
        fail "build: 'make re' failed (rc=$build_rc) -> $build_log"
    elif grep -qiE "warning:" "$build_log"; then
        fail "build: compiler warnings present despite -Werror expectation -> $build_log"
    else
        pass "build: 'make re' succeeded cleanly"
    fi
else
    warn "build: no Makefile found in current directory, skipping build checks"
fi

if [ ! -x "$BIN" ]; then
    echo -e "${RED}FATAL: binary '$BIN' not found or not executable. Aborting.${NC}"
    exit 1
fi

###############################################################################
# SECTION 1 — Argument validation
# Subject: "All arguments are mandatory. Reject invalid inputs such as
# negative numbers, non-integers, or a scheduler other than fifo or edf."
###############################################################################
echo ""
echo "== Section 1: Argument validation =="

check_rejects() {
    local name="$1"; shift
    local log="$LOGDIR/parse_${name}.log"
    timeout 3 "$BIN" "$@" > "$log" 2>&1
    local rc=$?
    if [ "$rc" -eq 124 ]; then
        fail "parse: $name HUNG instead of rejecting (rc=124) -> $log"
    elif [ "$rc" -eq 0 ]; then
        fail "parse: $name was accepted (rc=0) but should have been rejected -> $log"
    else
        pass "parse: $name correctly rejected (rc=$rc)"
    fi
}

check_accepts() {
    local name="$1" budget="$2"; shift 2
    local log="$LOGDIR/parse_${name}.log"
    timeout "$budget" "$BIN" "$@" > "$log" 2>&1
    local rc=$?
    if [ "$rc" -eq 124 ]; then
        fail "parse: $name hung on a value that should be valid -> $log"
    else
        pass "parse: $name accepted, ran to completion (rc=$rc)"
    fi
}

check_rejects no_args
check_rejects too_few_args 3 3 3
check_rejects too_many_args 3 3 3 3 3 3 3 fifo extra
check_rejects missing_scheduler 3 3 3 3 3 3 3

check_rejects bad_scheduler_typo 3 3 3 3 3 3 3 fif
check_rejects bad_scheduler_case 3 3 3 3 3 3 3 FIFO
check_rejects bad_scheduler_word 3 3 3 3 3 3 3 random

check_rejects negative_coders -1 3 3 3 3 3 3 fifo
check_rejects negative_burnout 3 -1 3 3 3 3 3 fifo
check_rejects negative_compile 3 3 -1 3 3 3 3 fifo
check_rejects negative_debug 3 3 3 -1 3 3 3 fifo
check_rejects negative_refactor 3 3 3 3 -1 3 3 fifo
check_rejects negative_required 3 3 3 3 3 -1 3 fifo
check_rejects negative_cooldown 3 3 3 3 3 3 -1 fifo

check_rejects non_numeric 3 a 3 3 3 3 3 fifo
check_rejects float_arg 3 3.5 3 3 3 3 3 fifo
check_rejects empty_arg 3 "" 3 3 3 3 3 fifo
check_rejects whitespace_arg 3 " " 3 3 3 3 3 fifo

check_rejects zero_coders 0 3 3 3 3 3 3 fifo

# Subject doesn't explicitly say every field must reject 0 — verify behavior
# (no hang/crash) rather than assume a hard rejection is required.
check_field_zero_no_hang() {
    local name="$1"; shift
    local log="$LOGDIR/zerofield_${name}.log"
    timeout 3 "$BIN" "$@" > "$log" 2>&1
    local rc=$?
    if [ "$rc" -eq 124 ]; then
        fail "parse: $name (value 0) HUNG -> $log"
    elif [ "$rc" -eq 139 ] || [ "$rc" -eq 134 ]; then
        fail "parse: $name (value 0) crashed (rc=$rc) -> $log"
    else
        pass "parse: $name (value 0) handled without hang/crash (rc=$rc)"
    fi
}
check_field_zero_no_hang debug_zero 3 3000 300 0 300 2 300 fifo
check_field_zero_no_hang refactor_zero 3 3000 300 300 0 2 300 fifo
check_field_zero_no_hang cooldown_zero 3 3000 300 300 300 2 0 fifo

check_accepts valid_fifo_multi 15 3 300 300 300 300 2 300 fifo
check_accepts valid_edf_multi 15 3 300 300 300 300 2 300 edf
check_accepts valid_single_coder 10 1 5000 500 500 500 2 500 fifo

###############################################################################
# SECTION 2 — Topology (single coder = one dongle, no self-deadlock)
# Subject: "There is one dongle between each pair of coders... If there is
# only one coder, there should be only one dongle on the table."
###############################################################################
echo ""
echo "== Section 2: Topology (single-coder dongle count) =="

topo_log="$LOGDIR/topology_single_coder.log"
timeout 10 "$BIN" 1 5000 500 500 500 2 500 fifo > "$topo_log" 2>&1
topo_rc=$?

if [ "$topo_rc" -eq 124 ]; then
    fail "topology: single-coder run HUNG (likely self-deadlock on shared dongle) -> $topo_log"
else
    takes_before_first_compile=$(awk '
        /has taken a dongle/ { t++ }
        /is compiling/ { print t; exit }
    ' "$topo_log")
    if [ "$takes_before_first_compile" = "1" ]; then
        pass "topology: single coder takes exactly ONE dongle before compiling"
    elif [ -z "$takes_before_first_compile" ]; then
        warn "topology: could not find a completed take->compile sequence to check -> $topo_log"
    else
        fail "topology: single coder took $takes_before_first_compile dongle(s) before compiling, expected 1 -> $topo_log"
    fi
fi

###############################################################################
# SECTION 3 — Log format compliance
# Subject: exact per-line format for each state change; "A displayed state
# message should not be mixed up with another message."
###############################################################################
echo ""
echo "== Section 3: Log format compliance =="

check_log_format() {
    local log="$1" label="$2"
    [ -f "$log" ] || { warn "format ($label): no log to check"; return; }
    local bad
    bad=$(grep -vE "^[0-9]+ [0-9]+ (has taken a dongle|has released a dongle|is compiling|is debugging|is refactoring|burned out)$" "$log" \
          | grep -v "^All memory cleaned" \
          | grep -v "^$")
    if [ -n "$bad" ]; then
        fail "format ($label): non-conforming line(s) found, e.g.:"
        echo "$bad" | head -3 | sed 's/^/       /'
    else
        pass "format ($label): all lines match required spec format"
    fi
}
check_log_format "$LOGDIR/parse_valid_fifo_multi.log" "fifo multi-coder run"
check_log_format "$LOGDIR/parse_valid_edf_multi.log" "edf multi-coder run"
check_log_format "$topo_log" "single-coder run"

###############################################################################
# SECTION 4 — Dongle acquisition sequencing before "is compiling"
# Subject: "Compiling quantum code requires two dongles plugged in
# simultaneously... a coder takes their left and right dongles to compile."
###############################################################################
echo ""
echo "== Section 4: Dongle sequencing before compile =="

check_dongle_sequencing() {
    local log="$1" label="$2" expected_takes="$3"
    [ -f "$log" ] || { warn "sequencing ($label): no log to check"; return; }
    if [ "$HAVE_PY" -ne 1 ]; then
        warn "sequencing ($label): python3 unavailable, skipping detailed check"
        return
    fi
    python3 - "$log" "$expected_takes" <<'PYEOF'
import sys, re
log, expected = sys.argv[1], int(sys.argv[2])
lines = open(log).read().splitlines()
pending_takes = {}
violations = 0
checked = 0
for line in lines:
    m = re.match(r"(\d+) (\d+) has taken a dongle", line)
    c = re.match(r"(\d+) (\d+) is compiling", line)
    if m:
        cid = m.group(2)
        pending_takes[cid] = pending_takes.get(cid, 0) + 1
    if c:
        cid = c.group(2)
        checked += 1
        if pending_takes.get(cid, 0) != expected:
            violations += 1
        pending_takes[cid] = 0
if checked == 0:
    print("NODATA")
elif violations > 0:
    print(f"FAIL:{violations}/{checked}")
else:
    print(f"OK:{checked}")
PYEOF
}

result=$(check_dongle_sequencing "$LOGDIR/parse_valid_fifo_multi.log" "fifo multi" 2)
case "$result" in
    OK:*) pass "sequencing: fifo multi-coder — every compile preceded by 2 takes (${result#OK:} checked)";;
    FAIL:*) fail "sequencing: fifo multi-coder — ${result#FAIL:} compiles had wrong take count";;
    NODATA) warn "sequencing: fifo multi-coder — no compile events found to check";;
    *) warn "sequencing: fifo multi-coder — check skipped";;
esac

result=$(check_dongle_sequencing "$topo_log" "single coder" 1)
case "$result" in
    OK:*) pass "sequencing: single coder — every compile preceded by 1 take (${result#OK:} checked)";;
    FAIL:*) fail "sequencing: single coder — ${result#FAIL:} compiles had wrong take count";;
    NODATA) warn "sequencing: single coder — no compile events found to check";;
    *) warn "sequencing: single coder — check skipped";;
esac

###############################################################################
# SECTION 5 — Burnout precision (within 10ms) and full simulation stop
# Subject: burnout log within 10ms of actual burnout; burnout must stop the
# WHOLE simulation, not just the burned-out coder.
###############################################################################
echo ""
echo "== Section 5: Burnout precision + simulation-wide stop =="

burnout_log="$LOGDIR/burnout_forced.log"
timeout 15 "$BIN" 3 400 3000 500 500 5 3000 fifo > "$burnout_log" 2>&1

if ! grep -q "burned out" "$burnout_log"; then
    warn "burnout: scenario did not trigger a burnout — cannot verify precision/stop here (adjust params if needed)"
else
    warn "burnout: exact 10ms precision cannot be fully verified black-box without per-coder deadline instrumentation — spot-check $burnout_log manually against last compile-start timestamps"

    burnout_line_num=$(grep -n "burned out" "$burnout_log" | head -1 | cut -d: -f1)
    after_lines=$(tail -n +$((burnout_line_num+1)) "$burnout_log" | grep -cE "has taken a dongle|is compiling|is debugging|is refactoring")
    if [ "$after_lines" -gt 0 ]; then
        fail "burnout: $after_lines activity line(s) logged AFTER burnout — simulation did not stop fully -> $burnout_log"
    else
        pass "burnout: no further coder activity logged after burnout"
    fi

    exit_wait_log="$LOGDIR/burnout_exit_check.log"
    timeout 8 "$BIN" 3 400 3000 500 500 5 3000 fifo > "$exit_wait_log" 2>&1
    exit_rc=$?
    if [ "$exit_rc" -eq 124 ]; then
        fail "burnout: process did not exit promptly after burnout (threads not joined?) -> $exit_wait_log"
    else
        pass "burnout: process exited promptly after burnout (rc=$exit_rc)"
    fi
fi

###############################################################################
# SECTION 6 — Dongle cooldown enforcement
# Subject: dongle cannot be retaken until dongle_cooldown ms after release.
###############################################################################
echo ""
echo "== Section 6: Dongle cooldown =="
echo "# LIMITATION: the required log format does not identify WHICH physical"
echo "# dongle is taken/released, only which coder acted. We approximate by"
echo "# tracking each coder's own take/release timestamps as a proxy for the"
echo "# dongles it uses — this is not a fully precise per-dongle check."

cooldown_log="$LOGDIR/cooldown_check.log"
COOLDOWN_VAL=600
timeout 20 "$BIN" 1 30000 500 200 200 4 "$COOLDOWN_VAL" fifo > "$cooldown_log" 2>&1

if [ "$HAVE_PY" -eq 1 ]; then
    py_result=$(python3 - "$cooldown_log" "$COOLDOWN_VAL" <<'PYEOF'
import sys, re
log, cooldown = sys.argv[1], int(sys.argv[2])
events = []
for line in open(log):
    r = re.match(r"(\d+) (\d+) has released a dongle", line)
    t = re.match(r"(\d+) (\d+) has taken a dongle", line)
    if r:
        events.append(("release", int(r.group(1)), r.group(2)))
    elif t:
        events.append(("take", int(t.group(1)), t.group(2)))
if not events:
    print("NODATA"); sys.exit(0)
if not any(e[0]=="release" for e in events):
    print("NORELEASE"); sys.exit(0)
last_release = {}
violations = 0
for kind, ts, cid in events:
    if kind == "release":
        last_release[cid] = ts
    else:
        if cid in last_release and (ts - last_release[cid]) < cooldown:
            violations += 1
print(f"VIOLATIONS:{violations}" if violations else "OK")
PYEOF
)
    case "$py_result" in
        OK) pass "cooldown: no release->retake gap under ${COOLDOWN_VAL}ms found (proxy check)";;
        VIOLATIONS:*) fail "cooldown: ${py_result#VIOLATIONS:} gap(s) under ${COOLDOWN_VAL}ms found -> $cooldown_log";;
        NORELEASE) warn "cooldown: log has no 'has released a dongle' lines — cannot verify cooldown timing from this format -> $cooldown_log";;
        NODATA) warn "cooldown: no take/release events captured -> $cooldown_log";;
        *) warn "cooldown: check produced no result, review manually -> $cooldown_log";;
    esac
else
    warn "cooldown: python3 unavailable, skipping automated timestamp check -> $cooldown_log"
fi

###############################################################################
# SECTION 7 — Compile-count cap and clean stop condition
# Subject: simulation stops once every coder reaches number_of_compiles_required;
# no coder should exceed it; clean exit (rc=0) with no trailing output.
###############################################################################
echo ""
echo "== Section 7: Compile-count cap + clean stop =="

REQUIRED=3
compile_cap_log="$LOGDIR/compile_cap.log"
timeout 20 "$BIN" 3 20000 300 150 150 "$REQUIRED" 200 fifo > "$compile_cap_log" 2>&1
cap_rc=$?

max_compiles=$(grep -oE "^[0-9]+ [0-9]+ is compiling" "$compile_cap_log" \
                | awk '{print $2}' | sort | uniq -c | awk '{print $1}' | sort -n | tail -1)

if [ -z "$max_compiles" ]; then
    warn "compile_cap: no 'is compiling' lines found, cannot verify cap -> $compile_cap_log"
elif [ "$max_compiles" -gt "$REQUIRED" ]; then
    fail "compile_cap: a coder compiled $max_compiles times, exceeds required=$REQUIRED -> $compile_cap_log"
else
    pass "compile_cap: max compiles per coder ($max_compiles) within required=$REQUIRED"
fi

if grep -q "burned out" "$compile_cap_log"; then
    warn "compile_cap: burnout occurred in this scenario instead of a clean compile-count stop — params may be too tight, adjust and rerun if you want to isolate this path"
else
    if [ "$cap_rc" -eq 0 ]; then
        pass "compile_cap: process exited cleanly (rc=0) after reaching required compiles"
    elif [ "$cap_rc" -eq 124 ]; then
        fail "compile_cap: process HUNG instead of stopping after required compiles -> $compile_cap_log"
    else
        fail "compile_cap: process exited with non-zero rc=$cap_rc on the no-burnout path -> $compile_cap_log"
    fi
fi

###############################################################################
# SECTION 8 — EDF priority ordering under contention (best-effort black-box)
# Subject: EDF must serve the coder with the earliest deadline first.
###############################################################################
echo ""
echo "== Section 8: EDF ordering under contention =="
echo "# LIMITATION: without internal deadline instrumentation, we cannot"
echo "# directly observe 'who was waiting' at the instant a dongle freed up."
echo "# This is a best-effort scenario check, not a proof."

edf_log="$LOGDIR/edf_contention.log"
timeout 15 "$BIN" 4 5000 500 200 200 3 400 edf > "$edf_log" 2>&1
edf_rc=$?

if [ "$edf_rc" -eq 124 ]; then
    fail "edf: contention scenario HUNG -> $edf_log"
elif grep -q "burned out" "$edf_log"; then
    warn "edf: a burnout occurred under edf with these params — either the scenario is infeasible as configured, or edf failed to prevent starvation; manually review $edf_log against the liveness guarantee"
else
    pass "edf: contention scenario completed without hang or burnout (liveness holds for this scenario)"
fi
warn "edf: fine-grained 'earliest deadline served first' ordering was NOT independently verified — requires white-box/instrumented testing or reading the scheduler source directly"

###############################################################################
# SECTION 9 — Memory safety (ASan) across a spread of scenarios
# Subject: no leaks; no segfault/bus error/double free/UB crashes.
###############################################################################
echo ""
echo "== Section 9: Memory safety (ASan) =="

check_clean_run() {
    local name="$1" budget="$2"; shift 2
    local log="$LOGDIR/asan_${name}.log"
    timeout "$budget" "$BIN" "$@" > "$log" 2>&1
    local rc=$?
    if [ "$rc" -eq 124 ]; then
        fail "asan: $name HUNG, cannot confirm clean exit/free -> $log"
        return
    fi
    if grep -qE "ERROR: (Leak|AddressSanitizer)|SUMMARY: AddressSanitizer|heap-use-after-free|heap-buffer-overflow|double-free|SEGV|stack-use-after-return|stack-buffer-overflow" "$log"; then
        fail "asan: $name triggered a sanitizer error -> $log"
    else
        pass "asan: $name clean (rc=$rc)"
    fi
}

check_clean_run single_coder 10 1 5000 500 500 500 2 500 fifo
check_clean_run many_coders 15 8 5000 300 300 300 2 300 fifo
check_clean_run many_coders_edf 15 8 5000 300 300 300 2 300 edf
check_clean_run forced_burnout 10 3 400 3000 500 500 5 3000 fifo
check_clean_run zero_debug_refactor 10 3 5000 500 0 0 2 500 fifo
check_clean_run invalid_args_early_exit 3 -1 -1 -1 -1 -1 -1 -1 fifo
check_clean_run high_contention 15 6 2000 200 100 100 3 150 fifo

###############################################################################
# SECTION 10 — Hang / deadlock detection (dedicated pass)
# Focus: number_of_coders=1 (self-deadlock risk) and high-contention
# short-cooldown/short-burnout scenarios.
###############################################################################
echo ""
echo "== Section 10: Hang / deadlock detection =="

check_no_hang() {
    local name="$1" budget="$2"; shift 2
    local log="$LOGDIR/hang_${name}.log"
    timeout "$budget" "$BIN" "$@" > "$log" 2>&1
    local rc=$?
    if [ "$rc" -eq 124 ]; then
        fail "hang: $name did NOT exit within ${budget}s -> $log"
    else
        pass "hang: $name exited within ${budget}s (rc=$rc)"
    fi
}

check_no_hang single_coder_deadlock_risk 10 1 5000 500 500 500 3 500 fifo
check_no_hang single_coder_edf 10 1 5000 500 500 500 3 500 edf
check_no_hang high_contention_short_cooldown 15 10 3000 200 100 100 3 100 fifo
check_no_hang high_contention_short_cooldown_edf 15 10 3000 200 100 100 3 100 edf
check_no_hang burnout_should_exit_fast 6 3 200 3000 500 500 2 3000 fifo

###############################################################################
# Summary
###############################################################################
echo ""
echo "==================================================="
echo "PASS: $PASS   FAIL: $FAIL   WARN: $WARN"
echo "Logs saved in: $LOGDIR/"
echo "==================================================="

[ "$FAIL" -eq 0 ] && exit 0 || exit 1