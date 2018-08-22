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
 *
 * \file CubeTauAtomicValue.cpp
 * \brief Defines the methods of the "TauAtomicValue".
 ************************************************/

#include "config.h"

#include <cmath>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "CubeConnection.h"
#include "CubeError.h"
#include "CubeValues.h"

using namespace std;
using namespace cube;

std::vector<cube::TauAtomicValueConfig> cube::TauAtomicValue::parameters;

TauAtomicValue::TauAtomicValue() : N( 0 ), MinValue(), MaxValue(), Sum( 0. ), Sum2( 0. )
{
    isSingleValue = false;
}

TauAtomicValue::TauAtomicValue( unsigned n,
                                double   min,
                                double   max,
                                double   sum,
                                double   sum2 ) : N( n ), MinValue( min ), MaxValue( max ), Sum( sum ), Sum2( sum2 )
{
    isSingleValue = false;
}


uint16_t
TauAtomicValue::getUnsignedShort()
{
    calcAvgVar();
    return ( uint16_t )Average;
}

int16_t
TauAtomicValue::getSignedShort()
{
    calcAvgVar();
    return ( int16_t )Average;
}

uint32_t
TauAtomicValue::getUnsignedInt()
{
    calcAvgVar();
    return ( uint32_t )Average;
}

int32_t
TauAtomicValue::getSignedInt()
{
    calcAvgVar();
    return ( int32_t )Average;
}


uint64_t
TauAtomicValue::getUnsignedLong()
{
    calcAvgVar();
    return ( uint64_t )Average;
}

int64_t
TauAtomicValue::getSignedLong()
{
    calcAvgVar();
    return ( int64_t )Average;
}


/**
 * As char will be returned just first char of the char representation
 */
char
TauAtomicValue::getChar()
{
    return ' ';
}



/**
 * Creates the string representation of the value.
 */
string
TauAtomicValue::getString()
{
// #warning " STRING representation of TAV Value should be developed"
    calcAvgVar();
    stringstream sstr;
    string       str;
    sstr << "(" <<  N.getString() << "," << MinValue.getString() << "," << MaxValue.getString() << "):";
    if ( N.getUnsignedInt() != 0. )
    {
        sstr << Average << "," << Deviation << endl;
    }
    else
    {
        sstr << "-" << "," << "-" << endl;
    }
    sstr >> str;
    return str;
}


double
TauAtomicValue::getDouble()
{
    calcAvgVar();
    TauAtomicValuePivot pivot = getConfig().pivot;
    switch ( pivot )
    {
        case TAU_DEV:
            return Deviation;
        case TAU_N:
            return N.getDouble();
        case TAU_MIN:
            return MinValue.getDouble();
        case TAU_MAX:
            return MaxValue.getDouble();
        case TAU_SUM:
            return Sum.getDouble();
        case TAU_SUM2:
            return Sum2.getDouble();
        case TAU_AVG:
        default:
            return Average;
    }
}


// overloaded new operator
void*
TauAtomicValue::operator new( size_t size )
{
    return ( void* )tau_preallocator.Get();
}
// delete operator overloaded
void
TauAtomicValue::operator delete( void* p )
{
    tau_preallocator.Put( ( TauAtomicValue* )p );
}


/**
 * Sets the value from stream and returns the position in stream right after the value.
 */
char*
TauAtomicValue::fromStream( char* cv )
{
    return Sum2.fromStream( Sum.fromStream( MaxValue.fromStream( MinValue.fromStream( N.fromStream( cv ) ) ) ) );
}

double*
TauAtomicValue::fromStreamOfDoubles( double* cv )
{
    return Sum2.fromStreamOfDoubles( Sum.fromStreamOfDoubles( MaxValue.fromStreamOfDoubles( MinValue.fromStreamOfDoubles( N.fromStreamOfDoubles( cv ) ) ) ) );
}



/**
 * Saves the value in the stream and returns the position in stream right after the value.
 */
char*
TauAtomicValue::toStream( char* cv )
{
    return Sum2.toStream( Sum.toStream( MaxValue.toStream( MinValue.toStream( N.toStream( cv ) ) ) ) );
}

/**
 * Transforms the endianness in the stream according its layout
 */
char*
TauAtomicValue::transformStream( char* cv, SingleValueTrafo* trafo )
{
    return Sum2.transformStream( Sum.transformStream( MaxValue.transformStream( MinValue.transformStream( N.transformStream( cv, trafo ), trafo ), trafo ), trafo ), trafo );
}




void
TauAtomicValue::operator=( double d )
{
    throw  RuntimeError( "Impossible to assign a  single double to TauAtomicValue" );
}


void
TauAtomicValue::operator=( Value* v )
{
    throw  RuntimeError( "Impossible to assign a singlegeneral Value to TauAtomicValue" );
}


// -------------------- PRIVATE METHODS- ------------------------

void
TauAtomicValue::calcAvgVar()
{
    uint32_t n  = N.getUnsignedInt();
    double   dn = N.getDouble();
    double   s  = Sum.getDouble();
    double   s2 = Sum2.getDouble();
    Average   =  ( n == 0 ) ? s / ( dn + 1e-256 )  : s / dn;
    double variance =  ( n == 0 )? 0. : (s2  - (s*s)/dn );
    double epsilon = (  1. - (  s *s) / (dn * s2 ) );
    Deviation = ( ( n == 0 ) || ( std::fabs(epsilon) <= 1E-14 ) ) ? 0. : sqrt( (1/ dn ) *  variance );
}


void
TauAtomicValue::normalizeWithClusterCount( uint64_t norm )
{
    N.normalizeWithClusterCount( norm );
    Sum.normalizeWithClusterCount( norm );
    Sum2.normalizeWithClusterCount( norm );
}



void
TauAtomicValue::init_new()
{
    TauAtomicValueConfig a;
    a.pivot = TAU_AVG;
    ( cube::TauAtomicValue::parameters ).push_back( a );
    index = ( cube::TauAtomicValue::parameters ).size() - 1;
};

void
TauAtomicValue::clone_new( Value* v )
{
    index = ( static_cast<TauAtomicValue*>( v ) )->index;
}

void
cube::TauAtomicValue::fromStream( Connection& connection )
{
    N.fromStream( connection );
    MinValue.fromStream( connection );
    MaxValue.fromStream( connection );
    Sum.fromStream( connection );
    Sum2.fromStream( connection );
}

void
cube::TauAtomicValue::toStream( Connection& connection ) const
{
    N.toStream( connection );
    MinValue.toStream( connection );
    MaxValue.toStream( connection );
    Sum.toStream( connection );
    Sum2.toStream( connection );
}
TauAtomicValueConfig&
TauAtomicValue::getConfig()
{
    return TauAtomicValue::parameters[ index ];
}
