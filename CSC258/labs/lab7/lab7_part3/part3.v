module part3
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
	wire writeEn;
	wire [7:0] x_out;
	wire [6:0] y_out;
	wire [2:0] colour_out;
	
	// Create an Instance of a VGA controller - there can be only one!
	// Define the number of colours as well as the initial background
	// image file (.MIF) for the controller.
	vga_adapter VGA(
			.resetn(resetn),
			.clock(CLOCK_50),
			.colour(colour_out),
			.x(x_out),
			.y(y_out),
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
	wire [7:0] x;
	wire [7:0] y;
	wire ld_black, ld_in, enable, ld_colour;
	wire en_plot, en_wait, en_erase, reset_plot, reset_erase, reset_wait, to_wait, to_move, to_erase;
	wire en_cx, en_cy;
	
	ratedivider r1(en_plot, 28'd16, CLOCK_50, reset_plot, to_wait);
	ratedivider r2(en_erase, 28'd16, CLOCK_50, reset_erase, to_move);
	ratedivider r3(en_wait, 28'd12499995, CLOCK_50, reset_wait, to_erase);
	
	xycounter c_x(8'd0, 8'd156, CLOCK_50, resetn, en_cx, x);
	xycounter c_y(8'd60, 8'd116, CLOCK_50, resetn, en_cy, y);
	
	
	control c1(CLOCK_50, resetn, 
			   to_wait, to_erase, to_move,
			   en_wait, en_erase, en_plot,
			   en_cx, 
			   en_cy,
			   ld_in, ld_colour, ld_black, enable,
			   reset_wait, reset_erase, reset_plot,
			   writeEn
			   );
		
	datapath d1(CLOCK_50, resetn, 
				x, 
				y[6:0], 
				SW[9:7], 
				ld_in, ld_colour, ld_black, enable, 
				x_out,
				y_out, 
				colour_out);
				
endmodule

module datapath(
        input clk, reset_n,
        input [7:0] x,
		input [6:0] y,
        input [2:0] color_in,
        input ld_in, ld_colour, ld_black, enable,
        output [7:0] x_out,
        output [6:0] y_out,
        output reg [2:0] color_out
    );
    
    reg [3:0] count;
	reg [7:0] x_reg;
	reg [6:0] y_reg;
    
    always @(posedge clk) begin
        if (!reset_n) begin
            x_reg <= 8'b0;
            y_reg <= 7'b0;
            color_out <= 3'b0;
        end
        else begin
            if (ld_in) begin
                x_reg <= x;
				y_reg <= y;
			end
            if (ld_colour) 
                color_out <= ld_black ? 3'b000 : color_in;
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
        
    assign x_out = x_reg + count[1:0];
    assign y_out = y_reg + count[3:2];
    
endmodule

module control(
    input clk, reset_n, 
	input to_wait, to_erase, to_move,
	output reg en_wait, en_erase, en_plot,
	output reg en_cx, en_cy,
    output reg ld_in, ld_colour, ld_black, enable, 
	output reg reset_wait, reset_erase, reset_plot,
    output reg writeEn
    );

    reg [3:0] current_state, next_state; 
    reg up_down, left_right;
    localparam  S_INIT = 4'd0, 
				S_PLOT = 4'd1,
				S_WAIT = 4'd2,
				S_ERASE = 4'd3,
				S_MOVE = 4'd4,
				S_LOAD_1 = 4'd5,
				S_LOAD_2 = 4'd6;
    

    always@(*)
    begin: state_table 
            case (current_state)
                S_INIT: next_state = S_LOAD_1;
				S_LOAD_1: next_state = S_PLOT;
				S_PLOT: next_state = to_wait ? S_WAIT: S_PLOT;
				S_WAIT: next_state = to_erase ? S_LOAD_2 : S_WAIT;
				S_LOAD_2: next_state = S_ERASE;
				S_ERASE: next_state = to_move ? S_MOVE : S_ERASE;
				S_MOVE: next_state = S_LOAD_1;
            default:   next_state = S_INIT;
        endcase
    end // state_table
   

    always @(*) begin: enable_signals
        ld_in = 1'b0;
        ld_colour = 1'b0;
		ld_black = 1'b0;
        writeEn = 1'b0;
        enable = 1'b0;
		en_cx = 1'b0;
		en_cy = 1'b0;
		reset_erase = 1'b0;
		reset_plot = 1'b0;
		reset_wait = 1'b0;
		en_erase = 1'b0;
		en_plot = 1'b0;
		en_wait = 1'b0;

        case (current_state)
			S_LOAD_1: begin
				ld_in = 1'b1;
				ld_colour = 1'b1;
			end
			S_PLOT: begin
				writeEn = 1'b1;
				enable = 1'b1;
				reset_plot = 1'b1;
				en_plot = 1'b1;
			end
			S_WAIT: begin
				reset_wait = 1'b1;
				en_wait = 1'b1;
			end
			S_LOAD_2: begin
				ld_in = 1'b1;
				ld_colour = 1'b1;
				ld_black = 1'b1;
			end
			S_ERASE: begin
				writeEn = 1'b1;
				enable = 1'b1;
				reset_erase = 1'b1;
				en_erase = 1'b1;
			end
			S_MOVE: begin
				en_cx = 1'b1;
				en_cy = 1'b1;
			end	
        endcase
    end // enable_signals
	
    // current_state registers
    always@(posedge clk) begin
        if(!reset_n)
            current_state <= S_INIT;
        else
            current_state <= next_state;
    end // state_FFS
endmodule
	
module ratedivider(enable, load_n, clk, reset_n, z);
	input enable, clk, reset_n;
	input [27:0] load_n;
	output reg z;
	
	reg [27:0] q;
	
	always @(posedge clk) begin
		if (reset_n == 1'b0)
			q <= load_n;
		else if (enable == 1'b1)
			begin
				if (q == 0)
					q <= load_n;
				else
					q <= q - 1'b1;
			end
	end
	
	always @(posedge clk) begin
		z <= (q == 1'b0) ? 1'b1 : 1'b0;
	end
endmodule

module xycounter(
	input [7:0] load_n, 
	input [7:0] bound_n, 
	input clk, reset_n, enable, 
	output reg [7:0] outpos
	);
	
	reg [7:0] boundary;
	reg dir;
	
	always @(posedge clk) begin
		if (reset_n == 1'b0) begin
			outpos <= load_n;
			boundary <= bound_n;
			dir = 1'b1;
		end
		else if (enable == 1'b1) begin
			if (dir == 1'b1) begin
				if (outpos == boundary) begin
					dir = 1'b0;
					outpos = outpos - 1'b1;
				end
				else 
					outpos = outpos + 1'b1;
			end
			if (dir == 1'b0) begin
				if (outpos == 1'b0) begin
					dir = 1'b1;
					outpos = outpos + 1'b1;
				end
				else 
					outpos = outpos - 1'b1;
			end
		end
	end
endmodule