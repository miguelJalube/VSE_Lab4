#include "fpgaaccesssimulator.h"


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
    ++nbCompute;
    return a * a * a + b * c;
}


uint32_t FPGAAccess::getNbCompute()
{
    return nbCompute;
}

void FPGAAccess::resetNbCompute()
{
    nbCompute = 0;
}
