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
 * \file CubeUnsignedLongValue.h
 * \brief Provides the "unsigned long" (64bit) build-in type as "Value"
 ************************************************/

#ifndef __UNSIGNED_LONG_VALUE_H
#define __UNSIGNED_LONG_VALUE_H

#include <istream>
#include "CubeValue.h"

#include <iostream>


namespace cube
{
class Connection;
class UnsignedLongValue;
extern Preallocator<UnsignedLongValue> uint64_preallocator;


/**
 * Value works with 8 bytes of the unsigned long as teh whole int value, and as 8 bytes (endianess). Therefore it
 * defined a union.
 */
typedef union
{
    uint64_t ulValue;
    char     aValue[ sizeof( uint64_t ) ];
} ul_value_t;

class UnsignedLongValue : public Value
{
private:
    ul_value_t value;                     // / "heart" of the value.
public:
    UnsignedLongValue();
    UnsignedLongValue( uint64_t );
    virtual
    ~UnsignedLongValue()
    {
    };
    virtual unsigned
    getSize()
    {
        return sizeof( uint64_t );
    }

    inline
    virtual double
    getDouble()
    {
        return ( double )value.ulValue;
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
        return new UnsignedLongValue( 0 );
    }
    inline
    virtual Value*
    copy()
    {
        return new UnsignedLongValue( value.ulValue );
    }


    inline
    virtual void
    operator+=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.ulValue += ( ( UnsignedLongValue* )chval )->value.ulValue;
    }

    inline
    virtual void
    operator-=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        value.ulValue -= ( ( UnsignedLongValue* )chval )->value.ulValue;
    }

    inline
    virtual void
    operator*=( double dval )
    {
        value.ulValue *= dval;
    }                                 // needed by algebra tools

    inline
    virtual void
    operator/=( double dval )
    {
        if ( dval == 0. )
        {
            std::cerr << "ERROR: DEVISION BY ZERO!" << std::endl;
        }
        value.ulValue /= dval;
    }

    void*
    operator new( size_t size );
    void
    operator delete( void* p );

    virtual void
    Free()
    {
        delete ( UnsignedLongValue* )this;
    }

    virtual void
    operator=( double );

    virtual void
    operator=( Value* );

    virtual bool
    isZero()
    {
        return value.ulValue == 0UL;
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_UINT64;
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
