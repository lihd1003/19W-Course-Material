
vlog -timescale 1ns/1ns part1.v

# Load simulation using mux as the top level simulation module.
vsim mux7to1

# Log all signals and add some signals to waveform window.
log {/*}
# add wave {/*} would add all items in top level simulation module.
add wave {/*}

# test group 1
force {SW[0]} 1 
force {SW[1]} 0
force {SW[2]} 1
force {SW[3]} 0
force {SW[4]} 0
force {SW[5]} 1
force {SW[6]} 0
force {SW[9]} 0 0, 1 40
force {SW[8]} 0 0, 1 20 -r 40
force {SW[7]} 0 0, 1 10 -r 20
run 70ns

# test group 2
force {SW[0]} 0 
force {SW[1]} 1
force {SW[2]} 0
force {SW[3]} 1
force {SW[4]} 1
force {SW[5]} 1
force {SW[6]} 0
force {SW[9]} 0 0, 1 40
force {SW[8]} 0 0, 1 20 -r 40
force {SW[7]} 0 0, 1 10 -r 20
run 70ns