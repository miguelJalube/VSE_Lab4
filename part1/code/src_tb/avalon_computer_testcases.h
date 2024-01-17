/* MTI_DPI */

/*
 * Copyright 2002-2020 Mentor Graphics Corporation.
 *
 * Note:
 *   This file is automatically generated.
 *   Please do not edit this file - you will lose your edits.
 *
 * Settings when this file was generated:
 *   PLATFORM = 'linux_x86_64'
 */
#ifndef INCLUDED_AVALON_COMPUTER_TESTCASES
#define INCLUDED_AVALON_COMPUTER_TESTCASES

#ifdef __cplusplus
#define DPI_LINK_DECL  extern "C" 
#else
#define DPI_LINK_DECL 
#endif

#include "svdpi.h"



DPI_LINK_DECL DPI_DLLESPEC
int
CTask();

DPI_LINK_DECL int
avalon_read(
    unsigned int address,
    unsigned int byteenable,
    unsigned int* data);

DPI_LINK_DECL int
avalon_write(
    unsigned int address,
    unsigned int byteenable,
    unsigned int data);

#endif 
