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
 * \file CubeServices.cpp
 * \brief Provides an implementation of the service functions for cube library.

 */
#include "config.h"

// #ifndef CUBE_COMPRESSED
#include <fstream>
#include <regex.h>
// #else
#include "CubeZfstream.h"
// #endif


#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <inttypes.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctime>
#include <stdlib.h>
#include <algorithm>
#include <cctype>
#include <functional>
#include  <iomanip>
#include <cwchar>
#include <sstream>

#include "CubeTypes.h"
#include "CubeServices.h"
#include "CubeError.h"
#include "CubeValue.h"
#include "CubeCnode.h"
#include "Cube.h"
#include "CubeFileBaseLayout.h"
//
//
#include "CubePlatformsCompat.h"




using namespace std;

enum ReplaceDirection { SERVICES_REPLACE_FORWARD, SERVICES_REPLACE_BACKWARD };

/**
 * Performs the actual replacing (escaping).
 *
 * Replacement happens in two steps
 * 1. First the list of the positions of parts, which have to be replaced, is created.
 * 2. Then replacement only at saved positions is performed.
 *
 * Note, that in the case  "String -> XML", first ampersands are replaced, then all another symbols.
 * in the case "XML ->String", first all another symbols, and then ampersand.
 * It removes possible infinite loop with ampersands.
 *********************************/
static
std::string
replaceSymbols( unsigned from, unsigned to, std::string str, ReplaceDirection direction )
{
    static const unsigned MapSize                    = 5;
    static std::string    ReplaceMap[ MapSize ][ 2 ] = { { "&", "&amp;" }, { "<", "&lt;" }, { ">", "&gt;" }, { "'", "&apos;" }, { "\"", "&quot;" } };

    std::string::size_type pos;
    int                    Start     = 0;
    int                    Condition = MapSize;
    int                    increment = 1;
    if ( direction == SERVICES_REPLACE_BACKWARD )
    {
        Start     = MapSize - 1;
        Condition = -1;
        increment = -1;
    }

    for ( int i = Start; i != Condition; i = i + ( increment ) )
    {
        std::vector<unsigned>PositionsToReplace;
        PositionsToReplace.clear();

        pos = str.find( ReplaceMap[ i ][ from ].c_str() );
        while ( pos != std::string::npos )
        {
            PositionsToReplace.push_back( pos );
            pos = str.find( ReplaceMap[ i ][ from ].c_str(), pos + ReplaceMap[ i ][ from ].size() );
        }

        unsigned n_replace = 0;
        for ( std::vector<unsigned>::iterator iter = PositionsToReplace.begin(); iter != PositionsToReplace.end(); ++iter, ++n_replace )
        {
            str.replace( *iter + n_replace * ( ( int )ReplaceMap[ i ][ to ].size()  - ( int )ReplaceMap[ i ][ from ].size() ), ReplaceMap[ i ][ from ].size(),  ReplaceMap[ i ][ to ].c_str() );
        }
    }
    return str;
}

/**
 *  Replaces all symbols like "<" or ">" with corresponding HTML equivalents "lt;", "gt;"
 *********************************/
std::string
cube::services::escapeToXML( std::string str )
{
    return replaceSymbols( 0, 1, str, SERVICES_REPLACE_FORWARD );
}

/**
 *  Replaces all symbols like "gl;" or "lt;" with corresponding HTML equivalents ">", "<"
 *********************************/
std::string
cube::services::escapeFromXML( std::string str )
{
    return replaceSymbols( 1, 0, str, SERVICES_REPLACE_BACKWARD );
}


/**
 *  Returns system errorno as a string for error handling
 */
std::string
cube::services::cube_errno()
{
    int         _er     = errno;
    std::string message = strerror( _er );
    return message;
}



//^(all|mpi_wait){1}[,\s]{1}|[,\s]{1}(all|mpi_waitall){1}[,\s]{1}|[,\s]{1}(all|mpi_wait)?$

/// checks, if in the first list of metric unique names current is given or "all"
bool
cube::services::is_metric_present( const std::string& _list, const std::string& _name )
{
    char    msgbuf[ 255 ];
    string  reg = "^(all|" + _name + "){1}[,\\s]{1}|[,\\s]{1}(all|" + _name + "){1}[,\\s]{1}|[,\\s]{1}(all|" + _name + "){1}$|^[,\\s]*(all|" + _name + "){1}[,\\s]*$";
    string  val = _list;
    regex_t rgT;
    int     reti = regcomp( &rgT, reg.c_str(), REG_EXTENDED );
    if ( reti != 0 )
    {
        regerror( reti, &rgT, msgbuf, sizeof( msgbuf ) );
        std::cerr << "Cannot compile RegEx " << reg << std::endl;
        regfree( &rgT );
        return false;
    }

    reti = regexec( &rgT, val.c_str(), 0, NULL, 0 );

    if ( reti == 0 )
    {
        regfree( &rgT );
        return true;
    }
    else
    if ( reti == REG_NOMATCH )
    {
        regfree( &rgT );
        return false;
    }
    else
    {
        regerror( reti, &rgT, msgbuf, sizeof( msgbuf ) );
        std::cerr << "Error while execution RegEx " << reg << std::endl;
        regfree( &rgT );
        return false;
    };
}




/** Gets sa file name as XXX.cube and returns XXX
 *    used to work with input paameters in tools
 */

std::string
cube::services::get_cube_name( std::string cubename )
{
    if ( is_cube3_name( cubename ) )
    {
        return get_cube3_name( cubename );
    }
    if ( is_cube3_gzipped_name( cubename ) )
    {
        return get_cube3_gzipped_name( cubename );
    }

    if ( is_cube4_name( cubename ) )
    {
        return get_cube4_name( cubename );
    }
    std::cerr <<  "File " << cubename << " is neither cube3 nor cube4" << std::endl;
    return "___NO_FILE___";
}


std::string
cube::services::get_cube3_name( std::string cube3name )
{
    int found =  cube3name.rfind( ".cube" );
    found = ( found < 0 ) ? cube3name.length() : found;
    return cube3name.substr( 0, found );
}

std::string
cube::services::get_cube3_gzipped_name( std::string cube3name )
{
    int found =  cube3name.rfind( ".cube.gz" );
    found = ( found < 0 ) ? cube3name.length() : found;
    return cube3name.substr( 0, found );
}


std::string
cube::services::get_cube4_name( std::string cube4name )
{
    if ( cube4name.rfind( ".cubex" ) != string::npos )
    {
        return cube4name.substr( 0, cube4name.rfind( ".cubex" ) );
    }
    if ( cube4name.rfind( ".cube.gz" ) != string::npos )
    {
        return cube4name.substr( 0, cube4name.rfind( ".cube.gz" ) );
    }
    if ( cube4name.rfind( ".cube" ) != string::npos )
    {
        return cube4name.substr( 0, cube4name.rfind( ".cube" ) );
    }
    return cube4name.substr( 0, cube4name.rfind( ".cubex" ) );
}



bool
cube::services::is_cube3_name( std::string cube3name )
{
    int  found  = cube3name.rfind( ".cube" );
    int  found2 = -1;
    bool result =  ( found == ( ( int )cube3name.size() - 5 ) );

// #ifdef CUBE_COMPRESSED
    found2 = cube3name.rfind( ".cube.gz" );
    result = result || ( found == ( ( int )cube3name.size() - 8 ) );
// #endif
    if ( found < 0 and found2 < 0 )
    {
        return false;
    }

    return result;
}

bool
cube::services::is_cube3_gzipped_name( std::string cube3name )
{
    int found = cube3name.rfind( ".cube.gz" );
    if ( found < 0 )
    {
        return false;
    }
    bool result = ( found == ( ( int )cube3name.size() - 8 ) );
    return result;
}


bool
cube::services::is_cube4_name( std::string cube4name )
{
    int  pos  = cube4name.rfind( cube::FileBaseLayout::getAnchorExtension() );
    bool test = true;
    if ( pos < 0 )
    {
        test = false;
    }
    else
    {
        test = ( pos == ( ( int )cube4name.size() - 6 ) );
    }
    return is_cube4_tared( cube4name ) || ( test )  || is_cube4_embedded_anchor( cube4name );
}

bool
cube::services::is_cube4_tared( std::string cube4name )
{
    int pos = cube4name.rfind( ".tar" );
    if ( pos < 0 )
    {
        return false;
    }
    return pos == ( ( int )cube4name.size() - 4 );
}


// bool
// cube::services::is_cube4_hyb_dir( std::string cube4name )
// {
//     return remove_last_slashes( cube4name ).rfind( cube::FileHybridLayout::getDataDirExtension() ) == ( cube4name.size() - 11 );
// }


bool
cube::services::is_cube4_embedded_anchor( std::string cube4name )
{
    int found = cube4name.rfind( "anchor.xml" );
    if ( found < 0 )
    {
        return false;
    }
    return found == ( ( int )cube4name.size() - 10 );
}





// ----------------- filesystem service functions ----------------------------------------

bool
cube::services::is_path( std::string filename )
{
    return filename.find( '/' ) != std::string::npos;
}


std::string
cube::services::dirname( std::string filename )
{
    std::string tmp =  filename.substr( 0, ( filename.rfind( '/' ) ) );
    if ( tmp.compare( filename ) == 0 ) // no slash found
    {
        return "";
    }
    else
    {
        return tmp + "/";
    }
}

std::string
cube::services::filename( std::string filename )
{
    size_t _slash = filename.rfind( '/' );
    if ( _slash == std::string::npos )
    {
        return filename;
    }

    std::string tmp =  filename.substr( _slash + 1 );
    return tmp;
}



std::string
cube::services::remove_dotted_path( std::string path )
{
    size_t      pos   = 0;
    std::string _tmp  = path;
    std::string _str1 = "/";
    while ( ( pos = _tmp.find( "/./" ) ) != std::string::npos )
    {
        _tmp.replace( pos, 3, _str1 );
    }
    pos = 0;
    while ( ( pos = _tmp.find( "/../" ) ) != std::string::npos )
    {
        size_t _pos_of_slash = _tmp.rfind( '/', pos - 1 );
        if ( _pos_of_slash == std::string::npos )
        {
            break;
        }
        _tmp.replace( _pos_of_slash, pos + 4 - _pos_of_slash, _str1 );
    }
    return _tmp;
}


void
cube::services::create_path_for_file( std::string path )
{
    size_t   pos = 0;
    unsigned i   = 0;
    while ( ( pos = path.find( '/', pos ) ) != std::string::npos && ++i < 1000 )
    {
        std::string subpath = path.substr( 0, pos++ );   // get from "live" to the end
        int         result  = cube::services::_mkdir( subpath.c_str(), S_IRWXU | S_IROTH | S_IXOTH | S_IRGRP |  S_IXGRP );
        if ( result != 0 )
        {
            switch ( errno )
            {
                case 0:
                case EEXIST:
                    break;
                case EPERM:
                    cerr << "Cannot create " << subpath << " : not sufficient permissions" << endl;
                    break;
#ifndef __MINGW32__
                case ELOOP:
                    cerr << "Cannot create " << subpath << " : there is a loop in the name of symlink" << endl;
                    break;
#endif
                case EMLINK:
                    cerr << "Cannot create " << subpath << " : link count of parent directory would exceed " << endl;
                    break;
                case ENAMETOOLONG:
                    cerr << "Cannot create " << subpath << " : name is too long" << endl;
                    break;
                case ENOENT:
                    //                 cerr << "Cannot create " << subpath << " : a path prefix is not an existing directory or empty" << endl;
                    break;
                case ENOSPC:
                    cerr << "Cannot create " << subpath << " : file system is full" << endl;
                    break;
                case ENOTDIR:
                    cerr << "Cannot create " << subpath << " : prefix is not a directory" << endl;
                    break;
                case EROFS:
                    cerr << "Cannot create " << subpath << " : parent directory resides on read-only file system" << endl;
                    break;
            }
        }
    }
}



int
cube::services::_mkdir( const char* path, mode_t mode )
{
    return __MKDIR( path, mode );
}




std::string
cube::services::remove_last_slashes( std::string path )
{
    while ( path.at( path.length() - 1 ) == '/' )
    {
        path =  path.substr( 0, path.length() - 1 );
    }
    return path;
}



std::string
cube::services::create_random_string( uint64_t seed, unsigned length )
{
    srand( seed );
    static const unsigned size               = 79;
    static const char     asciitable[ size ] = {
        'v', 'G', 'L', '_', 'X', 'M', 'q', 'z', 'o',
        'N', 'R', 'i', '6', 'A', 'J', 'H', 'F', 'P',
        'f', '-', 's', 'I', 'K', '4', 'e', '0', '3',
        '4', 'E', 'x', 'u', 'r', 'p', '8', 'D', 'G',
        '7', 'j', 'n', '-', '_', 't', 'h', 'M', 'A',
        'l', 'Y', 'w', '3', 'B', 'd', 'Q', 'm', 'O',
        'Q', 'g', 'V', 'Z', '2', '_', 'k', '-', 'W',
        'Q', 'D', 'T', '_', 'c', 'U', '-', 'D', '9',
        'S', '1', 'y', 'a', 'b', 'C', '5'
    };
    std::string           to_return;
    to_return.reserve( length + 10 );
    for ( unsigned i = 0; i < length; ++i )
    {
        unsigned pos = ( unsigned )( size * ( rand() / ( RAND_MAX + 1.0 ) ) );
        to_return.push_back( asciitable[ pos ] );
    }
    return to_return;
}


uint64_t
cube::services::hr_time()
{
    struct timeval tv;

    if ( 0 == gettimeofday( &tv, NULL ) )
    {
        return tv.tv_sec * 1000000 + tv.tv_usec;
    }
    return 0;
}


std::string
cube::services::get_tmp_files_location()
{
    char* tmpdir = getenv( "CUBE_TMPDIR" );
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "CUBE_TEMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "CUBE_TMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "SCALASCA_TMPDIR" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "SCALASCA_TEMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "SCALASCA_TMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "SCOREP_TMPDIR" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "SCOREP_TEMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "SCOREP_TMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "TEMP" );
    }
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "TMP" );
    }
#ifdef __MINGW32__
    return "./";   // Windows we use current directory to store temparary files.... as temparary solution.
#endif
    if ( tmpdir == NULL )
    {
        tmpdir = getenv( "TMPDIR" );
    }
#ifdef __MINGW32__
    if ( tmpdir == NULL )
    {
        return std::string( "C:\\Temp\\" );
    }
#endif
    if ( tmpdir == NULL )
    {
        return std::string( "/tmp/" );
    }
    return string( tmpdir ) + "/";
}





/**
 * STL conform lowercase in cube:: namespace.
 */
string
cube::services::lowercase( const string& str )
{
    string result( str );

    transform( str.begin(), str.end(), result.begin(), fo_tolower() );

    return result;
}

/**
 * STL conform lowercase in cube:: namespace.
 */
string
cube::services::uppercase( const string& str )
{
    string result( str );

    transform( str.begin(), str.end(), result.begin(), fo_toupper() );

    return result;
}


uint64_t
cube::services::parse_clustering_key( std::string& key )
{
    return 0;
}

std::vector<uint64_t>
cube::services::parse_clustering_value( std::string& value )
{
    std::string          _value = value;
    std::vector<uint64_t>ids;
    size_t               comma_pos = ( size_t )-1;
    while ( ( comma_pos = _value.find( "," ) ) != string::npos )
    {
        std::string _number = _value.substr( 0, comma_pos );
        _value = _value.erase( 0, comma_pos + 1 );
        uint64_t id = cube::services::string2int( _number );
        ids.push_back( id );
    }
    uint64_t id = cube::services::string2int( _value );
    ids.push_back( id );
    return ids;
}


uint32_t
cube::services::string2int( const std::string& str )
{
    stringstream sstr( str );
    uint32_t     value;
    sstr >> value;
    return value;
}

std::string
cube::services::numeric2string( int value )
{
    std::string  to_return;
    stringstream sstr;
    sstr << value;
    sstr >> to_return;
    return to_return;
}

std::string
cube::services::numeric2string( double value )
{
    std::string  to_return;
    stringstream sstr;
    sstr << value;
    sstr >> to_return;
    return to_return;
}


std::string
cube::services::numeric2string( uint64_t value )
{
    std::string  to_return;
    stringstream sstr;
    sstr << value;
    sstr >> to_return;
    return to_return;
}

std::string
cube::services::numeric2string( int64_t value )
{
    std::string  to_return;
    stringstream sstr;
    sstr << value;
    sstr >> to_return;
    return to_return;
}


bool
cube::services::get_children( std::vector<cube::Cnode*>& roots,
                              uint64_t                   cluster_id,
                              std::vector<cube::Cnode*>& children
                              )
{
    for ( std::vector<cube::Cnode*>::iterator citer = roots.begin(); citer != roots.end(); ++citer )
    {
        cube::Cnode* cnode = *citer;
        if ( get_children( cnode, cluster_id, children ) )
        {
            return true; // only one loop with iteration is possible : found children - interupt further looking
        }
    }
    return false;
}

bool
cube::services::get_children( cube::Cnode*               cnode,
                              uint64_t                   cluster_id,
                              std::vector<cube::Cnode*>& children
                              )
{
    if ( cnode->get_id() == cluster_id )
    {
        for ( uint32_t i = 0; i < cnode->num_children(); ++i )
        {
            children.push_back( cnode->get_child( i ) );
        }
        return true;
    }
    for ( uint32_t i = 0; i < cnode->num_children(); ++i )
    {
        cube::Cnode* _cnode = cnode->get_child( i );
        if (   get_children( _cnode, cluster_id, children ) )
        {
            return true;
        }
    }
    return false;
}

void
cube::services::copy_tree( const cube::Cnode* cnode,
                           cube::Cnode*& clusters_root,
                           uint64_t cluster_id,
                           cube::Cnode* parent,
                           cube::Cube* cube,
                           std::map<uint64_t, uint64_t>* normalisation_factor,
                           std::vector<uint64_t>* cluster_position
                           )
{
    cube::Cnode* newCnode =  ( cube != NULL ) ?
                            cube->def_cnode(
        cnode->get_callee(),
        cnode->get_mod(),
        cnode->get_line(),
        parent
        )
                            : new cube::Cnode(
        cnode->get_callee(),
        cnode->get_mod(),
        cnode->get_line(),
        parent,
        cnode->get_id()
        );
    if ( cube != NULL )
    {
        cube->store_ghost_cnode( cnode );
    }
    if ( normalisation_factor == NULL )
    {
        newCnode->set_remapping_cnode( cnode );
    }
    else
    {
        for ( size_t i = 0; i < cluster_position->size(); ++i )
        {
            newCnode->set_remapping_cnode( ( *cluster_position )[ i ], cnode, ( *normalisation_factor )[ ( *cluster_position )[ i ] ] );
        }
    }
    if ( cnode->get_id() == cluster_id )
    {
        clusters_root = newCnode;
        return;
    }
    for ( unsigned int i = 0; i < cnode->num_children(); ++i )
    {
        cube::services::copy_tree( cnode->get_child( i ), clusters_root, cluster_id, newCnode, cube, normalisation_factor, cluster_position );
    }
}


void
cube::services::merge_tree( const cube::Cnode* parent, const cube::Cnode* merge, cube::Cube* cube,
                            std::map<uint64_t, uint64_t>* normalisation_factor,
                            std::vector<uint64_t>* cluster_position )
{
    cube::Cnode* _dummy = NULL;
    for ( unsigned i = 0; i < merge->num_children(); ++i )
    {
        cube::Cnode* child1 = merge->get_child( i );
        bool         in     = false;
        for ( unsigned j = 0; j < parent->num_children(); ++j )
        {
            cube::Cnode* child2 = parent->get_child( j );
            if ( child1->weakEqual( child2 ) )
            {
                in = true;
                if ( normalisation_factor == NULL )
                {
                    child2->set_remapping_cnode( child1 );
                }
                else
                {
                    for ( size_t i = 0; i < cluster_position->size(); ++i )
                    {
                        child2->set_remapping_cnode( ( *cluster_position )[ i ], child1,  ( *normalisation_factor )[ ( *cluster_position )[ i ] ] );
                    }
                }
                cube::services::merge_tree( child2, child1, cube, normalisation_factor, cluster_position );
                break;
            }
        }
        if ( !in )
        {
            cube::Cnode* newCnode = ( cube != NULL ) ?
                                    cube->def_cnode(
                child1->get_callee(),
                child1->get_mod(),
                child1->get_line(),
                const_cast<cube::Cnode*>( parent )
                )
                                    : new cube::Cnode(
                child1->get_callee(),
                child1->get_mod(),
                child1->get_line(),
                const_cast<cube::Cnode*>( parent ),
                child1->get_id()
                );
            if ( cube != NULL )
            {
                cube->store_ghost_cnode( child1 );
            }
            if ( normalisation_factor == NULL )
            {
                newCnode->set_remapping_cnode( child1 );
            }
            else
            {
                for ( size_t i = 0; i < cluster_position->size(); ++i )
                {
                    newCnode->set_remapping_cnode( ( *cluster_position )[ i ], child1,  ( *normalisation_factor )[ ( *cluster_position )[ i ] ] );
                }
            }
            cube::services::copy_tree( child1, _dummy, ( uint64_t )-1, newCnode, cube, normalisation_factor, cluster_position );
        }
    }
}

void
cube::services::merge_trees( const std::vector<cube::Cnode*>& cnodes, cube::Cnode* common_parent, cube::Cube* cube,
                             std::map<uint64_t, uint64_t>* normalisation_factor,
                             std::vector<uint64_t>* cluster_position )
{
    cube::Cnode* _dummy = NULL;
    for ( std::vector<cube::Cnode*>::const_iterator citer = cnodes.begin(); citer != cnodes.end(); ++citer )
    {
        cube::Cnode* cnode_to_copy = *citer;
        bool         in            = false;
        for ( unsigned i = 0; i < common_parent->num_children(); ++i )
        {
            cube::Cnode* child = common_parent->get_child( i );
            if ( child->weakEqual( cnode_to_copy ) )
            {
                in = true;
                if ( normalisation_factor == NULL )
                {
                    child->set_remapping_cnode( cnode_to_copy );
                }
                else
                {
                    for ( size_t i = 0; i < cluster_position->size(); ++i )
                    {
                        child->set_remapping_cnode( ( *cluster_position )[ i ], cnode_to_copy, ( *normalisation_factor )[ ( *cluster_position )[ i ] ] );
                    }
                }
                cube::services::merge_tree( child, cnode_to_copy, cube, normalisation_factor, cluster_position );
            }
        }
        if ( !in )
        {
            cube::services::copy_tree( cnode_to_copy, _dummy, ( uint64_t )-1,  common_parent, cube, normalisation_factor, cluster_position );
        }
    }
}


void
cube::services::gather_children( std::vector<cube::Cnode*>& children, cube::Cnode* cnode )
{
    for ( unsigned i = 0; i < cnode->num_children(); ++i )
    {
        children.push_back( cnode->get_child( i ) );
    }
}


vector<uint64_t>
cube::services::sort_and_collapse_clusters( vector<uint64_t>& clusters )
{
    vector<uint64_t>_sorted = clusters;
    std::sort( _sorted.begin(), _sorted.end() );
    vector<uint64_t>_collapsed_and_sorted;

    uint64_t _current_element = _sorted[ 0 ];
    _collapsed_and_sorted.push_back( _current_element );
    for ( size_t i = 1; i < _sorted.size(); ++i )
    {
        if ( _sorted[ i ] != _current_element )
        {
            _current_element = _sorted[ i ];
            _collapsed_and_sorted.push_back( _current_element );
        }
    }
    return _collapsed_and_sorted;
}

map<uint64_t, vector<uint64_t> >
cube::services::get_cluster_positions( vector<uint64_t>& clusters )
{
    map<uint64_t, vector<uint64_t> >positions;
    for ( size_t i = 0; i < clusters.size(); ++i )
    {
        positions[ clusters[ i ] ].push_back( i );
    }
    return positions;
}




// ----service calls to operata on rows of Values, chars or doubles
cube::Value**
cube::services::create_row_of_values( size_t _size )  // === new Value*[locationv.size()];
{
    return new cube::Value*[ _size ]();
}

double*
cube::services::create_row_of_doubles( size_t _size )  // === new double[locationv.size()](0.); ;
{
    return new double[ _size ]();
}

char*
cube::services::create_raw_row( size_t _size )  // === new double[locationv.size()](0.); ;
{
    return new char[ _size ]();
}

void
cube::services::delete_row_of_values( cube::Value** _v, size_t _size )
{
    if ( _v == NULL )
    {
        return;
    }
    for ( size_t i = 0; i < _size; i++ )
    {
        _v[ i ]->Free();
    }
    delete[] _v;
}
void
cube::services::delete_raw_row( char* v )     // == delete[] _v;
{
    delete[] v;
}

void
cube::services::delete_raw_row( double* v )     // == delete[] _v;
{
    delete[] v;
}



double*
cube::services::transform_values_to_doubles( cube::Value** row, size_t _size )
{
    if ( row == NULL )
    {
        return NULL;
    }
    double* _to_return = cube::services::create_row_of_doubles( _size );
    for ( size_t i = 0; i < _size; i++ )
    {
        _to_return[ i ] = row[ i ]->getDouble();
    }
    return _to_return;
}

char*
cube::services::transform_values_to_raw( cube::Value** row, cube::Value* v, size_t _size )
{
    if ( row == NULL )
    {
        return NULL;
    }
    char* _to_return = cube::services::create_raw_row( _size * v->getSize() );
    char* start      = ( char* )_to_return;
    if ( row != NULL )
    {
        for ( size_t i = 0; i < _size; i++ )
        {
            start = row[ i ]->toStream( start );
        }
    }
    return _to_return;
}


cube::Value**
cube::services::transform_doubles_to_values( double* row, cube::Value* v, size_t _size )
{
    if ( row == NULL )
    {
        return NULL;
    }
    cube::Value** _to_return = cube::services::create_row_of_values( _size );
    double*       start      = row;
    for ( size_t i = 0; i < _size; i++ )
    {
        cube::Value* _v = v->clone();
        start           = _v->fromStreamOfDoubles( start );
        _to_return[ i ] = _v;
    }
    return _to_return;
}


cube::Value**
cube::services::transform_raw_to_values( char* row, cube::Value* v, size_t _size )
{
    if ( row == NULL )
    {
        return NULL;
    }
    cube::Value** _to_return = cube::services::create_row_of_values( _size );
    char*         start      = ( char* )row;
    for ( size_t i = 0; i < _size; i++ )
    {
        cube::Value* _v = v->clone();
        start           = _v->fromStream( start );
        _to_return[ i ] = _v;
    }
    return _to_return;
}

double*
cube::services::transform_raw_to_doubles( char* row, cube::Value* v, size_t _size )
{
    if ( row == NULL )
    {
        return NULL;
    }
    double* _to_return = cube::services::create_row_of_doubles( _size );
    char*   start      = row;
    for ( size_t i = 0; i < _size; i++ )
    {
        start           = v->fromStream( start );
        _to_return[ i ] = v->getDouble();
    }
    return _to_return;
}




void
cube::services::print_raw_row( char* _row, size_t _size  )
{
    if ( _row == NULL )
    {
        cout << "0xNULL" << endl;
        return;
    }
    char* pos    = _row;
    char* endpos = _row  + ( _size );
    // sum up the values in the memory
    cout << " =====================charwise=========================== " << endl;
    cout << hex;
    while ( pos < endpos  )
    {
        cout << ( unsigned short )( *( ( uint8_t* )( pos ) ) ) << " ";
        pos++;
    }
    cout << dec;
    cout << endl << " ================================================ " << endl;
}



void
cube::services::print_row_of_doubles( double* _row, size_t _size  )
{
    if ( _row == NULL )
    {
        cout << "0xNULL" << endl;
        return;
    }
    double* pos    = _row;
    double* endpos = _row  + ( _size );
    // sum up the values in the memory
    cout << " =====================doubles=========================== " << endl;
    while ( pos < endpos  )
    {
        cout << ( *pos )  << " ";
        pos++;
    }
    cout << endl << " ================================================ " << endl;
}
