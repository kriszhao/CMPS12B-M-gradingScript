#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/Evelynchengusa/CMPS12B-M-gradingScript/master/pa3
EXAMP=https://classes.soe.ucsc.edu/cmps012b/Summer19/Examples/Programs/pa3

if [ ! -e backup ]; then
  echo "WARNING: a backup has been created for you in the \"backup\" folder"
  mkdir backup
fi

cp *.c Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelDictionaryTest.c > ModelDictionaryTest.c
curl $EXAMP/DictionaryClient2.c > DictionaryClient2.c 

echo ""
echo ""

make
make memcheck

if [ ! -e DictionaryTest ] || [ ! -x DictionaryTest ]; then # exist and executable
  echo ""
  echo "Makefile doesn't create DictionaryTest Executable!!! It is fine. Try to create DictionaryClient2 Executable"
  echo ""
  rm -f *.o DictionaryTest
else
  echo "Great,  DictionaryTest Executable is correctly generated."
fi


if [ ! -e DictionaryClient2 ] || [ ! -x DictionaryClient2 ]; then # exist and executable
  echo ""
  echo "Makefile doesn't create DictionaryClient2 Executable!!! If you create DictionaryTest Executable, you will be fine."
  echo ""
  rm -f *.o DictionaryClient2
else
  echo "Great,  DictionaryClent2 Executable is correctly generated."
fi
echo ""
echo ""

make clean

if [ -e DictionaryTest ] || [ -e *.o ]|| [ -e DictionaryClient2 ]; then
  echo "WARNING: Makefile didn't successfully clean all files"
fi

echo ""
echo ""

echo "Press Enter To Continue with DictionaryTest Results"
read verbose

gcc -std=c99 -Wall -g ModelDictionaryTest.c Dictionary.c -o ModelDictionaryTest >> garbage &>> garbage
cat garbage

echo ""
echo ""

timeout 45 ModelDictionaryTest -v > DictionaryTest-out.txt &>> DictionaryTest-out.txt
cat DictionaryTest-out.txt

echo ""
echo ""

rm -f *out.txt

rm -f *.class ModelDictionaryTest.java garbage*
