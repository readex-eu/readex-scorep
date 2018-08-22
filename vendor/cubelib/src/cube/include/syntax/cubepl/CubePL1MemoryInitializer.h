/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2016                                                **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2009-2015                                                **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


#ifndef __CUBEPL1_MEMORY_INITIALIZER_H
#define __CUBEPL1_MEMORY_INITIALIZER_H 0

#include <vector>
#include <stack>
#include <iostream>
#include <float.h>
#include <cmath>
#include <map>

#include "CubePLMemoryInitializer.h"
#include "CubePL1MemoryManager.h"

namespace cube
{
class CubePL1MemoryInitializer : public CubePLMemoryInitializer
{
protected:
    CubePL1MemoryManager* cubepl1_memory_manager;

public:
    CubePL1MemoryInitializer( CubePLMemoryManager* _manager ) : CubePLMemoryInitializer( _manager )
    {
        cubepl1_memory_manager = ( CubePL1MemoryManager* )_manager;
    };

    virtual
    ~CubePL1MemoryInitializer()
    {
    }

    virtual
    void
    memory_init();

    virtual
    void
    memory_new( uint32_t _id = ( uint32_t )-1 );

    virtual
    void
    memory_cleanup( uint32_t _id = ( uint32_t )-1 );



    virtual
    void
    memory_setup( cube::Cube* );

    virtual
    void
    memory_setup( cube::Metric*,
                  uint32_t _id = ( uint32_t )-1 );

    virtual
    void
    memory_setup( cube::Cnode*,
                  uint32_t _id = ( uint32_t )-1 );

    virtual
    void
    memory_setup( cube::Region*,
                  uint32_t _id = ( uint32_t )-1 );

    virtual
    void
    memory_setup( cube::Sysres*,
                  uint32_t _id = ( uint32_t )-1 );
};
}

#endif
