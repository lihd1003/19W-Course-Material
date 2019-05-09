vlib work

vlog -timescale 1ns/1ns part2.v

vsim control

log {/*}
add wave {/*}

force {clk} 0 0, 1 1 -r 2

force {reset_n} 0 0, 1 4 

force {go} 0 0, 1 15, 0 20 

force {load} 0 0, 1 5, 0 10 -r 40

run 70ns