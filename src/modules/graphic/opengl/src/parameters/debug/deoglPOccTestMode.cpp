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

#include "deoglPOccTestMode.h"
#include "../../deGraphicOpenGl.h"
#include "../../configuration/deoglConfiguration.h"

#include <dragengine/common/exceptions.h>



// Class deoglPOccTestMode
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPOccTestMode::deoglPOccTestMode( deGraphicOpenGl &ogl ) : deoglParameter( ogl ){
	SetName( "occlusionTestMode" );
	SetDescription( "Sets the occlusion test mode." );
	SetType( deModuleParameter::eptSelection );
	SetCategory( ecExpert );
	SetDisplayName( "Occlusion Test Mode" );
	
	const deModuleParameter::SelectionEntry entries[ 3 ] = {
		{ "none", "None", "Disable occlusion testing." },
		{ "vboToTexture", "VBO to Texture", "Uses a VBO to texture render technique to test for occlusion." } ,
		{ "transformFeedback", "Transform Feedback", "Uses the transform feedback extension to test for occlusion." }
	};
	AddSelectionEntries( entries, 3 );
	SetDefaultValue( "transformFeedback" );
}

deoglPOccTestMode::~deoglPOccTestMode(){
}



// Parameter Value
////////////////////

decString deoglPOccTestMode::GetParameterValue(){
	switch( pOgl.GetConfiguration().GetOcclusionTestMode() ){
	case deoglConfiguration::eoctmNone:
		return "none";
		
	case deoglConfiguration::eoctmVBOToTexture:
		return "vboToTexture";
		
	case deoglConfiguration::eoctmTransformFeedback:
		return "transformFeedback";
		
	default:
		DETHROW( deeInvalidParam );
	}
}

void deoglPOccTestMode::SetParameterValue( const char *value ){
	const decString checkValue( decString( value ).GetLower() );
	
	if( checkValue == "none" ){
		pOgl.GetConfiguration().SetOcclusionTestMode( deoglConfiguration::eoctmNone );
		
	}else if( checkValue == "vbototexture" ){
		pOgl.GetConfiguration().SetOcclusionTestMode( deoglConfiguration::eoctmVBOToTexture );
		
	}else if( checkValue == "transformfeedback" ){
		pOgl.GetConfiguration().SetOcclusionTestMode( deoglConfiguration::eoctmTransformFeedback );
		
	}else{
		pOgl.GetConfiguration().SetOcclusionTestMode( deoglConfiguration::eoctmTransformFeedback );
	}
}
