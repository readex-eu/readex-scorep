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
 * \file CubeSignedLongValue.h
 * \brief Provides the "signed long" (64bit) build-in type as "Value"
 ************************************************/
#ifndef __SIGNED_LONG_VALUE_H
#define __SIGNED_LONG_VALUE_H

#include <istream>
#include "CubeValue.h"

#include <iostream>

namespace cube
{
class Connection;
class SignedLongValue;
extern Preallocator<SignedLongValue> int64_preallocator;


/**
 * Value works with 8 bytes of the signed long as the whole int value, and as 8 bytes (endianess). Therefore it
 * defined a union.
 */
typedef union
{
    int64_t ilValue;
    char    aValue[ sizeof( int64_t ) ];
} il_value_t;

class SignedLongValue : public Value
{
protected:
    il_value_t value;                          // / "heart" of the value.
public:
    SignedLongValue();

    SignedLongValue( int64_t );


    virtual
    ~SignedLongValue()
    {
    };
    virtual unsigned
    getSize()
    {
        return sizeof( int64_t );
    }

    inline
    virtual double
    getDouble()
    {
        return ( double )value.ilValue;
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
    virtual Value*
    clone()
    {
        return new SignedLongValue( 0 );
    }
    inline
    virtual Value*
    copy()
    {
        return new SignedLongValue( value.ilValue );
    }

    inline
    virtual void
    operator+=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.ilValue += ( ( SignedLongValue* )chval )->value.ilValue;
    }

    inline
    virtual void
    operator-=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.ilValue -= ( ( SignedLongValue* )chval )->value.ilValue;
    }

    inline
    virtual void
    operator*=( double dval )
    {
        value.ilValue *= dval;
    }                                 // needed by algebra tools

    inline
    virtual void
    operator/=( double dval )
    {
        if ( dval == 0. )
        {
            std::cerr << "ERROR: DEVISION BY ZERO!" << std::endl;
        }
        value.ilValue /= dval;
    }


    void*
    operator new( size_t size );
    void
    operator delete( void* p );


    virtual void
    Free()
    {
        delete ( SignedLongValue* )this;
    }

    virtual void
    operator=( double );

    virtual void
    operator=( Value* );


    virtual bool
    isZero()
    {
        return value.ilValue == 0L;
    };
    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_INT64;
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
