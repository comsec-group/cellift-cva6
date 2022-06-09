// Copyright 2021 ETH Zurich and University of Bologna.
// Copyright and related rights are licensed under the Solderpad Hardware
// License, Version 0.51 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
// http://solderpad.org/licenses/SHL-0.51. Unless required by applicable law
// or agreed to in writing, software, hardware and materials distributed under
// this License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
// CONDITIONS OF ANY KIND, either express or implied. See the License for the
// specific language governing permissions and limitations under the License.
//
// Author: Florian Zaruba, ETH Zurich
// Author: Flavien Solt, ETH Zurich
// Date: 19.03.2017
// Description: Test-harness for Ariane
//              Instantiates an AXI-Bus and memories

module ariane_mem_top import axi_pkg::*; #(
  parameter int unsigned AXI_USER_WIDTH    = 1,
  parameter int unsigned AXI_ADDRESS_WIDTH = 64,
  parameter int unsigned AXI_DATA_WIDTH    = 64
) (
  input  logic                    clk_i,
  input  logic                    rst_ni,
  input  logic [riscv::VLEN-1:0]  boot_addr_i,
  input  logic [riscv::XLEN-1:0]  hart_id_i,
  input  logic [1:0]              irq_i,
  input  logic                    ipi_i,
  input  logic                    time_irq_i,
  input  logic                    debug_req_i,
  output logic                    mem_req_o,
  input  logic                    mem_gnt_i,
  output addr_t                   mem_addr_o,
  output data_t                   mem_wdata_o,
  output strb_t                   mem_strb_o,
  output logic                    mem_we_o,
  input  data_t                   mem_rdata_i

  // SRAM:    .addr_i     ( addr[$clog2(NUM_WORDS)-1+$clog2(AXI_DATA_WIDTH/8):$clog2(AXI_DATA_WIDTH/8)] ),

);

  AXI_BUS #(
    .AXI_ADDR_WIDTH ( AXI_ADDRESS_WIDTH   ),
    .AXI_DATA_WIDTH ( AXI_DATA_WIDTH      ),
    .AXI_ID_WIDTH   ( ariane_soc::IdWidth ),
    .AXI_USER_WIDTH ( AXI_USER_WIDTH      )
  ) slave();

  AXI_BUS #(
    .AXI_ADDR_WIDTH ( AXI_ADDRESS_WIDTH   ),
    .AXI_DATA_WIDTH ( AXI_DATA_WIDTH      ),
    .AXI_ID_WIDTH   ( ariane_soc::IdWidth ),
    .AXI_USER_WIDTH ( AXI_USER_WIDTH      )
  ) master();

  axi_join i_axi_join (
    .in(slave),
    .out(master)
  );

  ariane_axi_soc::req_t    axi_ariane_req;
  ariane_axi_soc::resp_t   axi_ariane_resp;

  axi_master_connect i_axi_master_connect_ariane (
    .axi_req_i(axi_ariane_req),
    .axi_resp_o(axi_ariane_resp),
    .master(slave)
  );

  axi2mem #(
    .AXI_ID_WIDTH   ( ariane_soc::IdWidth      ),
    .AXI_ADDR_WIDTH ( AXI_ADDRESS_WIDTH        ),
    .AXI_DATA_WIDTH ( AXI_DATA_WIDTH           ),
    .AXI_USER_WIDTH ( AXI_USER_WIDTH           )
  ) i_axi2mem (
    .clk_i      ( clk_i        ),
    .rst_ni     ( rst_ni       ),
    .slave      ( master       ),
    .req_o      ( mem_req_o    ),
    .we_o       ( mem_we_o     ),
    .addr_o     ( mem_addr_o   ),
    .be_o       ( mem_strb_o   ),
    .data_o     ( mem_wdata_o  ),
    .data_i     ( mem_rdata_i  )
  );

  ariane #(
    .ArianeCfg  ( ariane_pkg::ArianeDefaultConfig )
  ) i_ariane (
    .clk_i,
    .rst_ni,
    .boot_addr_i,
    .hart_id_i,
    .irq_i,
    .ipi_i,
    .time_irq_i,
    .debug_req_i,
    .axi_req_o  ( axi_ariane_req   ),
    .axi_resp_i ( axi_ariane_resp  )
  );

endmodule




