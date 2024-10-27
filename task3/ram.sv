module dualPortRam # (
    parameter ADDRESS_WIDTH = 9,
              DATA_WIDTH = 8
) (
    input logic clk,  // clk
    input logic wen,  // write en
    input logic ren,  // read en
    input logic [ADDRESS_WIDTH-1:0] wad,  // address to write to
    input logic [ADDRESS_WIDTH-1:0] rad,  // address to read from
    input logic [DATA_WIDTH-1:0] din,     // data to write in
    output logic [DATA_WIDTH-1:0] dout    // data which was read
);

// initialise ram array of length 2**9 = 512, and each element is 8 bits hence 512x8
logic [DATA_WIDTH-1:0] ram_arr [2**ADDRESS_WIDTH-1:0];

/*
Whenever on rising edge of clock, if the WEN is 1, write the din to the specified address
If read enable, assign dout to the stored value at the specified read address
*/
always_ff @ (posedge clk) begin
    if (wen == 1'b1)
        ram_arr[wad] <= din;
    if (ren == 1'b1)
        dout <= ram_arr[rad];
end

endmodule
