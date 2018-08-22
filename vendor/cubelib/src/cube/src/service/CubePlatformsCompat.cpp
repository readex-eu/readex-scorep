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


/**
 * \file>CubePlatformsCompat.cpp
 * \brief Source code of the  definitions for missiong calls on some platform different than linux.
 *
 *
 *
 */

#ifdef __MINGW32__ // for windows we compile only on MinGW32. Here are some calls different or missing

#include "config.h"
#include "CubePlatformsCompat.h"

#warning "On MinGW32 platform doesn't exist function fnmatch. Replace it with dummy macro (FNM_MATCH) to enable tools compilation. No shell patterns will be available under windows. "
#warning "On MinGW32 platform we redefine fseeko using standard fseek."
#warning "On MinGW32 platforw we create cube reports using faked user and group id = 131071 (6th Marsenne prime number)"
gid_t
getgid( void )
{
    return 131071; // faked gid on MinGW32
}

uid_t
getuid( void )
{
    return 131071; // faked uid on MinGW32
}


#endif

