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
    top -> incr = 5;  // increment by 5 each time so to have a higher constant frequency
    top -> offset = vbdValue();  // initialise the offset to the vbdvalue

    // i is the index showing which clock cycle we are currently on
    for (i = 0; i < 30000; i++) {
        
        // on each cycle, we toggle clock two times so that clock switches between 0 and 1 on each cycle
        for (clk = 0; clk < 2; clk++) {
            // dump the variables into the vcd file
            tfp -> dump(2 * i + clk);  // units in ps
            top -> clk = !top -> clk;
            // eval basically sends the corresponding input signals into the module to update the output
            top -> eval();
        }

        /*
        So basically, every cycle we plot the two output signals dout1 and dout2
        Then, we also update top -> offset to be the current vbdvalue
        So that on the next cycle, when we evaluate again, 
        the top module will take into account the new offset when computing addr2
        */

        vbdPlot(top -> dout1, 0, 255);
        vbdPlot(top -> dout2, 0, 255);
        vbdCycle(i + 1);  // increment cycle number on vbuddy
        top -> offset = vbdValue();

        if (Verilated::gotFinish() || vbdGetkey() == 113) exit(0);  // exit if completed or 'q' key pressed

    }

    tfp -> close();
    exit(0);

}