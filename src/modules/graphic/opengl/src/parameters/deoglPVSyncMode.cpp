/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglPVSyncMode.h"
#include "../deGraphicOpenGl.h"
#include "../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>


// class deoglPVSyncMode
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglPVSyncMode::deoglPVSyncMode( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "vsyncMode" );
	SetDescription( "Sets V-Sync mode. This can be overriden by GPU drivers." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecAdvanced );
	SetDisplayName( "V-Sync Mode" );
	
	const deModuleParameter::SelectionEntry entries[ 3 ] = {
		{ "adaptive", "Adaptive",
			"Enables V-Sync if frame rate is higher than monitor frame rate. "
			"Disables V-Sync if frame rate is lower than monitor frame rate. "
			"This can prevent stuttering due to frame rate dips. "
			"If system does not support V-Sync control behaves the same as 'On'." },
		
		{ "on", "On", "Enables V-Sync." },
		
		{ "off", "Off",
			"Disables V-Sync. "
			"If system does not support V-Sync control behaves the same as 'On'." }
	};
	AddSelectionEntries( entries, 3 );
	SetDefaultValue( "adaptive" );
}

deoglPVSyncMode::~deoglPVSyncMode(){
}



// Parameter Value
////////////////////

decString deoglPVSyncMode::GetParameterValue(){
	switch( pOgl.GetConfiguration().GetVSyncMode() ){
	case deoglConfiguration::evsmAdaptive:
		return "adaptive";
		
	case deoglConfiguration::evsmOn:
		return "on";
		
	case deoglConfiguration::evsmOff:
		return "off";
		
	default:
		return "adaptive";
	}
}

void deoglPVSyncMode::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "adaptive" ){
		pOgl.GetConfiguration().SetVSyncMode( deoglConfiguration::evsmAdaptive );
		
	}else if( checkValue == "on" ){
		pOgl.GetConfiguration().SetVSyncMode( deoglConfiguration::evsmOn );
		
	}else if( checkValue == "off" ){
		pOgl.GetConfiguration().SetVSyncMode( deoglConfiguration::evsmOff );
		
	}else{
		pOgl.GetConfiguration().SetVSyncMode( deoglConfiguration::evsmAdaptive );
	}
}
