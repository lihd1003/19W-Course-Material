vlib work

vlog -timescale 1ns/1ns part3.v

vsim morse

log {/*}
add wave {/*}

# reset
force {reset_n} 0 0, 1 5, 0 250, 1 255

force {in[2:0]} 101 0, 110 150, 010 200

# clk
force {clk} 0 0, 1 1 -r 2

force {load_n} 0 0, 1 10 -r 100

run 400ns