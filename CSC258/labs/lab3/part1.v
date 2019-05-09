module mux7to1(SW, LEDR);
	input [9:0] SW;
	output [1:0] LEDR; 
	
	reg Out;
	always @(*)
	begin
		case(SW[9:7])
			3'b000: Out = SW[0];
			3'b001: Out = SW[1];
			3'b010: Out = SW[2];
			3'b011: Out = SW[3];
			3'b100: Out = SW[4];
			3'b101: Out = SW[5];
			3'b110: Out = SW[6];
			default:Out = 1'b0;
		endcase
	end
	assign LEDR[0] = Out;
endmodule