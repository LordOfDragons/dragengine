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

#include "meUObjectAddUsedTextures.h"
#include "../../../../utils/meHelpers.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decStringList.h>
#include <deigde/gamedefinition/class/igdeGDClass.h>
#include <deigde/gamedefinition/class/igdeGDClassInherit.h>
#include <deigde/gamedefinition/class/component/igdeGDCComponent.h>
#include <deigde/gamedefinition/class/component/igdeGDCCTexture.h>



// Class meUObjectAddUsedTextures
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectAddUsedTextures::meUObjectAddUsedTextures( meObject *object ){
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = object->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	pObject = NULL;
	
	SetShortInfo( "Add Used Object Textures" );
	
	decStringList modelTextureNames;
	object->GetModelTextureNameList( modelTextureNames );
	const int count = modelTextureNames.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ! object->HasTextureNamed( modelTextureNames.GetAt( i ).GetString() ) ){
			pTextureNameList.Add( modelTextureNames.GetAt( i ) );
		}
	}
	
	pObject = object;
	object->AddReference();
}

meUObjectAddUsedTextures::~meUObjectAddUsedTextures(){
	pTextureList.RemoveAllTextures();
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectAddUsedTextures::Undo(){
	const int count = pTextureList.GetTextureCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pObject->RemoveTexture( pTextureList.GetTextureAt( i ) );
	}
}

void meUObjectAddUsedTextures::Redo(){
	deObjectReference texture;
	int i, count;
	
	if( pTextureList.GetTextureCount() == 0 ){
		const igdeGDCComponent * const gdcomponent = meHelpers::FindFirstComponent( pObject->GetGDClass() );
		
		const igdeGDCCTextureList *gdctexlist = NULL;
		const igdeGDCCTextureList *gdccomptex = NULL;
		
		if( gdcomponent ){
			gdctexlist = &gdcomponent->GetTextureList();
		}
		if( pObject->GetGDClass() ){
			gdccomptex = &pObject->GetGDClass()->GetComponentTextures();
		}
		
		count = pTextureNameList.GetCount();
		for( i=0; i<count; i++ ){
			const decString &textureName = pTextureNameList.GetAt( i );
			
			igdeGDCCTexture *gdctex = NULL;
			if( gdctexlist ){
				gdctex = gdctexlist->GetNamed( textureName );
			}
			if( ! gdctex && gdccomptex ){
				gdctex = gdccomptex->GetNamed( textureName );
			}
			
			meHelpers::CreateTexture( texture, pObject, textureName, gdctex );
			pTextureList.AddTexture( ( meObjectTexture* )( deObject* )texture );
		}
	}
	
	count = pTextureList.GetTextureCount();
	for( i=0; i<count; i++ ){
		pObject->AddTexture( pTextureList.GetTextureAt( i ) );
	}
}
