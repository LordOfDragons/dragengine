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

#include "meTaskSyncGameDefinition.h"
#include "meWindowMain.h"
#include "properties/meWindowProperties.h"
#include "../world/meWorld.h"
#include "../world/decal/meDecal.h"
#include "../world/object/meObject.h"
#include "../world/object/texture/meObjectTexture.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/wrapper/igdeWSky.h>
#include <deigde/gui/wrapper/igdeWObject.h>

#include <dragengine/common/exceptions.h>



// Class meTaskSyncGameDefinition
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meTaskSyncGameDefinition::meTaskSyncGameDefinition( meWindowMain &windowMain ) :
pWindowMain( windowMain ),
pState( esProcessWorld ),
pObjectIndex( 0 ),
pDecalIndex( 0 ),
pStepIndex( 0 ),
pStepCount( 0 ),
pBatchSizeObjects( 100 ),
pBatchSizeDecals( 100 )
{
	SetMessage( "World Editor: Synchronize Game Definition" );
}

meTaskSyncGameDefinition::~meTaskSyncGameDefinition(){
}



// Management
///////////////

bool meTaskSyncGameDefinition::Step(){
	if( ! pWindowMain.GetWorld() ){
		return false;
	}
	
	meWorld &world = *pWindowMain.GetWorld();
	
	switch( pState ){
	case esProcessWorld:
		pStepCount = 1; // esProcessWorld
		pStepCount += ( decMath::max( world.GetObjects().GetCount() - 1, 0 )
			/ pBatchSizeObjects ) + 1;
		pStepCount += ( decMath::max( world.GetDecals().GetCount() - 1, 0 )
			/ pBatchSizeDecals ) + 1;
		pStepCount += 1; // esProcessWindows
		
		world.GetSky()->OnGameDefinitionChanged();
		
		pState = esProcessObjects;
		pStepIndex++;
		return true;
		
	case esProcessObjects:{
		const meObjectList &objects = world.GetObjects();
		const int objectCount = objects.GetCount();
		const int lastIndex = decMath::min( pObjectIndex + pBatchSizeObjects, objectCount );
		int i;
		
		SetProgress( ( float )pStepIndex / ( float )pStepCount );
		
		decString message;
		message.Format( "World Editor: Synchronize Game Definition: Objects (%d/%d)",
			pObjectIndex, objectCount );
		SetMessage( message );
		
		for( ; pObjectIndex<lastIndex; pObjectIndex++ ){
			meObject &object = *objects.GetAt( pObjectIndex );
			object.OnGameDefinitionChanged();
			
			const int textureCount = object.GetTextureCount();
			for( i=0; i<textureCount; i++ ){
				meObjectTexture &texture = *object.GetTextureAt( i );
				texture.LoadSkin();
			}
			
			const int decalCount = object.GetDecalCount();
			for( i=0; i<decalCount; i++ ){
				meDecal &decal = *object.GetDecalAt( i );
				decal.LoadSkin();
			}
		}
		
		pStepIndex++;
		if( pObjectIndex == objectCount ){
			pState = esProcessDecals;
		}
		}
		return true;
		
	case esProcessDecals:{
		const meDecalList &decals = world.GetDecals();
		const int decalCount = decals.GetCount();
		const int lastIndex = decMath::min( pDecalIndex + pBatchSizeDecals, decalCount );
		
		SetProgress( ( float )pStepIndex / ( float )pStepCount );
		decString message;
		message.Format( "World Editor: Synchronize Game Definition: Decals (%d/%d)",
			pDecalIndex, pDecalIndex );
		SetMessage( message );
		
		for( ; pDecalIndex<lastIndex; pDecalIndex++ ){
			meDecal &decal = *decals.GetAt( pDecalIndex );
			decal.LoadSkin();
		}
		
		pStepIndex++;
		if( pDecalIndex == decalCount ){
			pState = esProcessWindows;
		}
		}
		return true;
		
	case esProcessWindows:
		SetProgress( ( float )pStepIndex / ( float )pStepCount );
		SetMessage( "World Editor: Synchronize Game Definition" );
		
		pWindowMain.GetWindowProperties()->OnGameDefinitionChanged();
		pState = esFinished;
		pStepIndex++;
		return false;
		
	case esFinished:
		return false;
	}
	
	return false;
}
