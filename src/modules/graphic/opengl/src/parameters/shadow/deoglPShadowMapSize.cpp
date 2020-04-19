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
#include "deoglPShadowMapSize.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"
#include "dragengine/common/exceptions.h"



// class deoglPShadowMapSize
///////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPShadowMapSize::deoglPShadowMapSize( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "shadowMapSize" );
	SetDescription( "Size of the shadow maps. Influences quality versus performance." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecBasic );
	SetDisplayName( "Shadow Map Size" );
	
	const deModuleParameter::SelectionEntry entries[ 5 ] = {
		{ "veryLow", "Very Low", "Very low shadow map size" },
		{ "low", "Low", "Low shadow map size" },
		{ "medium", "Medium", "Medium shadow map size" },
		{ "high", "High", "High shadow map size" },
		{ "veryHigh", "Very High", "Very high shadpw map size" }
	};
	AddSelectionEntries( entries, 5 );
}

deoglPShadowMapSize::~deoglPShadowMapSize(){
}



// Parameter Value
////////////////////

decString deoglPShadowMapSize::GetParameterValue(){
	const int size = pOgl.GetConfiguration().GetShadowMapSize();
	
	if( size <= 256 ){
		return "veryLow";
		
	}else if( size <= 512 ){
		return "low";
		
	}else if( size <= 1024 ){
		return "medium";
		
	}else if( size <= 2048 ){
		return "high";
		
	}else{
		return "veryHigh";
	}
}

void deoglPShadowMapSize::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "verylow" ){
		pOgl.GetConfiguration().SetShadowMapSize( 256 );
		
	}else if( checkValue == "low" ){
		pOgl.GetConfiguration().SetShadowMapSize( 512 );
		
	}else if( checkValue == "medium" ){
		pOgl.GetConfiguration().SetShadowMapSize( 1024 );
		
	}else if( checkValue == "high" ){
		pOgl.GetConfiguration().SetShadowMapSize( 2048 );
		
	}else if( checkValue == "veryhigh" ){
		pOgl.GetConfiguration().SetShadowMapSize( 4096 );
		
	}else{
		pOgl.GetConfiguration().SetShadowMapSize( 2048 );
	}
}
