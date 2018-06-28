############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
############################################################
open_project hough_sim
add_files hough_sim/PE.h
add_files hough_sim/PE2.h
add_files hough_sim/PE_func.cpp
add_files hough_sim/fil.cpp
add_files hough_sim/fil.h
add_files hough_sim/hough.cpp
add_files hough_sim/hough_class.cpp
add_files hough_sim/im_top.cpp
add_files hough_sim/rle3.cpp
add_files -tb hough_sim/bmp_header.h
add_files -tb hough_sim/city06.bmp
add_files -tb hough_sim/hough_tb.cpp
add_files -tb hough_sim/opencountry_land298.bmp
add_files -tb hough_sim/rocks01.bmp
add_files -tb hough_sim/rocks02.bmp
add_files -tb hough_sim/scenery14.bmp
add_files -tb hough_sim/scenery18.bmp
add_files -tb hough_sim/sub.h
open_solution "solution1"
set_part {xczu9eg-ffvb1156-2-i} -tool vivado
create_clock -period 10 -name default
#source "./hough_sim/solution1/directives.tcl"
csim_design -clean -O
csynth_design
cosim_design
export_design -format ip_catalog
