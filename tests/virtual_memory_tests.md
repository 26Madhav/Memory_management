# Virtual Memory Tests

## Test 1: Valid Page Access

Virtual Address:
VPN = 2, Offset = 10

Expected Behavior:
- Page table entry is valid
- Physical address is generated correctly
- Cache access follows translation

## Test 2: Page Fault Scenario

Virtual Address:
VPN = 5 (not present)

Expected Behavior:
- Page table entry is invalid
- Page replacement is triggered
- Page table is updated

