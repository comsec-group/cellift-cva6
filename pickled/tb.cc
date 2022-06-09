#include "tb.h"

static bool do_trace = true;

int main(int argc, char **argv, char **env) {

  Verilated::commandArgs(argc, argv);

  // Instantiate the DUT instance
  Testbench *tb = new Testbench();

  tb->eval();
  tb->module_->clk_i = 1;
  tb->eval();
  tb->module_->clk_i = 0;
  tb->eval();
  tb->module_->clk_i = 1;

  std::cout << "Testbench complete!" << std::endl;
  
  delete tb;
  exit(0);
}
