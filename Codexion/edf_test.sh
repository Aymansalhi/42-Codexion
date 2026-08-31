#!/bin/bash

BIN="${1:-./codexion}"
LOGDIR="./edf_test_logs"
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

run_case() {
	local name="$1"
	shift
	local budget="$1"
	shift
	local log="$LOGDIR/${name}.log"
	timeout "$budget" "$BIN" "$@" > "$log" 2>&1
	return $?
}

check_rejects() {
	local name="$1"
	shift
	local log="$LOGDIR/parse_${name}.log"
	timeout 2 "$BIN" "$@" > "$log" 2>&1
	local rc=$?
	if grep -qiE "error" "$log" && [ "$rc" -ne 0 ]; then
		pass "parse: $name rejected"
	else
		fail "parse: $name should be rejected (rc=$rc)"
	fi
}

check_accepts() {
	local name="$1"
	shift
	local log="$LOGDIR/parse_${name}.log"
	timeout 10 "$BIN" "$@" > "$log" 2>&1
	local rc=$?
	if [ "$rc" -eq 0 ] && ! grep -qiE "error|AddressSanitizer|SEGV|LeakSanitizer" "$log"; then
		pass "parse: $name accepted"
	else
		fail "parse: $name should be accepted"
	fi
}

check_clean_run() {
	local name="$1"
	shift
	local log="$LOGDIR/${name}.log"
	timeout 15 "$BIN" "$@" > "$log" 2>&1
	local rc=$?
	if [ "$rc" -eq 0 ] && ! grep -qE "AddressSanitizer|LeakSanitizer|SEGV|heap-use-after-free|double-free|buffer-overflow" "$log"; then
		pass "run: $name clean"
	else
		fail "run: $name failed or triggered sanitizer output"
	fi
}

check_burnout_stops() {
	local log="$LOGDIR/burnout_edf.log"
	run_case "burnout_edf" 8 3 300 3000 500 500 5 3000 edf
	local rc=$?
	if [ "$rc" -ne 0 ]; then
		fail "burnout: scenario did not exit cleanly"
		return
	fi
	if ! grep -q "burned out" "$log"; then
		warn "burnout: no burnout occurred in this run"
		return
	fi
	local burnout_line
	burnout_line=$(grep -n "burned out" "$log" | head -1 | cut -d: -f1)
	local after_lines
	after_lines=$(tail -n +$((burnout_line + 1)) "$log" | grep -cE "has taken a dongle|is compiling|is debugging|is refactoring")
	if [ "$after_lines" -gt 0 ]; then
		fail "burnout: simulation kept logging activity after burnout"
	else
		pass "burnout: simulation stops after burnout"
	fi
}

echo "== EDF Codexion Test Suite =="

echo ""
echo "== Parsing =="
check_rejects no_args
check_rejects too_few 3 3 3
check_rejects too_many 3 3 3 3 3 3 3 edf extra
check_rejects bad_scheduler 3 3 3 3 3 3 3 fif
check_rejects non_numeric 3 a 3 3 3 3 3 edf
check_rejects zero_coders 0 3 3 3 3 3 3 edf
check_accepts valid_edf 3 500 500 500 500 2 500 edf
check_accepts single_coder 1 500 500 500 500 2 500 edf

echo ""
echo "== Runtime / sanitizer =="
check_clean_run normal_edf 3 3000 500 500 500 2 500 edf
check_clean_run single_edf 1 3000 500 500 500 2 500 edf
check_clean_run many_coders 8 3000 300 300 300 2 300 edf
check_clean_run fast_cycle 3 4000 200 200 200 3 200 edf

echo ""
echo "== Shutdown behavior =="
check_burnout_stops

echo ""
echo "== Summary =="
echo "PASS=$PASS FAIL=$FAIL WARN=$WARN"
if [ "$FAIL" -ne 0 ]; then
	exit 1
fi
exit 0