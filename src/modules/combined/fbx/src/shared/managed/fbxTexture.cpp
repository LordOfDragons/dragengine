/* 
 * FBX Model Module
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
#include <inttypes.h>

#include "fbxTexture.h"
#include "../fbxConnection.h"
#include "../fbxNode.h"
#include "../fbxScene.h"
#include "../fbxProperty.h"
#include "../property/fbxPropertyLong.h"
#include "../property/fbxPropertyString.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/modules/deBaseModule.h>


// Class fbxTexture
//////////////////////

// Constructor, destructor
////////////////////////////

fbxTexture::fbxTexture( fbxScene &scene, fbxNode &nodeTexture ) :
pScene( scene ),
pNodeTexture( nodeTexture ),
pTextureID( nodeTexture.GetPropertyAt( 0 )->CastLong().GetValue() ),
pName( nodeTexture.GetPropertyAt( 1 )->CastString().GetValue() ),
pUVSet( nodeTexture.GetPropertyString( "UVSet", "" ) ),
pUseMipMap( nodeTexture.GetPropertyBool( "UseMipMap", true ) )
{
	fbxNode *node;
	
	node = nodeTexture.FirstNodeNamedOrNull( "FileName" );
	if( node ){
		pFilename = node->GetPropertyAt( 0 )->CastString().GetValue();
	}
	
	node = nodeTexture.FirstNodeNamedOrNull( "RelativeFilename" );
	if( node ){
		pRelativeFilename = node->GetPropertyAt( 0 )->CastString().GetValue();
	}
	
// 	node = nodeTexture.FirstNodeNamedOrNull( "Media" );
// 	if( node ){
// 		pRelativeFilename = node->GetPropertyAt( 0 )->CastString().GetValue();
// 	}
}

fbxTexture::~fbxTexture(){
}



// Management
///////////////

void fbxTexture::SetName( const char *name ){
	pName = name;
}



void fbxTexture::DebugPrintStructure( deBaseModule &module, const decString &prefix, bool verbose ) const{
	module.LogInfoFormat( "%sTexture '%s':", prefix.GetString(), pName.GetString() );
}
