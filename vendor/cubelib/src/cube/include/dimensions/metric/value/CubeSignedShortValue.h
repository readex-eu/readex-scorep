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
 * \file CubeSignedShortValue.h
 * \brief   Provides the "signed short" (16bit) build-in type as "Value"
 ************************************************/
#ifndef __SIGNED_SHORT_VALUE_H
#define __SIGNED_SHORT_VALUE_H

#include <istream>
#include "CubeValue.h"

#include <iostream>

namespace cube
{
class Connection;
class SignedShortValue;
extern Preallocator<SignedShortValue> int16_preallocator;

/**
 * Value works with 2 bytes of the signed sort as teh whole int value, and as 2 bytes (endianess). Therefore it
 * defined a union.
 */
typedef union
{
    int16_t isValue;
    char    aValue[ sizeof( int16_t ) ];
} is_value_t;

class SignedShortValue : public Value
{
protected:
    is_value_t value;                        // / "heart" of the value.
public:
    SignedShortValue();
    SignedShortValue( int16_t );

    virtual
    ~SignedShortValue()
    {
    };

    inline
    virtual unsigned
    getSize()
    {
        return sizeof( int16_t );
    }

    inline
    virtual double
    getDouble()
    {
        return ( double )value.isValue;
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
        return new SignedShortValue( 0 );
    }
    inline
    virtual Value*
    copy()
    {
        return new SignedShortValue( value.isValue );
    }

    inline
    virtual void
    operator+=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.isValue += ( ( SignedShortValue* )chval )->value.isValue;
    }

    inline
    virtual void
    operator-=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.isValue -= ( ( SignedShortValue* )chval )->value.isValue;
    }

    inline
    virtual void
    operator*=( double dval )
    {
        value.isValue *= dval;
    }                                 // needed by algebra tools

    inline
    virtual void
    operator/=( double dval )
    {
        if ( dval == 0. )
        {
            std::cerr << "ERROR: DEVISION BY ZERO!" << std::endl;
        }
        value.isValue /= dval;
    }

    void*
    operator new( size_t size );
    void
    operator delete( void* p );

    virtual void
    Free()
    {
        delete ( SignedShortValue* )this;
    }


    virtual void
    operator=( double );

    virtual void
    operator=( Value* );


    virtual bool
    isZero()
    {
        return value.isValue == 0;
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_INT16;
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
