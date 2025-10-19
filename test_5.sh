#!/bin/bash

# Test 5: Update a field in an existing record
echo "=== Test 5: Update Record ==="
echo "Changing SLV005 status from Scheduled to Success"
echo ""

./sltm << EOF
load space_launches.csv
update SLV005 status=Success
find SLV005
exit
EOF

echo ""
echo "Expected: SLV005 now shows status=Success instead of Scheduled"
echo ""
