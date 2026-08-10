#!/bin/bash
# leak_test.sh — run codexion through a matrix of arg combos and flag any ASan leaks
# usage: ./leak_test.sh [path_to_binary]

BIN="${1:-./codexion}"
LOGDIR="./leak_logs"
mkdir -p "$LOGDIR"

RED='\033[0;31m'
GRN='\033[0;32m'
NC='\033[0m'

PASS=0
FAIL=0
i=0

# format: description|args...
# empty args = call with no args
TESTS=(
  "valid_fifo|3 3 3 3 3 3 3 fifo"
  "valid_edf|3 3 3 3 3 3 3 edf"
  "valid_min|1 1 1 1 1 1 1 fifo"
  "valid_high_coders|20 3 3 3 3 3 3 fifo"
  "no_args|"
  "too_few_args|3 3 3"
  "too_many_args|3 3 3 3 3 3 3 fifo extra"
  "bad_scheduler|3 3 3 3 3 3 3 fif"
  "bad_scheduler2|3 3 3 3 3 3 3 xyz"
  "missing_scheduler|3 3 3 3 3 3 3"
  "zero_coders|0 3 3 3 3 3 3 fifo"
  "zero_everything|0 0 0 0 0 0 0 fifo"
  "negative_arg|3 -1 3 3 3 3 3 fifo"
  "non_numeric|3 a 3 3 3 3 3 fifo"
  "float_arg|3 3.5 3 3 3 3 3 fifo"
  "huge_number|3 999999999999 3 3 3 3 3 fifo"
  "overflow_number|3 99999999999999999999 3 3 3 3 3 fifo"
  "leading_zero|03 3 3 3 3 3 3 fifo"
  "leading_plus|+3 3 3 3 3 3 3 fifo"
  "whitespace_arg|3  3 3 3 3 3 3 fifo"
  "empty_string_arg|3 '' 3 3 3 3 3 fifo"
  "one_coder|1 3 3 3 3 3 3 edf"
  "many_coders_stress|50 1 1 1 1 5 1 fifo"
)

run_case() {
  local name="$1" args="$2"
  local log="$LOGDIR/${name}.log"
  i=$((i+1))

  # eval so quoted-empty args expand correctly
  eval "$BIN $args" > "$log" 2>&1
  local rc=$?

  if grep -qE "ERROR: (Leak|AddressSanitizer)|SUMMARY: AddressSanitizer|heap-use-after-free|heap-buffer-overflow|double-free" "$log"; then
    echo -e "${RED}[LEAK]${NC} $name  (args: $args)  -> see $log"
    FAIL=$((FAIL+1))
  else
    echo -e "${GRN}[OK]${NC}   $name"
    PASS=$((PASS+1))
  fi
}

echo "Testing binary: $BIN"
echo "-----------------------------------"

for t in "${TESTS[@]}"; do
  name="${t%%|*}"
  args="${t#*|}"
  run_case "$name" "$args"
done

echo "-----------------------------------"
echo "Total: $i   Pass: $PASS   Fail: $FAIL"
echo "Logs: $LOGDIR/"

[ "$FAIL" -eq 0 ] && exit 0 || exit 1