library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;

entity math_ip_v1_0 is
	generic (
		-- Users to add parameters here

		-- User parameters ends
		-- Do not modify the parameters beyond this line


		-- Parameters of Axi Slave Bus Interface S00_AXI
		C_S00_AXI_DATA_WIDTH	: integer	:= 32;
		C_S00_AXI_ADDR_WIDTH	: integer	:= 4
	);
	port (
		-- Users to add ports here

		-- User ports ends
		-- Do not modify the ports beyond this line


		-- Ports of Axi Slave Bus Interface S00_AXI
		s00_axi_aclk	: in std_logic;
		s00_axi_aresetn	: in std_logic;
		s00_axi_awaddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_awprot	: in std_logic_vector(2 downto 0);
		s00_axi_awvalid	: in std_logic;
		s00_axi_awready	: out std_logic;
		s00_axi_wdata	: in std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_wstrb	: in std_logic_vector((C_S00_AXI_DATA_WIDTH/8)-1 downto 0);
		s00_axi_wvalid	: in std_logic;
		s00_axi_wready	: out std_logic;
		s00_axi_bresp	: out std_logic_vector(1 downto 0);
		s00_axi_bvalid	: out std_logic;
		s00_axi_bready	: in std_logic;
		s00_axi_araddr	: in std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
		s00_axi_arprot	: in std_logic_vector(2 downto 0);
		s00_axi_arvalid	: in std_logic;
		s00_axi_arready	: out std_logic;
		s00_axi_rdata	: out std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
		s00_axi_rresp	: out std_logic_vector(1 downto 0);
		s00_axi_rvalid	: out std_logic;
		s00_axi_rready	: in std_logic
	);
end math_ip_v1_0;

architecture arch_imp of math_ip_v1_0 is

	-- component declaration
	component math_ip_v1_0_S00_AXI is
		generic (
		C_S_AXI_DATA_WIDTH	: integer	:= 32;
		C_S_AXI_ADDR_WIDTH	: integer	:= 4
		);
		port (
		S_AXI_ACLK	: in std_logic;
		S_AXI_ARESETN	: in std_logic;
		S_AXI_AWADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_AWPROT	: in std_logic_vector(2 downto 0);
		S_AXI_AWVALID	: in std_logic;
		S_AXI_AWREADY	: out std_logic;
		S_AXI_WDATA	: in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_WSTRB	: in std_logic_vector((C_S_AXI_DATA_WIDTH/8)-1 downto 0);
		S_AXI_WVALID	: in std_logic;
		S_AXI_WREADY	: out std_logic;
		S_AXI_BRESP	: out std_logic_vector(1 downto 0);
		S_AXI_BVALID	: out std_logic;
		S_AXI_BREADY	: in std_logic;
		S_AXI_ARADDR	: in std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0);
		S_AXI_ARPROT	: in std_logic_vector(2 downto 0);
		S_AXI_ARVALID	: in std_logic;
		S_AXI_ARREADY	: out std_logic;
		S_AXI_RDATA	: out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
		S_AXI_RRESP	: out std_logic_vector(1 downto 0);
		S_AXI_RVALID	: out std_logic;
		S_AXI_RREADY	: in std_logic;
		datain0, datain1, datain2, datain3 : in std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        dataout0, dataout1, dataout2, dataout3 : out std_logic_vector(C_S_AXI_DATA_WIDTH-1 downto 0);
        latched_waddr, latched_raddr  : out std_logic_vector(C_S_AXI_ADDR_WIDTH-1 downto 0)
		);
	end component math_ip_v1_0_S00_AXI;

    signal datain0, datain1, datain2, datain3: std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
    signal dataout0, dataout1, dataout2, dataout3: std_logic_vector(C_S00_AXI_DATA_WIDTH-1 downto 0);
    signal lwaddr, lraddr  : std_logic_vector(C_S00_AXI_ADDR_WIDTH-1 downto 0);
    signal temp1,temp2: std_logic_vector(7 downto 0);
    signal one : std_logic_vector(7 downto 0):= x"01";
    signal temp3: std_logic_vector(31 downto 0);
begin

-- Instantiation of Axi Bus Interface S00_AXI
math_ip_v1_0_S00_AXI_inst : math_ip_v1_0_S00_AXI
	generic map (
		C_S_AXI_DATA_WIDTH	=> C_S00_AXI_DATA_WIDTH,
		C_S_AXI_ADDR_WIDTH	=> C_S00_AXI_ADDR_WIDTH
	)
	port map (
		S_AXI_ACLK	=> s00_axi_aclk,
		S_AXI_ARESETN	=> s00_axi_aresetn,
		S_AXI_AWADDR	=> s00_axi_awaddr,
		S_AXI_AWPROT	=> s00_axi_awprot,
		S_AXI_AWVALID	=> s00_axi_awvalid,
		S_AXI_AWREADY	=> s00_axi_awready,
		S_AXI_WDATA	=> s00_axi_wdata,
		S_AXI_WSTRB	=> s00_axi_wstrb,
		S_AXI_WVALID	=> s00_axi_wvalid,
		S_AXI_WREADY	=> s00_axi_wready,
		S_AXI_BRESP	=> s00_axi_bresp,
		S_AXI_BVALID	=> s00_axi_bvalid,
		S_AXI_BREADY	=> s00_axi_bready,
		S_AXI_ARADDR	=> s00_axi_araddr,
		S_AXI_ARPROT	=> s00_axi_arprot,
		S_AXI_ARVALID	=> s00_axi_arvalid,
		S_AXI_ARREADY	=> s00_axi_arready,
		S_AXI_RDATA	=> s00_axi_rdata,
		S_AXI_RRESP	=> s00_axi_rresp,
		S_AXI_RVALID	=> s00_axi_rvalid,
		S_AXI_RREADY	=> s00_axi_rready,
		datain0   => datain0,
		datain1   => datain1,
		datain2   => datain2,
		datain3   => datain3,
		dataout0   => dataout0,
		dataout1   => dataout1,
		dataout2   => dataout2,
		dataout3   => dataout3,
		latched_waddr => lwaddr,
		latched_raddr => lraddr
	);

	-- Add user logic here
    
    -- input: dataout
    -- input addr: lwaddr, lraddr
    -- output: datain
    
    
    -- datain0: enable bit
    -- 
    with dataout1(7) select
        temp1 <= std_logic_vector(unsigned(not dataout1(7 downto 0))+unsigned(one)) when '1',
                dataout1(7 downto 0) when others;
    with dataout2(7) select
                        temp2 <= std_logic_vector(unsigned(not dataout2(7 downto 0))+unsigned(one)) when '1',
                                dataout2(7 downto 0) when others;
    
    datain1 <= X"00000000";
    datain2 <= X"00000000";
    datain3 <= X"00000000";
    temp3<=x"0000"&(temp1 * temp2);
    with dataout1(7) xor dataout2(7) select
        datain0 <= temp3 when '0',
                   std_logic_vector(unsigned(not temp3)+unsigned(one)) when others;
                   
--    process (datain0, dataout0, dataout1, dataout2)
--    begin
     
--    end process;
    
    
    
    
	-- User logic ends

end arch_imp;
