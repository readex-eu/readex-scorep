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
 * \file CubeStatLevel2Value.h
 * \brief Provide a value, which carries an "average" value and variance. Sum of two "CubeStatLevel2Value" results again a valid average value and variance. .
 *
 * It saves an int N and (\f$ \sum_i^N {x_i} \f$) for calculation of average.
 *
 *
 */

#ifndef __STAT_LEVEL2_VALUE_H
#define __STAT_LEVEL2_VALUE_H

#include <istream>

#include <cstring>
#include <string>
#include <iostream>
#include "CubeValues.h"
#include "CubeTrafos.h"


namespace cube
{
class Connection;
class StatLevel2Value;
extern Preallocator<StatLevel2Value> stat2_preallocator;

typedef   struct
{
    int pivot;
}  __attribute__ ( ( __packed__ ) ) StatLevel2ValueConfig;

/**
 *  The TAU value for atomic metrics. NOT TESTED
 */
class StatLevel2Value : public Value
{
private:

    static
    std::vector<StatLevel2ValueConfig> parameters;
    size_t                             index;

    double Average;                                                                // /< Saves an average value, calculated like \$ \frac{Sum}{N}. \$ Is not saved in file, but calculated always on the fly.
    double Variance;                                                               // /< Saves a variance value, calculated like like \$ (N=0)?0: \sqrt{ \frac{1}{N-1}( Sum2 - \frac{Sum \times Sum}{N}  )}\$ Is not saved in file, but calculated always on the fly.
    void
    calcAvgVar();                                                                  // /< Performs the calcualtion of Average

protected:
    UnsignedValue N;
    DoubleValue   Sum;
    DoubleValue   Sum2;

public:
    StatLevel2Value();
    StatLevel2Value( uint32_t,
                     double,
                     double );

    virtual
    ~StatLevel2Value()
    {
    };                                                                 // /< Vitual destructor allows, that every inherited class destroy itself properly.
    virtual unsigned
    getSize();                                                         // /< Value has to define size of it in bytes. Used to apply transformations and saving/loading in the file.
    virtual double
    getDouble();                                                       // /< Returns the double representation of itself, if possible.
    virtual uint16_t
    getUnsignedShort();                                                // /< Returns the unsigned 32bit representation of itself, if possible.
    virtual int16_t
    getSignedShort();                                                  // /< Returns the signed 32bit representation of itself, if possible.
    virtual uint32_t
    getUnsignedInt();                                                  // /< Returns the unsigned 32bit representation of itself, if possible.
    virtual int32_t
    getSignedInt();                                                    // /< Returns the signed 32bit representation of itself, if possible.
    virtual uint64_t
    getUnsignedLong();                                                 // /< Returns the unsigned 64bit representation of itself, if possible.
    virtual int64_t
    getSignedLong();                                                   // /< Returns the signed 64bit representation of itself, if possible.
    virtual char
    getChar();                                                         // /< Returns the single byte representation of itself, if possible.
    virtual std::string
    getString();                                                       // /< Returns the textual representation of itself, if possible. Used to save itself in Cube3 format.

    virtual UnsignedValue
    getN()
    {
        return N;
    };
    virtual DoubleValue
    getSum()
    {
        return Sum;
    };
    virtual DoubleValue
    getSum2()
    {
        return Sum2;
    };



    virtual char*
    fromStream( char* );                                                        // /< Constructs itself from the stream. The first stream byte used as starting point.
    virtual double*
    fromStreamOfDoubles( double* stream );
    virtual char*
    toStream( char* );                                                          // /< Write to the stream the stream-representation of itself. The first stream byte used as starting point.

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
                     SingleValueTrafo* );                                           // /< Applyes the transformation on the stream according to its layout. Used to deal with different endianess. Needed for comples types with non trivial layout.

    virtual Value*
    clone();                                                                     // /< Makes a copy of itself and sets the value of the copy to 0.
    virtual Value*
    copy();                                                                      // /< Makes a copy of itself and keeps the value.

    void*
    operator new( size_t size );
    void
    operator delete( void* p );

    virtual void
    Free()
    {
        delete ( StatLevel2Value* )this;
    }

    virtual void
    operator+=( Value* );                                                         // /< Defines + operation (for calculation of the excl/incl values).
    virtual void
    operator-=( Value* );                                                         // /< Defines - operation (for calculation of the excl values).
    virtual void
    operator*=( double );                                                         // /< Defines * operation (for calculation of the excl/incl values). Needed by algebra tools.
    virtual void
    operator/=( double );                                                         // /< Defines / operation (for calculation of the excl/incl values). Needed by algebra tools.


    virtual void
    operator=( double );          // /< Allows to assign its value from the build in double.

    virtual void
    operator=( Value* );          // /< Allows to assign its value from another object of same type. [WARNING, real type is not checked]


    virtual bool
    isZero()
    {
        return Sum.isZero() && Sum2.isZero() && N.isZero();
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_NONE;
    };                                             // not supported yet
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
