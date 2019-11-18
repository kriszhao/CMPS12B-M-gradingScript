#!/usr/bin/bash

SRCDIR=https://raw.githubusercontent.com/Evelynchengusa/CMPS12B-M-gradingScript/master/pa4

if [ ! -e backup ]; then
  echo "WARNING: a backup has been created for you in the \"backup\" folder"
  mkdir backup
fi

cp *.c *.h Makefile backup   # copy all files of importance into backup

curl $SRCDIR/ModelDictionaryTest.c > ModelDictionaryTest.c

echo "Press Enter To Continue with DictionaryTest Results"
read verbose

gcc -std=c99 -Wall -g ModelDictionaryTest.c Dictionary.c -o ModelDictionaryTest > garbage &>> garbage
cat garbage

timeout 5 valgrind --leak-check=full ./ModelDictionaryTest -v > DictionaryTest-out.txt &>> DictionaryTest-out.txt
cat DictionaryTest-out.txt

rm -f *out.txt

rm -f *.o ModelDictionaryTest* garbage*
