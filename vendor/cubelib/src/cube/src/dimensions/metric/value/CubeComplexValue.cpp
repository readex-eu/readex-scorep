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
 * \file CubeComplexValue.cpp
 * \brief   Defines the methods of the "ComplexValue".
 ************************************************/

#include "config.h"

#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

#include "CubeConnection.h"
#include "CubeValues.h"

using namespace std;
using namespace cube;

std::vector<cube::ComplexValueConfig> cube::ComplexValue::parameters;

ComplexValue::ComplexValue() : r_value( 0. ), i_value( 0. )
{
    isSingleValue = false;
}



ComplexValue::ComplexValue( double r_dv,
                            double i_dv ) : r_value( r_dv ), i_value( i_dv )
{
    isSingleValue = false;
}



uint16_t
ComplexValue::getUnsignedShort()
{
    return ( uint16_t )getDouble();
}


int16_t
ComplexValue::getSignedShort()
{
    return ( int16_t )getDouble();
}

uint32_t
ComplexValue::getUnsignedInt()
{
    return ( uint32_t )getDouble();
}


int32_t
ComplexValue::getSignedInt()
{
    return ( int32_t )getDouble();
}


uint64_t
ComplexValue::getUnsignedLong()
{
    return ( uint64_t )getDouble();
}


int64_t
ComplexValue::getSignedLong()
{
    return ( int64_t )getDouble();
}




char
ComplexValue::getChar()
{
    return ' ';
}

string
ComplexValue::getString()
{
    string tmp = "(" + r_value.getString() + ")";
    tmp += " +i(";
    tmp += i_value.getString();
    tmp += ")";
    return tmp;
}


char*
ComplexValue::fromStream( char* cv )
{
    return i_value.fromStream( r_value.fromStream( cv ) );
}


double*
ComplexValue::fromStreamOfDoubles( double* cv )
{
    return ( double* )( i_value.fromStream( r_value.fromStream( ( char* )cv ) ) );
}

char*
ComplexValue::toStream( char* cv )
{
//     cout << " FRO MSTREAM" << endl;
    return i_value.toStream( r_value.toStream( cv ) );
}

char*
ComplexValue::transformStream( char* stream, SingleValueTrafo* trafo )
{
//     cout << " COMLEX VALUE TRANSFORM STREAM " << (void *)stream << endl;
    return i_value.transformStream( r_value.transformStream( stream, trafo ), trafo );
}


// overloaded new operator
void*
ComplexValue::operator new( size_t size )
{
    return ( void* )complex_preallocator.Get();
}
// delete operator overloaded
void
ComplexValue::operator delete( void* p )
{
    complex_preallocator.Put( ( ComplexValue* )p );
}

/*
   ComplexValue
   ComplexValue::operator+( const ComplexValue& ch )
   {
    ComplexValue tmp = *this;
    tmp.r_value = tmp.r_value + ch.r_value;
    tmp.i_value = tmp.i_value + ch.i_value;
    return tmp;
   }

   ComplexValue
   ComplexValue::operator-( const ComplexValue& ch )
   {
    ComplexValue tmp = *this;
    tmp.r_value = tmp.r_value - ch.r_value;
    tmp.i_value = tmp.i_value - ch.i_value;
    return tmp;
   }*/

/*
   void
   ComplexValue::operator+=( Value* chval )
   {
    if ( chval == NULL )
    {
        return;
    }
    r_value += ( ( Value* )( &( ( ( ComplexValue* )chval )->r_value ) ) );
    i_value += ( ( Value* )( &( ( ( ComplexValue* )chval )->i_value ) ) );
   }

   void
   ComplexValue::operator-=( Value* chval )
   {
    if ( chval == NULL )
    {
        return;
    }
    r_value -= ( ( Value* )( &( ( ( ComplexValue* )chval )->r_value ) ) );
    i_value -= ( ( Value* )( &( ( ( ComplexValue* )chval )->i_value ) ) );
   }


   void
   ComplexValue::operator*=( double dval )
   {
    r_value *= dval;
    i_value *= dval;
   }

   void
   ComplexValue::operator/=( double dval )
   {
    if ( dval == 0. )
    {
        cerr << "ERROR: DEVISION BY ZERO!" << endl;
    }
    r_value /= dval;
    i_value /= dval;
   }
 */



void
ComplexValue::operator=( double d )
{
    throw RuntimeError( "Impossible to assign a single double value  to ComplexValue" );
}
/*
   void
   ComplexValue::operator=( char c )
   {
    throw RuntimeError( "Impossible to assign a single char value  to ComplexValue" );
   }


   void
   ComplexValue::operator=( uint16_t us )
   {
    throw RuntimeError( "Impossible to assign a single unsigned short value  to ComplexValue" );
   }


   void
   ComplexValue::operator=( uint32_t ui )
   {
    throw RuntimeError( "Impossible to assign a single unsigned integer value  to ComplexValue" );
   }

   void
   ComplexValue::operator=( uint64_t ul )
   {
    throw RuntimeError( "Impossible to assign a single unsigned long long  value  to ComplexValue" );
   }



   void
   ComplexValue::operator=( int16_t us )
   {
    throw RuntimeError( "Impossible to assign a single signed short value to ComplexValue" );
   }


   void
   ComplexValue::operator=( int32_t ui )
   {
    throw RuntimeError( "Impossible to assign a single signed integer value to ComplexValue" );
   }

   void
   ComplexValue::operator=( int64_t ul )
   {
    throw RuntimeError( "Impossible to assign a single signed long long value to ComplexValue" );
   }

   void
   ComplexValue::operator=( string v )
   {
   //     cout << " WHAT THE F!" << endl;
   // #warning " ComplexValue method operator=(string) is implemented in not optimal way. FLEX+BISON would be better approach"
    size_t pos = v.find( ") +i(" );    // position of "live" in str
    r_value = v.substr( 1, pos - 1 );

    string istr    = v.substr( pos + 5 );
    size_t end_pos = istr.find( ")" );
    i_value = istr.substr( 0, end_pos );
   }



   ComplexValue
   ComplexValue::operator=( ComplexValue comp )
   {
    r_value = comp.getRealValue();
    i_value = comp.getImaginaryValue();
    return *this;
   }*/


void
ComplexValue::operator=( Value* val )
{
    throw RuntimeError( "Impossible to assign a single general Value to ComplexValue" );
}



void
ComplexValue::normalizeWithClusterCount( uint64_t N )
{
    r_value.normalizeWithClusterCount( N );
    i_value.normalizeWithClusterCount( N );
}

void
ComplexValue::init_new()
{
    ComplexValueConfig a;
    ( cube::ComplexValue::parameters ).push_back( a );
    index = ( cube::ComplexValue::parameters ).size() - 1;
};

void
ComplexValue::clone_new( Value* v )
{
    index = ( static_cast<ComplexValue*>( v ) )->index;
}

void
cube::ComplexValue::fromStream( Connection& connection )
{
    r_value.fromStream( connection );
    i_value.fromStream( connection );
}

void
cube::ComplexValue::toStream( Connection& connection ) const
{
    r_value.toStream( connection );
    i_value.toStream( connection );
}
