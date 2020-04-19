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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deEffectOverlayImage.h"
#include "deEffectFilterKernel.h"
#include "deEffectColorMatrix.h"
#include "deEffectDistortImage.h"
#include "deEffectManager.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/dePhysicsSystem.h"
#include "../../deEngine.h"
#include "../../common/exceptions.h"


// class deEffectManager
/////////////////////////////

// constructor, destructor
////////////////////////////

deEffectManager::deEffectManager( deEngine *engine ) : deResourceManager( engine, ertEffect ){
	SetLoggingName( "effect" );
}

deEffectManager::~deEffectManager(){
	ReleaseLeakingResources();
}



// management
///////////////

int deEffectManager::GetEffectCount() const{
	return pEffects.GetCount();
}

deEffect *deEffectManager::GetRootEffect() const{
	return ( deEffect* )pEffects.GetRoot();
}

deEffectFilterKernel *deEffectManager::CreateEffectFilterKernel(){
	deEffectFilterKernel *effect = NULL;
	// create and add effect
	try{
		effect = new deEffectFilterKernel( this );
		if( ! effect ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadEffect( effect );
		//GetPhysicsSystem()->LoadEffect( effect );
		pEffects.Add( effect );
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	// finished
	return effect;
}

deEffectOverlayImage *deEffectManager::CreateEffectOverlayImage(){
	deEffectOverlayImage *effect = NULL;
	// create and add effect
	try{
		effect = new deEffectOverlayImage( this );
		if( ! effect ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadEffect( effect );
		pEffects.Add( effect );
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	// finished
	return effect;
}

deEffectColorMatrix *deEffectManager::CreateEffectColorMatrix(){
	deEffectColorMatrix *effect = NULL;
	// create and add effect
	try{
		effect = new deEffectColorMatrix( this );
		if( ! effect ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadEffect( effect );
		pEffects.Add( effect );
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	// finished
	return effect;
}

deEffectDistortImage *deEffectManager::CreateEffectDistortImage(){
	deEffectDistortImage *effect = NULL;
	
	// create and add effect
	try{
		effect = new deEffectDistortImage( this );
		if( ! effect ) DETHROW( deeOutOfMemory );
		GetGraphicSystem()->LoadEffect( effect );
		pEffects.Add( effect );
		
	}catch( const deException & ){
		if( effect ){
			effect->FreeReference();
		}
		throw;
	}
	
	// finished
	return effect;
}



void deEffectManager::ReleaseLeakingResources(){
	if( GetEffectCount() > 0 ){
		LogWarnFormat( "%i leaking effects", GetEffectCount() );
		pEffects.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deEffectManager::SystemGraphicLoad(){
	deEffect *effect = ( deEffect* )pEffects.GetRoot();
	
	while( effect ){
		if( ! effect->GetPeerGraphic() ){
			GetGraphicSystem()->LoadEffect( effect );
		}
		
		effect = ( deEffect* )effect->GetLLManagerNext();
	}
}

void deEffectManager::SystemGraphicUnload(){
	deEffect *effect = ( deEffect* )pEffects.GetRoot();
	
	while( effect ){
		effect->SetPeerGraphic( NULL );
		effect = ( deEffect* )effect->GetLLManagerNext();
	}
}
/*
void deEffectManager::SystemPhysicsLoad(){
	deEffect *effect;
	int i;
	for( i=0; i<pEffects.GetResourceCount(); i++ ){
		effect = ( deEffect* )pEffects.GetResourceAt( i );
		if( effect->GetPhysicsEffect() ) continue;
		GetPhysicsSystem()->LoadEffect( effect );
	}
}

void deEffectManager::SystemPhysicsUnload(){
	deEffect *effect;
	int i;
	for( i=0; i<pEffects.GetResourceCount(); i++ ){
		effect = ( deEffect* )pEffects.GetResourceAt( i );
		effect->SetPhysicsEffect( NULL );
	}
}
*/
void deEffectManager::RemoveResource( deResource *resource ){
	pEffects.RemoveIfPresent( resource );
}
