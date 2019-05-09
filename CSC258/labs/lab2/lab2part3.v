module seven_seg_decoder(SW, HEX0);
	input [3:0]SW;
	output [6:0]HEX0;
	
	assign HEX0[0] = (~SW[3] & ~SW[2] & ~SW[1] & SW[0]) | 
						  (~SW[3] & SW[2] & ~SW[1] & ~SW[0]) | 
						  (SW[3] & SW[2] & ~SW[1] & SW[0]) |
						  (SW[3] & ~SW[2] & SW[1] & SW[0]);
						  
	assign HEX0[1] = (SW[3] & SW[2] & ~SW[0]) | 
						  (~SW[3] & SW[2] & ~SW[1] & SW[0]) | 
						  (SW[3] & SW[1] & SW[0]) | 
						  (SW[2] & SW[1] & ~SW[0]);
						  
	assign HEX0[2] = (~SW[3] & ~SW[2] & SW[1] & ~SW[0]) | 
						  (SW[3] & SW[2] & ~SW[0]) | 
						  (SW[3] & SW[2] & SW[1]);
						  
	assign HEX0[3] = (~SW[3] & ~SW[2] & ~SW[1] & SW[0]) | 
						  (~SW[3] & SW[2] & ~SW[1] & ~SW[0]) | 
						  (SW[2] & SW[1] & SW[0]) | 
						  (SW[3] & ~SW[2] & SW[1] & ~SW[0]);
						  
	assign HEX0[4] = (~SW[3] & SW[0]) | 
						  (~SW[3] & SW[2] & ~SW[1]) | 
						  (~SW[2] & ~SW[1] & SW[0]);
						  
	assign HEX0[5] = (~SW[3] & ~SW[2] & SW[0]) | 
						  (~SW[3] & ~SW[2] & SW[1]) | 
						  (~SW[3] & SW[1] & SW[0]) | 
						  (SW[3] & SW[2] & ~SW[1] & SW[0]);
						  
	assign HEX0[6] = (~SW[3] & ~SW[2] & ~SW[1]) | 
						  (~SW[3] & SW[2] & SW[1] & SW[0]) | 
						  (SW[3] & SW[2] & ~SW[1] & ~SW[0]);
endmodule