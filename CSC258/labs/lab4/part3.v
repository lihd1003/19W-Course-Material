module lab4part3(SW, KEY, LEDR);
	input [9:0] SW;
	input [3:0] KEY;
	output [7:0] LEDR;
	
	shift s(.load_val(SW[7:0]), .load_n(KEY[1]), .shiftRight(KEY[2]), .ASR(KEY[3]), .clk(KEY[0]), .reset_n(SW[9]), .shiftOut(LEDR[7:0]));
endmodule

module shift(load_val, load_n, shiftRight, ASR, clk, reset_n, shiftOut);
	input [7:0] load_val;
	input load_n;
	input shiftRight;
	input ASR;
	input clk;
	input reset_n;
	output [7:0] shiftOut;
	
	wire wm;
	wire w7;
	wire w6;
	wire w5;
	wire w4;
	wire w3;
	wire w2;
	wire w1;
	wire w0;
	
	mux2to1 m1(.x(1'b0), .y(w7), .s(ASR), .m(wm));
	ShifterBit sb7(.in(wm), .shift(shiftRight), .load_val(load_val[7]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w7));
	ShifterBit sb6(.in(w7), .shift(shiftRight), .load_val(load_val[6]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w6));
	ShifterBit sb5(.in(w6), .shift(shiftRight), .load_val(load_val[5]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w5));
	ShifterBit sb4(.in(w5), .shift(shiftRight), .load_val(load_val[4]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w4));
	ShifterBit sb3(.in(w4), .shift(shiftRight), .load_val(load_val[3]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w3));
	ShifterBit sb2(.in(w3), .shift(shiftRight), .load_val(load_val[2]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w2));
	ShifterBit sb1(.in(w2), .shift(shiftRight), .load_val(load_val[1]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w1));
	ShifterBit sb0(.in(w1), .shift(shiftRight), .load_val(load_val[0]), .load_n(load_n), .clk(clk), .reset_n(reset_n), .sbout(w0));
	assign shiftOut[7:0] = {w7,w6,w5,w4,w3,w2,w1,w0};
	
endmodule

module ShifterBit(in, shift, load_val, load_n, clk, reset_n, sbout);
	input in;
	input shift;
	input load_val;
	input load_n;
	input clk;
	input reset_n;
	
	wire out;
	output sbout;
	
	wire w1;
	wire w2;
	
	mux2to1 m1(.x(out), .y(in), .s(shift), .m(w1));
	mux2to1 m2(.x(load_val), .y(w1), .s(load_n), .m(w2));
	flipflop f1(.d(w2), .q(out), .clock(clk), .reset_n(reset_n));
	
	assign sbout = out;
	
endmodule

module mux2to1(x, y, s, m);
    input x; //selected when s is 0
    input y; //selected when s is 1
    input s; //select signal
    output m; //output
  
    assign m = s & y | ~s & x;
endmodule

module flipflop(d, q, clock, reset_n);
	input d;
	reg q_r;
	output q;
	input clock;
	input reset_n;
	
	always @(posedge clock)
	
	begin
		if (reset_n == 1'b0)
			q_r <= 0;
		else 
			q_r <= d;
	end
	assign q = q_r;
endmodule