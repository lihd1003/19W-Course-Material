vlib work

vlog -timescale 1ns/1ns part2.v

vsim eightbitcounter

log {/*}
add wave {/*}

force {clk} 0 0, 1 5 -r 10
force {enable} 1
force {clear_b} 0 0, 1 50
run 150ns