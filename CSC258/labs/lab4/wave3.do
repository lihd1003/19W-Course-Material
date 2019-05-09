vlib work
vlog -timescale 1ns/1ns part3.v

# Load simulation using mux as the top level simulation module.
vsim lab4part3

# Log all signals and add some signals to waveform window.
log {/*}
# add wave {/*} would add all items in top level simulation module.
add wave {/*}

# reset
force {SW[9]} 0 0, 1 5, 0 100, 1 105, 0 200, 1 205

# test group 1, 2, 3
force {SW[7: 0]} 11010101 0, 10001001 100, 01000000 200

# clk
force {KEY[0]} 0 0, 1 5 -r 10

# load
force {KEY[1]} 0 10, 1 20, 0 110, 1 120, 0 210, 1 220

# shift
force {KEY[2]} 0 0, 1 20 -r 100

# ASR
force {KEY[3]} 0 0, 1 100
run 290ns