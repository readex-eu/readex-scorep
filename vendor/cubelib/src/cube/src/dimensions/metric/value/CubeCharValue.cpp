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
 * \file CubeCharValue.cpp
 * \brief
 ************************************************/

#include "config.h"

#include <cstring>
#include <sstream>
#include <string>

#include "CubeConnection.h"
#include "CubeValues.h"

using namespace std;
using namespace cube;



CharValue::CharValue()
{
    isSingleValue = true;
    value.cValue  = 0;
}
// CharValue::CharValue( uint16_t uv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )uv;
// }
//
// CharValue::CharValue( uint32_t uv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )uv;
// }
// CharValue::CharValue( uint64_t uv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )uv;
// }
// CharValue::CharValue( int16_t uv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )uv;
// }
// CharValue::CharValue( int32_t uv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )uv;
// }
// CharValue::CharValue( int64_t uv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )uv;
// }
//
//
//
// CharValue::CharValue( double dv )
// {
//     isSingleValue = true;
//     value.cValue  = ( char )dv;
// }
CharValue::CharValue( char cv )
{
    isSingleValue = true;
    value.cValue  = cv;
}
// CharValue::CharValue( char* cv )
// {
//     isSingleValue = true;
//     memcpy( value.aValue, cv,  sizeof( char ) );
// }
//
// unsigned
// CharValue::getSize()
// {
//     return sizeof( char );
// }
// double
// CharValue::getDouble()
// {
//     return ( double )value.cValue;
// }


uint16_t
CharValue::getUnsignedShort()
{
    return ( uint16_t )value.cValue;
}
int16_t
CharValue::getSignedShort()
{
    return ( int16_t )value.cValue;
}


uint32_t
CharValue::getUnsignedInt()
{
    return ( uint32_t )value.cValue;
}
int32_t
CharValue::getSignedInt()
{
    return ( int32_t )value.cValue;
}


uint64_t
CharValue::getUnsignedLong()
{
    return ( uint64_t )value.cValue;
}
int64_t
CharValue::getSignedLong()
{
    return ( int64_t )value.cValue;
}


char
CharValue::getChar()
{
    return value.cValue;
}

string
CharValue::getString()
{
    stringstream sstr;
    string       str;
    sstr << value.cValue;
    sstr >> str;
    return str;
}

// Value*
// CharValue::clone()
// {
//     return new CharValue( 0 );
// }
//
// Value*
// CharValue::copy()
// {
//     return new CharValue( value.cValue );
// }
//
//
char*
CharValue::fromStream( char* cv )
{
    memcpy( value.aValue, cv, sizeof( char ) );
    return cv + sizeof( char );
}


double*
CharValue::fromStreamOfDoubles( double* cv )
{
    value.cValue = ( char )( *cv );
    return cv + sizeof( double );
}



char*
CharValue::toStream( char* cv )
{
//     c_value_t   tmp;
    memcpy( cv, value.aValue, sizeof( char ) );
//     value.cValue = tmp.cValue;
    return cv + sizeof( char );
}

// overloaded new operator
void*
CharValue::operator new( size_t size )
{
    return ( void* )char_preallocator.Get();
}
// delete operator overloaded
void
CharValue::operator delete( void* p )
{
    char_preallocator.Put( ( CharValue* )p );
}

/*
   CharValue
   CharValue::operator+( const CharValue& ch )
   {
    CharValue tmp = *this;
    tmp.value.cValue += ch.value.cValue;
    return tmp;
   }

   CharValue
   CharValue::operator-( const CharValue& ch )
   {
    CharValue tmp = *this;
    tmp.value.cValue -= ch.value.cValue;
    return tmp;
   }


   void
   CharValue::operator+=( Value* chval )
   {
    if ( chval == NULL )
    {
        return;
    }
    value.cValue += ( ( CharValue* )chval )->value.cValue;
   }

   void
   CharValue::operator-=( Value* chval )
   {
    if ( chval == NULL )
    {
        return;
    }
    value.cValue -= ( ( CharValue* )chval )->value.cValue;
   }


   void
   CharValue::operator*=( double dval )
   {
    double _d = ( double )value.cValue;
    _d          *= dval;
    value.cValue = ( char )_d;
   }

   void
   CharValue::operator/=( double dval )
   {
    if ( dval == 0. )
    {
        cerr << "ERROR: DEVISION BY ZERO!" << endl;
    }
    value.cValue /= ( char )dval;
   }




 */
void
CharValue::operator=( double d )
{
// #warning LOST PRECISION!
    value.cValue = ( char )d;
}
/*
   void
   CharValue::operator=( char c )
   {
    value.cValue = c;
   }


   void
   CharValue::operator=( uint16_t ui )
   {
   // #warning LOST PRECISION
    value.cValue = ( char )ui;
   }

   void
   CharValue::operator=( uint32_t i )
   {
   // #warning LOST PRECISION
    value.cValue = ( char )i;
   }

   void
   CharValue::operator=( uint64_t ul )
   {
   // #warning LOST PRECISION
    value.cValue = ( char )ul;
   }

   void
   CharValue::operator=( int16_t si )
   {
   // #warning LOST PRECISION
    value.cValue = ( char )si;
   }

   void
   CharValue::operator=( int32_t i )
   {
   // #warning LOST PRECISION
    value.cValue = ( char )i;
   }

   void
   CharValue::operator=( int64_t sl )
   {
   // #warning LOST PRECISION
    value.cValue = ( char )sl;
   }


   void
   CharValue::operator=( string v )
   {
    value.cValue =  ( v.size() > 0 ) ? v[ 0 ] : ' ';
   }




   CharValue
   CharValue::operator=( CharValue v )
   {
    if ( &v == this )
    {
        return *this;
    }
    value.cValue = v.getChar();
    return *this;
   }
 */

void
CharValue::operator=( Value* v )
{
    value.cValue = v->getChar();
}

void
CharValue::normalizeWithClusterCount( uint64_t N )
{
    value.cValue = ( char )( ( double )value.cValue / ( double )N );
}

void
cube::CharValue::fromStream( Connection& connection )
{
    connection >> value.cValue;
}

void
cube::CharValue::toStream( Connection& connection ) const
{
    connection << value.cValue;
}
