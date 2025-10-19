#!/bin/bash

# Test 3: Search functionality
echo "=== Test 3: Find Command (search for starlink) ==="
echo "Testing case-insensitive keyword search"
echo ""

./sltm << EOF
load space_launches.csv
find starlink
exit
EOF

echo ""
echo "Expected: 6 Starlink missions found"
echo ""
