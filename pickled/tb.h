#include "Variane.h"
#include "verilated.h"
#include <verilated_fst_c.h>

#include <iostream>
#include <stdlib.h>

#include "Variane.h"
#include "verilated.h"
#include <verilated_fst_c.h>

#include <iostream>
#include <stdlib.h>

#ifndef TESTBENCH_H
#define TESTBENCH_H

typedef Variane Module;

class Testbench {
 public:

  Testbench()
      : module_(new Module){
  }

  ~Testbench(void) { }

  void eval() {
    std::cout << "It hangs here..." << std::endl;
    module_->eval();
    std::cout << "This log is never reached." << std::endl;
  }

  std::unique_ptr<Module> module_;
};

#endif // TESTBENCH_H
