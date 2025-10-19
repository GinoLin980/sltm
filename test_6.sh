#!/bin/bash

# Test 6: Delete a record and make sure it's gone
echo "=== Test 6: Delete Record ==="
echo "Deleting SLV010, then searching for starlink to verify it's gone"
echo ""

./sltm << EOF
load space_launches.csv
delete SLV010
find starlink
exit
EOF

echo ""
echo "Expected: Only 5 Starlink missions found (SLV010 removed)"
echo ""
