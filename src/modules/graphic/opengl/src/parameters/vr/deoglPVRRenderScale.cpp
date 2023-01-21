/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglPVRRenderScale.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPVRRenderScale
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPVRRenderScale::deoglPVRRenderScale( deGraphicOpenGl &ogl ) :
deoglParameter( ogl )
{
	SetName( "vrRenderScale" );
	SetDescription( "Percentage scale of VR Rendering relative to size requested by VR System."
		" Down scaling improves performance at the cost of quality."
		" You can adjust scaling in VR Systems (like SteamVR)."
		" This parameter allows to dynamically adjust render scale.");
	SetType( eptRanged );
	SetCategory( ecBasic );
	SetDisplayName( "VR Render Scale" );
	SetMinimumValue( 50.0f ); // 50%
	SetMaximumValue( 100.0f ); // 100% (beyond not possible with VR systems)
	SetValueStepSize( 5.0f ); // 5%
	SetDefaultValue( "100" );
}

deoglPVRRenderScale::~deoglPVRRenderScale(){
}



// Parameter Value
////////////////////

decString deoglPVRRenderScale::GetParameterValue(){
	decString value;
	value.Format( "%.0f", pOgl.GetConfiguration().GetVRRenderScale() * 100.0f + 0.01f );
	return value;
}

void deoglPVRRenderScale::SetParameterValue( const char *value ){
	pOgl.GetConfiguration().SetVRRenderScale( 0.01f * ( float )( decString( value ).ToInt() ) );
}
