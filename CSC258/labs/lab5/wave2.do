vlib work

vlog -timescale 1ns/1ns part2.v

vsim counter

log {/*}
add wave {/*}

force {rate[0]} 0 0, 1 40 -r 80
force {rate[1]} 0 0, 1 80
force {enable} 1
force {load} 0 0, 1 100, 0 105
force {load_n[3:0]} 1110 
force {reset_n} 0 0, 1 8
force {clk} 0 0, 1 2 -r 4
run 200ns
