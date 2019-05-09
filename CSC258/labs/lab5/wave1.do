vlib work

vlog -timescale 1ns/1ns part1.v

vsim lab5part1

log {/*}
add wave {/*}

# clk
force {KEY[0]} 0 0, 1 5 -r 10

# enable
force {SW[1]} 1 0, 0 200, 1 300

# clear
force {SW[0]} 0 0, 1 10, 0 312, 1 315
run 400ns