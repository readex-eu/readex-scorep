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
 *
 * \file CubeHistogramValue.h
 * \brief  Provides the experimental value, savinng a histogram.
 ************************************************/
#ifndef __HISTOGRAM_VALUE_H
#define __HISTOGRAM_VALUE_H

#include <istream>
#include "CubeError.h"
#include "CubeValues.h"

#include <iostream>

namespace cube
{
class Connection;
class HistogramValue;
extern Preallocator<HistogramValue>histogram_preallocator;


typedef struct plet
{
    unsigned array;
    double   point;
    size_t   index;
} plet;

typedef struct wplet
{
    double point;
    int    curr_index;
    int    next_index;
    double curr_weight;
    double next_weight;
} wplet;

bool
pair_compare( plet a,
              plet b );




typedef   struct
{
    int pivot;
}  __attribute__ ( ( __packed__ ) ) HistogramValueConfig;


class HistogramValue :
    public Value
{
private:
    static
    std::vector<HistogramValueConfig>parameters;
    size_t                           index;
    size_t                           num_terms;  // / supposed number of terms stored, set by Metric()

    std::vector<wplet> in;
    std::vector<plet>  merger;
    std::vector<double>bins;

    uint64_t N;                       // / Number of values.
    double*  values;                  // / "heart" of the value.

    double min;
    double max;


    bool valid;


public:
    HistogramValue();
    HistogramValue( uint64_t n );
    HistogramValue( double,
                    double,
                    uint64_t );
//                                 HistogramValue(double);
    HistogramValue( double,
                    double,
                    uint64_t,
                    double* );

    HistogramValue( uint64_t,
                    double* );


    virtual
    ~HistogramValue();

    virtual HistogramValue*
    clone();
    virtual HistogramValue*
    copy();


    void
    processArguments( std::vector<std::string>args );  // / parse arguments from metric
    void
    setNumTerms( size_t num );                         // / set number of terms stored according to metric



    inline
    virtual unsigned
    getSize()
    {
        return ( 2 + N ) * sizeof( double );
    }

    // /////////

    void*
    operator new( size_t size );
    void
    operator delete( void* p );


    virtual void
    Free()
    {
        ::delete[] values;
        values = NULL;
        histogram_preallocator.Put( this );
    }

    virtual bool
    asInclusiveMetric()
    {
        return false;
    }


    const double*
    get_values( void )
    {
        return values;
    }

    uint64_t
    get_number_of_values( void )
    {
        return N;
    }

    void
    prepare();

    void
    calculate_distance( std::vector<plet>&  _merger,
                        std::vector<wplet>& _in1,
                        std::vector<wplet>& _in2,
                        std::vector<wplet>& _in );

    double
    sum();

    double
    get_min( void )
    {
        return min;
    }

    double
    get_max( void )
    {
        return max;
    }

    std::vector<double>
    get_bins( void )
    {
        return bins;
    }

    std::vector<wplet>&
    get_points()
    {
        return in;
    };



    virtual void
    operator+=( Value* );
    virtual void
    operator-=( Value* );

    virtual void
    operator*=( double );                            // needed by algebra tools
    virtual void
    operator/=( double );                            // needed by algebra tools



    virtual void
    operator=( double );

    virtual void
    operator=( Value* );


    virtual char*
    fromStream( char* );
    virtual double*
    fromStreamOfDoubles( double* stream );

    virtual char*
    toStream( char* );

    /**
     * Assign value from a Cube connection.
     * @param connection Cube connection stream.
     */
    virtual void
    fromStream( Connection& connection );

    /**
     * Send value through a Cube connection.
     * @param connection Cube connection stream.
     */
    virtual void
    toStream( Connection& connection ) const;

    virtual char*
    transformStream( char*,
                     SingleValueTrafo* );


    inline
    virtual double
    getDouble()
    {
        double sum = 0;
        for ( unsigned i = 0; i < N; i++ )
        {
            sum += values[ i ];
        }
        return sum;
    }
    virtual uint16_t
    getUnsignedShort();
    virtual int16_t
    getSignedShort();
    virtual uint32_t
    getUnsignedInt();
    virtual int32_t
    getSignedInt();
    virtual uint64_t
    getUnsignedLong();
    virtual int64_t
    getSignedLong();
    virtual char
    getChar();

    virtual std::string
    getString();

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_HISTOGRAM;
    };                                             // not supported yet



    virtual bool
    isZero();                        // faked check. Should be fixed if NDoubles get to use in cube.

    virtual void
        normalizeWithClusterCount( uint64_t );

    virtual
    void
    init_new();

    virtual
    void
    clone_new( Value* );
};
}
#endif
