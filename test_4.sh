#!/bin/bash

# Test 4: Adding a new record and checking if it goes in the right spot
echo "=== Test 4: Add New Record ==="
echo "Adding TEST01 on March 20, should appear between March 10 and March 25"
echo ""

./sltm << EOF
load space_launches.csv
add "TEST01,2025-03-20,Falcon 9,Test Mission,CCSFS SLC-40,Success"
range 2025-03-10 2025-03-25
exit
EOF

echo ""
echo "Expected: TEST01 shows up between SLV006 (March 10) and SLV007 (March 25)"
echo ""
