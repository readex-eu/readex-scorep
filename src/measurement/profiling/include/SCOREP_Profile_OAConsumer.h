/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2011,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2011, 2014,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2011,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2011, 2013, 2015,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2011,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2011, 2015-2016,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license.  See the COPYING file in the package base
 * directory for details.
 *
 */

#ifndef SCOREP_PROFILE_OACONSUMER_H
#define SCOREP_PROFILE_OACONSUMER_H

/**
 * @file
 *
 * @brief Interface called by the OA module to get the profile measurements.
 *
 */
#include "SCOREP_Types.h"
#include "SCOREP_Location.h"

#define MAX_COUNTER_NAME_LENGTH                         256
#define MAX_COUNTER_UNIT_LENGTH                         10
#define MAX_REGION_NAME_LENGTH                          500
#define MAX_FILE_NAME_LENGTH                            256

#define SCOREP_OA_COUNTER_TIME                  0


typedef struct SCOREP_OA_CallPathCounterDef_struct
{
    char     name[ MAX_COUNTER_NAME_LENGTH ];
    char     unit[ MAX_COUNTER_UNIT_LENGTH ];
    uint32_t status;
} SCOREP_OA_CallPathCounterDef;

typedef struct SCOREP_OA_CallPathRegionDef_struct
{
    uint32_t region_id;
    char     name[ MAX_REGION_NAME_LENGTH ];
    char     file[ MAX_FILE_NAME_LENGTH ];
    uint32_t rfl;
    uint32_t rel;
    uint32_t paradigm_type;
} SCOREP_OA_CallPathRegionDef;

typedef struct SCOREP_OA_CallTreeDef_struct
{
    uint32_t region_id;
    char     name[ MAX_REGION_NAME_LENGTH ];
    uint32_t scorep_id;
    uint32_t parent_scorep_id;
} SCOREP_OA_CallTreeDef;

typedef struct SCOREP_OA_RtsMeasurement_struct
{
    uint64_t rank;
    uint32_t thread;
    uint64_t count;
    uint32_t metric_id;
    uint64_t int_val;
    uint32_t scorep_id;
} SCOREP_OA_RtsMeasurement;

typedef struct SCOREP_OA_FlatProfileMeasurement_struct
{
    uint32_t measurement_id;
    uint64_t rank;
    uint32_t thread;
    uint32_t region_id;
    uint64_t samples;
    uint32_t metric_id;
    uint64_t int_val;
} SCOREP_OA_FlatProfileMeasurement;

typedef enum scorep_oaconsumer_data_types
{
    FLAT_PROFILE = 0,
    MERGED_REGION_DEFINITIONS,
    REGION_DEFINITIONS,
    COUNTER_DEFINITIONS,
    CALLPATH_PROFILE_CONTEXTS,
    CALLPATH_PROFILE_MEASUREMENTS,
    NUMBER_OF_THREADS,
    CALLTREE_DEFINITIONS,
    RTS_MEASUREMENTS
} scorep_oaconsumer_data_types;


void
SCOREP_OAConsumer_Initialize( SCOREP_RegionHandle phaseHandle );

void
SCOREP_OAConsumer_DismissData( void );

void*
SCOREP_OAConsumer_GetData( scorep_oaconsumer_data_types dataType );

uint32_t
SCOREP_OAConsumer_GetDataSize( scorep_oaconsumer_data_types dataType );


#endif /* SCOREP_PROFILE_OACONSUMER_H */
