/*
 * This file is part of the Score-P software (http://www.score-p.org)
 *
 * Copyright (c) 2009-2011,
 * RWTH Aachen University, Germany
 *
 * Copyright (c) 2009-2011,
 * Gesellschaft fuer numerische Simulation mbH Braunschweig, Germany
 *
 * Copyright (c) 2009-2011,
 * Technische Universitaet Dresden, Germany
 *
 * Copyright (c) 2009-2011,
 * University of Oregon, Eugene, USA
 *
 * Copyright (c) 2009-2011,
 * Forschungszentrum Juelich GmbH, Germany
 *
 * Copyright (c) 2009-2011,
 * German Research School for Simulation Sciences GmbH, Juelich/Aachen, Germany
 *
 * Copyright (c) 2009-2011, 2015-2016,
 * Technische Universitaet Muenchen, Germany
 *
 * This software may be modified and distributed under the terms of
 * a BSD-style license. See the COPYING file in the package base
 * directory for details.
 *
 */

 /**
 * @file
 *
 * @brief   automatically generated file for the MRI string parser
 *
 */

/* *INDENT-OFF* */
%{

/*----------------------------------------------------------------------------*/
/* Loads system interface modules.                                            */
/*----------------------------------------------------------------------------*/
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------------*/
/* Loads own interface modules.                                               */
/*----------------------------------------------------------------------------*/
#include "scorep_oa_mri_control.h"
#include "SCOREP_OA_Request.h"
#include "scorep_oa_connection.h"
#include "SCOREP_OA_RuntimeManagement.h"
#include <UTILS_Debug.h>
#include <UTILS_CStr.h>
#include <scorep_substrates_definition.h>


/*----------------------------------------------------------------------------*/
/* Defines Macros                                                             */
/*----------------------------------------------------------------------------*/
#define DEFAULT_BUFFER_SIZE 10000


/*----------------------------------------------------------------------------*/
/* Declaration of types                                                       */
/*----------------------------------------------------------------------------*/
struct
{
    int   region_id;
    int   kind;
    char* action_name;
    int   value;
} tuning_action;

//TODO: Find another way of passing tuning action type. -RM
//This is dangerous as it is already defined in the tuning substrate plugin. -RM
enum
{
    SCOREP_TUNING_VARIABLE = 0,
    SCOREP_TUNING_FUNCTION,
    SCOREP_TUNING_UNDEFINED
} SCOREP_TuningActionKind;

/*----------------------------------------------------------------------------*/
/* Variable declarations:                                                     */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Declaration of Procedures/Functions prototypes:                            */
/*----------------------------------------------------------------------------*/
void yyerror( char *s );
int yylex( void );

char* json = NULL;
%}

/******************************************************************************/
/******************************************************************************/
/* Section with Yacc specification follows                                    */
/******************************************************************************/
/******************************************************************************/
%union
{
  int   Number;
  char *String;
}

%token <Number> T_NUMBER
%token <String> T_QSTRING
%token <String> T_STRING

%token T_RUNTOSTART
%token T_RUNTOEND
%token T_TERMINATE

%token T_SETNUMITERATIONS

%token T_BEGINREQUESTS
%token T_MEASUREMENTREQUEST
%token T_TUNINGREQUEST
%token T_RTSTUNINGREQUESTS
%token T_ENDREQUESTS
%token T_GETSUMMARYDATA

%token T_GLOBAL

%token T_MPI
%token T_METRIC
%token T_EXECUTION_TIME

%token T_PAPI
%token T_PERF
%token T_RUSAGE
%token T_PLUGIN
%token T_OTHER

%token T_VARIABLE
%token T_FUNCTION

%token T_INTPARAMS
%token T_UINTPARAMS
%token T_STRINGPARAMS

%start Command

%%

/******************************************************************************/
/* Section with Yacc rules follows                                            */
/******************************************************************************/

Command             : ExecutionControl ';'
                    | Configuration ';'
                    | Requests

ExecutionControl    : T_RUNTOSTART '(' T_NUMBER ',' T_NUMBER ')'          {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Execution request run_to_beginning received" );
                                                                              scorep_oa_mri_set_appl_control( SCOREP_OA_MRI_STATUS_RUNNING_TO_BEGINNING );
                                                                          }
                    | T_RUNTOEND   '(' T_NUMBER ',' T_NUMBER ')'          {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Execution request run_to_end received" );
                                                                              scorep_oa_mri_set_appl_control( SCOREP_OA_MRI_STATUS_RUNNING_TO_END );
                                                                          }
                    | T_TERMINATE                                         {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Termination request received" );
                                                                              scorep_oa_mri_set_appl_control( SCOREP_OA_MRI_EXEC_REQUEST_TERMINATE );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }

Configuration       : T_SETNUMITERATIONS T_NUMBER                         {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Request to set number of iterations" );
                                                                              scorep_oa_mri_set_num_iterations( $2 );
                                                                          }

Requests            : BeginRequests
                    | RequestCommand
                    | EndRequests

BeginRequests       : T_BEGINREQUESTS ';'                                 {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Standby for requests..." );
                                                                              scorep_oa_requests_begin();
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }

RequestCommand      : MeasurementRequests ';'
                    | TuningRequests ';'
                    | RTSTuningRequests ';'
                    | SummaryRequest ';'

EndRequests         : T_ENDREQUESTS ';'                                   {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Requests submitted" );
                                                                              scorep_oa_requests_end();
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }

MeasurementRequests : MeasurementRequests MeasurementRequest
                    | MeasurementRequest

TuningRequests      : TuningRequests TuningRequest
                    | TuningRequest

MeasurementRequest  : T_MEASUREMENTREQUEST '[' NodeList ']' RequestKind   {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Measurements are requested" );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }

RTSTuningRequests   : T_RTSTUNINGREQUESTS '('                             {
                                                                              json = ( char* ) calloc(1, DEFAULT_BUFFER_SIZE );
                                                                              sprintf( json, "{\n"
                                                                                             "    \"genericEventType\":\"tuningRequest\",\n"
                                                                                             "    \"genericEventTypeVersion\":0.2,\n"
                                                                                             "    \"data\":\n"
                                                                                             "    {\n"
                                                                                             "        \"RegionCallpath\":\n"
                                                                                             "        [\n" );
                                                                          }
                      RegionCallPaths ')' '=' '('                         {
                                                                              sprintf( json + strlen( json ) - 2, "\n"
                                                                                                                  "        ],\n"
                                                                                                                  "        \"TuningParameters\":\n" 
                                                                                                                  "        {\n" );
                                                                                                                  
                                                                          }
                      TuningParameters ')'                                {
                      
                                                                              sprintf( json + strlen( json ) - 2, "\n"
                                                                                                                  "        }\n"
                                                                                                                  "    }\n"
                                                                                                                  "}\n" );
                                                                              size_t size  = strlen( json ) + 1;
                                                                              json[ size ] = '\0';
                                                                              //printf( "%s\n", json );
                                                                              SCOREP_CALL_SUBSTRATE( GenericCommand, GENERIC_COMMAND, ( json, size ) );
                                  
                                                                              json = NULL;
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Runtime situation tuning actions are requested" );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }

RegionCallPaths     : RegionCallPaths ',' RegionCallPath
                    | RegionCallPath

RegionCallPath      : '('                                                 {
                                                                              sprintf( json + strlen( json ), "            {\n"
                                                                                                              "        " );
                                                                          }
                      RegionId ',' ParameterIdentifier ')'                {
                                                                              sprintf( json + strlen( json ), "            },\n" );
                                                                          }

ParameterIdentifier :                                                     {
                                                                              sprintf( json + strlen( json ), "                \"AdditionalIdentifiersInt\":\n"
                                                                                                              "                {\n" );
                                                                          }
                       ParamIntSpec ','                                   {
                                                                              sprintf( json + strlen( json ), "                },\n"
                                                                                                              "                \"AdditionalIdentifiersUInt\":\n"
                                                                                                              "                {\n" );
                                                                          }
                       ParamUIntSpec ','                                  {
                                                                              sprintf( json + strlen( json ), "                },\n"
                                                                                                              "                \"AdditionalIdentifiersString\":\n"
                                                                                                              "                {\n" );
                                                                          }
                       ParamStringSpec                                    {
                                                                              sprintf( json + strlen( json ), "                }\n" );
                                                                          }

ParamIntSpec        : T_INTPARAMS '=' '(' ParameterInts ')'               {
                                                                              sprintf( json + strlen( json ) - 2, "\n" );
                                                                          }
                    | T_INTPARAMS '=' '(' ')'

ParamUIntSpec       : T_UINTPARAMS '=' '(' ParameterUInts ')'             {
                                                                              sprintf( json + strlen( json ) - 2, "\n" );
                                                                          }
                    | T_UINTPARAMS '=' '(' ')'

ParamStringSpec     : T_STRINGPARAMS '=' '(' ParameterStrings ')'         {
                                                                              sprintf( json + strlen( json ) - 2, "\n" );
                                                                          }
                    | T_STRINGPARAMS '=' '(' ')'

ParameterInts       : ParameterInts ',' ParameterInt
                    | ParameterInt

ParameterUInts      : ParameterUInts ',' ParameterUInt
                    | ParameterUInt

ParameterStrings    : ParameterStrings ',' ParameterString
                    | ParameterString

ParameterInt        : T_QSTRING '=' T_NUMBER                              {
                                                                              sprintf( json + strlen( json ), "                    \"%s\":%d,\n", $1, $3 );
                                                                          }

ParameterUInt       : T_QSTRING '=' T_NUMBER                              {
                                                                              sprintf( json + strlen( json ), "                    \"%s\":%d,\n", $1, $3 );
                                                                          }

ParameterString     : T_QSTRING '=' T_QSTRING                             {
                                                                              sprintf( json + strlen( json ), "                    \"%s\":%s,\n", $1, $3 );
                                                                          }

TuningParameters    : TuningParameters ',' TuningParameter
                    | TuningParameter

TuningParameter     : T_QSTRING '=' T_NUMBER                              {

                                                                              sprintf( json + strlen( json ), "                    \"%s\":%d,\n", $1, $3 );
                                                                           }

SummaryRequest      : T_GETSUMMARYDATA                                    {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Measured data requested" );
                                                                              scorep_oa_mri_return_summary_data( scorep_oa_connection );
                                                                          }

NodeList            : NodeNrs
                    | '*'                                                 {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Node lists are ignored" );
                                                                          }

NodeNrs             : T_NUMBER                                            {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Node lists are ignored" );
                                                                          }

RequestKind         : T_GLOBAL MeasurementTypeList

MeasurementTypeList : MeasurementType ',' MeasurementTypeList
                    | MeasurementType

TuningRequest       : T_TUNINGREQUEST                                     {
                                                                              json = ( char* ) malloc( DEFAULT_BUFFER_SIZE );
                                                                              sprintf( json, "{\n"
                                                                                             "    \"genericEventType\":\"tuningRequest\",\n"
                                                                                             "    \"genericEventTypeVersion\":0.1,\n"
                                                                                             "    \"data\":\n"
                                                                                             "    {\n"
                                                                                             "        \"RegionName\":\"%s\",\n",
                                                                                             "Name" );
                                                                          }
                      '(' RegionId ')' '=' TuningAction                   {
                                                                              SCOREP_CALL_SUBSTRATE( AddTuningAction, ADD_TUNING_ACTION,
                                                                                                     ( tuning_action.region_id,
                                                                                                     tuning_action.kind,
                                                                                                     tuning_action.action_name,
                                                                                                     tuning_action.value ) );
                                                                              sprintf( json + strlen( json ), "        \"TuningParameters\":\n"
                                                                                                              "        {\n"
                                                                                                              "            \"%s\":%d\n"
                                                                                                              "        }\n"
                                                                                                              "    }\n"
                                                                                                              "}\n",
                                                                                                              tuning_action.action_name,
                                                                                                              tuning_action.value );
                                                                              size_t size  = strlen( json ) + 1;
                                                                              json[ size ] = '\0';
                                                                              //printf( "%s\n", json );
                                                                              SCOREP_CALL_SUBSTRATE( GenericCommand, GENERIC_COMMAND, ( json, size ) );
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning actions are requested" );
                                                                              scorep_oa_connection_send_string( scorep_oa_connection, "OK\n" );
                                                                          }

MeasurementType     : T_MPI                                               {
                                                                              scorep_oa_mri_set_mpiprofiling( 1 );
                                                                              scorep_oa_requests_add_metric_by_name( UTILS_CStr_dup( "late_send" ), NULL, SCOREP_METRIC_SOURCE_MPI_PROFILING );
                                                                              scorep_oa_requests_add_metric_by_name( UTILS_CStr_dup( "late_receive" ), NULL, SCOREP_METRIC_SOURCE_MPI_PROFILING );
                                                                          }
                    | T_METRIC MetricRequest
                    | T_EXECUTION_TIME                                    {
                                                                              scorep_oa_requests_add_metric_by_name( UTILS_CStr_dup( "execution_time" ), NULL, SCOREP_METRIC_TIMER );
                                                                          }

MetricRequest       : T_PAPI T_QSTRING                                    {
                                                                              scorep_oa_requests_add_metric_by_name( $2, NULL, SCOREP_METRIC_SOURCE_PAPI );
                                                                          }
                    | T_PERF T_QSTRING                                    {
                                                                              scorep_oa_requests_add_metric_by_name( $2, NULL, SCOREP_METRIC_SOURCE_PERF );
                                                                          }
                    | T_RUSAGE T_QSTRING                                  {
                                                                              scorep_oa_requests_add_metric_by_name( $2, NULL, SCOREP_METRIC_SOURCE_RUSAGE );
                                                                          }
                    | T_PLUGIN T_QSTRING T_QSTRING                        {
                                                                              scorep_oa_requests_add_metric_by_name( $3, $2, SCOREP_METRIC_SOURCE_PLUGIN );
                                                                          }
                    | T_OTHER T_QSTRING                                   {
                                                                              scorep_oa_requests_add_metric_by_name( $2, NULL, SCOREP_METRIC_OTHER );
                                                                          }

RegionId            : T_NUMBER                                            {
                                                                              sprintf( json + strlen( json ), "        \"RegionID\":%d,\n", $1 );
                                                                              tuning_action.region_id = $1;
                                                                          }

TuningAction        : '(' TuningActionKind ',' T_QSTRING ',' T_NUMBER ')' {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning action: %s = %d", $4, $6 );
                                                                              tuning_action.action_name = $4;
                                                                              tuning_action.value = $6;
                                                                          }

TuningActionKind    : T_VARIABLE                                          {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning action type: VARIABLE" );
                                                                              tuning_action.kind = SCOREP_TUNING_VARIABLE;
                                                                          }
                    | T_FUNCTION                                          {
                                                                              UTILS_DEBUG_PRINTF( SCOREP_DEBUG_OA, "Tuning action type: FUNCTION" );
                                                                              tuning_action.kind = SCOREP_TUNING_FUNCTION;
                                                                          }
%%


/******************************************************************************/
/* Error handling                                                             */
/******************************************************************************/
void yyerror( char* s )
{
    fprintf( stderr, "Unknown online access request: %s\n", s );
    fprintf( stderr, "Aborting...\n" );
    abort();
}
