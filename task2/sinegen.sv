module sinegen # (
    parameter A_WIDTH = 8,
              D_WIDTH = 8
) (
    // interface signals
    input logic clk,
    input logic rst,
    input logic en,
    input logic [D_WIDTH-1:0] incr,  // increment for the counter (how much we want it to increment each time)
    input logic [D_WIDTH-1:0] offset,
    output logic [D_WIDTH-1:0] dout1,
    output logic [D_WIDTH-1:0] dout2
);

/*
Task 2: basically we modify the ROM such that it is a dual port ROM.
The first read port is simply the counter's output, as before.
However, now we modify the top level module to also take in another input offset,
such that the address of the second port, is going to be the counter's output, added together with the offset
Hence, by doing so, we are able to sample the same sinusoidal wave at two different locations,
thus generating two sine waves which are out of phase.
We can interface this with the vbuddy by passing in the vbuddy's vbdvalue as the offset input
*/

logic [A_WIDTH-1:0] addr1;  // interconnect wire
logic [A_WIDTH-1:0] addr2;

counter addrCounter (
    .clk (clk),
    .rst (rst),
    .en (en),
    .incr (incr),
    .count (addr1)
);

// once we have the counter's output, add it together with the offset
assign addr2 = addr1 + offset;

rom sineRom (
    .clk (clk),
    .addr1 (addr1),
    .addr2 (addr2),
    .dout1 (dout1),
    .dout2 (dout2)
);

endmodule
