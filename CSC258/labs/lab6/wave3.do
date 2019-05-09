vlib work

vlog -timescale 1ns/1ns part3.v

vsim dividor

log {/*}
add wave {/*}

force {clk} 0 0, 1 2 -r 4

force {resetn} 0 0, 1 4 -r 100

force {go} 0 0, 1 5, 0 10 -r 100

# divided = 0111, divisor = 0011, data_result = 00010010
#           1011,           0011,               00100011
force {data_in} 01110011 0, 10110011 100


run 200ns