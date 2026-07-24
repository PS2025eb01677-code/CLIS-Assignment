#!/bin/bash
# process_submissions.sh
# Usage: ./process_submissions.sh <submissions_dir>

SUBMIT_DIR="$1"
BACKUP_DIR="./backup_unique_$(date +%F_%T | tr ':' '-')"
REPORT_FILE="report.txt"
ERROR_LOG="errors.log"
HASH_FILE="hashes.tmp"

mkdir -p "$BACKUP_DIR" 2>>"$ERROR_LOG"

total=0
dupes=0
backed_up=0

> "$HASH_FILE"
> "$REPORT_FILE"
> "$ERROR_LOG"

if [ ! -d "$SUBMIT_DIR" ]; then
    echo "Error: Directory $SUBMIT_DIR not found" >> "$ERROR_LOG"
    exit 1
fi

for file in "$SUBMIT_DIR"/*; do
    if [ -f "$file" ]; then
        total=$((total+1))
        checksum=$(md5sum "$file" 2>>"$ERROR_LOG" | awk '{print $1}')

        if grep -q "$checksum" "$HASH_FILE" 2>>"$ERROR_LOG"; then
            dupes=$((dupes+1))
        else
            echo "$checksum" >> "$HASH_FILE"
            cp "$file" "$BACKUP_DIR/" 2>>"$ERROR_LOG"
            if [ $? -eq 0 ]; then
                backed_up=$((backed_up+1))
            fi
        fi
    fi
done

{
    echo "===== Submission Processing Report ====="
    echo "Date: $(date)"
    echo "Total files processed : $total"
    echo "Duplicate files found : $dupes"
    echo "Files backed up       : $backed_up"
    echo "Backup location        : $BACKUP_DIR"
} >> "$REPORT_FILE"

rm -f "$HASH_FILE"

echo "Done. See $REPORT_FILE and $ERROR_LOG"
