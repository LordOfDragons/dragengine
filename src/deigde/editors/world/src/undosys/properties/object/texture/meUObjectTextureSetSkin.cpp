/* 
 * Drag[en]gine IGDE World Editor
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

#include "meUObjectTextureSetSkin.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"
#include "../../../../world/object/texture/meObjectTextureList.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectTextureSetSkin
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectTextureSetSkin::meUObjectTextureSetSkin( meObjectTexture *texture, const char *newskin ){
	if( ! texture || ! newskin ) DETHROW( deeInvalidParam );
	
	meObject *object = texture->GetObject();
	if( ! object ) DETHROW( deeInvalidParam );
	
	meWorld *world = object->GetWorld();
	if( ! world ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Set object texture skin" );
	
	pTextures = NULL;
	pTextureCount = 0;
	
	try{
		pTextures = new sTexture[ 1 ];
		if( ! pTextures ) DETHROW( deeOutOfMemory );
		
		pTextures[ 0 ].oldskin = texture->GetSkinPath();
		pTextures[ 0 ].newskin = newskin;
		pTextures[ 0 ].texture = texture;
		texture->AddReference();
		
		pTextureCount = 1;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUObjectTextureSetSkin::meUObjectTextureSetSkin( meObjectTextureList &textures, const char *newskin ){
	int count = textures.GetTextureCount();
	meObjectTexture *texture;
	
	if( count == 0 || ! newskin ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Set object texture skins" );
	
	pTextures = NULL;
	pTextureCount = 0;
	
	try{
		pTextures = new sTexture[ count ];
		if( ! pTextures ) DETHROW( deeOutOfMemory );
		
		for( pTextureCount=0; pTextureCount<count; pTextureCount++ ){
			texture = textures.GetTextureAt( pTextureCount );
			pTextures[ pTextureCount ].oldskin = texture->GetSkinPath();
			pTextures[ pTextureCount ].newskin = newskin;
			pTextures[ pTextureCount ].texture = texture;
			texture->AddReference();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUObjectTextureSetSkin::~meUObjectTextureSetSkin(){
	pCleanUp();
}



// Management
///////////////

void meUObjectTextureSetSkin::Undo(){
	int t;
	
	for( t=0; t<pTextureCount; t++ ){
		pTextures[ t ].texture->SetSkinPath( pTextures[ t ].oldskin.GetString() );
	}
}

void meUObjectTextureSetSkin::Redo(){
	int t;
	
	for( t=0; t<pTextureCount; t++ ){
		pTextures[ t ].texture->SetSkinPath( pTextures[ t ].newskin.GetString() );
	}
}



// Private Functions
//////////////////////

void meUObjectTextureSetSkin::pCleanUp(){
	if( pTextures ){
		while( pTextureCount > 0 ){
			pTextureCount--;
			pTextures[ pTextureCount ].texture->FreeReference();
		}
		
		delete [] pTextures;
	}
}
