#!/bin/bash

tmpfile=$(mktemp /tmp/test_output.XXXXXX)

function cleanup() {
    if [ -f $tmpfile ]; then
        rm $tmpfile
    fi
}

function fail() {
    echo FAIL: $1
    cleanup
    exit -1
}

function test_make() {
    pushd $1 > /dev/null
    make
    if [ ! -f $1 ]; then
        fail "make did not create object file $1"
    fi
    popd > /dev/null
    echo PASS: make for $1 looks good!
}

function test_make_clean() {
    pushd $1 > /dev/null
    make clean
    if [ -f $1 ]; then
        fail "make clean did not remove object file $1" 
    fi
    popd > /dev/null
    echo PASS: make clean for $1 looks good!
}

function check_output() {
    diff -q $tmpfile $1 > /dev/null
    if [ $? -ne 0 ]; then
        echo Your result:
        cat $tmpfile
        echo
        echo correct answer:
        cat $1
        echo Please use diff command to see the difference
        echo
        fail "result or output format incorrect!"
    fi
    cleanup
}

function print_info() {
    echo Testing $1
    echo Input file: $2
    echo Answer file: $3
}

function test_avgcsv() {
    input_file=./simple_test_cases/avg_csv_input$1
    answer_file=./simple_test_cases/avg_csv_output$1
    print_info avgcsv $input_file $answer_file
    ./avgcsv/avgcsv $input_file > $tmpfile || fail "running your program"
    check_output $answer_file
    echo PASS: output result and format correct!
}

function test_gameoflife() {
    input_file=./simple_test_cases/gameoflife_input
    answer_file=./simple_test_cases/gameoflife_output
    tick=2
    print_info gameoflife $input_file $answer_file
    echo Tick: $tick
    ./gameoflife/gameoflife $input_file $tick > $tmpfile || fail "running your program"
    check_output $answer_file
    echo PASS: output result and format correct!
}

function remove_obj() {
    rm -f $1/$1
}

# clean up first in case students commit in object file
remove_obj avgcsv
test_make avgcsv
test_avgcsv 1
test_avgcsv 2
test_make_clean avgcsv
echo

# clean up first in case students commit in object file
remove_obj gameoflife
test_make gameoflife
test_gameoflife
test_make_clean gameoflife
echo

echo Output format check PASS!!! 
echo Go ahead and test harder cases!
cleanup
