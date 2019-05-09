
vlog -timescale 1ns/1ns part2.v

# Load simulation using mux as the top level simulation module.
vsim ripplecarry

# Log all signals and add some signals to waveform window.
log {/*}
# add wave {/*} would add all items in top level simulation module.
add wave {/*}

# a = 0101, b = 1010, ci = 0
force {SW[0]} 1 
force {SW[1]} 0
force {SW[2]} 1
force {SW[3]} 0
force {SW[4]} 0
force {SW[5]} 1
force {SW[6]} 0
force {SW[7]} 1
force {SW[9]} 0
run 20ns

# a = 0000, b = 0000, ci = 0
force {SW[0]} 0
force {SW[1]} 0
force {SW[2]} 0
force {SW[3]} 0
force {SW[4]} 0
force {SW[5]} 0
force {SW[6]} 0
force {SW[7]} 0
force {SW[9]} 0
run 20ns

# a = 1111, b = 0001, ci = 0
force {SW[0]} 1 
force {SW[1]} 1
force {SW[2]} 1
force {SW[3]} 1
force {SW[4]} 1
force {SW[5]} 0
force {SW[6]} 0
force {SW[7]} 0
force {SW[9]} 0
run 20ns

# a = 1111, b = 1111, ci = 0
force {SW[0]} 1 
force {SW[1]} 1
force {SW[2]} 1
force {SW[3]} 1
force {SW[4]} 1
force {SW[5]} 1
force {SW[6]} 1
force {SW[7]} 1
force {SW[9]} 0
run 20ns