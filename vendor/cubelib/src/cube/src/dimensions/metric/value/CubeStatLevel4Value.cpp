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
 * \file CubeStatLevel4Value.cpp
 * \brief Defines the methods of the "StatLevel4Value".
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

std::vector<cube::StatLevel4ValueConfig> cube::StatLevel4Value::parameters;

StatLevel4Value::StatLevel4Value() : N( 1 ), Sum( 0. ), Sum2( 0. ), Sum3( 0. ), Sum4( 0. )
{
    isSingleValue = false;
}

StatLevel4Value::StatLevel4Value( uint32_t n,
                                  double   sum,
                                  double   sum2,
                                  double   sum3,
                                  double   sum4 ) : N( n ), Sum( sum ), Sum2( sum2 ), Sum3( sum3 ), Sum4( sum4 )
{
    isSingleValue = false;
}


unsigned
StatLevel4Value::getSize()
{
    return
        N.getSize() +
        Sum.getSize() +
        Sum2.getSize() +
        Sum3.getSize() +
        Sum4.getSize();
}

/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
double
StatLevel4Value::getDouble()
{
    return 0.;
}

/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
uint16_t
StatLevel4Value::getUnsignedShort()
{
    return ( uint16_t )getDouble();
}

/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
int16_t
StatLevel4Value::getSignedShort()
{
    return ( int16_t )getDouble();
}


/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
uint32_t
StatLevel4Value::getUnsignedInt()
{
    return ( uint32_t )getDouble();
}

/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
int32_t
StatLevel4Value::getSignedInt()
{
    return ( int32_t )getDouble();
}



/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
uint64_t
StatLevel4Value::getUnsignedLong()
{
    return ( uint64_t )getDouble();
}

/**
 * Single value of StatLevel4Value is impossible. Therefore an exception is thrown.
 */
int64_t
StatLevel4Value::getSignedLong()
{
    return ( int64_t )getDouble();
}



/**
 * Char is meaningless representation of StatLevel4Value. Therefore an exception is thrown.
 */
char
StatLevel4Value::getChar()
{
    return ' ';
}



/**
 * Creates the string representation of the value.
 */
string
StatLevel4Value::getString()
{
// #warning " STRING representation of TAV Value should be developed"
    calcAvgVarSkewKur();
    stringstream sstr;
    string       str;
    sstr << "(" << Average << "," << Variance << "," << Skewness << "," << Kurtosis << ")"  << endl;
    sstr >> str;
    return str;
}




/**
 * Creates the copy and sets the value to 0.
 */
Value*
StatLevel4Value::clone()
{
    return new StatLevel4Value( 1, 0., 0., 0., 0. );
}


/**
 * Creates the copy.
 */
Value*
StatLevel4Value::copy()
{
// #warning "Missing interface for unsigned long long"
    return new StatLevel4Value( N.getUnsignedInt(), Sum.getDouble(), Sum2.getDouble(), Sum3.getDouble(), Sum4.getDouble() );
}


/**
 * Sets the value from stream and returns the position in stream right after the value.
 */
char*
StatLevel4Value::fromStream( char* cv )
{
    return Sum4.fromStream(
               Sum3.fromStream(
                   Sum2.fromStream(
                       Sum.fromStream(
                           N.fromStream( cv ) ) ) ) );
}

double*
StatLevel4Value::fromStreamOfDoubles( double* cv )
{
    return Sum4.fromStreamOfDoubles(
               Sum3.fromStreamOfDoubles(
                   Sum2.fromStreamOfDoubles(
                       Sum.fromStreamOfDoubles(
                           N.fromStreamOfDoubles( cv ) ) ) ) );
}



/**
 * Saves the value in the stream and returns the position in stream right after the value.
 */
char*
StatLevel4Value::toStream( char* cv )
{
    return Sum4.toStream(
               Sum3.toStream(
                   Sum2.toStream(
                       Sum.toStream(
                           N.toStream( cv ) ) ) ) );
}

/**
 * Transforms the endianness in the stream according its layout
 */
char*
StatLevel4Value::transformStream( char* cv, SingleValueTrafo* trafo )
{
    return Sum4.transformStream(
               Sum3.transformStream(
                   Sum2.transformStream(
                       Sum.transformStream(
                           N.transformStream( cv, trafo ), trafo ), trafo ), trafo ), trafo );
}


/*


   StatLevel4Value
   StatLevel4Value::operator+( const StatLevel4Value& ch )
   {
    StatLevel4Value tmp = *this;
    tmp.N    = tmp.N + ch.N;
    tmp.Sum  = tmp.Sum + ch.Sum;
    tmp.Sum2 = tmp.Sum2 + ch.Sum2;
    tmp.Sum3 = tmp.Sum3 + ch.Sum3;
    tmp.Sum4 = tmp.Sum4 + ch.Sum4;
    return tmp;
   }

   StatLevel4Value
   StatLevel4Value::operator-( const StatLevel4Value& ch )
   {
    StatLevel4Value tmp = *this;
    tmp.N    = tmp.N - ch.N;
    tmp.Sum  = tmp.Sum - ch.Sum;
    tmp.Sum2 = tmp.Sum2 - ch.Sum2;
    tmp.Sum3 = tmp.Sum3 - ch.Sum3;
    tmp.Sum4 = tmp.Sum4 - ch.Sum4;
    return tmp;
   }*/


void
StatLevel4Value::operator+=( Value* chval )
{
    if ( chval == NULL )
    {
        return;
    }
    N    += ( ( Value* )( &( ( ( StatLevel4Value* )chval )->N ) ) );
    Sum  += ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum ) ) );
    Sum2 += ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum2 ) ) );
    Sum3 += ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum3 ) ) );
    Sum4 += ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum4 ) ) );
}

void
StatLevel4Value::operator-=( Value* chval )
{
    if ( chval == NULL )
    {
        return;
    }
    N    -= ( ( Value* )( &( ( ( StatLevel4Value* )chval )->N ) ) );
    Sum  -= ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum ) ) );
    Sum2 -= ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum2 ) ) );
    Sum3 -= ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum3 ) ) );
    Sum4 -= ( ( Value* )( &( ( ( StatLevel4Value* )chval )->Sum4 ) ) );
}


void
StatLevel4Value::operator*=( double dval )
{
// multiplication with an scalas destroys the relation between variance and average.
//     N *= dval;
    Sum  *= dval;
    Sum2 *= dval * dval;
    Sum3 *= dval * dval * dval;
    Sum4 *= dval * dval * dval * dval;
}

void
StatLevel4Value::operator/=( double dval )
{
    if ( dval == 0. )
    {
        cerr << "ERROR: DEVISION BY ZERO!" << endl;
    }
    else
    {
//         N /= dval;
        Sum  /= dval;
        Sum2 /= dval;
        Sum3 /= dval;
        Sum4 /= dval;
    }
}


void
StatLevel4Value::operator=( double d )
{
    throw  RuntimeError( "Impossible to assign a  single double to StatLevel4Value" );
}
/*
   void
   StatLevel4Value::operator=( char c )
   {
    throw RuntimeError( "Impossible to assign a  single char to StatLevel4Value" );
   }


   void
   StatLevel4Value::operator=( uint16_t ui )
   {
    throw RuntimeError( "Impossible to assign a single unsigned  short int to StatLevel4Value" );
   }

   void
   StatLevel4Value::operator=( uint32_t i )
   {
    throw RuntimeError( "Impossible to assign a single unsigned  int to StatLevel4Value" );
   }

   void
   StatLevel4Value::operator=( uint64_t ul )
   {
    throw RuntimeError( "Impossible to assign a  single unswigned long long  int to StatLevel4Value" );
   }

   void
   StatLevel4Value::operator=( int16_t si )
   {
    throw RuntimeError( "Impossible to assign a  single signed  short int to StatLevel4Value" );
   }

   void
   StatLevel4Value::operator=( int32_t i )
   {
    throw RuntimeError( "Impossible to assign a single  int to StatLevel4Value" );
   }

   void
   StatLevel4Value::operator=( int64_t sl )
   {
    throw RuntimeError( "Impossible to assign a single signed long long  short int to StatLevel4Value" );
   }

   void
   StatLevel4Value::operator=( string v )
   {
   // #warning " StatLevel4Value method operator=(string) is not implemented yet"
    cerr << " StatLevel4Value method operator=(string) is not implemented yet" << endl;
   }


   StatLevel4Value
   StatLevel4Value::operator=( StatLevel4Value v )
   {
    if ( &v == this )
    {
        return *this;
    }
   //     resetValue();
   //     v.resetValue();
    N    = v.getN();
    Sum  = v.getSum();
    Sum2 = v.getSum2();
    Sum3 = v.getSum3();
    Sum4 = v.getSum4();
    return *this;
   }

 */



// overloaded new operator
void*
StatLevel4Value::operator new( size_t size )
{
    return ( void* )stat4_preallocator.Get();
}
// delete operator overloaded
void
StatLevel4Value::operator delete( void* p )
{
    stat4_preallocator.Put( ( StatLevel4Value* )p );
}


void
StatLevel4Value::operator=( Value* v )
{
// #warning "Error handling has to be done"
    throw RuntimeError( "Impossible to assign a single general Value to StatLevel4Value" );
}

// -------------------- PRIVATE METHODS- ------------------------

void
StatLevel4Value::calcAvgVarSkewKur()
{
    uint32_t n  = N.getUnsignedInt();
    double   dn = N.getDouble();
    double   s  = Sum.getDouble();
    double   s2 = Sum2.getDouble();
    double   s3 = Sum3.getDouble();
    double   s4 = Sum4.getDouble();

    Average  =  ( n == 0 ) ? s / ( dn + 1e-256 )  : s / dn;
    Variance = ( n == 1 ) ? 0.             : 1. / ( dn - 1. ) * sqrt( s2 - 1. / dn * s * s );
    Skewness = ( n == 1 ) ? 0.             : ( 1. / dn *  ( s3 - 3. / dn * s2 * s + 2. / ( dn * dn ) * s * s * s ) ) / ( Variance * Variance * Variance );
    Kurtosis = ( n == 1 ) ? 0.             : ( 1. / dn * ( s4 - 4. / dn * s3 * s + 6. / ( dn * dn ) * s2 * s * s - 3. / ( dn * dn * dn ) * s * s * s * s ) ) / ( Variance * Variance * Variance * Variance ) - 3.;
}




void
StatLevel4Value::normalizeWithClusterCount( uint64_t _N )
{
    N.normalizeWithClusterCount( _N );
    Sum.normalizeWithClusterCount( _N );
    Sum2.normalizeWithClusterCount( _N );
    Sum3.normalizeWithClusterCount( _N );
    Sum4.normalizeWithClusterCount( _N );
}

void
StatLevel4Value::init_new()
{
    StatLevel4ValueConfig a;
    ( cube::StatLevel4Value::parameters ).push_back( a );
    index = ( cube::StatLevel4Value::parameters ).size() - 1;
};

void
StatLevel4Value::clone_new( Value* v )
{
    index = ( static_cast<StatLevel4Value*>( v ) )->index;
}

void
cube::StatLevel4Value::fromStream( Connection& connection )
{
    N.fromStream( connection );
    Sum.fromStream( connection );
    Sum2.fromStream( connection );
    Sum3.fromStream( connection );
    Sum4.fromStream( connection );
}

void
cube::StatLevel4Value::toStream( Connection& connection ) const
{
    N.toStream( connection );
    Sum.toStream( connection );
    Sum2.toStream( connection );
    Sum3.toStream( connection );
    Sum4.toStream( connection );
}

