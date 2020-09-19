#!/bin/bash
exec 3>&1 4>&2
trap 'exec 2>&4 1>&3' 0 1 2 3
exec 1>>log.out 2>&1
# Everything below will go to the file 'log.out':

echo "$(date): " 
git diff --stat --ignore-space-at-eol -b -w --ignore-blank-lines treemap.c

if gcc test.c -o a.out ; then
    echo  " tests:" $(($(./a.out create | grep -c 'OK') + $(./a.out insert | grep -c 'OK') + $(./a.out search | grep -c 'OK') + $(./a.out erase | grep -c 'OK') +  $(./a.out first | grep -c 'OK') + $(./a.out next | grep -c 'OK') + $(./a.out ub | grep -c 'OK'))) ok, $(($(./a.out create | grep -c 'FAILED') + $(./a.out insert | grep -c 'FAILED') + $(./a.out search | grep -c 'FAILED') + $(./a.out erase | grep -c 'FAILED') +  $(./a.out first | grep -c 'FAILED') + $(./a.out next | grep -c 'FAILED') + $(./a.out ub | grep -c 'FAILED'))) failed
else
    echo "Compilation failed"
fi

git commit -am "new try" &> /dev/null
git rev-parse --short HEAD
git push &> /dev/null








