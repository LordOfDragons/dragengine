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

#include "deoglPSSRReduction.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPSSRReduction
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPSSRReduction::deoglPSSRReduction( deGraphicOpenGl &ogl ) :
deoglParameterInt( ogl )
{
	SetName( "ssrReduction" );
	SetDescription( "Sets the screen space reflection (SSR) reduction."
		" SSR is costly to calculated especially when done at full resolution."
		" Rendering SSR at lower resolution then up-scaling improves render speed"
		" especially on fillrate limited GPUs or high screen solutions the result." );
	SetType( deModuleParameter::eptSelection );
	SetDisplayName( "SSR Reduction" );
	
	const deModuleParameter::SelectionEntry entries[ 4 ] = {
		{ "1", "1x", "Render SSR at full resolution. For strong graphic cards." },
		{ "2", "2x", "Render SSR at half resolution then up-scale to full resolution." },
		{ "4", "4x", "Render SSR at quarter resolution then up-scale to full resolution." },
		{ "8", "8x", "Render SSR at 1/8 resolution then up-scale to full resolution." }
	};
	AddSelectionEntries( entries, 4 );
	
	SetDefaultValue( "1" );
}

deoglPSSRReduction::~deoglPSSRReduction(){
}



// Parameter Value
////////////////////

int deoglPSSRReduction::GetParameterInt(){
	return pOgl.GetConfiguration().GetSSRReduction();
}

void deoglPSSRReduction::SetParameterInt( int value ){
	pOgl.GetConfiguration().SetSSRReduction( value );
}
