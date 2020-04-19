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
#include "deAnimator.h"
#include "deAnimatorManager.h"
#include "../../systems/deModuleSystem.h"
#include "../../systems/modules/deLoadableModule.h"
#include "../../systems/modules/animator/deBaseAnimatorModule.h"
#include "../../systems/deAnimatorSystem.h"
#include "../../deEngine.h"
#include "../../common/file/decPath.h"
#include "../../common/file/decBaseFileReader.h"
#include "../../common/file/decBaseFileWriter.h"
#include "../../common/exceptions.h"


// class deAnimatorManager
////////////////////////////

// Constructor, destructor
////////////////////////////

deAnimatorManager::deAnimatorManager( deEngine *engine ) : deResourceManager( engine,ertAnimator ){
	SetLoggingName( "animator" );
}

deAnimatorManager::~deAnimatorManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deAnimatorManager::GetAnimatorCount() const{
	return pAnimators.GetCount();
}

deAnimator *deAnimatorManager::GetRootAnimator() const{
	return ( deAnimator* )pAnimators.GetRoot();
}

deAnimator *deAnimatorManager::CreateAnimator(){
	deAnimator *animator = NULL;
	// load skin
	try{
		// create and add animator
		animator = new deAnimator( this );
		if( ! animator ) DETHROW( deeOutOfMemory );
		GetAnimatorSystem()->LoadAnimator( animator );
		// add animator
		pAnimators.Add( animator );
	}catch( const deException & ){
		if( animator ){
			animator->FreeReference();
		}
		throw;
	}
	// finished
	return animator;
}



void deAnimatorManager::ReleaseLeakingResources(){
	int count = GetAnimatorCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking animators", count );
		pAnimators.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// systems support
////////////////////

void deAnimatorManager::SystemAnimatorLoad(){
	deAnimator *animator = ( deAnimator* )pAnimators.GetRoot();
	deAnimatorSystem &aniSys = *GetAnimatorSystem();
	
	while( animator ){
		if( ! animator->GetPeerAnimator() ){
			aniSys.LoadAnimator( animator );
		}
		
		animator = ( deAnimator* )animator->GetLLManagerNext();
	}
}

void deAnimatorManager::SystemAnimatorUnload(){
	deAnimator *animator = ( deAnimator* )pAnimators.GetRoot();
	
	while( animator ){
		animator->SetPeerAnimator( NULL );
		animator = ( deAnimator* )animator->GetLLManagerNext();
	}
}

void deAnimatorManager::RemoveResource( deResource *resource ){
	pAnimators.RemoveIfPresent( resource );
}
