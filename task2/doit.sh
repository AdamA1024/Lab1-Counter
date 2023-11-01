#!/bin/sh
#cleanup
rm -rf obj-dir
rm -f counter.vcd

#run Verilator to translate Verilog into c++,including c++ testbench
verilator -Wall --cc --trace counter.sv --exe counter_tb.cpp

#build C++ project via make automatically generated by Verilator
make -j -C obj_dir/ -f Vcounter.mk Vcounter

# run executable simulation file
sudo obj_dir/Vcounter