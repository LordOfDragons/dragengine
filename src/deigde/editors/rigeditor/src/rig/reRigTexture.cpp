/* 
 * Drag[en]gine IGDE Rig Editor
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

#include "reRig.h"
#include "reRigTexture.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Class reRigTexture
///////////////////////

// Constructor, destructor
////////////////////////////

reRigTexture::reRigTexture( deEngine *engine, const char *name ){
	if( ! engine || ! name ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	
	pName = name;
	pEngSkin = NULL;
}

reRigTexture::~reRigTexture(){
	pCleanUp();
}



// Management
///////////////

void reRigTexture::SetSkinPath( const char *skinPath ){
	if( ! skinPath ) DETHROW( deeInvalidParam );
	
	if( ! pSkinPath.Equals( skinPath ) ){
		pSkinPath = skinPath;
		LoadSkin();
	}
}

void reRigTexture::LoadSkin(){
	deSkin *engSkin = NULL;
	
	if( ! pSkinPath.IsEmpty() ){
		deSkinManager &skinMgr = *pEngine->GetSkinManager();
		
		try{
			engSkin = skinMgr.LoadSkin( pSkinPath.GetString(), "/" );
			
		}catch( const deException & ){
			// TODO needs access to igdeEnvironment to call GetErrorSkin()
		}
	}
	
	
	if( pEngSkin ){
		pEngSkin->FreeReference();
	}
	pEngSkin = engSkin;
}



// Private Functions
//////////////////////

void reRigTexture::pCleanUp(){
	if( pEngSkin ){
		pEngSkin->FreeReference();
	}
}
