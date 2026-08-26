*This project has been created as part of the 42 curriculum by molahrac.*

# Codexion
![CODEXION_BANNER](https://raw.githubusercontent.com/mirr-x/42-CC-1337/refs/heads/main/images/CODEXION_BANNER.png)

## Description

Codexion is a systems-level C project focused on concurrent resource management, where independent coder threads compete for shared dongles under FIFO/EDF scheduling. It tackles deadlock prevention, synchronization, starvation, resource arbitration, and precise timeout detection using POSIX threading primitives.

The project focuses on POSIX threads, mutexes, condition variables, priority
queues, deadlock prevention, cooldown timing, burnout detection, and safe
cleanup of shared resources.

## Instructions

Build the program from the project directory:

```sh
make
```

Run it with these arguments:

```text
./codexion number_of_coders time_to_burnout time_to_compile \
time_to_debug time_to_refactor number_of_compiles_required \
dongle_cooldown scheduler
```

The scheduler must be either `fifo` or `edf`. Times are expressed in
milliseconds. For example:

```sh
./codexion 3 3000 500 500 500 2 500 edf
```

Useful Makefile commands are `make clean`, `make fclean`, and `make re`.

## Thread Lifecycle

Each coder repeatedly follows this sequence:

```text
request dongles -> compile -> release dongles -> debug -> refactor
```
MORE INFO
```
Start Simulation
      │
      ▼
Try to get 2 dongles
      │
      ▼
Start Compiling
      │
      │  <-- time_to_compile -->
      ▼
Finish Compiling
      │
      ▼
Debug
      │
      │  <-- time_to_debug -->
      ▼
Finish Debugging
      │
      ▼
Refactor
      │
      │  <-- time_to_refactor -->
      ▼
Finish Refactoring
      │
      ▼
Immediately try to get 2 dongles again
```

The coder holds the required dongle mutexes during compilation. The simulation
ends when every coder reaches the required compile count or when a coder burns
out.

## Blocking Cases Handled

- Dongles are locked in a consistent ID order to prevent circular-wait
      deadlocks.
- Each dongle has its own mutex, preventing duplicate ownership.
- Dongle cooldown is checked after release before another acquisition.
- FIFO requests use heap insertion order.
- EDF requests use the earliest burnout deadline, with coder ID as a
      deterministic tie-breaker.
- A condition variable wakes waiting coders when resources or scheduling state
      changes.
- The monitor checks burnout deadlines and stops the simulation when necessary.
- Output is protected by a print mutex so state messages do not interleave.
- Initialization counters and flags prevent cleanup from destroying
      uninitialized synchronization objects.
- Shared simulation and completion state is protected by the state mutex.

## Thread Synchronization Mechanisms

The program uses POSIX `pthread_mutex_t` objects for each dongle, the priority
queue, output, coder burnout timestamps, and shared simulation state.

The priority queue mutex protects heap operations and scheduler predicates.
Dongle mutexes protect ownership and availability changes. Coder burnout data is
protected by each coder's burnout mutex. The print mutex serializes complete log
messages.

The `coder_wait_cond` condition variable is the program's event mechanism. A
coder waits while it is not the scheduler-selected request or while a required
dongle is unavailable. Resource release and simulation shutdown broadcast the
condition variable so waiting threads re-check the predicate safely.

The state mutex protects `simulation_running`, `is_finished`, and
`compiles_done`. For example, the monitor changes the simulation state while a
coder reads it through synchronized helpers, preventing unsynchronized
shutdown decisions.




## Resources

- [the anatomy of execution](https://denim-bosworth-b13.notion.site/The-Anatomy-of-Execution-391e6e3c12ea806abdb7f1e5825bc9d1?source=copy_link)
- [virtual memory](https://denim-bosworth-b13.notion.site/Virtual-Memory-391e6e3c12ea807a997dfe3682638a94?source=copy_link)
- [cpu registers](https://denim-bosworth-b13.notion.site/CPU-Registers-391e6e3c12ea806b8637c73b8b6ecc53?source=copy_link)
- [The Stack: Function Calls and Execution Context](https://denim-bosworth-b13.notion.site/The-Stack-Function-Calls-and-Execution-Context-391e6e3c12ea8017a2cacd6146a0f157?source=copy_link)
- [The Heap: Dynamic Memory Management](https://denim-bosworth-b13.notion.site/The-Heap-Dynamic-Memory-Management-392e6e3c12ea80f5b004c068082f4025?source=copy_link)
- [Processes: The Running Instance](https://denim-bosworth-b13.notion.site/Processes-The-Running-Instance-392e6e3c12ea80589cffd6b16d8c4896?source=copy_link)
- [Processes vs. Threads: Memory and Register Sharing](https://denim-bosworth-b13.notion.site/Processes-vs-Threads-Memory-and-Register-Sharing-392e6e3c12ea80b5ae8bc685ce52cce8?source=copy_link)
- [What do you mean by another execution path?](https://denim-bosworth-b13.notion.site/What-do-you-mean-by-another-execution-path-393e6e3c12ea80a7ae66e125ead60e32?source=copy_link)
- [Context Switching](https://denim-bosworth-b13.notion.site/Context-Switching-393e6e3c12ea8098a9d5fcb066356e99?source=copy_link)
- [Race Conditions: A Precise Definition](https://denim-bosworth-b13.notion.site/Race-Conditions-A-Precise-Definition-394e6e3c12ea80b2bd4bdfef8733eba9?source=copy_link)
- [What Is a Thread?](https://denim-bosworth-b13.notion.site/What-Is-a-Thread-395e6e3c12ea8096aa0ee490d555cdaa?source=copy_link)
- [Thread Scheduling](https://denim-bosworth-b13.notion.site/Thread-Scheduling-395e6e3c12ea8055b209fd263466e091?source=copy_link)
- [Your First Thread: Creating and Managing Threads with pthread_create](https://denim-bosworth-b13.notion.site/Your-First-Thread-Creating-and-Managing-Threads-with-pthread_create-395e6e3c12ea8029a7b1eed1704b6e8a?source=copy_link)
- [pthread_join: Waiting for Threads](https://denim-bosworth-b13.notion.site/pthread_join-Waiting-for-Threads-396e6e3c12ea80f3ae86cd77a2a93479?source=copy_link)
- [Passing Data to Threads: The Role of void *](https://denim-bosworth-b13.notion.site/Passing-Data-to-Threads-The-Role-of-void-396e6e3c12ea80e288d9ed4d30813de2?source=copy_link)
- [Thread Return Values](https://denim-bosworth-b13.notion.site/Thread-Return-Values-396e6e3c12ea80c89757f1bb980fbbf7?source=copy_link)
- [Detached Threads](https://denim-bosworth-b13.notion.site/Detached-Threads-396e6e3c12ea80579c59e67f321895ed?source=copy_link)
- [The Complete Life of a Thread](https://denim-bosworth-b13.notion.site/The-Complete-Life-of-a-Thread-396e6e3c12ea8075a732d33913841dc1?source=copy_link)
- [Race Conditions](https://denim-bosworth-b13.notion.site/Race-Conditions-397e6e3c12ea80b6a074c81ca6ae75b6?source=copy_link)
- [Mutexes](https://denim-bosworth-b13.notion.site/Mutexes-397e6e3c12ea806990edd8c67bdf8db3?source=copy_link)
- [Critical Sections](https://denim-bosworth-b13.notion.site/Critical-Sections-397e6e3c12ea80a586b2f05d7fc82a32?source=copy_link)
- [Deadlocks](https://denim-bosworth-b13.notion.site/Deadlocks-397e6e3c12ea801a9446efd638a24008?source=copy_link)
- [Condition Variables](https://denim-bosworth-b13.notion.site/Condition-Variables-397e6e3c12ea8054b084d0763d81fdb1?source=copy_link)
- [Time Management](https://denim-bosworth-b13.notion.site/Time-Management-399e6e3c12ea80fb9ae1ed8189dcc3e2?source=copy_link)

## AI usage
AI was used to help review concurrency requirements, identify possible race and
deadlock paths, suggest focused tests, and improve documentation structure.