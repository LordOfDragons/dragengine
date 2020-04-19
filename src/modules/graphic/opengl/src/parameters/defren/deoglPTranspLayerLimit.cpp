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
	SetDescription( "Limits transparency rendering to N layers. Caps the maximum time required"
		" to calculate transparency with small visual impact. Smaller values increase performance"
		" but introduce visual errors if higher levels of transparency are not rendered. Higher"
		" values remove these visual errors at the cost of lower performance. The value range is"
		" between and including 1 to 15. A good value is between 4 to 8 layers. The default value"
		" is 4." );
	SetType( deModuleParameter::eptRanged );
	SetMinimumValue( 1.0f );
	SetMaximumValue( 15.0f );
	SetValueStepSize( 1.0f );
	SetCategory( ecBasic );
	SetDisplayName( "Transparency Layer Limit" );
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
