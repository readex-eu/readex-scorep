/****************************************************************************
**  CUBE        http://www.scalasca.org/                                   **
*****************************************************************************
**  Copyright (c) 2016                                                     **
**  Forschungszentrum Juelich GmbH, Juelich Supercomputing Centre          **
**                                                                         **
**  Copyright (c) 2015                                                     **
**  German Research School for Simulation Sciences GmbH,                   **
**  Laboratory for Parallel Programming                                    **
**                                                                         **
**  Copyright (c) 2015                                                     **
**  RWTH Aachen University, JARA-HPC                                       **
**                                                                         **
**  This software may be modified and distributed under the terms of       **
**  a BSD-style license.  See the COPYING file in the package base         **
**  directory for details.                                                 **
****************************************************************************/


/*-------------------------------------------------------------------------*/
/**
 *  @file
 *  @ingroup CUBE_lib.base
 *  @brief   Definition of the class CubeProxy.
 **/
/*-------------------------------------------------------------------------*/


#include "config.h"

#include "CubeProxy.h"

#include <cassert>
#include <iostream>
#include <limits>

#include "CubeIoProxy.h"
#include "CubeNetworkProxy.h"
#include "CubeUrl.h"

using namespace std;
using namespace cube;

namespace cube
{
CubeProxy::CubeProxy()
    : mUrl(""),
      mInitialized(false)
{
}


CubeProxy::~CubeProxy()
{
}


CubeProxy*
CubeProxy::create(std::string url_string)
{
    CubeProxy* obj = NULL;
    Url        url = Url(url_string);
    if (url.getProtocol() == "cube")
    {
        // open remote Cube via Client/Server
        obj = new CubeNetworkProxy(url.toString());
        obj->defineAttribute("cubename", obj->mUrl.toString());
    }
    else
    {
        // open local Cube
        obj = new CubeIoProxy();
        obj->defineAttribute("cubename", obj->mUrl.getPath());
    }

    obj->mUrl = url;
    obj->setInitialized(true);

    return obj;
}


bool
CubeProxy::isInitialized() const
{
    return mInitialized;
}


void
CubeProxy::setInitialized(bool initialized)
{
    mInitialized = initialized;
}


const Url&
CubeProxy::getUrl() const
{
    return mUrl;
}


void
CubeProxy::setUrlPath(const std::string& path)
{
    mUrl.setPath(path);
}
}    /* namespace cube */
