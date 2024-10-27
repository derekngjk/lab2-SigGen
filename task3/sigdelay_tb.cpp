#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vsigdelay.h"

#include "vbuddy.cpp"     // include vbuddy code
#define MAX_SIM_CYC 1000000
#define ADDRESS_WIDTH 9
#define RAM_SZ pow(2,ADDRESS_WIDTH)  // 512

int main(int argc, char **argv, char **env) {
  int simcyc;     // simulation clock count
  int tick;       // each clk cycle has two ticks for two edges

  Verilated::commandArgs(argc, argv);
  // init top verilog instance
  Vsigdelay* top = new Vsigdelay;
  // init trace dump
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top->trace (tfp, 99);
  tfp->open ("sigdelay.vcd");
 
  // init Vbuddy
  if (vbdOpen()!=1) return(-1);
  vbdHeader("L2T3:Delay");
  //vbdSetMode(1);        // Flag mode set to one-shot

  // initialize simulation input 
  top->clk = 1;
  top->rst = 0;
  top->wr = 1;
  top->rd = 1;
  top->offset = 64;
  
  // intialize variables for analogue output
  /*
  This line informs vbuddy the size of the audio buffer to reserve to store the captured sample
  RAM_SZ is simply 512, since the ram we are using has size 512.
  So we just maintain a buffer of 512x8 audio signal, so that everytime we call vbdMicValue(),
  we get back the next 8-bit value in that buffer.
  */
  vbdInitMicIn(RAM_SZ);

  // run simulation for MAX_SIM_CYC clock cycles
  for (simcyc=0; simcyc<MAX_SIM_CYC; simcyc++) {
    // dump variables into VCD file and toggle clock
    for (tick=0; tick<2; tick++) {
      tfp->dump (2*simcyc+tick);
      top->clk = !top->clk;
      top->eval ();
    }

    /*
    So basically here, on every cycle, we first update top -> mic_signal (the data input) as vbdMicValue(),
    and we also update top -> offset to be the current value of the rotary encoder vbdValue(),
    so that on the next cycle, when we call evaluate, we will pass in the updated din and offset to the module,
    so that the ram will write the audio signal into the internal address computed by counter.
    Then at the same time, we take the dout of this current cycle (the delayed signal) and just plot it
    */

    top->mic_signal = vbdMicValue();
    top->offset = abs(vbdValue());     // adjust delay by changing incr

    // plot RAM input/output, send sample to DAC buffer, and print cycle count
    vbdPlot(int (top->mic_signal), 0, 255);
    vbdPlot(int (top->delayed_signal), 0, 255);
    vbdCycle(simcyc);

    // either simulation finished, or 'q' is pressed
    if ((Verilated::gotFinish()) || (vbdGetkey()=='q')) 
      exit(0);
  }

  vbdClose();     // ++++
  tfp->close(); 
  printf("Exiting\n");
  exit(0);
}
