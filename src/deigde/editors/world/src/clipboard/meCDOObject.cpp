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

#include "meCDOObject.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meCDOObject
///////////////////////////

// Constructor, destructor
////////////////////////////

meCDOObject::meCDOObject( const meObject &object ) :
pClassname( object.GetClassName() ),
pPosition( object.GetPosition() ),
pSize( object.GetSize() ),
pRotation( object.GetRotation() ),
pProperties( object.GetProperties() ),
pAttachToIndex( -1 )
{
	const int count = object.GetTextureCount();
	meObjectTexture *texture = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			texture = new meObjectTexture( *object.GetTextureAt( i ) );
			pTextures.AddTexture( texture );
			texture->FreeReference();
			texture = NULL;
		}
		
	}catch( const deException & ){
		if( texture ){
			texture->FreeReference();
			throw;
		}
	}
}

meCDOObject::~meCDOObject(){
}



// Management
///////////////

void meCDOObject::SetAttachToIndex( int index ){
	pAttachToIndex = index;
}

void meCDOObject::SetAttachToID( const char *id ){
	pAttachToID = id;
}

void meCDOObject::UpdateObject( meObject &object ) const{
	object.SetClassName( pClassname );
	object.SetPosition( pPosition );
	object.SetSize( pSize );
	object.SetRotation( pRotation );
	object.SetProperties( pProperties );
	
	const int count = pTextures.GetTextureCount();
	meObjectTexture *texture = NULL;
	int i;
	
	try{
		for( i=0; i<count; i++ ){
			texture = new meObjectTexture( *pTextures.GetTextureAt( i ) );
			object.AddTexture( texture );
			texture->FreeReference();
			texture = NULL;
		}
		
	}catch( const deException & ){
		if( texture ){
			texture->FreeReference();
			throw;
		}
	}
	
	object.SetAttachedToID( pAttachToID ); // attach to non-pasted object
}
