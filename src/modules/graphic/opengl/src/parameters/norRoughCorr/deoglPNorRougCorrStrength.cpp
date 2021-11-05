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

#include "deoglPNorRougCorrStrength.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPNorRougCorrStrength
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPNorRougCorrStrength::deoglPNorRougCorrStrength( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "normalRoughnessCorrectionStrength" );
	SetDescription( "Sets the roughness correction based on normal variance."
		" Mip mapping averages normals at the distance turning smooth but bumpy surfaces"
		" into near-like mirrors."
		" Furthermore this results in shimmering artifacts while moving the camera."
		" The variance lost in the mip map process has be applied to the roughness to"
		" compensate for the lost bumpiness."
		" This works since roughness is a micro-scale bumpiness of a surface while normals"
		" are macro-scale bumpiness."
		" Values are in the range from 0 to 1 where 0 indicating the percentage of lost"
		" normal variance applied to the roughness."
		" Too low values cause shimmering on bumpy but otherwise smooth surfaces."
		" Too high values cause surfaces to turn dull too strong."
		" The default value is 0.75."
		" This applies roughly 75% of normal variance to the roughness."
		" This accounts for interreflections happening reducing the correction at the distance a bit."
		" Values between 50-100% are reasonable." );
	SetCategory( ecExpert );
	SetDisplayName( "Normal Roughness Correction" );
	SetDefaultValue( "0.75" );
}

deoglPNorRougCorrStrength::~deoglPNorRougCorrStrength(){
}



// Parameter Value
////////////////////

float deoglPNorRougCorrStrength::GetParameterFloat(){
	return pOgl.GetConfiguration().GetNormalRoughnessCorrectionStrength();
}

void deoglPNorRougCorrStrength::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetNormalRoughnessCorrectionStrength( value );
}
