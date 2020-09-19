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



#clear
echo " Que funcionalidad desea testear?" >&3
echo "1. Create"
echo "2. Insert"
echo "3. Search"
echo "4. Erase"
echo "5. First"
echo "6. Next"
echo "7. UpperBound"
echo -n "Seleccione una opcion [1 - 7]"
read opcion
case $opcion in
1) ./a.out create  &>1;;
2) ./a.out insert;;
3) ./a.out search;;
4) ./a.out erase;;
5) ./a.out first;;
6) ./a.out next;;
7) ./a.out ub;;
*) echo "$opc es una opcion invalida?";;




