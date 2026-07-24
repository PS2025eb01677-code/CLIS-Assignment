#!/bin/bash
# monitor_log.sh
# Usage: ./monitor_log.sh <logfile>

LOGFILE="$1"
ERROR_REPORT="error_report.txt"

touch "$ERROR_REPORT"

echo "Monitoring $LOGFILE for ERROR entries..."

# Real-time monitoring pipeline
tail -f "$LOGFILE" 2>/dev/null | grep --line-buffered "ERROR" | tee -a "$ERROR_REPORT"
