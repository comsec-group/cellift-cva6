// Copyright Flavien Solt, ETH Zurich.
// Licensed under the General Public License, Version 3.0, see LICENSE for details.
// SPDX-License-Identifier: GPL-3.0-only

// Toplevel module with taints.

module ariane_tiny_soc #(
    parameter int unsigned NumTaints = 1,

    parameter int unsigned NumWords = 1 << 17,
    parameter int unsigned AddrWidth = 64,
    parameter int unsigned DataWidth = 64,

    parameter int unsigned StrbWidth = DataWidth >> 3,
    localparam type addr_t = logic [AddrWidth-1:0],
    localparam type data_t = logic [DataWidth-1:0],
    localparam type strb_t = logic [StrbWidth-1:0]

    // localparam int unsigned NumOutTaintWires = 323
) (
  input logic clk_i,
  input logic rst_ni,

  output data_t mem_rdata_o // To prevent the whole internal logic from being optimized out.
);

  logic  mem_req;
  logic  mem_gnt;
  addr_t mem_addr;
  data_t mem_wdata;
  strb_t mem_strb;
  logic  mem_we;
  // data_t mem_rdata;

  logic  mem_req_taint;
  addr_t mem_addr_taint;
  strb_t mem_strb_taint;
  logic  mem_we_taint;

  data_t mem_rdata_taint;
  data_t mem_wdata_taint;



  ariane_mem_top #(
    // ariane_mem_top is pre-parametrized
    // .AXI_USER_WIDTH(1),
    // .AXI_ADDRESS_WIDTH(AddrWidth),
    // .AXI_DATA_WIDTH(DataWidth)
  ) i_ariane_mem_top (
    .clk_i,
    .rst_ni,
    .mem_req_o(mem_req),
    .mem_gnt_i(mem_gnt),
    .mem_addr_o(mem_addr),
    .mem_wdata_o(mem_wdata),
    .mem_strb_o(mem_strb),
    .mem_we_o(mem_we),
    .mem_rdata_i(mem_rdata_o),
    .mem_addr_o_t0(mem_addr_taint),
    .mem_gnt_i_t0('0),
    .mem_rdata_i_t0(mem_rdata_taint),
    .mem_req_o_t0(mem_req_taint),
    .mem_strb_o_t0(mem_strb_taint),
    .mem_wdata_o_t0(mem_wdata_taint),
    .mem_we_o_t0(mem_we_taint),

    .boot_addr_i('0),
    .hart_id_i('0),
    .irq_i('0),
    .ipi_i('0),
    .time_irq_i('0),
    .debug_req_i('0),

    .boot_addr_i_t0('0),
    .hart_id_i_t0('0),
    .irq_i_t0('0),
    .ipi_i_t0('0),
    .time_irq_i_t0('0),
    .debug_req_i_t0('0)

  );

  ///////////////////////////////
  // Instruction SRAM instance //
  ///////////////////////////////

  ift_sram_mem #(
    .Width(DataWidth),
    .Depth(NumWords),
    .NumTaints(NumTaints),
    .RelocateRequestUp(64'h00000000)
  ) i_sram (
    .clk_i,
    .rst_ni,

    .req_i(mem_req),
    .write_i(mem_we),
    .addr_i(mem_addr >> 3), // 64-bit words
    .wdata_i(mem_wdata),
    .wmask_i({{8{mem_strb[7]}}, {8{mem_strb[6]}}, {8{mem_strb[5]}}, {8{mem_strb[4]}}, {8{mem_strb[3]}}, {8{mem_strb[2]}}, {8{mem_strb[1]}}, {8{mem_strb[0]}}}),
    .rdata_o(mem_rdata_o),
    .req_i_taint(mem_req_taint),
    .write_i_taint(mem_we_taint),
    .addr_i_taint(mem_addr_taint),
    .wdata_i_taint(mem_wdata_taint),
    .wmask_i_taint({{8{mem_strb_taint[7]}}, {8{mem_strb_taint[6]}}, {8{mem_strb_taint[5]}}, {8{mem_strb_taint[4]}}, {8{mem_strb_taint[3]}}, {8{mem_strb_taint[2]}}, {8{mem_strb_taint[1]}}, {8{mem_strb_taint[0]}}}),
    .rdata_o_taint(mem_rdata_taint)
  );

  assign mem_gnt = 1'b1;

endmodule
