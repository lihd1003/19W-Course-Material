module lab7part2_a(
	input clk, resetn, load_x, go,
	input [7:0] in,
	input [2:0] colour,
	output [7:0] x_out,
	output [6:0] y_out,
	output [2:0] colour_out,
	output plot
	);
	
	wire ld_x, ld_y, ld_color, ld_alu_out;
	wire [1:0] alu_op;
	
	datapath d(in, 
			   colour, 
			   resetn, 
			   ld_x, ld_y, ld_color, ld_alu_out,
			   alu_op,
			   clk,
			   x_out,
			   y_out,
			   colour_out
			  );
	
	control c(clk, resetn, load_x, go,
			  ld_alu_out, ld_x, ld_y, ld_color,
			  alu_op,
			  plot
			 );
			  
endmodule


module control(
	input clk, resetn, load_x, go,
	
	output reg ld_alu_out, ld_x, ld_y, ld_color,
	output reg [1:0] alu_op,
	output reg plot
	);
	
	reg [3:0] current_state, next_state;
	reg [1:0] count_plot;
	reg [1:0] count_switchline;
	
	localparam S_LOAD = 4'd0;
			   S_LOAD_WAIT = 4'd1;
			   S_GO = 4'd2;
			   S_PLOT = 4'd3;
			   S_SWITCHCOL = 4'd4;
			   S_SWITCHROW = 4'd5;
	
	always @(*) begin 
		case (current_state)
			S_LOAD: next_state = load_x ? S_LOAD_WAIT : S_LOAD;
			S_LOAD_WAIT: next_state = go ? S_GO : S_LOAD_WAIT;
			S_GO: next_state = S_PLOT;
			S_PLOT: begin
				if (count_plot == 2'd0)
					next_state = S_SWITCHROW;
				else if (count_switchline == 2'd0)
					next_state = S_LOAD
				else
					next_state = S_SWITCHCOL;
			end
			S_SWITCHCOL: next_state = S_PLOT;
			S_SWITCHROW: next_state = S_PLOT;
		endcase
	end
	
	always @(*) begin
		ld_alu_out = 1'b0;
		ld_x = 1'b0;
		ld_y = 1'b0;
		ld_color = 1'b0;
		alu_op = 2'd0;
		plot = 1'b0;
		
		case (current_state)
			S_LOAD: ld_x = 1'b1;
			S_GO: begin
				ld_y = 1'b1;
				ld_color = 1'b1;
				count_plot <= 2'd2;
				count_switchline <= 2'd2;
			end
			S_PLOT: begin
				plot = 1'b1;
			end
			S_SWITCHCOL: begin
				ld_x = 1'd1;
				ld_y = 1'd1;
				ld_alu_out = 1'd1;
				alu_op = 2'd1;
				count_plot <= count_plot - 1;
			end
			S_SWITCHROW: begin
				ld_x = 1'd1;
				ld_y = 1'd1;
				ld_alu_out = 1'd1;
				alu_op = 2'd2;
				count_switchline <= count_switchline - 1;
			end
		endcase
	end
	
	always @(posedge clk) begin
		if (!resetn)
			current_state <= S_LOAD;
		else 
			current_state <= next_state;
	end
endmodule


module datapath(
	input [7:0] in, 
	input [2:0] colour,
	input resetn,
	input ld_x, ld_y, ld_color, ld_alu_out,
	input [1:0] alu_op,
	input clk,
	output reg [7:0] x_out,
	output reg [6:0] y_out,
	output reg [2:0] colour_out
	);
	
	reg [7:0] x_pos;
	reg [6:0] y_pos;
	
	always @(posedge clk) begin
		if (!resetn) begin
			x_pos_s <= 8'd0;
			y_pos_s <= 7'd0;
			colour_out <= 3'd0;
		end
		else begin
			if (ld_x)
				x_pos <= ld_alu_out ? x_out : in;
			if (ld_y)
				y_pos <= ld_alu_out ? y_out : in;
			if (ld_color)
				colour_out <= colour;
		end
	end
	
	always @(*) begin
		case (alu_op)
			2'b01: begin
				x_out = x_pos + 1'b1;
				y_out = y_pos;
			end
			2'b10: begin
				x_out = x_pos - 2'd2;
				y_out = y_pos + 1'b1;
			end
			default: 
				x_out = x_pos;
				y_out = y_pos;
		endcase
	end
endmodule