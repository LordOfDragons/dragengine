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

#include "deBillboard.h"
#include "deBillboardManager.h"
#include "../../deEngine.h"
#include "../../systems/deGraphicSystem.h"
#include "../../common/exceptions.h"



// Class deBillboardManager
/////////////////////////////

// Constructor, destructor
////////////////////////////

deBillboardManager::deBillboardManager( deEngine *engine ) : deResourceManager( engine, ertBillboard ){
	SetLoggingName( "billboard" );
}

deBillboardManager::~deBillboardManager(){
	ReleaseLeakingResources();
}



// Management
///////////////

int deBillboardManager::GetBillboardCount() const{
	return pBillboards.GetCount();
}

deBillboard *deBillboardManager::GetRootBillboard() const{
	return ( deBillboard* )pBillboards.GetRoot();
}

deBillboard *deBillboardManager::CreateBillboard(){
	deBillboard *billboard = NULL;
	
	try{
		billboard = new deBillboard( this );
		if( ! billboard ) DETHROW( deeOutOfMemory );
		
		GetGraphicSystem()->LoadBillboard( billboard );
		
		pBillboards.Add( billboard );
		
	}catch( const deException & ){
		if( billboard ){
			billboard->FreeReference();
		}
		throw;
	}
	
	return billboard;
}



void deBillboardManager::ReleaseLeakingResources(){
	if( GetBillboardCount() > 0 ){
		LogWarnFormat( "%i leaking billboards", GetBillboardCount() );
		pBillboards.RemoveAll(); // wo do not delete them to avoid crashes. better leak than crash
	}
}



// Systems Support
////////////////////

void deBillboardManager::SystemGraphicLoad(){
	deBillboard *billboard = ( deBillboard* )pBillboards.GetRoot();
	
	while( billboard ){
		if( ! billboard->GetPeerGraphic() ){
			GetGraphicSystem()->LoadBillboard( billboard );
		}
		
		billboard = ( deBillboard* )billboard->GetLLManagerNext();
	}
}

void deBillboardManager::SystemGraphicUnload(){
	deBillboard *billboard = ( deBillboard* )pBillboards.GetRoot();
	
	while( billboard ){
		billboard->SetPeerGraphic( NULL );
		billboard = ( deBillboard* )billboard->GetLLManagerNext();
	}
}

void deBillboardManager::RemoveResource( deResource *resource ){
	pBillboards.RemoveIfPresent( resource );
}
