/* 
 * Drag[en]gine IGDE Skin Editor
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
#include <stdlib.h>
#include <string.h>

#include "seDynamicSkin.h"
#include "seDynamicSkinRenderable.h"
#include "../seSkin.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkin.h>
#include <dragengine/resources/skin/dynamic/deDynamicSkinManager.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Skin Editor"



// Class seDynamicSkin
////////////////////////

// Constructor, destructor
////////////////////////////

seDynamicSkin::seDynamicSkin( seSkin *parentSkin ){
	if( ! parentSkin ){
		DETHROW( deeInvalidParam );
	}
	
	pParentSkin = parentSkin;
	
	pEngDynamicSkin = NULL;
	
	pActiveRenderable = NULL;
	
	try{
		pEngDynamicSkin = parentSkin->GetEngine()->GetDynamicSkinManager()->CreateDynamicSkin();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

seDynamicSkin::~seDynamicSkin(){
	pCleanUp();
}



// Management
///////////////

void seDynamicSkin::Dispose(){
	RemoveAllRenderables();
}

void seDynamicSkin::Reset(){
}

void seDynamicSkin::Update( float elapsed ){
	const int count = pRenderableList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pRenderableList.GetAt( i )->Update( elapsed );
	}
}



// Renderables
////////////////

void seDynamicSkin::AddRenderable( seDynamicSkinRenderable *renderable ){
	if( ! renderable || pRenderableList.HasNamed( renderable->GetName().GetString() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pRenderableList.Add( renderable );
	renderable->SetDynamicSkin( this );
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
	
	if( ! pActiveRenderable ){
		SetActiveRenderable( renderable );
	}
}

void seDynamicSkin::RemoveRenderable( seDynamicSkinRenderable *renderable ){
	if( ! pRenderableList.Has( renderable ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( renderable->GetActive() ){
		if( pRenderableList.GetCount() > 1 ){
			seDynamicSkinRenderable *activeRenderable = pRenderableList.GetAt( 0 );
			
			if( activeRenderable == renderable ){
				activeRenderable = pRenderableList.GetAt( 1 );
			}
			
			SetActiveRenderable( activeRenderable );
			
		}else{
			SetActiveRenderable( NULL );
		}
	}
	
	renderable->SetDynamicSkin( NULL );
	pRenderableList.Remove( renderable );
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
}

void seDynamicSkin::RemoveAllRenderables(){
	const int count = pRenderableList.GetCount();
	int t;
	
	SetActiveRenderable( NULL );
	
	for( t=0; t<count; t++ ){
		pRenderableList.GetAt( t )->SetDynamicSkin( NULL );
	}
	pRenderableList.RemoveAll();
	
	pParentSkin->NotifyDynamicSkinRenderableStructureChanged();
}

bool seDynamicSkin::HasActiveRenderable() const{
	return pActiveRenderable != NULL;
}

void seDynamicSkin::SetActiveRenderable( seDynamicSkinRenderable *renderable ){
	if( renderable != pActiveRenderable ){
		if( pActiveRenderable ){
			pActiveRenderable->SetActive( false );
			pActiveRenderable->FreeReference();
		}
		
		pActiveRenderable = renderable;
		
		if( renderable ){
			renderable->AddReference();
			renderable->SetActive( true );
		}
		
		pParentSkin->NotifyDynamicSkinActiveRenderableChanged();
	}
}



// Private Functions
//////////////////////

void seDynamicSkin::pCleanUp(){
	SetActiveRenderable( NULL );
	RemoveAllRenderables();
	
	if( pEngDynamicSkin ){
		pEngDynamicSkin->FreeReference();
	}
}
