#!/bin/bash
 exec 3>&1 4>&2
 trap 'exec 2>&4 1>&3' 0 1 2 3

# Everything below will go to the file 'log':

target_file="treemap.c"
testcode="test.c"

#Verifca si se realizaron cambios 
CHANGED=$(git diff --ignore-space-at-eol -b -w --ignore-blank-lines $target_file)

#Si hubieron cambios se actualiza el log
if [ -n "$CHANGED" ]; then
  git checkout -- $testcode log &> /dev/null
  exec 1>>log 2>&1

  echo "$(date): "
  git diff --stat --ignore-space-at-eol -b -w --ignore-blank-lines $target_file

  #Compilation
  if gcc $testcode -Wall -Werror -o a.out ; then
      echo  " tests: " $(($(./a.out | grep -c 'OK')))\|$(($(./a.out | grep -c 'FAILED')))
  else
      echo "Compilation failed"
      gcc $testcode -Wall -Werror -o a.out 2>&3
      exit 1
  fi

  ./a.out | tail -n1
  #previous commit hash
  git rev-parse --short HEAD

  #git add and commit
  git add $target_file log &> /dev/null 
  git commit -m 'new try' &> /dev/null 
  
fi

#Retroalimentation

if ./a.out &> /dev/null ; then
  
  ./a.out >&3

  echo "Quiere actualizar su puntaje en el servidor?(s|n): " >&3
  read response

  if [ "$response" == "s" ]; then
      git push &> /dev/null 
  fi

else

  gcc -g test.c >&3
  gdb -ex 'run' -ex q ./a.out >&3

fi