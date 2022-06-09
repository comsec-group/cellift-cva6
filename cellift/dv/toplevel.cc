// Copyright Flavien Solt, ETH Zurich.
// Licensed under the General Public License, Version 3.0, see LICENSE for details.
// SPDX-License-Identifier: GPL-3.0-only

#if VM_TRACE
#include "Variane_tiny_soc__Syms.h"
#include "Variane_tiny_soc___024root.h"
#endif // VM_TRACE
#include "testbench.h"
#include "ticks.h"

static unsigned long run_test(Testbench *tb, int simlen) {
    return tb_run_ticks(tb, simlen);
}

int main(int argc, char **argv, char **env) {

  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(VM_TRACE);

  ////////
  // Get the env vars early to avoid Verilator segfaults.
  ////////

  std::cout << "Starting getting env variables." << std::endl;

  int simlen = get_sim_length_cycles(LEADTICKS);

  ////////
  // Initialize testbench.
  ////////

  Testbench *tb = new Testbench(cl_get_tracefile());

  ////////
  // Run the lead ticks.
  ////////

  // Do not count the leading ticks in the duration.
  bool benchmark_complete_during_lead_ticks = false;
  tb->reset();
  benchmark_complete_during_lead_ticks = tb->tick(ARIANE_FLUSH_TICKS); // During the cache flush.
  assert(!benchmark_complete_during_lead_ticks);

  ////////
  // Run the testbench.
  ////////

  unsigned int duration = run_test(tb, simlen);

  ////////
  // Display the results.
  ////////
#if !VM_TRACE
  std::cout << "Testbench complete!" << std::endl;
  std::cout << "Elapsed time: " << std::dec << duration << "." << std::endl;
#else // VM_TRACE
  std::cout << "Testbench with traces complete!" << std::endl;
  std::cout << "Elapsed time (traces enabled): " << std::dec << duration << "." << std::endl;
#endif // VM_TRACE

  delete tb;
  exit(0);
}
