#! /usr/bin/env bash

echo

if [[ -z "$1" ]]; then
    echo "ERROR: You need to specify a day number"
    echo "Format: $0 ##"
    echo
    exit 1
fi

day=$(printf "%02d" $1)

newDayFile="day${day}.cpp"
newDataFile="input${day}.txt"
echo "newDayFile = ${newDayFile}"
echo "newDataFile = ${newDataFile}"
echo

# Clean up build artifacts first
make clobber 2>&1 > /dev/null

echo "Makefile..."
cp Makefile Makefile.tmp
cat Makefile.tmp | sed -e "s/CXX_TARGETS = day../CXX_TARGETS = day${day}/" > Makefile
rm Makefile.tmp

# Check if code file already exists.
if [[ -f ${newDayFile} ]]; then
    # Don't overwrite
    echo "WARNING: ${newDayFile} already exists. Not overwriting."
else
    echo "Creating ${newDayFile}..."
    cat dayXX.cpp | sed -e "s/input..\.txt/input${day}\.txt/" > ${newDayFile}

    echo "Data files..."
    cp -v test.txt ${newDataFile}
    chmod 664 ${newDataFile}
fi

echo
