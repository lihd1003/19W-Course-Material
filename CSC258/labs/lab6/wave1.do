vlib work

vlog -timescale 1ns/1ns part1.v

vsim lab5part1

log {/*}
add wave {/*}

# clk
force {KEY[0]} 0 0, 1 5 -r 10

# input
force {SW[1]} 1 0, 0 100, 1 120

# reset
force {SW[0]} 0 0, 1 10,
run 150ns