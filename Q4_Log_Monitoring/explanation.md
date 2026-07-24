# Question 4 - Real-Time Log Monitoring Pipeline

## Commands and Explanations

**chmod +x monitor_log.sh**
Grants execute permission so the monitoring script can be run directly.

**touch system.log**
Creates an empty log file to simulate a fresh server log before any 
entries are written to it.

**./monitor_log.sh system.log &**
Runs the monitor in the background (&) so it continues watching the log 
file while the script writes new entries to it, simulating a real 
administrator running the monitor in a separate terminal while the 
server writes logs live.

**tail -f "$LOGFILE" 2>/dev/null**
tail -f continuously follows the log file, printing new lines as they 
are appended rather than reading the file once. 2>/dev/null discards 
any error messages (e.g. if the file were rotated or missing) by sending 
them to the null device, keeping the visible output clean.

**| grep --line-buffered "ERROR"**
Pipes the live tail output into grep, which filters and passes through 
only lines containing "ERROR". --line-buffered forces grep to output 
each match immediately instead of waiting to fill an internal buffer, 
which matters because the input stream never technically ends.

**| tee -a error_report.txt**
tee displays the filtered ERROR lines on screen while simultaneously 
appending them (-a) to a persistent error_report.txt file, avoiding the 
need to run separate display and logging commands.

**echo "... ERROR: disk full" >> system.log**
Appends new log entries to simulate server activity happening in real 
time; each new line is picked up immediately by the background tail -f 
process.

**kill $MONITOR_PID**
Terminates the background monitoring process once testing is complete, 
since tail -f would otherwise run indefinitely.

**cat system.log / cat error_report.txt**
Confirms the full log contains both INFO and ERROR entries, while the 
error report contains only the ERROR entries, proving the filtering 
pipeline worked correctly.

## Key Techniques Used
- Pipes (|): connect tail's live output stream directly into grep, then 
  into tee, so each stage processes data incrementally as it arrives.
- /dev/null: the null device that silently discards unwanted stderr 
  output, keeping the terminal clean.
- tee -a: displays output while also appending it to a file, avoiding 
  duplicate commands for screen display and file logging.
- --line-buffered: ensures grep doesn't delay output on a continuous 
  stream.
- Background execution (&) + kill: allows a long-running "live" monitor 
  to coexist with the script that generates log entries, simulating a 
  real production monitoring scenario.