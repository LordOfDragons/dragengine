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

#include "deoglPTranspLayerLimit.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPTranspLayerLimit
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPTranspLayerLimit::deoglPTranspLayerLimit( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "transpLayerLimit" );
	SetDescription( "Limits transparency rendering to N layers. Lower values increase performance"
		" but can introduce visual errors. Higher values are more accurate but more expensive."
		" Values ranges from 1 to 15 with good values between 4 to 8. Default is 4." );
	SetType( deModuleParameter::eptRanged );
	SetMinimumValue( 1.0f );
	SetMaximumValue( 15.0f );
	SetValueStepSize( 1.0f );
	SetCategory( ecBasic );
	SetDisplayName( "Transparency Layer Limit" );
	SetDefaultValue( "4" );
}

deoglPTranspLayerLimit::~deoglPTranspLayerLimit(){
}



// Management
///////////////

int deoglPTranspLayerLimit::GetParameterInt(){
	return pOgl.GetConfiguration().GetTranspLayerLimit();
}

void deoglPTranspLayerLimit::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetTranspLayerLimit( value );
}
