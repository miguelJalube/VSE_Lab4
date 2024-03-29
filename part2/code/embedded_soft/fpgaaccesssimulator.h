#ifndef FPGAACCESSSIMULATOR_H
#define FPGAACCESSSIMULATOR_H

#include <cstdint>

class FPGAAccess
{
    uint32_t nbCompute;

public:
    static FPGAAccess& getInstance();

    FPGAAccess();

    uint32_t compute(uint32_t a, uint32_t b, uint32_t c);

    uint32_t getNbCompute();

    void resetNbCompute();

    void init();
};

#endif // FPGAACCESSSIMULATOR_H
