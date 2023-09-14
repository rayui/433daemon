#!/bin/sh
./433daemon $1 $2 $3 $4 $5 $6 $7 | socat -b $((($3 * 2) + 1)) - UDP:$8