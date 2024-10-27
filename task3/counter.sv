module counter # (
    parameter WIDTH = 9  // set the default value for WIDTH, can be overridden during instantiation
) (
    // interface signals: in the perspective of the top-level module instantiating counter, this is what it inputs and outputs
    input logic clk,                // clock
    input logic rst,                // reset
    output logic [WIDTH-1:0] count  // count output of with WIDTH number of bits
);

always_ff @ (posedge clk)
    // if reset, set count to 0, else increment by 1
    if (rst) count <= {WIDTH{1'b0}};
    else count <= count + {{WIDTH-1{1'b0}}, 1'b1};

endmodule
