
onerror {resume}
# Create the library.
if [file exists work] {
    vdel -all
}
vlib work


quietly set Path_VHDL     "../src_vhdl"
quietly set Path_TB       "../src_tb"

# Compile the HDL source(s)
vcom -2008 $Path_VHDL/math_computer.vhd
vcom -2008 $Path_VHDL/avalon_computer.vhd
vlog -mixedsvvh -sv -dpiheader $Path_TB/avalon_computer_testcases.h $Path_TB/avalon_computer_testcases.c $Path_TB/avalon_computer_tb.sv

# Simulate the design
onerror {quit -sim}
vsim -c avalon_computer_tb
add wave -r *
run -all
# quit -f
