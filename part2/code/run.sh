#!/bin/bash

sim=0 # 0=connexion à la fpga 1=simulation de la fpga
gui=0 # 0=cli 1=gui

while getopts 's?g?' arg
do
    case ${arg} in
        s) sim=1;;
        g) gui=1;;
        *) exit 1 # illegal option
    esac
done

# Compile and start server between GUI and FPGA
cd embedded_soft
mkdir -p build
cd build
if [ $sim = 1 ]; then
    qmake CONFIG=config_fpgasimulator ..
else
    qmake CONFIG=config_fpgaremote ..
fi
make clean
make -j4
lxterminal -e "./embedded" &
cd ..
cd ..

# Compile and start systemverilog server
if [ $sim = 0 ]; then
    cd fpga_sim_remote
    sleep 1;
    lxterminal -e "export PROJ_HOME=$(pwd); ./arun.sh -tool questa"
    cd ..
    sleep 15 # Wait before starting because questasim must be ready
fi

# Compile and start GUI
cd gui_soft
if [ $gui = 1 ]; then
    cd gui
else
    cd noguitests
fi
mkdir -p build
cd build
qmake ..
make -j4
if [ $gui = 1 ]; then
    lxterminal -e "./mathcomputergui" &
else
    lxterminal -e "./noguitest;read -n1 -r -p 'Press any key to continue...' key" &
fi
cd ..
cd ..
cd ..
