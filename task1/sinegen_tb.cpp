// include header files
#include "Vsinegen.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "vbuddy.cpp"


// purpose of the test bench is to simulate the behaviour of the module under specified input conditions
int main(int argc, char **argv, char **env) {
    // i is number of cycles to simulate. clk varies between 0 and 1 to emulate the clock signal on each cycle
    int i;
    int clk;

    Verilated::commandArgs(argc, argv);
    // create top verilog instance which is the device under test (DUT)
    Vsinegen* top = new Vsinegen;
    // turn on signal tracing, and tell verilator to dump the waveform data to counter.cvd
    Verilated::traceEverOn(true);
    VerilatedVcdC* tfp = new VerilatedVcdC;
    top -> trace(tfp, 99);
    tfp -> open("counter.vcd");

    // initialise vbuddy connection. port path is in vbuddy.cfg
    if (vbdOpen() != 1) return (-1);
    // note: will have error if the header name is too long
    vbdHeader("Lab 2: Sinegen");

    // set the initial conditions of the top-level module. Just use rst 0 and en 1 to count normally
    top -> clk = 1;
    top -> rst = 0;
    top -> en = 1;
    top -> incr = 1;  // increment by 1 each time

    // constant params
    const int max_vbd_val = 255;
    const int min_vbd_val = 0;
    const int max_incr = 20;
    const int min_incr = 1;

    // i is the index showing which clock cycle we are currently on. So simulate for 300 clock cycles
    for (i = 0; i < 30000; i++) {
        
        // on each cycle, we toggle clock two times so that clock switches between 0 and 1 on each cycle
        for (clk = 0; clk < 2; clk++) {
            // dump the variables into the vcd file
            tfp -> dump(2 * i + clk);  // units in ps
            top -> clk = !top -> clk;
            // eval basically sends the corresponding input signals into the module to update the output
            top -> eval();
        }

        // so basically on each cycle, the counter counts up by an amount incr
        // then this updated count value is used as the address so that on the next cycle,
        // the value stored at that address is used as dout
        // so now we just need to display dout to see how it changes
        vbdPlot(top -> dout, 0, 255);
        vbdCycle(i + 1);  // increment cycle number on vbuddy

        int vbd_val = vbdValue();
        double r = (vbd_val - (double)min_vbd_val) / (max_vbd_val - min_vbd_val);
        int new_incr = r * (max_incr - min_incr) + min_incr;
        
        top -> incr = new_incr;

        if (Verilated::gotFinish() || vbdGetkey() == 113) exit(0);  // exit if completed or 'q' key pressed

    }

    tfp -> close();
    exit(0);

}