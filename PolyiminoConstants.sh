#!/bin/bash

# Build up the internal constants
FILE_PREFIX=$1
UPPER_PREFIX=`echo "$1" | tr '[:lower:]' '[:upper:]'`
UPPER_SUFFIX="_CONSTANTS_INCLUDE"
FILE_SUFFIX="Constants.h"

# Collect the data
cat /dev/null > $FILE_PREFIX$FILE_SUFFIX
MAX_ROWS=`grep '[01]' $FILE_PREFIX.data | wc -l`
MAX_COLS=`sed -n '1p' $FILE_PREFIX.data | wc -w`
MAX_DATA=`sed -e 's/, /\n/g' $FILE_PREFIX.data | grep '1' | wc -l`

# Construct the header file
echo "#ifndef $UPPER_PREFIX$UPPER_SUFFIX" >> $FILE_PREFIX$FILE_SUFFIX
echo "#define $UPPER_PREFIX$UPPER_SUFFIX" >> $FILE_PREFIX$FILE_SUFFIX
echo "#define MAX_ROWS ($MAX_ROWS)" >> $FILE_PREFIX$FILE_SUFFIX
echo "#define MAX_COLS ($MAX_COLS)" >> $FILE_PREFIX$FILE_SUFFIX
echo "#define MAX_DATA ($MAX_DATA)" >> $FILE_PREFIX$FILE_SUFFIX
echo "#endif" >> $FILE_PREFIX$FILE_SUFFIX
