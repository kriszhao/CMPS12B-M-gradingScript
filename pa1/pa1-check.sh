#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/Evelynchengusa/CMPS12B-M-gradingScript/master/pa1/

NUMTESTS=8
PNTSPERTEST=5
let MAXPTS=$NUMTESTS*$PNTSPERTEST

if [ ! -e backup ]; then
  echo "WARNING: a backup has been created for you in the \"backup\" folder"
  mkdir backup
fi

cp *.c Makefile backup   # copy all files of importance into backup

for NUM in $(seq 1 $NUMTESTS); do
    curl $SRCDIR/infile$NUM.txt > infile$NUM.txt
    curl $SRCDIR/model-out$NUM.txt > model-out$NUM.txt
    curl $SRCDIR/modelunit-out$NUM.txt > modelunit-out$NUM.txt
done

curl $SRCDIR/ModelSubsetTest.c > ModelSubsetTest.c
curl $SRCDIR/Makefile1 > Makefile1
echo ""
echo ""

make

if [ ! -e Subset ] || [ ! -x Subset ]; then # exist and executable
  echo ""
  echo "Makefile doesn't correctly create Executable!!!"
  echo ""
  rm -f *.o
  gcc -std=c99 -Wall -g Subset.c -o Subset > garbage &>> garbage
fi

echo ""
echo ""

testspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout/stderr"
echo "Subset tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do

  rm -f out$NUM.txt

  # generous 5 second timeout
  timeout 5 cat infile$NUM.txt | xargs Subset &> out$NUM.txt >> out$NUM.txt

  diff -ywBZbi --suppress-common-lines out$NUM.txt model-out$NUM.txt &> diff$NUM.txt >> diff$NUM.txt
  
  echo "Test $NUM:"
  echo "=========="
  cat diff$NUM.txt
  echo "=========="

  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    let testspassed+=1
  fi

  rm -f infile$NUM.txt out$NUM.txt diff$NUM* model-out$NUM.txt

done

echo ""
echo ""

let testpoints=$PNTSPERTEST*$testspassed

echo "Passed $testspassed / $NUMTESTS Subset tests"
echo "This gives a total of $testpoints / $MAXPTS points"

echo ""
echo ""

make clean

if [ -e Subset ] || [ -e *.o ]; then
  echo "WARNING: Makefile didn't successfully clean all files"
  rm -f Subset *.o
fi

echo ""
echo ""

echo "Press Enter To Continue with SubsetTest Results"
read verbose

#javac *.java >> garbage &>> garbage >> garbage &>> garbage
make -f Makefile1 ModelSubsetTest 
#cat garbage

timeout 5 ./ModelSubsetTest -v > SubsetTest-out.txt &>> SubsetTest-out.txt
cat SubsetTest-out.txt

rm -f *out[0-9].txt

rm -f *.o ModelSubsetTest* garbage*
