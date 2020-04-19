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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "deoglCombinedTexture.h"
#include "deoglCombinedTextureList.h"

#include <dragengine/common/exceptions.h>



// Class deoglCombinedTextureList
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCombinedTextureList::deoglCombinedTextureList( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pRoot( NULL ),
pTail( NULL ),
pCount( 0 ){
}

deoglCombinedTextureList::~deoglCombinedTextureList(){
	while( pRoot ){
		pTail = pRoot;
		pRoot = pRoot->GetLLNext();
		delete pTail;
	}
	pTail = NULL;
}



// Management
///////////////

deoglCombinedTexture *deoglCombinedTextureList::GetWith( const decColor &color, deoglRImage *images[ 4 ] ){
	deoglCombinedTexture *combinedTexture = pRoot;
	
	// if there exists already a combined texture with these properties add a usage and return it
	while( combinedTexture ){
		if( combinedTexture->Equals( color, images ) ){
			combinedTexture->AddUsage();
			return combinedTexture;
		}
		
		combinedTexture = combinedTexture->GetLLNext();
	}
	
	// otherwise create a new combined texture with these parameters and return it
	combinedTexture = new deoglCombinedTexture( pRenderThread, color, images );
	
	try{
		combinedTexture->CalcHashCode();
		
		if( pTail ){
			pTail->SetLLNext( combinedTexture );
			combinedTexture->SetLLPrev( pTail );
			pTail = combinedTexture;
			
		}else{
			pRoot = combinedTexture;
			pTail = combinedTexture;
		}
		pCount++;
		
	}catch( const deException & ){
		delete combinedTexture;
		throw;
	}
	
	return combinedTexture;
}



void deoglCombinedTextureList::Remove( deoglCombinedTexture *config ){
	// WARNING remove usage typically happens during main thread. if the combined texture
	//         would be removed from it can be potentially deleted. combined texture
	//         though holds references to render images (which can cause deletion loop)
	//         as well as opengl objects (which would incorrectly try to delete them in
	//         the opengl context free main thread). if removing is desired the combined
	//         texture list has to take care of delaying deletion of the opengl objects
	//         until a later time. this makes this call main thread safe
	
	if( ! config ){
		DETHROW( deeInvalidParam );
	}
	
	if( config->GetLLPrev() ){
		config->GetLLPrev()->SetLLNext( config->GetLLNext() );
	}
	if( config->GetLLNext() ){
		config->GetLLNext()->SetLLPrev( config->GetLLPrev() );
	}
	if( pRoot == config ){
		pRoot = config->GetLLNext();
	}
	if( pTail == config ){
		pTail = config->GetLLPrev();
	}
	pCount--;
	
	delete config;
}
