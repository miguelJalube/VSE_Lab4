#ifndef FPGAACCESS_H
#define FPGAACCESS_H

#include <cstdint>

class FPGAAccess
{
public:
    static FPGAAccess& getInstance();

    FPGAAccess();

    uint32_t compute(uint32_t a, uint32_t b, uint32_t c);

    uint32_t getNbCompute();

    void resetNbCompute();

    void init();
};

#endif // FPGAACCESS_H
