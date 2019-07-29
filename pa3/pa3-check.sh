#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/Evelynchengusa/CMPS12B-M-gradingScript/master/pa3

if [ ! -e backup ]; then
  echo "WARNING: a backup has been created for you in the \"backup\" folder"
  mkdir backup
fi

cp *.c Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelDictionaryTest.c > ModelDictionaryTest.c

echo ""
echo ""

make

if [ ! -e DictionaryTest ] || [ ! -x DictionaryTest ]; then # exist and executable
  echo ""
  echo "Makefile doesn't correctly create Executable!!!"
  echo ""
  rm -f *.o DictionaryTest
fi

echo ""
echo ""

make clean

if [ -e DictionaryTest ] || [ -e *.o ]; then
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
