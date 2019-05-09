//SW[2:0] data inputs
//SW[9] select signal

//LEDR[0] output display

module mux(LEDR, SW);
    input [9:0] SW;
    output [9:0] LEDR;

    mux4to1 u0(
	.u(SW[3]), 
	.v(SW[2]),
	.w(SW[1]),
	.x(SW[0]),
	.s0(SW[8]),
	.s1(SW[9]),
	.m(LEDR[0])       
        );
endmodule

module mux2to1(x, y, s, m);
    input x; //selected when s is 0
    input y; //selected when s is 1
    input s; //select signal
    output m; //output
  
    assign m = s & y | ~s & x;
    // OR
    // assign m = s ? y : x;

endmodule

module mux4to1(u,v,w,x,s0,s1,m);
    input u; // selected when s1s0=00
    input v; // selected when s1s0=01
    input w; // selected when s1s0=10
    input x; // selected when s1s0=11
    input s0; // select signal
    input s1; // select signal
    output m; 
    wire c0;  
    wire c1;
    
    mux2to1 mux0(u,v,s0,c0);
    mux2to1 mux1(w,x,s0,c1);
    mux2to1 mux2(c0,c1,s1,m);

endmodule