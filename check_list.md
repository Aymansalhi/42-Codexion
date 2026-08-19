# Codexion check list

## Structure and logic
- [-] Each coder is represented by one thread
- [-] Each coder has left and right dongles when `number_of_coders > 1`
- [-] With one coder, only one dongle exists
- [-] Each dongle has its own mutex
- [-] Shared resources are protected against races
- [-] Coders cannot duplicate the same dongle
- [-] Dongle cooldown is enforced after release
- [-] Fair arbitration is implemented
- [-] FIFO policy is based on request arrival order
- [ ] EDF policy uses earliest deadline
- [ ] Tie-break rule exists for EDF determinism
- [ ] No coder starves under valid feasible conditions
- [ ] Simulation stops on burnout or after compile target is reached

## Monitor and burnout
- [ ] Separate monitor thread exists
- [ ] Monitor detects burnout precisely
- [ ] Burnout log is emitted within 10 ms of actual burnout
- [ ] Simulation is stopped when burnout occurs
- [ ] Simulation state is checked safely under mutex protection

## Logging
- [ ] Logs are serialized with a mutex
- [ ] Logs do not interleave on a single line
- [ ] Log format follows the required state transitions
- [ ] Required states are logged:
  - has taken a dongle
  - is compiling
  - is debugging
  - is refactoring
  - burned out
- [ ] Timestamp is in milliseconds
- [ ] Coder number is displayed correctly

## Scheduling
- [ ] FIFO queue is implemented correctly
- [ ] FIFO grants access in request order
- [ ] EDF scheduler compares deadlines
- [ ] Request ordering is deterministic
- [ ] Waiting threads are awakened properly when resources become available

## Thread synchronization
- [ ] Mutexes protect shared structures
- [ ] Condition variables are used when needed
- [ ] Waiting threads do not busy-wait incorrectly
- [ ] Shared queue updates are atomic
- [ ] Critical sections are minimal and safe

## README requirements
- [ ] README.md exists at the root
- [ ] First line is italicized and matches the required format
- [ ] Description section exists
- [ ] Instructions section exists
- [ ] Resources section exists
- [ ] AI usage is described
- [ ] Blocking cases handled section exists
- [ ] Thread synchronization mechanisms section exists
- [ ] README is in English

## Final validation
- [ ] `make` succeeds without warnings
- [ ] Program runs with valid arguments
- [ ] Program exits cleanly
- [ ] No crash / segfault / bus error
- [ ] Output looks coherent and follows the required format
- [ ] Simulation is reproducible enough for testing