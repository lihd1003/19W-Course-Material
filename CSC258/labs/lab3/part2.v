module fulladder(a, b, ci, co, s);
	input a;
	input b;
	input ci;
	output co;
	output s;
	
	assign s = ci ^ (a ^ b);
	assign co = ((a ^ b) & ci) | (~(a ^ b) & b);
endmodule

module ripplecarry(SW, LEDR);
	input [9:0] SW;
	output [9:0] LEDR;
	
	wire c0,c1,c2;
	
	fulladder fa1(.a(SW[0]), .b(SW[4]), .ci(SW[9]), .co(c0), .s(LEDR[0]));
	fulladder fa2(.a(SW[1]), .b(SW[5]), .ci(c0), .co(c1), .s(LEDR[1]));
	fulladder fa3(.a(SW[2]), .b(SW[6]), .ci(c1), .co(c2), .s(LEDR[2]));
	fulladder fa4(.a(SW[3]), .b(SW[7]), .ci(c2), .co(LEDR[9]), .s(LEDR[3]));
endmodule