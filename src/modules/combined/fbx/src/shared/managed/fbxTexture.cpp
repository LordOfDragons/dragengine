/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
