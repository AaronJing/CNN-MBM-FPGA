############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
############################################################
open_project -reset hls_lenet
set_top c1
add_files hls_lenet/cnn.cpp
add_files -tb hls_lenet/cnn_tb.cpp
open_solution -reset "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 15 -name default
#source "./hls_lenet/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -rtl vhdl -format ip_catalog

exit