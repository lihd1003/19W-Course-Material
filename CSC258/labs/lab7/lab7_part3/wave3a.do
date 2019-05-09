vlib work

vlog -timescale 1ns/1ns part3a.v

vsim part3_a

log {/*}
add wave {/*}

force {clk} 0 0, 1 1 -r 2

force {resetn} 0 0, 1 4 

force {colour_in} 010

run 300ns
