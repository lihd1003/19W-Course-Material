module lab5part3(SW, KEY, CLOCK_50, LEDR);
	input [2:0] SW;
	input [1:0] KEY;
	input CLOCK_50;
	output [1:0] LEDR;
	morse m(SW[2:0], KEY[1], KEY[0], CLOCK_50, LEDR[0]);
endmodule

module morse(in, load_n, reset_n, clk, out);
	input [2:0] in;
	input load_n, reset_n, clk;
	output out;
	reg [2:0] lutin;

	wire [15:0] load_val;
	wire [27:0] q;
	reg shiftRight;
	always @(negedge load_n, posedge clk)
	begin
		if (load_n == 1'b0)
		begin
			shiftRight = 0;
			lutin = in;
		end
		else if (q == 0)
			shiftRight = 1;
		else
			shiftRight = 0;
	end
	lut l(lutin, load_val);
	shift s(load_val, load_n, shiftRight, clk, reset_n, out);
	ratedivider r(1'b1, 28'd24999999, clk, reset_n, q);
endmodule

module ratedivider(enable, load_n, clk, reset_n, q);
	input enable, clk, reset_n;
	input [27:0] load_n;
	output reg [27:0] q;
	
	always @(posedge clk, negedge reset_n)
	begin
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
endmodule

module lut(in, out);
	input [2:0] in;
	output reg [15:0] out;
	always @(*)
	begin
		case(in)
			3'b000: out = 16'b1010100000000000;
			3'b001: out = 16'b1110000000000000;
			3'b010: out = 16'b1010111000000000;
			3'b011: out = 16'b1010101110000000;
			3'b100: out = 16'b1011101110000000;
			3'b101: out = 16'b1110101011100000;
			3'b110: out = 16'b1110101110111000;
			3'b111: out = 16'b1110111010100000;
		endcase
	end
endmodule

module shift(load_val, load_n, shiftRight, clk, reset_n, shiftOut);
	input [15:0] load_val;
	input load_n, shiftRight, clk, reset_n;
	output reg shiftOut;
	
	reg [15:0] q;
	always @(posedge clk, negedge reset_n, negedge load_n)
	begin
		if (reset_n == 0)
			begin
			shiftOut <= 0;
			q <= 16'b0;
			end
		else if (load_n == 0)
			begin
			shiftOut = 0;
			q <= load_val;
			end
		else if (shiftRight == 1)
			begin
			shiftOut <= q[15];
			q <= (q << 1'b1);
			end
	end
	
endmodule

