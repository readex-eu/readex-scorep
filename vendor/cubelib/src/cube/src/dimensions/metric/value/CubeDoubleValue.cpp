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
 * \file CubeDoubleValue.cpp
 * \brief   Defines the methods of the "DoubleValue".
 ************************************************/

#include "config.h"

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "CubeConnection.h"
#include "CubeValues.h"

using namespace std;
using namespace cube;

DoubleValue::DoubleValue()
{
    isSingleValue = true;

    value.dValue = 0.;
}





DoubleValue::DoubleValue( double dv )
{
    isSingleValue = true;

    value.dValue = dv;
}



uint16_t
DoubleValue::getUnsignedShort()
{
    return ( uint16_t )value.dValue;
}
int16_t
DoubleValue::getSignedShort()
{
    return ( int16_t )value.dValue;
}

uint32_t
DoubleValue::getUnsignedInt()
{
    return ( uint32_t )value.dValue;
}
int32_t
DoubleValue::getSignedInt()
{
    return ( int32_t )value.dValue;
}
uint64_t
DoubleValue::getUnsignedLong()
{
    return ( uint64_t )value.dValue;
}
int64_t
DoubleValue::getSignedLong()
{
    return ( int64_t )value.dValue;
}
/**
 * As char will be returned just first char of the char representation
 */
char
DoubleValue::getChar()
{
    return value.aValue[ 0 ];
}

/**
 * Creates the string representation of the value.
 */
string
DoubleValue::getString()
{
    stringstream sstr;
    string       str;
    sstr << value.dValue;
    sstr >> str;
    return str;
}



/**
 * Sets the value from stream and returns the position in stream right after the value.
 */
char*
DoubleValue::fromStream( char* cv )
{
    memcpy( value.aValue, cv, sizeof( double ) );
    return cv + sizeof( double );
}

double*
DoubleValue::fromStreamOfDoubles( double* cv )
{
    return ( double* )fromStream( ( char* )cv );
}

/**
 * Saves the value in the stream and returns the position in stream right after the value.
 */
char*
DoubleValue::toStream( char* cv )
{
    memcpy( cv, value.aValue, sizeof( double ) );
    return cv + sizeof( double );
}

// overloaded new operator
void*
DoubleValue::operator new( size_t size )
{
    return ( void* )double_preallocator.Get();
}
// delete operator overloaded
void
DoubleValue::operator delete( void* p )
{
    double_preallocator.Put( ( DoubleValue* )p );
}


/*
   DoubleValue
   DoubleValue::operator+( const DoubleValue& ch )
   {
    DoubleValue tmp = *this;
    tmp.value.dValue += ch.value.dValue;
    return tmp;
   }

   DoubleValue
   DoubleValue::operator-( const DoubleValue& ch )
   {
    DoubleValue tmp = *this;
    tmp.value.dValue -= ch.value.dValue;
    return tmp;
   }*/





void
DoubleValue::operator=( double d )
{
    value.dValue = d;
}
/*
   void
   DoubleValue::operator=( char c )
   {
    value.dValue = ( double )c;
   }


   void
   DoubleValue::operator=( uint16_t ui )
   {
    value.dValue = ( double )ui;
   }

   void
   DoubleValue::operator=( uint32_t i )
   {
    value.dValue = ( double )i;
   }

   void
   DoubleValue::operator=( uint64_t ul )
   {
   // #warning LOST PRECISION
    value.dValue = ( double )ul;
   }

   void
   DoubleValue::operator=( int16_t si )
   {
    value.dValue = ( double )si;
   }

   void
   DoubleValue::operator=( int32_t i )
   {
    value.dValue = ( double )i;
   }

   void
   DoubleValue::operator=( int64_t sl )
   {
   // #warning LOST PRECISION
    value.dValue = ( double )sl;
   }


   void
   DoubleValue::operator=( string v )
   {
    stringstream sstr;
    sstr << v;
    sstr >> value.dValue;
   }


   DoubleValue
   DoubleValue::operator=( DoubleValue v )
   {
    if ( &v == this )
    {
        return *this;
    }
    value.dValue = v.getDouble();
    return *this;
   }
 */

void
DoubleValue::normalizeWithClusterCount( uint64_t N )
{
    value.dValue = value.dValue / ( double )N;
}



void
DoubleValue::operator=( Value* v )
{
    value.dValue = v->getDouble();
}

void cube::DoubleValue::fromStream( Connection& connection )
{
    connection >> this->value.dValue;
}

void cube::DoubleValue::toStream( Connection& connection ) const
{
    connection << this->value.dValue;
}
