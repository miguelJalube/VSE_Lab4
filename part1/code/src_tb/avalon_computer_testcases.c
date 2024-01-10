

#include "stdio.h"
#include "avalon_computer_testcases.h"


DPI_DLLESPEC
int CTask()
{
    for (int i = 0; i < 10; i++) {
        if (avalon_write(0, 3, i)) {
            return 1;
        }

        unsigned int data;
        if (avalon_read(0, 3, &data)) {
            return 1;
        }
        if (data != i) {
            printf("Error. Expected %d, got %d\n", i, data);
        }
    }
    return 0;
}
