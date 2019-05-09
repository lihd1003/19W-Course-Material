vlib work

vlog -timescale 1ns/1ns poly_function.v

vsim part2

log {/*}
add wave {/*}

force {clk} 0 0, 1 2 -r 4

force {resetn} 0 0, 1 4

force {go} 0 0, 1 5, 0 10, 1 15, 0 20, 1 25, 0 30, 1 35, 0 40, 1 45, 0 60

# A = 4, B = 11, C = 62, x = 6 out == 335 == 00111110
force {data_in} 00000100 2, 00001011 12, 00111110 22, 00000110 32


run 100ns