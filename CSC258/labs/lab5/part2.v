module lab5part2(SW, HEX0, CLOCK_50);
	input CLOCK_50;
	input [9:0] SW; // 1:0: frequency; 2: enable; 3: reset_n; 7:4: load_n; 9: load
	output [6:0] HEX0;
	
	counter c(SW[2], SW[7:4], SW[9], CLOCK_50, SW[3], SW[1:0], HEX0);
endmodule

module counter(enable, load_n, load, clk, reset_n, rate, hexout);
	input clk, enable, load, reset_n;
	input [1:0] rate;
	input [3:0] load_n;
	output [6:0] hexout;
	
	wire [27:0] w;
	wire [3:0] binaryout;
	reg disenable;
	reg [27:0] countdown;
	
	always @(posedge clk)
	begin
		if (w == 1'b0)
			disenable = 1;
		else
			disenable = 0;
		case (rate)
			2'b00: countdown <= 28'd1;
			2'b01: countdown <= 28'd49999999;
			2'b10: countdown <= 28'd99999999;
			2'b11: countdown <= 28'd199999999;
		endcase
	end
	
	
	displaycounter d(disenable, load_n, load, clk, reset_n, binaryout);
	ratedivider r(enable, countdown, clk, reset_n, w);
	seven_seg_decoder s(binaryout, hexout);
endmodule


module displaycounter(enable, load_n, load, clk, reset_n, q);
	input enable, clk, load, reset_n;
	input [3:0] load_n;
	output reg [3:0] q;
	
	always @(posedge clk, negedge reset_n)
	begin
		if (reset_n == 1'b0)
			q <= 4'd0;
		else if (load == 1'b1)
			q <= load_n;
		else if (enable == 1'b1)
			begin
				q <= q + 1'b1;
			end
	end
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
