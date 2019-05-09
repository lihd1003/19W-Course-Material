module part3(SW, CLOCK_50, KEY, HEX0, HEX1, HEX2, HEX3, HEX4, HEX5);
	input [7:0] SW;
	input [1:0] KEY;
	input CLOCK_50;
	output [6:0] HEX0, HEX1, HEX2, HEX3, HEX4, HEX5;

	wire go;
	wire resetn;
	wire [7:0] data_result;

	assign go = ~KEY[1];
	assign resetn = KEY[0];
	
	dividor d0(
		CLOCK_50,
		resetn,
		go,
		SW[7:0],
		data_result
	);
	
	hex_decoder h0(SW[3:0], HEX0);
	assign HEX1 = 7'b0000001;
	hex_decoder h2(SW[7:4], HEX2);
	assign HEX3 = 7'b0000001;
	hex_decoder h4(data_result[3:0], HEX4);
	hex_decoder h5(data_result[7:4], HEX5);

endmodule


module dividor(
	input clk, 
	input resetn, 
	input go,
	input [7:0] data_in,
	output [7:0] data_result
	);
	
	wire ld_a, ld_ds, ld_de, ld_r, ld_alu_out, ld_q, ld_set;
	wire [1:0] alu_select_a, alu_select_b, alu_op;
	wire q0;
	
	datapath d0(
		clk, resetn, 
		data_in,  
		ld_a, ld_d, ld_alu_out, ld_q, ld_set, ld_ds,
		alu_op, 
		ld_r,
		data_result,
		q0
	);
	
	control c0(
		clk, q0, resetn, go,
		ld_a, ld_d, ld_alu_out, ld_q, ld_set, ld_ds,
		alu_op,
		ld_r
	);
	
endmodule

module control(
	input clk, q0, resetn, go,
	output reg ld_a, ld_d, ld_alu_out, ld_q, ld_set, ld_ds,
	output reg [1:0] alu_op,
	output reg ld_r
	);
	
	reg [2:0] iterate;
	reg [3:0] current_state, next_state;
	
	localparam LOAD = 4'd0, 
			   LOAD_WAIT = 4'd1,
	           SHIFT = 4'd2,
			   SUBTRACT = 4'd3,
			   SET = 4'd4,
			   ADD = 4'd5;
			   
	always @(*)
	begin
		case (current_state)
			LOAD: next_state = go ? LOAD_WAIT : LOAD;
			LOAD_WAIT: next_state = go ? LOAD_WAIT : SHIFT;
			SHIFT: next_state = SUBTRACT;
			SUBTRACT: next_state = SET;
			SET: next_state = ADD;
			ADD: begin 
				if (iterate == 3'b100) 
					next_state = LOAD;
				else 
					next_state = SHIFT;
			end 
			default: next_state = LOAD;
		endcase
	end
	
	always @(*)
	begin: enable_signals
		ld_a = 1'b0;
		ld_d = 1'b0; 
		ld_alu_out = 1'b0; 
		ld_q = 1'b0;
		ld_set = 1'b0;
		alu_op = 2'd0;
		ld_r = 1'b0;
		
		case (current_state)
			LOAD: begin 
				ld_a = 1'b1;
				ld_d = 1'b1;
				ld_ds = 1'b1;
				iterate = 2'd0;
			end
			SHIFT: begin
				ld_alu_out = 1'b1;
				ld_a = 1'b1;
				ld_d = 1'b1;
				alu_op = 2'd1;
				iterate = iterate + 1;
			end
			SUBTRACT: begin
				ld_alu_out = 1'b1;
				ld_a = 1'b1;
				alu_op = 2'd2;
				ld_set = 1'b1;
			end
			SET: ld_q = 1'b1;				
			ADD: begin
				if (q0 == 1'b1) begin
					ld_alu_out = 1'b1;
					ld_a = 1'b1;
					alu_op = 2'd3;
				end 
				if (iterate == 3'b100)
					ld_r = 1'b1;
			end

		endcase
	end
	
	always @(posedge clk)
    begin: state_FFs
        if(!resetn)
            current_state <= LOAD;
        else
            current_state <= next_state;
    end
	
endmodule

module datapath(
	input clk, 
	input resetn, 
	input [7:0] data_in,
	input ld_a, ld_d, ld_alu_out, ld_q, ld_set, ld_ds,
	input [1:0] alu_op,
	input ld_r, 
	output reg [7:0] data_result,
	output reg q0
	);
	
	reg [4:0] divisor;
	reg [4:0] a;
	reg [3:0] divided;
	
	reg [4:0] alu_a;
	reg [3:0] alu_b;
	
	always @(posedge clk) begin
		if (!resetn) begin
			divisor <= 5'd0;
			a <= 5'd0;
			divided <= 4'd0;
		end
		else begin
			if (ld_q)
				divided[0] <= ~a[4];
			if (ld_a)
				a <= ld_alu_out ? alu_a : 5'd0;
			if (ld_d)
				divided <= ld_alu_out ? alu_b : data_in[7:4];
			if (ld_ds)
				divisor <= data_in[3:0];
			if (ld_set)
				q0 <= alu_a[4];
		end
	end
	
	always @(posedge clk) begin
		if (!resetn) begin
			data_result <= 8'd0;
		end
		else begin
			
			if (ld_r)
				data_result <= {alu_a[3:0], alu_b};
		end
	end
	
	always @(*) begin
		case (alu_op) 
			2'd1: {alu_a, alu_b} = {a, divided} << 1'b1;
			2'd2: begin
				alu_a = a - divisor;
				alu_b = divided;
			end
			2'd3: begin
				alu_a = a + divisor;
				alu_b = divided;
			end
			default: begin
				alu_a = a;
				alu_b = divided;
			end
		endcase
	end
endmodule

module hex_decoder(hex_digit, segments);
    input [3:0] hex_digit;
    output reg [6:0] segments;
   
    always @(*)
        case (hex_digit)
            4'h0: segments = 7'b100_0000;
            4'h1: segments = 7'b111_1001;
            4'h2: segments = 7'b010_0100;
            4'h3: segments = 7'b011_0000;
            4'h4: segments = 7'b001_1001;
            4'h5: segments = 7'b001_0010;
            4'h6: segments = 7'b000_0010;
            4'h7: segments = 7'b111_1000;
            4'h8: segments = 7'b000_0000;
            4'h9: segments = 7'b001_1000;
            4'hA: segments = 7'b000_1000;
            4'hB: segments = 7'b000_0011;
            4'hC: segments = 7'b100_0110;
            4'hD: segments = 7'b010_0001;
            4'hE: segments = 7'b000_0110;
            4'hF: segments = 7'b000_1110;   
            default: segments = 7'h7f;
        endcase
endmodule