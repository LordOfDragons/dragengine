/* 
 * Drag[en]gine Basic Network Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debnPLogLevel.h"
#include "../../deNetworkBasic.h"
#include "../../configuration/debnConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class debnPLogLevel
////////////////////////

// Constructor, destructor
////////////////////////////

debnPLogLevel::debnPLogLevel( deNetworkBasic &network ) :
debnParameter( network )
{
	SetName( "logLevel" );
	SetDescription( "Log level." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecAdvanced );
	SetDisplayName( "Log Level" );
	
	const deModuleParameter::SelectionEntry entries[ 4 ] = {
		{ "error", "Error", "Log only errors." },
		{ "warning", "Warning", "Log errors and warnings." },
		{ "info", "Info", "Log errors, warnings and information." },
		{ "debug", "Debug", "Log everything" }
	};
	AddSelectionEntries( entries, 4 );
	SetDefaultValue( "warning" );
}

debnPLogLevel::~debnPLogLevel(){
}



// Parameter Value
////////////////////

decString debnPLogLevel::GetParameterValue(){
	switch( pNetwork.GetConfiguration().GetLogLevel() ){
	case debnConfiguration::ellError:
		return "error";
		
	case debnConfiguration::ellWarning:
		return "warning";
		
	case debnConfiguration::ellInfo:
		return "info";
		
	case debnConfiguration::ellDebug:
		return "debug";
		
	default:
		return "warning";
	}
}

void debnPLogLevel::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	if( checkValue == "error" ){
		pNetwork.GetConfiguration().SetLogLevel( debnConfiguration::ellError );
		
	}else if( checkValue == "warning" ){
		pNetwork.GetConfiguration().SetLogLevel( debnConfiguration::ellWarning );
		
	}else if( checkValue == "info" ){
		pNetwork.GetConfiguration().SetLogLevel( debnConfiguration::ellInfo );
		
	}else if( checkValue == "debug" ){
		pNetwork.GetConfiguration().SetLogLevel( debnConfiguration::ellDebug );
		
	}else{
		pNetwork.GetConfiguration().SetLogLevel( debnConfiguration::ellWarning );
	}
}
