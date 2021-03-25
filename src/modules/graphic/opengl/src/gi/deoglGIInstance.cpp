/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglGIInstance.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIInstance::cComponentListener
//////////////////////////////////////////////

deoglGIInstance::cComponentListener::cComponentListener( deoglGIInstance &instance ) :
pInstance( instance ){
}

void deoglGIInstance::cComponentListener::ComponentDestroyed( deoglRComponent& c){
	printf("ComponentDestroyed %p\n", &c);
	pInstance.SetChanged( true );
	pInstance.Clear();
}

void deoglGIInstance::cComponentListener::BoundariesChanged( deoglRComponent& ){
	pInstance.SetChanged( true );
}

void deoglGIInstance::cComponentListener::OcclusionMeshChanged( deoglRComponent& ){
	pInstance.SetChanged( true );
}



// Class deoglGIInstance
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIInstance::deoglGIInstance() :
pComponent( NULL ),
pOcclusionMesh( NULL ),
pIndexMaterial( 0 ),
pChanged( false ){
}

deoglGIInstance::~deoglGIInstance(){
	Clear();
}



// Management
///////////////

void deoglGIInstance::SetComponent( deoglRComponent *component ){
	if( component == pComponent ){
		return;
	}
	
	Clear();
	
	pComponent = component;
	pChanged = false;
	
	if( component ){
		if( ! pComponentListener ){
			pComponentListener.TakeOver( new cComponentListener( *this ) );
		}
		component->AddListener( ( deoglComponentListener* )( deObject* )pComponentListener );
	}
}

void deoglGIInstance::SetOcclusionMesh( deoglRComponent *occlusionMesh ){
	if( occlusionMesh == pOcclusionMesh ){
		return;
	}
	
	Clear();
	
	pOcclusionMesh = occlusionMesh;
	pChanged = false;
	
	if( occlusionMesh ){
		if( ! pComponentListener ){
			pComponentListener.TakeOver( new cComponentListener( *this ) );
		}
		occlusionMesh->AddListener( ( deoglComponentListener* )( deObject* )pComponentListener );
	}
}

void deoglGIInstance::SetIndexMaterial( int index ){
	pIndexMaterial = index;
}

void deoglGIInstance::SetChanged( bool changed ){
	pChanged = changed;
}

void deoglGIInstance::Clear(){
	if( pComponent ){
		pComponent->RemoveListener( ( deoglComponentListener* )( deObject* )pComponentListener );
		pComponent = NULL;
	}
	if( pOcclusionMesh ){
		pOcclusionMesh->RemoveListener( ( deoglComponentListener* )( deObject* )pComponentListener );
		pOcclusionMesh = NULL;
	}
}



// Private Functions
//////////////////////

