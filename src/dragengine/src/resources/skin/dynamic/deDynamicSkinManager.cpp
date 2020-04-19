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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deDynamicSkin.h"
#include "deDynamicSkinManager.h"

#include "../../../deEngine.h"
#include "../../../systems/deModuleSystem.h"
#include "../../../systems/deGraphicSystem.h"
#include "../../../systems/modules/deLoadableModule.h"
#include "../../../systems/modules/graphic/deBaseGraphicDynamicSkin.h"
#include "../../../common/exceptions.h"



// Class deDynamicSkinManager
///////////////////////////////

// Constructor, destructor
////////////////////////////

deDynamicSkinManager::deDynamicSkinManager( deEngine *engine ) : deResourceManager( engine, ertDynamicSkin ){
	SetLoggingName( "dynamic skin" );
}

deDynamicSkinManager::~deDynamicSkinManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deDynamicSkinManager::GetDynamicSkinCount() const{
	return pSkins.GetCount();
}

deDynamicSkin *deDynamicSkinManager::GetRootDynamicSkin() const{
	return ( deDynamicSkin* )pSkins.GetRoot();
}

deDynamicSkin *deDynamicSkinManager::CreateDynamicSkin(){
	deDynamicSkin *dynamicSkin = NULL;
	
	try{
		dynamicSkin = new deDynamicSkin( this );
		if( ! dynamicSkin ) DETHROW( deeOutOfMemory );
		
		GetGraphicSystem()->LoadDynamicSkin( dynamicSkin );
		
		pSkins.Add( dynamicSkin );
		
	}catch( const deException & ){
		if( dynamicSkin ){
			dynamicSkin->FreeReference();
		}
		throw;
	}
	
	return dynamicSkin;
}



void deDynamicSkinManager::ReleaseLeakingResources(){
	int count = GetDynamicSkinCount();
	
	if( count > 0 ){
		LogWarnFormat( "%i leaking dynamic skins", count );
		pSkins.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deDynamicSkinManager::SystemGraphicLoad(){
	deDynamicSkin *dynamicSkin = ( deDynamicSkin* )pSkins.GetRoot();
	deGraphicSystem &grasys = *GetGraphicSystem();
	
	while( dynamicSkin ){
		if( ! dynamicSkin->GetPeerGraphic() ){
			grasys.LoadDynamicSkin( dynamicSkin );
		}
		
		dynamicSkin = ( deDynamicSkin* )dynamicSkin->GetLLManagerNext();
	}
}

void deDynamicSkinManager::SystemGraphicUnload(){
	deDynamicSkin *dynamicSkin = ( deDynamicSkin* )pSkins.GetRoot();
	
	while( dynamicSkin ){
		dynamicSkin->SetPeerGraphic( NULL );
		dynamicSkin = ( deDynamicSkin* )dynamicSkin->GetLLManagerNext();
	}
}

void deDynamicSkinManager::RemoveResource( deResource *resource ){
	pSkins.RemoveIfPresent( resource );
}
