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
#include "deoglPHDRRMaximumIntensity.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"



// Class deoglPHDRRMaximumIntensity
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPHDRRMaximumIntensity::deoglPHDRRMaximumIntensity( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "hdrrMaximumIntensity" );
	SetDescription( "Sets the maximum intensity for the HDRR tone mapper. "
		"This parameter defines the maximum intensity of a color before becomg white. "
		"The default value is 2 which is double the intensity of a white pixel with "
		"full ambient and no other lights." );
	SetCategory( ecExpert );
	SetDisplayName( "HDRR Maximum Intensity" );
	SetDefaultValue( "1.5" );
}

deoglPHDRRMaximumIntensity::~deoglPHDRRMaximumIntensity(){
}



// Parameter Value
////////////////////

float deoglPHDRRMaximumIntensity::GetParameterFloat(){
	return pOgl.GetConfiguration().GetHDRRMaximumIntensity();
}

void deoglPHDRRMaximumIntensity::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetHDRRMaximumIntensity( value );
}
