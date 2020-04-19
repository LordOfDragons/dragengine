/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include "deoglPLogLevel.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPLogLevel
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglPLogLevel::deoglPLogLevel( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
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
}

deoglPLogLevel::~deoglPLogLevel(){
}



// Parameter Value
////////////////////

decString deoglPLogLevel::GetParameterValue(){
	switch( pOgl.GetConfiguration().GetLogLevel() ){
	case deoglConfiguration::ellError:
		return "error";
		
	case deoglConfiguration::ellWarning:
		return "warning";
		
	case deoglConfiguration::ellInfo:
		return "info";
		
	case deoglConfiguration::ellDebug:
		return "debug";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void deoglPLogLevel::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	if( checkValue == "error" ){
		pOgl.GetConfiguration().SetLogLevel( deoglConfiguration::ellError );
		
	}else if( checkValue == "warning" ){
		pOgl.GetConfiguration().SetLogLevel( deoglConfiguration::ellWarning );
		
	}else if( checkValue == "info" ){
		pOgl.GetConfiguration().SetLogLevel( deoglConfiguration::ellInfo );
		
	}else if( checkValue == "debug" ){
		pOgl.GetConfiguration().SetLogLevel( deoglConfiguration::ellDebug );
		
	}else{
		pOgl.GetConfiguration().SetLogLevel( deoglConfiguration::ellError );
	}
}
