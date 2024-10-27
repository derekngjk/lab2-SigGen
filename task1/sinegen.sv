module sinegen # (
    parameter A_WIDTH = 8,
              D_WIDTH = 8
) (
    // interface signals
    input logic clk,
    input logic rst,
    input logic en,
    input logic [D_WIDTH-1:0] incr,  // increment for the counter (how much we want it to increment each time)
    output logic [D_WIDTH-1:0] dout  // dout which is the data stored in the address
);

/*
Basically, what we want to do with this module is that we want to use the counter to count up
Each time the counter goes up, it goes up by the amount incr
Then, we use the output of the counter as the address of the rom
The overall dout of this top level module is just the dout of the rom at that address specified by the counter
So for example on cycle i = 5, the counter outputs 0x5, corresponding to value FD.
Then on cycle i = 6, the dout is set to FD, and the counter outputs 0x6 (assuming incr 1)
i.e. on cycle i = 6, the dout will be set to the value at address 0x5, NOT 0x6
*/

logic [A_WIDTH-1:0] addr;  // interconnect wire

counter addrCounter (
    .clk (clk),
    .rst (rst),
    .en (en),
    .incr (incr),
    .count (addr)
);

rom sineRom (
    .clk (clk),
    .addr (addr),
    .dout (dout)
);

endmodule
