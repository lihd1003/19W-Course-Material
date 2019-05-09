module lab4part2(SW, KEY, LEDR, HEX0, HEX1, HEX2, HEX3, HEX4, HEX5);
	input [9:0] SW;
	input [1:0] KEY;
	output [7:0] LEDR;
	output [7:0] HEX0; 
	output [7:0] HEX1; 
	output [7:0] HEX2; 
	output [7:0] HEX3; 
	output [7:0] HEX4;
	output [7:0] HEX5;
	
	assign HEX1[6:0] = 7'b1111111;
	assign HEX2[6:0] = 7'b1111111;
	assign HEX3[6:0] = 7'b1111111;
	
	alu a(.data(SW[3:0]), .clock(KEY[0]), .reset(SW[9]), .func(SW[7:5]), 
		  .ledout(LEDR[7:0]), .hexdata(HEX0), .hexout1(HEX4), .hexout2(HEX5));

endmodule

module alu(data, clock, reset, func, ledout, hexdata, hexout1, hexout2);
	input [3:0] data;
	input clock;
	input reset;
	input [2:0] func;
	output [7:0] ledout;
	output [7:0] hexdata;
	output [7:0] hexout1;
	output [7:0] hexout2;
	
	wire [7:0] w0;
	wire [7:0] w1;
	
	reg [7:0] alu;
	
	reg [7:0] q;
	
	always @(*)
	begin
		case(func[2:0])
			3'b000: alu = w0;
			3'b001: alu = w1;
			3'b010: alu = {3'b000, data[3:0] + q[3:0]};
			3'b011: alu = {data[3:0] | q[3:0], data[3:0] ^ q[3:0]};
			3'b100: alu = {7'b0000000, |{q[3:0],data[3:0]}};
			3'b101: alu = {4'b0000,q[3:0]} << {4'b0000, data[3:0]};
			3'b110: alu = {4'b0000,q[3:0]} >> {4'b0000, data[3:0]};
			3'b111: alu = data[3:0] * q[3:0];
			default: alu = 8'b00000000;
		endcase
	end
	
	always @(posedge clock)
	
	begin
		if (reset == 1'b0)
			q <= 0;
		else
			q <= alu;
	end
	
	seven_seg_decoder hex0(.in(data[3:0]), .out(hexdata));
	seven_seg_decoder hex4(.in(q[3:0]), .out(hexout1));
	seven_seg_decoder hex5(.in(q[7:4]), .out(hexout2));
	
	ripplecarry r0(.a(data[3:0]), .b(4'b0001), .out(w0));
	ripplecarry r1(.a(data[3:0]), .b(q[3:0]), .out(w1));
	
	assign ledout = q;
	
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
