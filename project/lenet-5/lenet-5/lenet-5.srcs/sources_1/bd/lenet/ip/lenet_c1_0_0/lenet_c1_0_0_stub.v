// Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2017.2 (win64) Build 1909853 Thu Jun 15 18:39:09 MDT 2017
// Date        : Wed Mar 27 23:48:59 2019
// Host        : DESKTOP-4PJQS5S running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub
//               f:/project/lenet-5/lenet-5/lenet-5.srcs/sources_1/bd/lenet/ip/lenet_c1_0_0/lenet_c1_0_0_stub.v
// Design      : lenet_c1_0_0
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7z020clg484-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
(* x_core_info = "c1,Vivado 2017.2" *)
module lenet_c1_0_0(s_axi_CRTL_BUS_AWADDR, 
  s_axi_CRTL_BUS_AWVALID, s_axi_CRTL_BUS_AWREADY, s_axi_CRTL_BUS_WDATA, 
  s_axi_CRTL_BUS_WSTRB, s_axi_CRTL_BUS_WVALID, s_axi_CRTL_BUS_WREADY, 
  s_axi_CRTL_BUS_BRESP, s_axi_CRTL_BUS_BVALID, s_axi_CRTL_BUS_BREADY, 
  s_axi_CRTL_BUS_ARADDR, s_axi_CRTL_BUS_ARVALID, s_axi_CRTL_BUS_ARREADY, 
  s_axi_CRTL_BUS_RDATA, s_axi_CRTL_BUS_RRESP, s_axi_CRTL_BUS_RVALID, 
  s_axi_CRTL_BUS_RREADY, s_axi_KERNEL_BUS_AWADDR, s_axi_KERNEL_BUS_AWVALID, 
  s_axi_KERNEL_BUS_AWREADY, s_axi_KERNEL_BUS_WDATA, s_axi_KERNEL_BUS_WSTRB, 
  s_axi_KERNEL_BUS_WVALID, s_axi_KERNEL_BUS_WREADY, s_axi_KERNEL_BUS_BRESP, 
  s_axi_KERNEL_BUS_BVALID, s_axi_KERNEL_BUS_BREADY, s_axi_KERNEL_BUS_ARADDR, 
  s_axi_KERNEL_BUS_ARVALID, s_axi_KERNEL_BUS_ARREADY, s_axi_KERNEL_BUS_RDATA, 
  s_axi_KERNEL_BUS_RRESP, s_axi_KERNEL_BUS_RVALID, s_axi_KERNEL_BUS_RREADY, ap_clk, 
  ap_rst_n, interrupt, input_image_TVALID, input_image_TREADY, input_image_TDATA, 
  input_image_TLAST, output_r_TVALID, output_r_TREADY, output_r_TDATA, output_r_TLAST)
/* synthesis syn_black_box black_box_pad_pin="s_axi_CRTL_BUS_AWADDR[3:0],s_axi_CRTL_BUS_AWVALID,s_axi_CRTL_BUS_AWREADY,s_axi_CRTL_BUS_WDATA[31:0],s_axi_CRTL_BUS_WSTRB[3:0],s_axi_CRTL_BUS_WVALID,s_axi_CRTL_BUS_WREADY,s_axi_CRTL_BUS_BRESP[1:0],s_axi_CRTL_BUS_BVALID,s_axi_CRTL_BUS_BREADY,s_axi_CRTL_BUS_ARADDR[3:0],s_axi_CRTL_BUS_ARVALID,s_axi_CRTL_BUS_ARREADY,s_axi_CRTL_BUS_RDATA[31:0],s_axi_CRTL_BUS_RRESP[1:0],s_axi_CRTL_BUS_RVALID,s_axi_CRTL_BUS_RREADY,s_axi_KERNEL_BUS_AWADDR[8:0],s_axi_KERNEL_BUS_AWVALID,s_axi_KERNEL_BUS_AWREADY,s_axi_KERNEL_BUS_WDATA[31:0],s_axi_KERNEL_BUS_WSTRB[3:0],s_axi_KERNEL_BUS_WVALID,s_axi_KERNEL_BUS_WREADY,s_axi_KERNEL_BUS_BRESP[1:0],s_axi_KERNEL_BUS_BVALID,s_axi_KERNEL_BUS_BREADY,s_axi_KERNEL_BUS_ARADDR[8:0],s_axi_KERNEL_BUS_ARVALID,s_axi_KERNEL_BUS_ARREADY,s_axi_KERNEL_BUS_RDATA[31:0],s_axi_KERNEL_BUS_RRESP[1:0],s_axi_KERNEL_BUS_RVALID,s_axi_KERNEL_BUS_RREADY,ap_clk,ap_rst_n,interrupt,input_image_TVALID,input_image_TREADY,input_image_TDATA[31:0],input_image_TLAST[0:0],output_r_TVALID,output_r_TREADY,output_r_TDATA[31:0],output_r_TLAST[0:0]" */;
  input [3:0]s_axi_CRTL_BUS_AWADDR;
  input s_axi_CRTL_BUS_AWVALID;
  output s_axi_CRTL_BUS_AWREADY;
  input [31:0]s_axi_CRTL_BUS_WDATA;
  input [3:0]s_axi_CRTL_BUS_WSTRB;
  input s_axi_CRTL_BUS_WVALID;
  output s_axi_CRTL_BUS_WREADY;
  output [1:0]s_axi_CRTL_BUS_BRESP;
  output s_axi_CRTL_BUS_BVALID;
  input s_axi_CRTL_BUS_BREADY;
  input [3:0]s_axi_CRTL_BUS_ARADDR;
  input s_axi_CRTL_BUS_ARVALID;
  output s_axi_CRTL_BUS_ARREADY;
  output [31:0]s_axi_CRTL_BUS_RDATA;
  output [1:0]s_axi_CRTL_BUS_RRESP;
  output s_axi_CRTL_BUS_RVALID;
  input s_axi_CRTL_BUS_RREADY;
  input [8:0]s_axi_KERNEL_BUS_AWADDR;
  input s_axi_KERNEL_BUS_AWVALID;
  output s_axi_KERNEL_BUS_AWREADY;
  input [31:0]s_axi_KERNEL_BUS_WDATA;
  input [3:0]s_axi_KERNEL_BUS_WSTRB;
  input s_axi_KERNEL_BUS_WVALID;
  output s_axi_KERNEL_BUS_WREADY;
  output [1:0]s_axi_KERNEL_BUS_BRESP;
  output s_axi_KERNEL_BUS_BVALID;
  input s_axi_KERNEL_BUS_BREADY;
  input [8:0]s_axi_KERNEL_BUS_ARADDR;
  input s_axi_KERNEL_BUS_ARVALID;
  output s_axi_KERNEL_BUS_ARREADY;
  output [31:0]s_axi_KERNEL_BUS_RDATA;
  output [1:0]s_axi_KERNEL_BUS_RRESP;
  output s_axi_KERNEL_BUS_RVALID;
  input s_axi_KERNEL_BUS_RREADY;
  input ap_clk;
  input ap_rst_n;
  output interrupt;
  input input_image_TVALID;
  output input_image_TREADY;
  input [31:0]input_image_TDATA;
  input [0:0]input_image_TLAST;
  output output_r_TVALID;
  input output_r_TREADY;
  output [31:0]output_r_TDATA;
  output [0:0]output_r_TLAST;
endmodule
