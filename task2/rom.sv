// task 2: we want to make it a dual port ROM instead of just a single port
module rom # (
    parameter ADDRESS_WIDTH = 8,
              DATA_WIDTH = 8
) (
    input logic                     clk,
    input logic [ADDRESS_WIDTH-1:0] addr1,
    input logic [ADDRESS_WIDTH-1:0] addr2,
    output logic [DATA_WIDTH-1:0]   dout1,
    output logic [DATA_WIDTH-1:0]   dout2
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
always_ff @ (posedge clk) begin
    dout1 <= rom_arr[addr1];
    dout2 <= rom_arr[addr2];
end

endmodule
