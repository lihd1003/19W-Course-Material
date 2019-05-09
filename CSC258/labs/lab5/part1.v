module lab5part1(SW, KEY, HEX0, HEX1);
	input [1:0] SW;
	input [1:0] KEY;
	output [6:0] HEX0;
	output [6:0] HEX1;
	
	wire [7:0] w;
	eightbitcounter c(SW[1], KEY[0], SW[0], w);
	seven_seg_decoder hex1(w[7:4], HEX1);
	seven_seg_decoder hex0(w[3:0], HEX0);
endmodule

module eightbitcounter(enable, clk, clear_b, out);
	input enable, clk, clear_b;
	output [7:0] out;
	
	wire w0, w1, w2, w3, w4, w5, w6;
	assign w0 = enable & out[0];
	assign w1 = w0 & out[1];
	assign w2 = w1 & out[2];
	assign w3 = w2 & out[3];
	assign w4 = w3 & out[4];
	assign w5 = w4 & out[5];
	assign w6 = w5 & out[6];
	
	tffunit t0(enable, clk, clear_b, out[0]);
	tffunit t1(w0, clk, clear_b, out[1]);
	tffunit t2(w1, clk, clear_b, out[2]);
	tffunit t3(w2, clk, clear_b, out[3]);
	tffunit t4(w3, clk, clear_b, out[4]);
	tffunit t5(w4, clk, clear_b, out[5]);
	tffunit t6(w5, clk, clear_b, out[6]);
	tffunit t7(w6, clk, clear_b, out[7]);
	

endmodule

module tffunit(in, clk, clear_b, out);
	input clk, clear_b, in;
	output reg out;
	always @(posedge clk, negedge clear_b)
	begin
		if (clear_b == 1'b0)
			out <= 1'b0;
		else if (in == 1'b1)
			out <= ~out;
	end
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
