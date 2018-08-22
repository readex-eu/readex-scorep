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
 * \file CubeUnsignedValue.h
 * \brief Provides the "unsigned int" (32bit) build-in type as "Value"
 ************************************************/
#ifndef __UNSIGNED_VALUE_H
#define __UNSIGNED_VALUE_H

#include <istream>
#include "CubeValue.h"

#include <iostream>

namespace cube
{
class Connection;
class UnsignedValue;
extern Preallocator<UnsignedValue> uint32_preallocator;

/**
 * Value works with 4 bytes of the unsigned int as teh whole int value, and as 4 bytes (endianess). Therefore it
 * defined a union.
 */
typedef union
{
    uint32_t uValue;
    char     aValue[ sizeof( uint32_t ) ];
} u_value_t;



class UnsignedValue : public Value
{
protected:
    u_value_t value;                     // / "heart" of the value.
public:
    UnsignedValue();
    UnsignedValue( uint32_t );

    virtual
    ~UnsignedValue()
    {
    };
    inline
    virtual unsigned
    getSize()
    {
        return sizeof( uint32_t );
    }

    inline
    virtual double
    getDouble()
    {
        return ( double )value.uValue;
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
        return new UnsignedValue( 0 );
    }
    inline
    virtual Value*
    copy()
    {
        return new UnsignedValue( value.uValue );
    }

    inline
    virtual void
    operator+=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.uValue += ( ( UnsignedValue* )chval )->value.uValue;
    }

    inline
    virtual void
    operator-=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.uValue -= ( ( UnsignedValue* )chval )->value.uValue;
    }

    inline
    virtual void
    operator*=( double dval )
    {
        value.uValue *= dval;
    }                                 // needed by algebra tools

    inline
    virtual void
    operator/=( double dval )
    {
        if ( dval == 0. )
        {
            std::cerr << "ERROR: DEVISION BY ZERO!" << std::endl;
        }
        value.uValue /= dval;
    }


    void*
    operator new( size_t size );
    void
    operator delete( void* p );

    virtual void
    Free()
    {
        delete ( UnsignedValue* )this;
    }

    virtual void
    operator=( double );

    virtual void
    operator=( Value* );

    virtual bool
    isZero()
    {
        return value.uValue == 0;
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_UINT32;
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
