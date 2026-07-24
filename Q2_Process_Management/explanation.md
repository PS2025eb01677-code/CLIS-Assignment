# Question 2 - Process Management with fork(), Zombie Prevention, and Signals

## Program Behavior
The program creates 5 child processes using fork(). Even-indexed children 
(0, 2, 4) simulate normal short-lived work and exit after 1 second. 
Odd-indexed children (1, 3) simulate unresponsive/hung processes by 
entering an infinite loop. The parent monitors all children and cleans 
them up appropriately.

## Commands and Explanations

**gcc process_manager.c -o process_manager && ./process_manager (via OneCompiler Run)**
Compiles the C source into an executable and runs it. The output shows 
child processes starting, some finishing normally, and hung children 
eventually being terminated by the parent.

**fork()**
Duplicates the calling process. The return value distinguishes the parent 
(pid > 0, receives the new child's PID) from the child (pid == 0). Each 
of the 5 iterations creates one child, so the parent ends up managing 
5 concurrent processes.

**signal(SIGCHLD, reap_zombies)**
Registers a handler that the kernel automatically invokes whenever any 
child process terminates. This lets the parent reap finished children 
immediately and asynchronously, instead of having to poll for them.

**waitpid(-1, &status, WNOHANG)**
Called inside the signal handler to collect the exit status of any child 
that has terminated. WNOHANG makes this non-blocking, so the handler 
returns immediately even if no child is ready, preventing the parent from 
stalling. Without this call, terminated children would remain as zombie 
processes (entries in the process table wait for their exit status to be 
read).

**kill(pid, 0)**
Sends signal 0, which performs no actual action but returns success only 
if the process still exists. This is used to check whether a child is 
still running before deciding to kill it.

**kill(pid, SIGTERM)**
Sends a termination request that a process can catch, ignore, or clean up 
after. It's tried first because it allows a well-behaved process to exit 
gracefully.

**kill(pid, SIGKILL)**
Sent only if the child is still alive after SIGTERM. SIGKILL cannot be 
caught, blocked, or ignored by the target process, so it forcibly 
terminates processes that don't respond to the polite request — 
representing the "excessive/unresponsive child process" being forcibly 
cleaned up, as required by the problem statement.

**Final while (waitpid(-1, NULL, WNOHANG) > 0);**
A final cleanup sweep to reap any remaining terminated children (e.g. 
those just killed via SIGKILL) before the parent exits, ensuring no 
zombies are left behind.

## How They Work Together
fork() creates the concurrent workload that mimics a web server spawning 
child processes to handle requests. The SIGCHLD handler combined with 
waitpid(WNOHANG) ensures that as soon as any child finishes, its resources 
are reclaimed immediately, preventing zombie process buildup — which is 
exactly the kind of resource leak that eventually makes a real server 
unresponsive. For children that don't finish on their own (simulating 
requests that hang or child processes stuck in a bad state), the parent 
uses graduated signal escalation (SIGTERM, then SIGKILL) to forcibly 
reclaim system resources, ensuring the server keeps functioning under 
load instead of accumulating stuck processes indefinitely.

## Note on testing environment
Tested on an online C compiler (OneCompiler) rather than a local Linux 
terminal. fflush(stdout) was added after every printf to ensure output 
from concurrently running parent and child processes is flushed 
immediately rather than buffered, since forcibly killed processes 
(via SIGKILL) do not get a chance to flush their own buffers on exit.