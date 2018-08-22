/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2013, 2016,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2015,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2016,
 * Technische Universitaet Darmstadt, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

/**
 * @file
 *
 * @brief      Inmplements class SCOREP_Score_Event.
 */

#include <config.h>
#include "SCOREP_Score_Event.hpp"

#include <sstream>
#include <utility>

using namespace std;

/* **************************************************************************************
 * class SCOREP_Score_Event
 ***************************************************************************************/

/*------------------------------------------------ public functions */
SCOREP_Score_Event::SCOREP_Score_Event( const string& name )
{
    m_name = name;
    m_size = 0;
}

SCOREP_Score_Event::~SCOREP_Score_Event()
{
}

const string&
SCOREP_Score_Event::getName( void )
{
    return m_name;
}

uint32_t
SCOREP_Score_Event::getEventSize( void ) const
{
    return m_size;
}

void
SCOREP_Score_Event::setEventSize( uint32_t size )
{
    m_size = size;
}

bool
SCOREP_Score_Event::occursInRegion( const string& regionName,
                                    bool          hasHits )
{
    return false;
}

bool
SCOREP_Score_Event::hasTimestamp( void ) const
{
    return true;
}

/* **************************************************************************************
 * class SCOREP_Score_EnterEvent
 ***************************************************************************************/
SCOREP_Score_EnterEvent::SCOREP_Score_EnterEvent( void ) : SCOREP_Score_Event( "Enter" )
{
}

bool
SCOREP_Score_EnterEvent::occursInRegion( const std::string& regionName,
                                         bool               hasHits )
{
    if ( hasHits == false && regionName.find( '=', 0 ) == string::npos )
    {
        return true;
    }
    return false; // Is a parameter region which has no enter/exit
}

/* **************************************************************************************
 * class SCOREP_Score_LeaveEvent
 ***************************************************************************************/
SCOREP_Score_LeaveEvent::SCOREP_Score_LeaveEvent( void ) : SCOREP_Score_Event( "Leave" )
{
}

bool
SCOREP_Score_LeaveEvent::occursInRegion( const std::string& regionName,
                                         bool               hasHits )
{
    if ( hasHits == false && regionName.find( '=', 0 ) == string::npos )
    {
        return true;
    }
    return false; // Is a parameter region which has no enter/exit
}

/* **************************************************************************************
 * class SCOREP_Score_CallingContextEnterEvent
 ***************************************************************************************/
SCOREP_Score_CallingContextEnterEvent::SCOREP_Score_CallingContextEnterEvent( void ) : SCOREP_Score_Event( "CallingContextEnter" )
{
}

bool
SCOREP_Score_CallingContextEnterEvent::occursInRegion( const std::string& regionName,
                                                       bool               hasHits )
{
    if ( hasHits == true && regionName.find( '=', 0 ) == string::npos )
    {
        return true;
    }
    return false; // Is a parameter region which has no enter/exit
}

/* **************************************************************************************
 * class SCOREP_Score_CallingContextLeaveEvent
 ***************************************************************************************/
SCOREP_Score_CallingContextLeaveEvent::SCOREP_Score_CallingContextLeaveEvent( void ) : SCOREP_Score_Event( "CallingContextLeave" )
{
}

bool
SCOREP_Score_CallingContextLeaveEvent::occursInRegion( const std::string& regionName,
                                                       bool               hasHits )
{
    if ( hasHits == true && regionName.find( '=', 0 ) == string::npos )
    {
        return true;
    }
    return false; // Is a parameter region which has no enter/exit
}

/* **************************************************************************************
 * class SCOREP_Score_CallingContextLeaveEvent
 ***************************************************************************************/
SCOREP_Score_CallingContextSampleEvent::SCOREP_Score_CallingContextSampleEvent( void ) : SCOREP_Score_Event( "CallingContextSample" )
{
}

/* **************************************************************************************
 * class SCOREP_Score_MetricEvent
 ***************************************************************************************/
SCOREP_Score_MetricEvent::SCOREP_Score_MetricEvent( uint64_t numDense )
    : SCOREP_Score_Event( "Metric" ),
    m_num_dense( numDense )
{
    stringstream name;
    name << m_name << " " << m_num_dense;
    m_name = name.str();
}

bool
SCOREP_Score_MetricEvent::occursInRegion( const string& regionName,
                                          bool          hasHits )
{
    if ( regionName.find( '=', 0 ) == string::npos )
    {
        return true;
    }
    return false; // Is a parameter region which has no enter/exit
}

void
SCOREP_Score_MetricEvent::setEventSize( uint32_t size )
{
    m_size = 2 * size;            /* Regions have two metric sets (enter & exit) */
}

/* **************************************************************************************
 * class SCOREP_Score_TimestampEvent
 ***************************************************************************************/
SCOREP_Score_TimestampEvent::SCOREP_Score_TimestampEvent( void )
    : SCOREP_Score_Event( "Timestamp" )
{
}

bool
SCOREP_Score_TimestampEvent::occursInRegion( const string& regionName,
                                             bool          hasHits )
{
    return false; // a timestamp never matches
}

bool
SCOREP_Score_TimestampEvent::hasTimestamp( void ) const
{
    return false; // Number of timestamps are added separately and not during
                  // Event iteration
}

/* **************************************************************************************
 * class SCOREP_Score_ParameterEvent
 ***************************************************************************************/
SCOREP_Score_ParameterEvent::SCOREP_Score_ParameterEvent( void )
    : SCOREP_Score_Event( "ParameterInt" )
{
}

bool
SCOREP_Score_ParameterEvent::occursInRegion( const string& regionName,
                                             bool          hasHits )
{
    if ( regionName.find( '=', 0 ) == string::npos )
    {
        return false;
    }
    if ( regionName.substr( 0, 9 ) == "instance=" )
    {
        return false;                                        // Dynamic region
    }
    return true;
}

/* **************************************************************************************
 * class SCOREP_Score_NameMatchEvent
 ***************************************************************************************/
SCOREP_Score_NameMatchEvent::SCOREP_Score_NameMatchEvent( const string&      eventName,
                                                          const set<string>& regionNames,
                                                          bool               hasTimestamp )
    : SCOREP_Score_Event( eventName )
{
    m_region_names  = regionNames;
    m_has_timestamp = hasTimestamp;
}

bool
SCOREP_Score_NameMatchEvent::occursInRegion( const string& regionName,
                                             bool          hasHits )
{
    return m_region_names.count( regionName ) == 1;
}

bool
SCOREP_Score_NameMatchEvent::hasTimestamp( void ) const
{
    return m_has_timestamp;
}

/* **************************************************************************************
 * class SCOREP_Score_PrefixMatchEvent
 ***************************************************************************************/
SCOREP_Score_PrefixMatchEvent::SCOREP_Score_PrefixMatchEvent(
    const string&        eventName,
    const deque<string>& regionPrefix,
    bool                 hasTimestamp )
    : SCOREP_Score_Event( eventName )
{
    m_region_prefix = regionPrefix;
    m_has_timestamp = hasTimestamp;
}

bool
SCOREP_Score_PrefixMatchEvent::occursInRegion( const string& regionName,
                                               bool          hasHits )
{
    for ( deque<string>::iterator i = m_region_prefix.begin();
          i != m_region_prefix.end(); i++ )
    {
        if ( *i == regionName.substr( 0, i->length() ) )
        {
            return true;
        }
    }
    return false;
}

bool
SCOREP_Score_PrefixMatchEvent::hasTimestamp( void ) const
{
    return m_has_timestamp;
}
