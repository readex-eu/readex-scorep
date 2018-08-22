/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 1998-2017                                                **
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


/**
 * @file
 *
 * Implements the representation and analysis of library dependencies
 */

#include "config.h"

#include <iostream>
#include <stdlib.h>
#include <cstring>

#include <cubelib_config.hpp>

#include <cubelib_config_data.hpp>

using namespace std;
#include <cubelib_config_library_dependencies_frontend_inc.hpp>

cubelib_config_data*
cubelib_config_get_frontend_data( void )
{
    static cubelib_config_data data;

    data.m_cc                    = " gcc";
    data.m_cxx                   = " g++";
    data.m_cppflags              = "-I/opt/scorep/include/cubelib";
    data.m_incldir               = "/opt/scorep/include/cubelib";
    data.m_use_rpath_flag        = 1;
    data.m_rpath_flag_cc         = "$wl-rpath $wl$libdir";
    data.m_wl_flag               = "-Wl,";
    data.m_aix_libpath           = "";
    data.m_sys_lib_dlsearch_path = "/lib /usr/lib /opt/tivoli/tsm/client/api/bin /lib /usr/lib/x86_64-linux-gnu/libfakeroot /lib/i386-linux-gnu /usr/lib/i386-linux-gnu /lib/i686-linux-gnu /usr/lib/i686-linux-gnu /usr/lib/i386-linux-gnu/mesa /usr/local/lib /usr/lib64 /lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu /usr/lib/x86_64-linux-gnu/mesa-egl /usr/lib/x86_64-linux-gnu/mesa ";

    deque<string> libs;
    deque<string> ldflags;
    deque<string> rpaths;
    deque<string> dependency_las;
    
    add_library_dependencies_frontend( libs, ldflags, rpaths, dependency_las, &data.m_la_objects ); 
    
    

    data.prepare_rpath_flags();

    return &data;
}
