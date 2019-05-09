#!/usr/bin/env bash

gcc -Wall -std=gnu99 -g -o echo_arg echo_arg.c
./echo_arg csc209 >./echo_out.txt
./echo_stdin < ./echo_stdin.c
./count 210 | wc -c
ls -S | ./echo_stdin
