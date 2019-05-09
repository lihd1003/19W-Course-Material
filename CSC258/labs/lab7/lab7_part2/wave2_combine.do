vlib work

vlog -timescale 1ns/1ns part2_combine.v

vsim lab7part2_a

log {/*}
add wave {/*}

force {clk} 0 0, 1 1 -r 2

force {reset_n} 0 0, 1 4 

force {go} 0 0, 1 15, 0 20 

force {load} 0 0, 1 5, 0 10, 1 50, 0 60 

force {pos} 0000001 0, 0000111 15

force {colour} 010


run 80ns