#!/bin/bash
# script for removing shared memory

ipcs -m | grep 'selarkin'
let x=$(ipcs -m | grep 'selarkin' | awk '{ printf("%u\n", $2)}')
echo $x
ipcrm -m $x


