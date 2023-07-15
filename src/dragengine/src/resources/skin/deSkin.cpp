/* 
 * Drag[en]gine Game Engine
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

#include <string.h>

#include "deSkin.h"
#include "deSkinTexture.h"
#include "deSkinManager.h"
#include "deSkinMapped.h"
#include "../../common/exceptions.h"
#include "../../systems/modules/graphic/deBaseGraphicSkin.h"
#include "../../systems/modules/audio/deBaseAudioSkin.h"
#include "../../systems/modules/physics/deBasePhysicsSkin.h"



// Class deSkin
/////////////////

// Constructor, destructor
////////////////////////////

deSkin::deSkin( deSkinManager *manager, deVirtualFileSystem *vfs, const char *filename,
	TIME_SYSTEM modificationTime ) :
deFileResource( manager, vfs, filename, modificationTime ),

pTextures( NULL ),
pTextureCount( 0 ),
pTextureSize( 0 ),

pPeerGraphic( NULL ),
pPeerAudio( NULL ),
pPeerPhysics( NULL ){
}

deSkin::~deSkin(){
	pCleanUp();
}



// Textures
/////////////

void deSkin::AddTexture( deSkinTexture *tex ){
	DEASSERT_NOTNULL( tex )
	
	if( pTextureCount == pTextureSize ){
		int newSize = pTextureSize * 3 / 2 + 1;
		deSkinTexture **newArray = new deSkinTexture*[ newSize ];
		if( pTextures ){
			memcpy( newArray, pTextures, sizeof( deSkinTexture* ) * pTextureSize );
			delete [] pTextures;
		}
		pTextures = newArray;
		pTextureSize = newSize;
	}
	
	pTextures[ pTextureCount ] = tex;
	pTextureCount++;
}

deSkinTexture *deSkin::GetTextureAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pTextureCount )
	
	return pTextures[ index ];
}

int deSkin::IndexOfTextureNamed( const char *name ) const{
	DEASSERT_NOTNULL( name )
	int i;
	
	for( i=0; i<pTextureCount; i++ ){
		if( strcmp( pTextures[ i ]->GetName(), name ) == 0 ) return i;
	}
	
	return -1;
}



// Mapped Values
//////////////////

int deSkin::GetMappedCount() const{
	return pMapped.GetCount();
}

deSkinMapped *deSkin::GetMappedAt( int index ) const{
	return ( deSkinMapped * )pMapped.GetAt( index );
}

deSkinMapped *deSkin::GetMappedNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deSkinMapped * const mapped = ( deSkinMapped * )pMapped.GetAt( i );
		if( mapped->GetName() == name ){
			return mapped;
		}
	}
	
	return nullptr;
}

int deSkin::IndexOfMapped( deSkinMapped *mapped ) const{
	return pMapped.IndexOf( mapped );
}

int deSkin::IndexOfMappedNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deSkinMapped * )pMapped.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool deSkin::HasMapped( deSkinMapped *mapped ) const{
	return pMapped.Has( mapped );
}

bool deSkin::HasMappedNamed( const char *name ) const{
	const int count = pMapped.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( deSkinMapped * )pMapped.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

void deSkin::AddMapped( deSkinMapped *mapped ){
	DEASSERT_NOTNULL( mapped )
	DEASSERT_FALSE( HasMapped( mapped ) )
	
	pMapped.Add( mapped );
}



// System Peers
/////////////////

void deSkin::SetPeerGraphic( deBaseGraphicSkin *peer ){
	if( peer != pPeerGraphic ){
		if( pPeerGraphic ) delete pPeerGraphic;
		pPeerGraphic = peer;
	}
}

void deSkin::SetPeerAudio( deBaseAudioSkin *peer ){
	if( peer != pPeerAudio ){
		if( pPeerAudio ) delete pPeerAudio;
		pPeerAudio = peer;
	}
}

void deSkin::SetPeerPhysics( deBasePhysicsSkin *peer ){
	if( peer != pPeerPhysics ){
		if( pPeerPhysics ) delete pPeerPhysics;
		pPeerPhysics = peer;
	}
}



// Private functions
//////////////////////

void deSkin::pCleanUp(){
	if( pPeerPhysics ){
		delete pPeerPhysics;
	}
	if( pPeerAudio ){
		delete pPeerAudio;
	}
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	
	if( pTextures ){
		while( pTextureCount > 0 ){
			pTextureCount--;
			delete pTextures[ pTextureCount ];
		}
		
		delete [] pTextures;
	}
}
