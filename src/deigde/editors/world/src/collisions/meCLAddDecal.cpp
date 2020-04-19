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

#include <stdlib.h>

#include "meCLAddDecal.h"
#include "meCLHitListEntry.h"
#include "../worldedit.h"
#include "../gui/meWindowMain.h"
#include "../world/meColliderOwner.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"
#include "../world/decal/meDecal.h"
#include "../world/decal/meDecalSelection.h"
#include "../world/object/meObject.h"
#include "../undosys/gui/decal/meUAddDecal.h"
#include "../undosys/gui/decal/meUAddObjectDecal.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/skin/igdeGDSkin.h>
#include <deigde/gamedefinition/skin/igdeGDSkinManager.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/collider/deCollider.h>
#include <dragengine/resources/collider/deCollisionInfo.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meCLAddDecal
///////////////////////

// Constructor, destructor
////////////////////////////

meCLAddDecal::meCLAddDecal( meWindowMain *windowMain, meWorld *world ){
	if( ! windowMain || ! world ) DETHROW( deeInvalidParam );
	
	pWndMain = windowMain;
	pWorld = world;
	
	pAlignWithNormal = true;
}

meCLAddDecal::~meCLAddDecal(){
}



// Management
///////////////

void meCLAddDecal::SetRay( const decDVector &rayOrigin, const decVector &rayDirection ){
	pRayOrigin = rayOrigin;
	pRayDirection = rayDirection;
}

void meCLAddDecal::SetAlignWithNormal( bool alignWithNormal ){
	pAlignWithNormal = alignWithNormal;
}



void meCLAddDecal::Reset(){
	if( pUndo ){
		pUndo->Undo();
	}
	
	pHitList.RemoveAllEntries();
}

void meCLAddDecal::RunAction(){
	pHitList.SortByDistance();
	
	if( pHitList.GetEntryCount() == 0 ){
		pUndo = NULL;
		return;
	}
	
	meCLHitListEntry * const entry = pHitList.GetEntryAt( 0 );
	meObject *object = entry->GetObject();
	decVector normal;
	
	if( pAlignWithNormal ){
		normal = entry->GetNormal().Normalized();
		
	}else{
		normal = -pRayDirection.Normalized();
	}
	
	decDVector hitPoint( pRayOrigin + pRayDirection * entry->GetDistance() );
	//pWndMain->GetLogger()->LogErrorFormat( LOGSOURCE, "debug: %g,%g,%g | %g,%g,%g\n", pRayOrigin.x, pRayOrigin.y, pRayOrigin.z, pRayDirection.x, pRayDirection.y, pRayDirection.z );
	//pWndMain->GetLogger()->LogErrorFormat( LOGSOURCE, "debug: %g | %g,%g,%g | %g,%g,%g\n", entry->GetDistance(), hitPoint.x, hitPoint.y, hitPoint.z, normal.x, normal.y, normal.z );
	
	if( ! object ){
		pUndo = NULL;
		return;
	}
	
	const meWorldGuiParameters &guiparams = pWorld->GetGuiParameters();
	const decVector rotation( pGetRotationForNormal( normal ) );
	const decString &browseSkin = guiparams.GetBrowseSkin();
	decVector decalSize;
	
	if( pUndo ){
		decalSize = ( ( meUAddDecal& )( igdeUndo& )pUndo ).GetDecal()->GetSize();
		
	}else{
		deObjectReference refDecal;
		
		refDecal.TakeOver( new meDecal( pWorld->GetEnvironment() ) );
		meDecal &decal = ( meDecal& )( deObject& )refDecal;
		
		decal.SetSkinPath( browseSkin );
		decal.SetSize( decal.GetDefaultSize( 0.5f ) );
		decalSize = decal.GetSize();
		
		pUndo.TakeOver( new meUAddDecal( pWorld, &decal ) );
	}
	
	meDecal &decal = *( ( meUAddDecal& )( igdeUndo& )pUndo ).GetDecal();
	decal.SetPosition( hitPoint + decDVector( normal * ( decalSize.z * 0.5f ) ) );
	decal.SetRotation( rotation );
	pUndo->Redo();
}

void meCLAddDecal::Finish(){
	if( ! pUndo ){
		return;
	}
	
	pWorld->GetUndoSystem()->Add( pUndo, false );
	pUndo = NULL;
}

void meCLAddDecal::Cancel(){
	pUndo = NULL;
}



// Notifications
//////////////////

void meCLAddDecal::CollisionResponse( deCollider *owner, deCollisionInfo *info ){
	meCLHitListEntry *entry = NULL;
	
	if( info->IsCollider() ){
		const meColliderOwner * const colliderOwner = meColliderOwner::GetColliderOwner(
			*pWorld->GetEnvironment(), info->GetCollider() );
		if( ! colliderOwner ){
			return;
		}
		
		if( colliderOwner->GetObject() ){
			try{
				entry = new meCLHitListEntry;
				entry->SetObject( colliderOwner->GetObject() );
				entry->SetDistance( info->GetDistance() );
				entry->SetNormal( info->GetNormal() );
				
				pHitList.AddEntry( entry );
				entry = NULL;
				
			}catch( const deException & ){
				if( entry ){
					delete entry;
				}
				throw;
			}
		}
		
	}else if( info->IsHTSector() ){
	}
}

bool meCLAddDecal::CanHitCollider( deCollider *owner, deCollider *collider ){
	return true;
}

void meCLAddDecal::ColliderChanged( deCollider *owner ){
}



// Private Functions
//////////////////////

decVector meCLAddDecal::pGetRotationForNormal( const decVector &normal ) const{
	decMatrix matrix;
	
	if( fabs( normal.y ) > 0.707106 ){
		matrix.SetVU( normal, decVector( 0.0f, 0.0f, 1.0f ) );
		
	}else{
		matrix.SetVU( normal, decVector( 0.0f, 1.0f, 0.0f ) );
	}
	
	return matrix.GetEulerAngles() / DEG2RAD;
}
