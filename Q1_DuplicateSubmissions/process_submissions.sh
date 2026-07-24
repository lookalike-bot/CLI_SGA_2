#!/bin/bash

SUBMISSIONS_DIR="submissions"
BACKUP_DIR="backup"
REPORT_FILE="report.txt"
ERROR_FILE="errors.txt"

mkdir -p "$BACKUP_DIR"
> "$REPORT_FILE"
> "$ERROR_FILE"

total_files=0
duplicate_count=0
backup_count=0

declare -A seen_hashes

for file in "$SUBMISSIONS_DIR"/*; do
    if [ -f "$file" ]; then
        total_files=$((total_files+1))
        hash=$(md5sum "$file" 2>>"$ERROR_FILE" | awk '{print $1}')
        if [ -z "$hash" ]; then
            echo "Error hashing $file" >> "$ERROR_FILE"
            continue
        fi
        if [ -n "${seen_hashes[$hash]}" ]; then
            duplicate_count=$((duplicate_count+1))
            echo "Duplicate: $file (same content as ${seen_hashes[$hash]})" >> "$REPORT_FILE"
        else
            seen_hashes[$hash]="$file"
            cp "$file" "$BACKUP_DIR/" 2>>"$ERROR_FILE"
            backup_count=$((backup_count+1))
        fi
    fi
done

{
echo "=== Submission Processing Report ==="
echo "Total files processed: $total_files"
echo "Duplicate files found: $duplicate_count"
echo "Unique files backed up: $backup_count"
} >> "$REPORT_FILE"
