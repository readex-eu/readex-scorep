/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2016,
 * Technische Universitaet Dresden, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

#include <new>

int
main( int ac, char* av[] )
{
    int* mem = new int[ 1024 ];
    delete[] mem;

    return 0;
}
