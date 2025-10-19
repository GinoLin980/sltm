#!/bin/bash

# Test 5: Update a field in an existing record
echo "=== Test 5: Update Record ==="
echo "Changing SLV005 status from Scheduled to Success"
echo ""

./sltm << EOF
load space_launches.csv
range 2025-03-01 2025-03-01
update SLV005 status=Success
range 2025-03-01 2025-03-01
exit
EOF

echo ""
echo "Expected: SLV005 now shows status=Success instead of Scheduled"
echo ""
