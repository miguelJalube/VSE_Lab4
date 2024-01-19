/*
HEIG-VD
2024-01-19

Leandro SARAIVA MAIA
Miguel JALUBE

Labo 4 VSE
Partie 1
*/

#include "math.h"
#include "stdio.h"
#include "avalon_computer_testcases.h"

#define GENERIC_VAL_N   3
#define OFFSET_A        0
#define OFFSET_B        1
#define OFFSET_C        2
#define OFFSET_RESULT   3
#define OFFSET_CMD      4
#define OFFSET_STATUS   5

#define BYTE_ENABLE     0b11  // Mask defining what bytes are active
#define AV_WRITE(adr, data) {if (avalon_write(adr, BYTE_ENABLE, data)) { printf("Error during avalon write\n"); return 1; } else { printf("[C]  write %5u at 0x%05x\n", data, adr); }}
#define AV_READ(adr, data) {if (avalon_read(adr, BYTE_ENABLE, data)) { printf("Error during avalon read\n"); return 1; } else { printf("[C]  read  %5u at 0x%05x\n", *data, adr); }}

unsigned c_compute(unsigned n, unsigned a, unsigned b, unsigned c) {
    return (unsigned)pow(a, n) + b * c;
}

unsigned fpga_compute(unsigned n, unsigned a, unsigned b, unsigned c) {
    unsigned data = 0, status = 0;

    // Write val a
    AV_WRITE(OFFSET_A, a);

    // Write val b
    AV_WRITE(OFFSET_B, b);

    // Write val c
    AV_WRITE(OFFSET_C, c);

    // Start computing
    AV_WRITE(OFFSET_CMD, 0);

    // Wait for the status to be ok
    do AV_READ(OFFSET_STATUS, &status) while (!status);

    // Read the results
    AV_READ(OFFSET_RESULT, &data);

    return data;
}

void testcase(char* testname, unsigned n, unsigned a, unsigned b, unsigned c) {
    unsigned fpga_res, c_res;
    fpga_res = fpga_compute(n, a, b, c);
    c_res = c_compute(n, a, b, c);
    printf("\"%s\"\n", testname);
    printf("n=%u a=%u b=%u c=%u\n", n, a, b, c);
    printf("expected:%u\n", c_res);
    printf("got:     %u\n", fpga_res);
    printf("%s\n", c_res == fpga_res ? "VALUES ARE THE SAME" : "VALUES ARE DIFFERENT");
}

DPI_DLLESPEC
int CTask() {
    testcase("normal case", GENERIC_VAL_N, 2, 4, 5);

    testcase("0 values", GENERIC_VAL_N, 0, 0, 0);

    testcase("0 multiplied by something must be 0 (c=0)", GENERIC_VAL_N, 0, 42, 0);
    testcase("0 multiplied by something must be 0 (b=0)", GENERIC_VAL_N, 0, 0, 42);

    testcase("max value (65535 with 16 bits)", GENERIC_VAL_N, 30, 7707, 5);

    testcase("big value (expected to fail)", GENERIC_VAL_N, 200, 400, 500);
    printf("This test was expected to fail since the FPGA doesn't have enough bits !\n");

    return 0;
}
