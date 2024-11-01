-- Copyright 1986-2017 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2017.2 (win64) Build 1909853 Thu Jun 15 18:39:09 MDT 2017
-- Date        : Wed Mar 27 23:48:58 2019
-- Host        : DESKTOP-4PJQS5S running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
--               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ lenet_c1_0_0_stub.vhdl
-- Design      : lenet_c1_0_0
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7z020clg484-1
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
  Port ( 
    s_axi_CRTL_BUS_AWADDR : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_CRTL_BUS_AWVALID : in STD_LOGIC;
    s_axi_CRTL_BUS_AWREADY : out STD_LOGIC;
    s_axi_CRTL_BUS_WDATA : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_CRTL_BUS_WSTRB : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_CRTL_BUS_WVALID : in STD_LOGIC;
    s_axi_CRTL_BUS_WREADY : out STD_LOGIC;
    s_axi_CRTL_BUS_BRESP : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_CRTL_BUS_BVALID : out STD_LOGIC;
    s_axi_CRTL_BUS_BREADY : in STD_LOGIC;
    s_axi_CRTL_BUS_ARADDR : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_CRTL_BUS_ARVALID : in STD_LOGIC;
    s_axi_CRTL_BUS_ARREADY : out STD_LOGIC;
    s_axi_CRTL_BUS_RDATA : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_CRTL_BUS_RRESP : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_CRTL_BUS_RVALID : out STD_LOGIC;
    s_axi_CRTL_BUS_RREADY : in STD_LOGIC;
    s_axi_KERNEL_BUS_AWADDR : in STD_LOGIC_VECTOR ( 8 downto 0 );
    s_axi_KERNEL_BUS_AWVALID : in STD_LOGIC;
    s_axi_KERNEL_BUS_AWREADY : out STD_LOGIC;
    s_axi_KERNEL_BUS_WDATA : in STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_KERNEL_BUS_WSTRB : in STD_LOGIC_VECTOR ( 3 downto 0 );
    s_axi_KERNEL_BUS_WVALID : in STD_LOGIC;
    s_axi_KERNEL_BUS_WREADY : out STD_LOGIC;
    s_axi_KERNEL_BUS_BRESP : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_KERNEL_BUS_BVALID : out STD_LOGIC;
    s_axi_KERNEL_BUS_BREADY : in STD_LOGIC;
    s_axi_KERNEL_BUS_ARADDR : in STD_LOGIC_VECTOR ( 8 downto 0 );
    s_axi_KERNEL_BUS_ARVALID : in STD_LOGIC;
    s_axi_KERNEL_BUS_ARREADY : out STD_LOGIC;
    s_axi_KERNEL_BUS_RDATA : out STD_LOGIC_VECTOR ( 31 downto 0 );
    s_axi_KERNEL_BUS_RRESP : out STD_LOGIC_VECTOR ( 1 downto 0 );
    s_axi_KERNEL_BUS_RVALID : out STD_LOGIC;
    s_axi_KERNEL_BUS_RREADY : in STD_LOGIC;
    ap_clk : in STD_LOGIC;
    ap_rst_n : in STD_LOGIC;
    interrupt : out STD_LOGIC;
    input_image_TVALID : in STD_LOGIC;
    input_image_TREADY : out STD_LOGIC;
    input_image_TDATA : in STD_LOGIC_VECTOR ( 31 downto 0 );
    input_image_TLAST : in STD_LOGIC_VECTOR ( 0 to 0 );
    output_r_TVALID : out STD_LOGIC;
    output_r_TREADY : in STD_LOGIC;
    output_r_TDATA : out STD_LOGIC_VECTOR ( 31 downto 0 );
    output_r_TLAST : out STD_LOGIC_VECTOR ( 0 to 0 )
  );

end decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix;

architecture stub of decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "s_axi_CRTL_BUS_AWADDR[3:0],s_axi_CRTL_BUS_AWVALID,s_axi_CRTL_BUS_AWREADY,s_axi_CRTL_BUS_WDATA[31:0],s_axi_CRTL_BUS_WSTRB[3:0],s_axi_CRTL_BUS_WVALID,s_axi_CRTL_BUS_WREADY,s_axi_CRTL_BUS_BRESP[1:0],s_axi_CRTL_BUS_BVALID,s_axi_CRTL_BUS_BREADY,s_axi_CRTL_BUS_ARADDR[3:0],s_axi_CRTL_BUS_ARVALID,s_axi_CRTL_BUS_ARREADY,s_axi_CRTL_BUS_RDATA[31:0],s_axi_CRTL_BUS_RRESP[1:0],s_axi_CRTL_BUS_RVALID,s_axi_CRTL_BUS_RREADY,s_axi_KERNEL_BUS_AWADDR[8:0],s_axi_KERNEL_BUS_AWVALID,s_axi_KERNEL_BUS_AWREADY,s_axi_KERNEL_BUS_WDATA[31:0],s_axi_KERNEL_BUS_WSTRB[3:0],s_axi_KERNEL_BUS_WVALID,s_axi_KERNEL_BUS_WREADY,s_axi_KERNEL_BUS_BRESP[1:0],s_axi_KERNEL_BUS_BVALID,s_axi_KERNEL_BUS_BREADY,s_axi_KERNEL_BUS_ARADDR[8:0],s_axi_KERNEL_BUS_ARVALID,s_axi_KERNEL_BUS_ARREADY,s_axi_KERNEL_BUS_RDATA[31:0],s_axi_KERNEL_BUS_RRESP[1:0],s_axi_KERNEL_BUS_RVALID,s_axi_KERNEL_BUS_RREADY,ap_clk,ap_rst_n,interrupt,input_image_TVALID,input_image_TREADY,input_image_TDATA[31:0],input_image_TLAST[0:0],output_r_TVALID,output_r_TREADY,output_r_TDATA[31:0],output_r_TLAST[0:0]";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "c1,Vivado 2017.2";
begin
end;
