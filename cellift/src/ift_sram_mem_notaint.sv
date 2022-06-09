// Copyright Flavien Solt, ETH Zurich.
// Licensed under the General Public License, Version 3.0, see LICENSE for details.
// SPDX-License-Identifier: GPL-3.0-only

// SRAM module without taints.

module noift_sram_mem #(
  parameter int Width           = 32, // bit
  parameter int Depth           = 1 << 15,

  parameter bit PreloadELF = 1,
  parameter bit PreloadTaints = 1,
  parameter logic [63:0] RelocateRequestUp = '0,

  // Derived parameters.
  localparam int WidthBytes = Width >> 3,
  localparam int Aw         = $clog2(Depth)  // derived parameter
) (
  input  logic             clk_i,
  input  logic             rst_ni,

  input  logic             req_i,
  input  logic             write_i,
  input  logic [Aw-1:0]    addr_i,
  input  logic [Width-1:0] wdata_i,
  input  logic [Width-1:0] wmask_i,
  output logic [Width-1:0] rdata_o
);

  logic                [Depth-1:0][Width-1:0] mem;
  logic [Width-1:0]    mem_taints [bit [31:0]];

  //
  // DPI
  //
  int sections [bit [31:0]];

  import "DPI-C" function read_elf(input string filename);
  import "DPI-C" function byte get_section(output longint address, output longint len);
  import "DPI-C" context function byte read_section(input longint address, inout byte buffer[]);

  import "DPI-C" function init_taint_vectors(input longint num_taints);
  import "DPI-C" function read_taints(input string filename);
  import "DPI-C" function byte get_taint_section(input longint taint_id, output longint address, output longint len);
  import "DPI-C" context function byte read_taint_section(input longint taint_id, input longint address, inout byte buffer[]);
  import "DPI-C" function string Get_SRAM_ELF_object_filename();
  import "DPI-C" function string Get_SRAM_TaintsPath();

  localparam int unsigned PreloadBufferSize = 100000;
  initial begin // Load the binary into memory.
    if (PreloadELF) begin
      automatic string binary = Get_SRAM_ELF_object_filename();
      longint section_addr, section_len;
      byte buffer[PreloadBufferSize];
      $display("Loading RAM ELF: %s", binary);
      void'(read_elf(binary));
      while (get_section(section_addr, section_len)) begin
        automatic int num_words = (section_len+(WidthBytes-1))/WidthBytes;
        sections[section_addr/WidthBytes] = num_words;
        // buffer = new [num_words*WidthBytes];
        assert(num_words*WidthBytes >= PreloadBufferSize);
        void'(read_section(section_addr, buffer));

        for (int i = 0; i < num_words; i++) begin
          automatic logic [WidthBytes-1:0][7:0] word = '0;
          for (int j = 0; j < WidthBytes; j++) begin
            word[j] = buffer[i*WidthBytes+j];
          end
          $display("Writing ELF word to SRAM addr %x: %x", section_addr/WidthBytes+i, word);
          mem[section_addr/WidthBytes+i] = word;
        end
      end
    end
  end

  //
  //  Data
  //

  always_ff @(posedge clk_i) begin
		if (req_i) begin
      if (write_i) begin
          for (int i = 0; i < Width; i = i + 1)
            if (wmask_i[i])
              mem[RelocateRequestUp | addr_i][i] <= wdata_i[i];
      end
      else
          rdata_o <= mem[RelocateRequestUp | addr_i];
    end
  end

endmodule
