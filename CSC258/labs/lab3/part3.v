module alu(SW, KEY, LEDR, HEX0, HEX1, HEX2, HEX3, HEX4, HEX5);
	input [7:0] SW;
	input [2:0] KEY;
	
	output [7:0] LEDR;
	output [6:0] HEX0;
	output [6:0] HEX1;
	output [6:0] HEX2;
	output [6:0] HEX3;
	output [6:0] HEX4;
	output [6:0] HEX5;
	
	wire [7:0] w0;
	wire [7:0] w1;
	wire [7:0] w2;
	wire [7:0] w3;
	wire [7:0] w4;
	wire [7:0] w5;
	
	assign HEX1[6:0] = 7'b0000001;
	assign HEX3[6:0] = 7'b0000001;
	
	reg [7:0] alu;
	
	always @(*)
	begin
		case(KEY[2:0])
			3'b000: alu = w0;
			3'b001: alu = w1;
			3'b010: alu = w2;
			3'b011: alu = w3;
			3'b100: alu = w4;
			3'b101: alu = w5;
			default: alu = 8'b00000000;
		endcase
	end
	
	assign LEDR[7:0] = alu[7:0]; 
	
	seven_seg_decoder h1(.in(alu[7:4]), .out(HEX5));
	seven_seg_decoder h2(.in(alu[3:0]), .out(HEX4));
	seven_seg_decoder h3(.in(SW[3:0]), .out(HEX0));
	seven_seg_decoder h4(.in(SW[7:4]), .out(HEX2));
	ripplecarry r1(.a(SW[7:4]), .b(4'b0001), .out(w0));
	ripplecarry r2(.a(SW[7:4]), .b(SW[3:0]), .out(w1));
	add r3(.a(SW[7:4]), .b(SW[3:0]), .out(w2));
	xoror r4(.a(SW[7:4]), .b(SW[3:0]), .out(w3));
	bitor r5(.a(SW[7:4]), .b(SW[3:0]), .out(w4));
	cat r6(.a(SW[7:4]), .b(SW[3:0]), .out(w5));
endmodule

module seven_seg_decoder(in, out);
	input [3:0]in;
	output [6:0]out;
	
	assign out[0] = (~in[3] & ~in[2] & ~in[1] & in[0]) | 
						  (~in[3] & in[2] & ~in[1] & ~in[0]) | 
						  (in[3] & in[2] & ~in[1] & in[0]) |
						  (in[3] & ~in[2] & in[1] & in[0]);
						  
	assign out[1] = (in[3] & in[2] & ~in[0]) | 
						  (~in[3] & in[2] & ~in[1] & in[0]) | 
						  (in[3] & in[1] & in[0]) | 
						  (in[2] & in[1] & ~in[0]);
						  
	assign out[2] = (~in[3] & ~in[2] & in[1] & ~in[0]) | 
						  (in[3] & in[2] & ~in[0]) | 
						  (in[3] & in[2] & in[1]);
						  
	assign out[3] = (~in[3] & ~in[2] & ~in[1] & in[0]) | 
						  (~in[3] & in[2] & ~in[1] & ~in[0]) | 
						  (in[2] & in[1] & in[0]) | 
						  (in[3] & ~in[2] & in[1] & ~in[0]);
						  
	assign out[4] = (~in[3] & in[0]) | 
						  (~in[3] & in[2] & ~in[1]) | 
						  (~in[2] & ~in[1] & in[0]);
						  
	assign out[5] = (~in[3] & ~in[2] & in[0]) | 
						  (~in[3] & ~in[2] & in[1]) | 
						  (~in[3] & in[1] & in[0]) | 
						  (in[3] & in[2] & ~in[1] & in[0]);
						  
	assign out[6] = (~in[3] & ~in[2] & ~in[1]) | 
						  (~in[3] & in[2] & in[1] & in[0]) | 
						  (in[3] & in[2] & ~in[1] & ~in[0]);
endmodule

module fulladder(a, b, ci, co, s);
	input a;
	input b;
	input ci;
	output co;
	output s;
	
	assign s = ci ^ (a ^ b);
	assign co = ((a ^ b) & ci) | (~(a ^ b) & b);
endmodule

module ripplecarry(a, b, out);
	input [3:0] a;
	input [3:0] b;
	output [7:0] out;
	
	wire c0,c1,c2;
	
	
	fulladder fa1(.a(a[0]), .b(b[0]), .ci(1'b0), .co(c0), .s(out[0]));
	fulladder fa2(.a(a[1]), .b(b[1]), .ci(c0), .co(c1), .s(out[1]));
	fulladder fa3(.a(a[2]), .b(b[2]), .ci(c1), .co(c2), .s(out[2]));
	fulladder fa4(.a(a[3]), .b(b[3]), .ci(c2), .co(out[4]), .s(out[3]));
	assign out[7:5] = 3'b000;
endmodule

module add(a, b, out);
	input [3:0] a;
	input [3:0] b;
	output [7:0] out;
	assign out[7:0] = a+b;
endmodule

module xoror(a, b, out);
	input [3:0] a;
	input [3:0] b;
	output [7:0] out;
	assign out[3:0] = a ^ b;
	assign out[7:4] = a | b;
endmodule

module bitor(a, b, out);
	input [3:0] a;
	input [3:0] b;
	output [7:0] out;
	assign out[0] = | {a,b};
	assign out[7:1] = 7'b0000000;
endmodule
	
module cat(a, b, out);
	input [3:0] a;
	input [3:0] b;
	output [7:0] out;
	assign out[3:0] = b[3:0];
	assign out[7:4] = a[3:0];
endmodule