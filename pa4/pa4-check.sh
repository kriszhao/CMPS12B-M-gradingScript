#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/kriszhao/CMPS12B-M-gradingScript/master/pa4
EXAMP=https://raw.githubusercontent.com/legendddhgf/cmps012b-pt.w18.grading/master/pa4

NUMTESTS=5
PNTSPERTEST=5
let MAXPTS=$NUMTESTS*$PNTSPERTEST

if [ ! -e backup ]; then
  echo "WARNING: a backup has been created for you in the \"backup\" folder"
  mkdir backup
fi

cp *.c Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelQueueTest.c > ModelQueueTest.c
# curl $EXAMP/DictionaryClient2.c > DictionaryClient2.c 

for NUM in $(seq 1 $NUMTESTS); do
  curl $EXAMP/infile$NUM.txt > infile$NUM.txt
  curl $EXAMP/model-rpt$NUM.txt > model-rpt$NUM.txt
  curl $EXAMP/model-trc$NUM.txt > model-trc$NUM.txt
done

echo ""
echo ""

make
make memcheck

if [ ! -e Simulation ] || [ ! -x Simulation ]; then # exist and executable
  echo ""
  echo "Makefile doesn't create Simulation Executable!!!"
  echo ""
  rm -f *.o Simulation
else
  echo "Great,  Simulation Executable is correctly generated."
fi


echo ""
echo ""

simulationtestspassed=$(expr 0)
echo "Please be warned that the following tests discard all output to stdout/stderr"
echo "Simulation tests: If nothing between '=' signs, then test is passed"
echo "Press enter to continue"
read verbose
for NUM in $(seq 1 $NUMTESTS); do
  if [ $NUM -eq 11 ]; then
    # students never cease to dissapoint me
    let simulationtestspassed+=1
    continue
  fi
  rm -f outfile$NUM.txt
  timeout 5 Simulation infile$NUM.txt &> garbage >> garbage
  diff -bBwu infile$NUM.txt.rpt model-rpt$NUM.txt &> diff-rpt$NUM.txt >> diff-rpt$NUM.txt
  diff -bBwu infile$NUM.txt.trc model-trc$NUM.txt &> diff-trc$NUM.txt >> diff-trc$NUM.txt
  echo "Report $NUM Test:"
  echo "=========="
  cat diff-rpt$NUM.txt
  echo "=========="
  echo "Trace $NUM Test:"
  echo "=========="
  cat diff-trc$NUM.txt
  echo "=========="

  cat diff-rpt$NUM.txt diff-trc$NUM.txt > diff$NUM.txt

  if [ -e diff$NUM.txt ] && [[ ! -s diff$NUM.txt ]]; then
    let simulationtestspassed+=1
  fi

  rm -f infile$NUM.txt *.rpt *.trc diff*$NUM* model-rpt$NUM.txt model-trc$NUM.txt

done

echo ""
echo ""

let simulationtestpoints=$PNTSPERTEST*$simulationtestspassed

echo "Passed $simulationtestspassed Simulation tests"
echo "This gives a total of $simulationtestpoints / $MAXPTS points"

echo ""
echo ""





make clean

if [ -e Simulation ] || [ -e *.o ]|| [ -e IntegerQueue ]; then
  echo "WARNING: Makefile didn't successfully clean all files"
fi

echo ""
echo ""

echo "Press Enter To Continue with ModelQueueTest Results"
read verbose

gcc -std=c99 -Wall -g ModelQueueTest.c IntegerQueue.c -o ModelQueueTest >> garbage &>> garbage
cat garbage

echo ""
echo ""

timeout 45 ModelQueueTest -v > QueueTest-out.txt &>> QueueTest-out.txt
cat QueueTest-out.txt

echo ""
echo ""

rm -f *.txt

rm -f *.o ModelQueueTest ModelQueueTest.c garbage*
