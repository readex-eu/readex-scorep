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
 * \file CubeRateValue.h
 * \brief  Provides the value, consisting of two "Value"'s. Result value is the fraction of them. Used to
 ************************************************/
#ifndef __RATE_VALUE_H
#define __RATE_VALUE_H

#include <istream>
#include "CubeDoubleValue.h"

#include <iostream>

namespace cube
{
class Connection;
class RateValue;
extern Preallocator<RateValue> rate_preallocator;

typedef   struct
{
    int pivot;
}  __attribute__ ( ( __packed__ ) ) RateValueConfig;


class RateValue : public Value
{
private:
    static
    std::vector<RateValueConfig> parameters;
    size_t                       index;

protected:
    DoubleValue main_value;                      // / "heart 1" of the value.
    DoubleValue duratio_value;                   // / "heart 2" of the value.

public:
    RateValue();

    RateValue( double,
               double );

    virtual
    ~RateValue()
    {
    };
    inline
    virtual unsigned
    getSize()
    {
        return main_value.getSize() + duratio_value.getSize();
    }

    inline
    virtual double
    getDouble()
    {
        double m_tmp = main_value.getDouble();
        double d_tmp = duratio_value.getDouble();

        if ( d_tmp == 0. )
        {
            return ( m_tmp < 0 ) ? -DBL_MAX : DBL_MAX;
        }
        else
        {
            return m_tmp / d_tmp;
        }
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

    virtual DoubleValue
    getMainValue()
    {
        return main_value;
    };
    virtual DoubleValue
    getDuratioValue()
    {
        return duratio_value;
    };

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
    virtual Value*
    clone()
    {
        return new RateValue( 0., 0. );
    }
    inline
    virtual Value*
    copy()
    {
        return new RateValue( main_value.getDouble(),
                              duratio_value.getDouble()
                              );
    }


    inline
    virtual void
    operator+=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        main_value    += ( ( Value* )( &( ( ( RateValue* )chval )->main_value ) ) );
        duratio_value += ( ( Value* )( &( ( ( RateValue* )chval )->duratio_value ) ) );
    }

    inline
    virtual void
    operator-=( Value* chval )
    {
        if ( chval == NULL )
        {
            return;
        }
        main_value    -= ( ( Value* )( &( ( ( RateValue* )chval )->main_value ) ) );
        duratio_value -= ( ( Value* )( &( ( ( RateValue* )chval )->duratio_value ) ) );
    }

    inline
    virtual void
    operator*=( double dval )
    {
        main_value    *= dval;
        duratio_value *= dval;
    }                                 // needed by algebra tools

    inline
    virtual void
    operator/=( double dval )
    {
        if ( dval == 0. )
        {
            std::cerr << "ERROR: DEVISION BY ZERO!" << std::endl;
            dval = DBL_MIN;
        }
        main_value    /= dval;
        duratio_value /= dval;
    }

    void*
    operator new( size_t size );
    void
    operator delete( void* p );

    virtual void
    Free()
    {
        delete ( RateValue* )this;
    }

    virtual void
    operator=( double );

    virtual void
    operator=( Value* );


    virtual bool
    isZero()
    {
        return main_value.isZero() && !duratio_value.isZero();
    };

    virtual DataType
    myDataType()
    {
        return CUBE_DATA_TYPE_RATE;
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
