/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2013,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2013,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2013, 2015,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2013,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2013,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_PLATFORM_H
#define SCOREP_PLATFORM_H

/**
 * @file
 *
 *
 */

#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <SCOREP_Types.h>
#include <UTILS_Error.h>

UTILS_BEGIN_C_DECLS

/**
 * Contains one element of the system tree path. Used to specify a path in the
 * system tree
 */
typedef struct SCOREP_Platform_SystemTreeProperty
{
    struct SCOREP_Platform_SystemTreeProperty* next;
    char*                                      property_name;
    char*                                      property_value;
} SCOREP_Platform_SystemTreeProperty;


/**
 * Contains one element of the system tree path. Used to specify a path in the
 * system tree
 */
typedef struct SCOREP_Platform_SystemTreePathElement
{
    struct SCOREP_Platform_SystemTreePathElement* next;
    SCOREP_SystemTreeDomain                       domains;
    char*                                         node_class;
    char*                                         node_name;
    SCOREP_Platform_SystemTreeProperty*           properties;
    SCOREP_Platform_SystemTreeProperty**          properties_tail;
} SCOREP_Platform_SystemTreePathElement;


/**
 * Returns the path of this process in the system tree, starting at the root.
 *
 * The path can be traversed with the @a SCOREP_PLATFORM_SYSTEM_TREE_FORALL()
 * macro.
 *
 * The path should be freed with @a SCOREP_Platform_FreePath().
 *
 * @param root A pointer to a @a SCOREP_Platform_SystemTreePathElement* element
 *             which represents the already existing root node of the system tree
 *             machine domain and the start of the path for this process in the
 *             system tree.
 * @param machineName  The machine name used for the root node.
 * @param platformName The platform name used for the root node.
 *
 * @return SCOREP_SUCCESS on success else an appropriate error code.
 */
extern SCOREP_ErrorCode
SCOREP_Platform_GetPathInSystemTree( SCOREP_Platform_SystemTreePathElement** root,
                                     const char*                             machineName,
                                     const char*                             platformName );


/**
 * Deletes the path returned from @a SCOREP_Platform_GetPathInSystemTree().
 *
 * @param path Pointer to the returned path from @a SCOREP_Platform_GetPathInSystemTree
 *             that should be freed.
 */
extern void
SCOREP_Platform_FreePath( SCOREP_Platform_SystemTreePathElement* path );


/**
 * Returns a unique node identifier.
 */
extern int32_t
SCOREP_Platform_GetNodeId( void );

/**
 * Used to iterate over a system tree path starting at the root.
 */
#define SCOREP_PLATFORM_SYSTEM_TREE_FORALL( _path, _node ) \
    for ( _node = _path; _node; _node = _node->next )

/**
 * Used to iterate over all properties of an system tree node.
 */
#define SCOREP_PLATFORM_SYSTEM_TREE_PROPERTY_FORALL( _node, _property ) \
    for ( _property = _node->properties; _property; _property = _property->next )

UTILS_END_C_DECLS

#endif /* SCOREP_PLATFORM_H */
