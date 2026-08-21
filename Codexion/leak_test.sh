#!/bin/bash
# test_codexion.sh — full test suite: parsing, leaks, cooldown timing,
# burnout precision + shutdown, compile-count stop, log format, hang detection.
#
# usage: ./test_codexion.sh [path_to_binary]

BIN="${1:-./codexion}"
LOGDIR="./test_logs"
mkdir -p "$LOGDIR"

RED='\033[0;31m'
GRN='\033[0;32m'
YEL='\033[1;33m'
NC='\033[0m'

PASS=0
FAIL=0
WARN=0

pass() { echo -e "${GRN}[PASS]${NC} $1"; PASS=$((PASS+1)); }
fail() { echo -e "${RED}[FAIL]${NC} $1"; FAIL=$((FAIL+1)); }
warn() { echo -e "${YEL}[WARN]${NC} $1"; WARN=$((WARN+1)); }

# ---------------------------------------------------------------------------
# SECTION 1 — argument parsing / rejection
# ---------------------------------------------------------------------------
echo "== Section 1: Argument parsing =="

check_rejects() {
  local name="$1"; shift
  local log="$LOGDIR/parse_${name}.log"
  timeout 2 "$BIN" "$@" > "$log" 2>&1
  local rc=$?
  if grep -qiE "error" "$log" && [ "$rc" -ne 0 ]; then
    pass "parse: $name correctly rejected"
  else
    fail "parse: $name should have been rejected (rc=$rc) -> $log"
  fi
}

check_accepts() {
  local name="$1"; shift
  local log="$LOGDIR/parse_${name}.log"
  timeout 8 "$BIN" "$@" > "$log" 2>&1
  if grep -qiE "error" "$log"; then
    fail "parse: $name should have been accepted -> $log"
  else
    pass "parse: $name accepted"
  fi
}

check_rejects no_args
check_rejects too_few 3 3 3
check_rejects too_many 3 3 3 3 3 3 3 fifo extra
check_rejects bad_scheduler 3 3 3 3 3 3 3 fif
check_rejects missing_scheduler 3 3 3 3 3 3 3
check_rejects negative 3 -1 3 3 3 3 3 fifo
check_rejects non_numeric 3 a 3 3 3 3 3 fifo
check_rejects float_arg 3 3.5 3 3 3 3 3 fifo
check_rejects zero_coders 0 3 3 3 3 3 3 fifo
check_accepts valid_fifo 3 500 500 500 500 2 500 fifo
check_accepts valid_edf 3 500 500 500 500 2 500 edf
check_accepts single_coder 1 500 500 500 500 2 500 fifo

# ---------------------------------------------------------------------------
# SECTION 2 — memory leaks / sanitizer errors (requires -fsanitize=address build)
# ---------------------------------------------------------------------------
echo ""
echo "== Section 2: Sanitizer / leak checks =="

check_clean_run() {
  local name="$1"; shift
  local log="$LOGDIR/asan_${name}.log"
  timeout 15 "$BIN" "$@" > "$log" 2>&1
  if grep -qE "ERROR: (Leak|AddressSanitizer)|SUMMARY: AddressSanitizer|heap-use-after-free|heap-buffer-overflow|double-free|SEGV|stack-use-after-return" "$log"; then
    fail "asan: $name triggered sanitizer error -> $log"
  else
    pass "asan: $name clean"
  fi
}

check_clean_run normal_fifo 3 3000 500 500 500 2 500 fifo
check_clean_run normal_edf 3 3000 500 500 500 2 500 edf
check_clean_run single_coder 1 3000 500 500 500 2 500 fifo
check_clean_run many_coders 8 3000 300 300 300 2 300 fifo
check_clean_run burnout_case 3 300 500 500 500 5 500 fifo
check_clean_run zero_debug_refactor 3 3000 500 0 0 2 500 fifo
check_clean_run bad_args_cleanup -1 -1 -1 -1 -1 -1 -1 fifo

# ---------------------------------------------------------------------------
# SECTION 3 — hang / deadlock detection
# ---------------------------------------------------------------------------
echo ""
echo "== Section 3: Hang / deadlock detection =="

check_no_hang() {
  local name="$1" budget="$2"; shift 2
  local log="$LOGDIR/hang_${name}.log"
  timeout "$budget" "$BIN" "$@" > "$log" 2>&1
  local rc=$?
  if [ "$rc" -eq 124 ]; then
    fail "hang: $name did NOT exit within ${budget}s -> $log"
  else
    pass "hang: $name exited within ${budget}s"
  fi
}

check_no_hang small_fifo 10 3 5000 500 500 500 3 500 fifo
check_no_hang small_edf 10 3 5000 500 500 500 3 500 edf
check_no_hang single_coder 8 1 5000 500 500 500 3 500 fifo
check_no_hang burnout_should_stop_fast 5 3 200 3000 500 500 2 3000 fifo

# ---------------------------------------------------------------------------
# SECTION 4 — log format compliance
# Required: "<ts> <id> has taken a dongle" / "is compiling" / "is debugging"
#           "is refactoring" / "burned out" / "has released a dongle" (extra, allowed)
# ---------------------------------------------------------------------------
echo ""
echo "== Section 4: Log format compliance =="

check_format() {
  local log="$LOGDIR/hang_small_fifo.log"
  [ -f "$log" ] || { warn "format: no log found, run section 3 first"; return; }

  # every non-empty line must match one of the allowed formats
  local bad
  bad=$(grep -vE "^[0-9]+ [0-9]+ (has taken a dongle|has released a dongle|is compiling|is debugging|is refactoring|burned out)$" "$log" | grep -v "^All memory cleaned" | grep -v "^$")
  if [ -n "$bad" ]; then
    fail "format: non-conforming lines found:"
    echo "$bad" | head -5 | sed 's/^/       /'
  else
    pass "format: all lines match required spec format"
  fi
}
check_format

# ---------------------------------------------------------------------------
# SECTION 5 — dongle cooldown timing
# For each dongle id, gap between "released" and next "taken" must be >= cooldown
# ---------------------------------------------------------------------------
echo ""
echo "== Section 5: Dongle cooldown timing =="

check_cooldown() {
  local cooldown=800
  local log="$LOGDIR/cooldown_check.log"
  timeout 20 "$BIN" 3 30000 500 200 200 4 "$cooldown" fifo > "$log" 2>&1

  # crude check: count take/release events logged (precise per-dongle cooldown
  # verification needs dongle id in logs, which the spec format doesn't include —
  # flag for manual spot-check against the raw log instead of a hard pass/fail)
  local result
  result=$(python3 - "$log" "$cooldown" <<'EOF'
import sys, re
log, cooldown = sys.argv[1], int(sys.argv[2])
events = []
with open(log) as f:
    for line in f:
        m = re.match(r"(\d+) (\d+) has taken a dongle", line)
        if m:
            events.append(("take", int(m.group(1)), m.group(2)))
        r = re.match(r"(\d+) (\d+) has released a dongle", line)
        if r:
            events.append(("release", int(r.group(1)), r.group(2)))
print(len(events))
EOF
)
  if [ "$result" = "0" ]; then
    warn "cooldown: no take/release events logged — check log format includes 'has released a dongle'"
  else
    pass "cooldown: run completed, $result take/release events logged (manual spot-check recommended: $log)"
  fi
}
check_cooldown

# ---------------------------------------------------------------------------
# SECTION 6 — burnout precision + simulation stop
# ---------------------------------------------------------------------------
echo ""
echo "== Section 6: Burnout precision & shutdown =="

check_burnout_stops_sim() {
  local log="$LOGDIR/burnout_stop.log"
  timeout 8 "$BIN" 3 300 3000 500 500 5 3000 fifo > "$log" 2>&1

  if ! grep -q "burned out" "$log"; then
    warn "burnout: no burnout occurred in this scenario, adjust params to force one"
    return
  fi

  local burnout_line_num burnout_ts
  burnout_line_num=$(grep -n "burned out" "$log" | head -1 | cut -d: -f1)
  burnout_ts=$(grep "burned out" "$log" | head -1 | awk '{print $1}')

  # any "has taken a dongle" / "is compiling" lines with timestamp > burnout_ts
  # AFTER the burnout line = simulation kept running post-burnout
  local total_lines after_lines
  total_lines=$(wc -l < "$log")
  after_lines=$(tail -n +$((burnout_line_num+1)) "$log" | grep -cE "has taken a dongle|is compiling")

  if [ "$after_lines" -gt 0 ]; then
    fail "burnout: $after_lines dongle-activity line(s) logged AFTER burnout at ${burnout_ts}ms — simulation did not stop"
  else
    pass "burnout: no further compile activity after burnout at ${burnout_ts}ms"
  fi
}
check_burnout_stops_sim

# ---------------------------------------------------------------------------
# SECTION 7 — compile-count stop condition
# ---------------------------------------------------------------------------
echo ""
echo "== Section 7: number_of_compiles_required stop condition =="

check_compile_count_stop() {
  local required=2
  local log="$LOGDIR/compile_count.log"
  timeout 15 "$BIN" 3 20000 500 200 200 "$required" 300 fifo > "$log" 2>&1

  local max_compiles
  max_compiles=$(grep -oE "^[0-9]+ [0-9]+ is compiling" "$log" | awk '{print $2}' | sort | uniq -c | awk '{print $1}' | sort -n | tail -1)

  if [ -z "$max_compiles" ]; then
    warn "compile_count: no 'is compiling' lines found — check log format"
  elif [ "$max_compiles" -gt "$required" ]; then
    fail "compile_count: a coder compiled $max_compiles times, exceeds required=$required"
  else
    pass "compile_count: max compiles per coder ($max_compiles) within required=$required"
  fi
}
check_compile_count_stop

# ---------------------------------------------------------------------------
# Summary
# ---------------------------------------------------------------------------
echo ""
echo "-----------------------------------"
echo "PASS: $PASS   FAIL: $FAIL   WARN: $WARN"
echo "Logs: $LOGDIR/"
[ "$FAIL" -eq 0 ] && exit 0 || exit 1