#include <iostream>
#include <cassert>

#include "fpgaaccess.h"


FPGAAccess::FPGAAccess()
{

}

FPGAAccess& FPGAAccess::getInstance()
{
    static FPGAAccess access;
    return access;
}

void FPGAAccess::init()
{
    return;
}

uint32_t FPGAAccess::compute(uint32_t a, uint32_t b, uint32_t c)
{
    std::cerr << "FPGA direct access not implemented" << std::endl;
    assert(false);
    return 0;
}


uint32_t FPGAAccess::getNbCompute()
{
    std::cerr << "FPGA direct access not implemented" << std::endl;
    assert(false);
    return 0;
}

void FPGAAccess::resetNbCompute()
{
    std::cerr << "FPGA direct access not implemented" << std::endl;
    assert(false);
}
