#!/bin/bash

LOGFILE="app.log"
REPORT="error_report.txt"

> "$REPORT"

echo "Monitoring $LOGFILE for ERROR messages... (Ctrl+C to stop)"

tail -f "$LOGFILE" 2>/dev/null | grep --line-buffered "ERROR" >> "$REPORT"
