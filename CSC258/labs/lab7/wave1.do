vlib work

vlog -timescale 1ns/1ns ram32x4.v

vsim -L altera_mf_ver ram32x4

log {/*}
add wave {/*}

force {clock} 0 0, 1 1 -r 2

force {wren} 1 0, 0 20, 1 30, 0 35

force {data} 0101 0, 1101 10, 1001 30

force {address} 00001 0, 00101 5, 01011 10, 10110 15, 00001 20, 01011 25, 01000 30, 10110 35

run 40ps