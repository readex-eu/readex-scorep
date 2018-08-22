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


#ifndef __DIVIDE_EVALUATION_H
#define __DIVIDE_EVALUATION_H 0

#include "CubeBinaryEvaluation.h"

namespace cube
{
class DivideEvaluation : public BinaryEvaluation
{
public:
    DivideEvaluation();
    DivideEvaluation( GeneralEvaluation*,
                      GeneralEvaluation* );
    virtual
    ~DivideEvaluation();

    inline
    virtual
    double
    eval() const
    {
        double denom = arguments[ 0 ]->eval();
        if ( denom == 0. )
        {
            return 0.;
        }
        double divid = arguments[ 1 ]->eval();

        if ( divid == 0 )
        {
            if ( denom > 0 )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }


    inline
    virtual
    double
    eval( Cnode* _cnode, CalculationFlavour _cf, Sysres* _sf, CalculationFlavour _tf ) const
    {
        double denom = arguments[ 0 ]->eval( _cnode, _cf,
                                             _sf, _tf );
        if ( denom == 0. )
        {
            return 0.;
        }
        double divid = arguments[ 1 ]->eval( _cnode, _cf,
                                             _sf, _tf );

        if ( divid == 0. )
        {
            if ( denom > 0. )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }


    inline
    virtual
    double
    eval( Cnode* _cnode, CalculationFlavour _cf ) const
    {
        double denom = arguments[ 0 ]->eval( _cnode, _cf );
        if ( denom == 0. )
        {
            return 0.;
        }
        double divid = arguments[ 1 ]->eval( _cnode, _cf );




        if ( divid == 0 )
        {
            if ( denom > 0 )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }

    inline
    virtual
    double*
    eval_row( Cnode*             _cnode,
              CalculationFlavour _cf ) const
    {
        double* denom = arguments[ 0 ]->eval_row( _cnode, _cf );
        if ( denom == NULL )
        {
            return NULL;
        }
        double* divid = arguments[ 1 ]->eval_row( _cnode, _cf );

        double* result = denom;
        for ( size_t i = 0; i < row_size; i++ )
        {
            if ( denom[ i ] == 0. )
            {
                result[ i ] = 0.;
                continue;
            }

            if ( divid == NULL ||  divid[ i ] == 0 )
            {
                if ( denom[ i ] > 0 )
                {
                    result[ i ] = DBL_MAX;
                }
                else
                {
                    result[ i ] = -DBL_MAX;
                }
            }
            else
            {
                result[ i ] = ( denom[ i ] ) / ( divid[ i ] );
            }
        }
        delete[] divid;
        return result;
    }


    inline
    virtual
    void
    print() const
    {
        std::cout << "(";
        arguments[ 0 ]->print();
        std::cout << "/";
        arguments[ 1 ]->print();
        std::cout << ")";
    };

    inline
    virtual
    double
    eval( double arg1,
          double arg2 ) const
    {
        double denom = arguments[ 0 ]->eval( arg1, arg2 );
        if ( denom == 0. )
        {
            return 0.;
        }

        double divid = arguments[ 1 ]->eval( arg1, arg2 );

        if ( divid == 0 )
        {
            if ( denom > 0 )
            {
                return DBL_MAX;
            }
            else
            {
                return -DBL_MAX;
            }
        }
        else
        {
            return ( denom ) / ( divid );
        }
    }
};
};

#endif
