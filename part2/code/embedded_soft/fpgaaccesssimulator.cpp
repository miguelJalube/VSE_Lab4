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
    return a * a * a + b * c;
}


uint32_t FPGAAccess::getNbCompute()
{
    // TODO
}

void FPGAAccess::resetNbCompute()
{
    // TODO
}
