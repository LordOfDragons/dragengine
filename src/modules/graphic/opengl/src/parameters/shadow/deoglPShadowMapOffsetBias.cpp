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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deoglPShadowMapOffsetBias.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "dragengine/common/exceptions.h"



// class deoglPShadowMapOffsetBias
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPShadowMapOffsetBias::deoglPShadowMapOffsetBias( deGraphicOpenGl &ogl ) :
deoglParameterFloat( ogl )
{
	SetName( "shadowMapOffsetBias" );
	SetDescription( "Sets the bias factor of the shadow map offset." );
	SetCategory( ecExpert );
	SetDisplayName( "Shadow Map Offset Bias" );
}

deoglPShadowMapOffsetBias::~deoglPShadowMapOffsetBias(){
}



// Parameter Value
////////////////////

float deoglPShadowMapOffsetBias::GetParameterFloat(){
	return pOgl.GetConfiguration().GetShadowMapOffsetBias();
}

void deoglPShadowMapOffsetBias::SetParameterFloat( float value ){
	pOgl.GetConfiguration().SetShadowMapOffsetBias( value );
}
