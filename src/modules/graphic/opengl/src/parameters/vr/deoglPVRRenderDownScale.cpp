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

#include "deoglPVRRenderDownScale.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPVRRenderDownScale
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPVRRenderDownScale::deoglPVRRenderDownScale( deGraphicOpenGl &ogl ) : deoglParameterInt( ogl ){
	SetName( "vrRenderDownScale" );
	SetDescription( "Render VR at lower resolution the up-scale. Improves performance at the cost of quality." );
	SetType( eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "VR Render Down-Scale" );
	
	const deModuleParameter::SelectionEntry entries[ 4 ] = {
		{ "1", "1x", "Render at full resolution." },
		{ "2", "2x", "Render at half resolution then up-scale to full resolution." },
		{ "4", "4x", "Render at quarter resolution then up-scale to full resolution." },
		{ "8", "8x", "Render at 1/8 resolution then up-scale to full resolution." }
	};
	AddSelectionEntries( entries, 4 );
}

deoglPVRRenderDownScale::~deoglPVRRenderDownScale(){
}



// Parameter Value
////////////////////

int deoglPVRRenderDownScale::GetParameterInt(){
	return pOgl.GetConfiguration().GetVRRenderDownScale();
}

void deoglPVRRenderDownScale::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetVRRenderDownScale( value );
}
