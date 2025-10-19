#!/bin/bash

# Test 2: Check date range filtering
echo "=== Test 2: Range Query (March-April 2025) ==="
echo "Should display 6 launches between March 1 and April 30"
echo ""

./sltm << EOF
load space_launches.csv
range 2025-03-01 2025-04-30
exit
EOF

echo ""
echo "Expected: 6 events shown (SLV005, SLV006, SLV007, SLV008, SLV009, SLV010)"
echo ""
