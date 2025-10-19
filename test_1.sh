#!/bin/bash

# Test 1: Load CSV and check if it handles valid/invalid records
echo "=== Test 1: Load CSV File ==="
echo "This should load 27 valid records and skip 7 invalid ones"
echo ""

./sltm << EOF
load space_launches.csv
exit
EOF

echo ""
echo "Expected output: Loaded 34 events, Valid: 27, Invalid: 7"
echo ""
