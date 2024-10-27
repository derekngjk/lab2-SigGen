module rom # (
    parameter ADDRESS_WIDTH = 8,
              DATA_WIDTH = 8
) (
    input logic                     clk,
    input logic [ADDRESS_WIDTH-1:0] addr,
    output logic [DATA_WIDTH-1:0]   dout
);

// declare an array of length 2 ** address_width, where each element has bit width data_width
logic [DATA_WIDTH-1:0] rom_arr [2**ADDRESS_WIDTH-1:0];

// initialisation, done once at the beginning
// populate the array with values from sinerom.mem
initial begin
    $display("Loading rom.");
    $readmemh("sinerom.mem", rom_arr);
end;

// synchronous rom: whenever there is a rising clock edge, assign the output to whatever is stored at the input address
always_ff @ (posedge clk)
    dout <= rom_arr[addr];

endmodule
