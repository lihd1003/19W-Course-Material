// Part 2 skeleton

module part2
	(
		CLOCK_50,						//	On Board 50 MHz
		// Your inputs and outputs here
        KEY,
        SW,
		// The ports below are for the VGA output.  Do not change.
		VGA_CLK,   						//	VGA Clock
		VGA_HS,							//	VGA H_SYNC
		VGA_VS,							//	VGA V_SYNC
		VGA_BLANK_N,						//	VGA BLANK
		VGA_SYNC_N,						//	VGA SYNC
		VGA_R,   						//	VGA Red[9:0]
		VGA_G,	 						//	VGA Green[9:0]
		VGA_B   						//	VGA Blue[9:0]
	);

	input			CLOCK_50;				//	50 MHz
	input   [9:0]   SW;
	input   [3:0]   KEY;

	// Declare your inputs and outputs here
	// Do not change the following outputs
	output			VGA_CLK;   				//	VGA Clock
	output			VGA_HS;					//	VGA H_SYNC
	output			VGA_VS;					//	VGA V_SYNC
	output			VGA_BLANK_N;				//	VGA BLANK
	output			VGA_SYNC_N;				//	VGA SYNC
	output	[9:0]	VGA_R;   				//	VGA Red[9:0]
	output	[9:0]	VGA_G;	 				//	VGA Green[9:0]
	output	[9:0]	VGA_B;   				//	VGA Blue[9:0]
	
	wire resetn;
	assign resetn = KEY[0];
	
	// Create the colour, x, y and writeEn wires that are inputs to the controller.
	wire [2:0] colour;
	wire [7:0] x;
	wire [6:0] y;
	wire writeEn;

	// Create an Instance of a VGA controller - there can be only one!
	// Define the number of colours as well as the initial background
	// image file (.MIF) for the controller.
	vga_adapter VGA(
			.resetn(resetn),
			.clock(CLOCK_50),
			.colour(colour),
			.x(x),
			.y(y),
			.plot(writeEn),
			/* Signals for the DAC to drive the monitor. */
			.VGA_R(VGA_R),
			.VGA_G(VGA_G),
			.VGA_B(VGA_B),
			.VGA_HS(VGA_HS),
			.VGA_VS(VGA_VS),
			.VGA_BLANK(VGA_BLANK_N),
			.VGA_SYNC(VGA_SYNC_N),
			.VGA_CLK(VGA_CLK));
		defparam VGA.RESOLUTION = "160x120";
		defparam VGA.MONOCHROME = "FALSE";
		defparam VGA.BITS_PER_COLOUR_CHANNEL = 1;
		defparam VGA.BACKGROUND_IMAGE = "black.mif";
			
	// Put your code here. Your code should produce signals x,y,colour and writeEn/plot
	// for the VGA controller, in addition to any other functionality your design may require.
    
	wire ld, draw, ld_x, ld_y, enable;
	assign ld = ~KEY[0];
	
	assign draw = ~KEY[3];
    // Instansiate datapath
	// datapath d0(...);

    // Instansiate FSM control
    // control c0(...);
	
	control c0(CLOCK_50, resetn, ld, draw, ld_x, ld_y, enable, writeEn);
    
    datapath d0(CLOCK_50, resetn, SW[6:0], SW[9:7], ld_x, ld_y, enable, x, y, colour);
    
endmodule

module datapath
    (
        input clk, reset_n,
        input [6:0] pos,        
        input [2:0] color_in,
        input ld_x, ld_y, enable,
        output [7:0] x_out,
        output [6:0] y_out,
        output reg [2:0] color_out
    );
    
    reg [3:0] count;
    reg [7:0] x;
    reg [6:0] y; 
    
    // register
    always @(posedge clk) begin
        if (!reset_n) begin
            x <= 8'b0;
            y <= 7'b0;
            color_out <= 3'b0;
        end
        else begin
            if (ld_x)
                x <= {1'b0, pos};
            if (ld_y) begin
                y <= pos;
                color_out <= color_in;
            end
        end
        
    end
    
    // counter
    always @(posedge clk) begin
        if (!reset_n)
            count <= 4'b0000;
        else if (enable) begin
            count <= count + 1'b1;
        end
    end
        
    assign x_out = x + count[1:0];
    assign y_out = y + count[3:2];
    
endmodule

module control(
    input clk, reset_n, load, go, 

    output reg  ld_x, ld_y,
    output reg enable, 
    output reg writeEn
    );

    reg [2:0] current_state, next_state; 
    
    localparam  S_LOAD_X        = 3'd0,
                S_LOAD_X_WAIT   = 3'd1,
                S_LOAD_Y        = 3'd2,
                S_DRAW          = 3'd3;
    
    // Next state logic aka our state table
    always@(*)
    begin: state_table 
            case (current_state)
                S_LOAD_X: next_state = load ? S_LOAD_X_WAIT : S_LOAD_X; 
                S_LOAD_X_WAIT: next_state = load ? S_LOAD_X_WAIT : S_LOAD_Y; 
                S_LOAD_Y: next_state = go ? S_DRAW : S_LOAD_Y;
                S_DRAW: next_state = load ? S_LOAD_X : S_DRAW;
            default:     next_state = S_LOAD_X;
        endcase
    end // state_table
   

    // Output logic aka all of our datapath control signals
    always @(*)
    begin: enable_signals
        // By default make all our signals 0
        ld_x = 1'b0;
        ld_y = 1'b0;
        writeEn = 1'b0;
        enable = 1'b0;

        case (current_state)
            S_LOAD_X: begin
                ld_x = 1'b1;
                end
            S_LOAD_Y: begin
                ld_y = 1'b1;
                end
            S_DRAW: begin
                enable = 1;
                writeEn = 1;
                end
        endcase
    end
   
    // current_state registers
    always@(posedge clk)
    begin: state_FFs
        if(!reset_n)
            current_state <= S_LOAD_X;
        else
            current_state <= next_state;
    end // state_FFS
endmodule
