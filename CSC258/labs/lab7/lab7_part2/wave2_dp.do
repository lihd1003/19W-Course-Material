vlib work

vlog -timescale 1ns/1ns part2.v

vsim datapath

log {/*}
add wave {/*}

force {clk} 0 0, 1 1 -r 2

force {reset_n} 0 0, 1 4 

force {pos} 0000100 0, 0000010 20

force {ld_x} 0 0, 1 5, 0 10

force {ld_y} 0 0, 1 15, 0 20 

force {enable} 0 0, 1 25, 0 80

run 100ns