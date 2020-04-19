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

#include "deoglPLODMaxErrorPerLevel.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPLODMaxErrorPerLevel
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPLODMaxErrorPerLevel::deoglPLODMaxErrorPerLevel( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "lodMaxErrorPerLevel" );
	SetDescription( "Maximum allowed error in meters per LOD level in LOD calculation."
		" Higher values chooses LOD geometry quicker improving performance at the cost"
		" of higher chance of popping." );
	SetType( deModuleParameter::eptRanged );
	SetMinimumValue( 0.05f );
	SetMaximumValue( 1.0f );
	SetValueStepSize( 0.05f );
	SetCategory( ecAdvanced );
	SetDisplayName( "LOD Max Error/Level" );
}

deoglPLODMaxErrorPerLevel::~deoglPLODMaxErrorPerLevel(){
}



// Parameter Value
////////////////////

float deoglPLODMaxErrorPerLevel::GetParameterFloat(){
	return pOgl.GetConfiguration().GetLODMaxErrorPerLevel();
}

void deoglPLODMaxErrorPerLevel::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetLODMaxErrorPerLevel( value );
}
