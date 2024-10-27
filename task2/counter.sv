module counter # (
    parameter WIDTH = 8  // set the default value for WIDTH, can be overridden during instantiation
) (
    // interface signals: in the perspective of the top-level module instantiating counter, this is what it inputs and outputs
    input logic clk,                // clock
    input logic rst,                // reset
    input logic en,                 // enable
    input logic [WIDTH-1:0] incr,    // amount to increment each time
    output logic [WIDTH-1:0] count  // count output of with WIDTH number of bits
);

always_ff @ (posedge clk)
    // if reset, set count to 0
    // else if enable, increment count by amount incr
    // else if enable false, do nothing to count
    if (rst) count <= {WIDTH{1'b0}};
    else if (en) count <= count + incr;

endmodule
