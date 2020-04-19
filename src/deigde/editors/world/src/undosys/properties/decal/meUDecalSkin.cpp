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

#include "meUDecalSkin.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/decal/meDecalList.h"
#include "../../../world/meWorldGuiParameters.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalSkin
///////////////////////

// Constructor, destructor
////////////////////////////

meUDecalSkin::meUDecalSkin( meDecal *decal, const char *newskin ){
	if( ! decal || ! newskin ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Decal Skin" );
	
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		pDecals = new sDecal[ 1 ];
		
		pDecals[ 0 ].oldskin = decal->GetSkinPath();
		pDecals[ 0 ].newskin = newskin;
		pDecals[ 0 ].decal = decal;
		decal->AddReference();
		
		pDecalCount = 1;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUDecalSkin::meUDecalSkin( meDecalList &decals, const char *newskin ){
	const int count = decals.GetCount();
	meDecal *decal;
	
	if( count == 0 || ! newskin ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Decal Skins" );
	
	pDecals = NULL;
	pDecalCount = 0;
	
	try{
		pDecals = new sDecal[ count ];
		
		for( pDecalCount=0; pDecalCount<count; pDecalCount++ ){
			decal = decals.GetAt( pDecalCount );
			
			pDecals[ pDecalCount ].oldskin = decal->GetSkinPath();
			pDecals[ pDecalCount ].newskin = newskin;
			pDecals[ pDecalCount ].decal = decal;
			decal->AddReference();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUDecalSkin::~meUDecalSkin(){
	pCleanUp();
}



// Management
///////////////

void meUDecalSkin::Undo(){
	meDecal *decal;
	int d;
	
	for( d=0; d<pDecalCount; d++ ){
		decal = pDecals[ d ].decal;
		decal->SetSkinPath( pDecals[ d ].oldskin.GetString() );
	}
	
	pDecals[ 0 ].decal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}

void meUDecalSkin::Redo(){
	meDecal *decal;
	int d;
	
	for( d=0; d<pDecalCount; d++ ){
		decal = pDecals[ d ].decal;
		decal->SetSkinPath( pDecals[ d ].newskin.GetString() );
	}
	
	pDecals[ 0 ].decal->GetWorld()->GetGuiParameters().SetElementMode( meWorldGuiParameters::eemDecal );
}



// Private Functions
//////////////////////

void meUDecalSkin::pCleanUp(){
	if( pDecals ){
		while( pDecalCount > 0 ){
			pDecalCount--;
			pDecals[ pDecalCount ].decal->FreeReference();
		}
		
		delete [] pDecals;
	}
}
