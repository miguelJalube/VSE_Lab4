#include "stdio.h"
#include "avalon_computer_testcases.h"

#define OFFSET_A        0
#define OFFSET_B        1
#define OFFSET_C        2
#define OFFSET_RESULT   3
#define OFFSET_CMD      4
#define OFFSET_STATUS   5

#define BYTE_ENABLE     0x11  // Mask defining what bytes are active
#define AV_WRITE(adr, data) if (avalon_write(adr, BYTE_ENABLE, data)) { printf("Error during avalon write\n"); return 1; }
#define AV_READ(adr, data) if (avalon_write(adr, BYTE_ENABLE, data)) { printf("Error during avalon read\n"); return 1; }

int compute(int n, int a, int b, int c) {
    return a^n + b * c;
}

void testcase(int n, int a, int b, int c) {
    int data

    // Write val a
    AV_WRITE(OFFSET_A, a);

    // Write val b
    AV_WRITE(OFFSET_B, b);

    // Write val c
    AV_WRITE(OFFSET_C, c);

    // Start computing
    AV_WRITE(OFFSET_CMD, 0);

    // Start computing
    AV_WRITE(OFFSET_CMD, 0);

    // Wait for the status to be ok
    while (!status) AV_READ(OFFSET_RESULT, &status);

    // Read the results
    AV_READ(OFFSET_RESULT, &data);

    printf("\n");
}

DPI_DLLESPEC
int CTask()
{
    
    return 0;
}