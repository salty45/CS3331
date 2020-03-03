#!/bin/bash
# script for removing shared memory

ipcs -m | grep 'selarkin'
let x=$(ipcs -m | grep 'selarkin' | awk '{ if ($6 == 0) printf("%u\n", $2)}')
echo $x
ipcrm -m $x


