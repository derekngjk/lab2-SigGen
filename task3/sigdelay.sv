module sigdelay # (
    parameter A_WIDTH = 9,
              D_WIDTH = 8
) (
    // interface signals
    input logic clk,
    input logic rst,
    input logic wr,  // write enable
    input logic rd,  // read enable
    input logic [A_WIDTH-1:0] offset,  // amount to offset the address
    input logic [D_WIDTH-1:0] mic_signal,  // data to write into RAM
    output logic [D_WIDTH-1:0] delayed_signal  // output data
);

logic [A_WIDTH-1:0] rad;  // read address, which is just the counter's output
logic [A_WIDTH-1:0] wad;  // write address, which is the counter's output + the offset

counter addrCounter (
    .clk (clk),
    .rst (rst),
    .count (rad)
);

// once we have the counter's output (read addr), add the offset to get write addr
assign wad = rad + offset;

dualPortRam audioRam (
    .clk (clk),
    .wen (wr),
    .ren (rd),
    .wad (wad),
    .rad (rad),
    .din (mic_signal),
    .dout (delayed_signal)
);

endmodule
