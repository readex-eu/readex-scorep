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
 * \file CubeDoubleValue.h
 * \brief  Provides the "double" (64bit) build-in type as "Value"
 ************************************************/
#ifndef __DOUBLE_VALUE_H
#define __DOUBLE_VALUE_H

#include <istream>
#include "CubeValue.h"
#include <iostream>
#include <string>


namespace cube
{
class Connection;
class DoubleValue;
extern Preallocator<DoubleValue>double_preallocator;



/**
 * Value works with 8 bytes of the double as the whole int value, and as 8 bytes (endianess). Therefore it
 * defined a union.
 */
typedef union
{
    double dValue;
    char   aValue[ sizeof( double ) ];
} d_value_t;

class DoubleValue :
    public Value
{
protected:
    d_value_t value;                        // / "heart" of the value.



public:
    DoubleValue();
    DoubleValue( double );

    virtual
    ~DoubleValue()
    {
    };

    virtual unsigned
    inline
    getSize()
    {
        return sizeof( double );
    }

    inline
    virtual double
    getDouble()
    {
        return value.dValue;
    };

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

    inline
    virtual DoubleValue*
    clone()
    {
        return new DoubleValue( 0. );
    }
    inline
    virtual DoubleValue*
    copy()
    {
        return new DoubleValue( value.dValue );
    }

    inline
    virtual void
    operator+=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.dValue += ( ( DoubleValue* )chval )->value.dValue;
    }

    inline
    virtual void
    operator-=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.dValue -= ( ( DoubleValue* )chval )->value.dValue;
    }

    inline
    virtual void
    operator*=( double dval )
    {
        value.dValue *= dval;
    }                                 // needed by algebra tools

    inline
    virtual void
    operator/=( double dval )
    {
        if ( dval == 0. )
        {
            std::cerr << "ERROR: DEVISION BY ZERO!" << std::endl;
        }
        value.dValue /= dval;
    }

    void*
    operator new( size_t size );

    void
    operator delete( void* p );

    inline
    virtual void
    Free()
    {
        delete ( DoubleValue* )this;
    }


    virtual void
    operator=( double );

    virtual void
    operator=( Value* );

    virtual bool
    isZero()
    {
        return value.dValue == 0.;
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_DOUBLE;
    };
    virtual void
        normalizeWithClusterCount( uint64_t );


    virtual
    void
    init_new()
    {
    };

    virtual
    void
    clone_new( Value* )
    {
    };
};
}
#endif
