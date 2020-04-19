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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mePathFindTest.h"
#include "types/mePathFindTestType.h"
#include "../meWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/navigation/navigator/deNavigatorType.h>
#include <dragengine/resources/navigation/navigator/deNavigatorManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>

#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"


 
// Class mePathFindTest
/////////////////////////

// Constructor, destructor
////////////////////////////

mePathFindTest::mePathFindTest( deEngine *engine ){
	if( ! engine ){
		DETHROW( deeInvalidParam );
	}
	
	pEngine = engine;
	pWorld = NULL;
	
	pDDSPath = NULL;
	pEngNavigator = NULL;
	
	pLayer = 0;
	pBlockingCost = 1000.0f;
	pSpaceType = deNavigationSpace::estMesh;
	pShowPath = false;
	pDirtyPath = true;
	
	try{
		pEngNavigator = engine->GetNavigatorManager()->CreateNavigator();
		pEngNavigator->SetSpaceType( pSpaceType );
		pEngNavigator->SetMaxOutsideDistance( 0.5f );
		pEngNavigator->SetBlockingCost( pBlockingCost );
		
		// create debug drawer and shapes
		pDebugDrawer = engine->GetDebugDrawerManager()->CreateDebugDrawer();
		pDebugDrawer->SetXRay( true );
		pDebugDrawer->SetVisible( pShowPath );
		
		pDDSPath = new igdeWDebugDrawerShape;
		pDDSPath->SetEdgeColor( decColor( 1.0f, 0.0f, 0.5f, 1.0f ) );
		pDDSPath->SetFillColor( decColor( 1.0f, 0.0f, 0.5f, 0.25f ) );
		pDDSPath->SetParentDebugDrawer( pDebugDrawer );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

mePathFindTest::~mePathFindTest(){
	pCleanUp();
}



// Management
///////////////

void mePathFindTest::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pWorld ){
		pWorld->GetEngineWorld()->RemoveNavigator( pEngNavigator );
		pWorld->GetEngineWorld()->RemoveDebugDrawer( pDebugDrawer );
	}
	
	pWorld = world;
	
	if( world ){
		world->GetEngineWorld()->AddDebugDrawer( pDebugDrawer );
		world->GetEngineWorld()->AddNavigator( pEngNavigator );
	}
	
	Invalidate();
}



void mePathFindTest::SetStartPosition( const decDVector &position ){
	if( pStartPosition.IsEqualTo( position ) ){
		return;
	}
	
	pStartPosition = position;
	
	pDDSPath->SetPosition( position );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetGoalPosition( const decDVector &position ){
	if( pGoalPosition.IsEqualTo( position ) ){
		return;
	}
	
	pGoalPosition = position;
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetStartOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pStartOrientation ) ){
		return;
	}
	
	pStartOrientation = orientation;
	
	//const decQuaternion realor = decQuaternion::CreateFromEuler( orientation * DEG2RAD );
	//pEngNavigator->SetOrientation( realor );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetGoalOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pGoalOrientation ) ){
		return;
	}
	
	pGoalOrientation = orientation;
	
	//const decQuaternion realor = decQuaternion::CreateFromEuler( orientation * DEG2RAD );
	//pEngNavigator->SetOrientation( realor );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetLayer( int layer ){
	if( layer == pLayer ){
		return;
	}
	
	pLayer = layer;
	
	pEngNavigator->SetLayer( layer );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetSpaceType( deNavigationSpace::eSpaceTypes spaceType ){
	if( spaceType == pSpaceType ){
		return;
	}
	
	pSpaceType = spaceType;
	
	pEngNavigator->SetSpaceType( spaceType );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetPath( const deNavigatorPath &path ){
	pPath = path;
}

void mePathFindTest::SetBlockingCost( float cost ){
	if( fabsf( cost - pBlockingCost ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBlockingCost = cost;
	
	pEngNavigator->SetBlockingCost( cost );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
	
	Invalidate();
}

void mePathFindTest::SetShowPath( bool showPath ){
	if( showPath == pShowPath ){
		return;
	}
	
	pShowPath = showPath;
	pDebugDrawer->SetVisible( showPath );
	
	if( pWorld ){
		pWorld->NotifyPathFindTestChanged();
	}
}



void mePathFindTest::NotifyTypesChanged(){
	pUpdateTypes();
	Invalidate();
	pWorld->NotifyPathFindTestChanged();
}



void mePathFindTest::Invalidate(){
	pDirtyPath = true;
}



void mePathFindTest::Update(){
	if( ! pDirtyPath || ! pShowPath ){
		return;
	}
	
	pEngNavigator->FindPath( pPath, pStartPosition, pGoalPosition );
	pUpdateDDPath();
	pDirtyPath = false;
}



// Private Functions
//////////////////////

void mePathFindTest::pCleanUp(){
	SetWorld( NULL );
	
	if( pEngNavigator ){
		pEngNavigator->FreeReference();
	}
	
	if( pDDSPath ){
		delete pDDSPath;
	}
	if( pDebugDrawer ){
		pDebugDrawer->FreeReference();
	}
}



void mePathFindTest::pUpdateDDPath(){
	if( ! pDDSPath ){
		return;
	}
	
	pDDSPath->RemoveAllFaces();
	
	const int count = pPath.GetCount();
	if( count == 0 ){
		return;
	}
	
	const int circlePointCount = 8;
	const float radius = 0.02;
	const float circleStep = DEG2RAD * ( 360.0f / ( float )circlePointCount );
	const decVector up( 0.0f, 1.0f, 0.0f );
	
	deDebugDrawerShapeFace *ddsFace = NULL;
	decVector lastPoints[ circlePointCount ], points[ circlePointCount ];
	decVector prevPos, pos, nextPos;
	decMatrix matrix;
	decVector view;
	float len;
	int i, j;
	
	try{
		for( i=0; i<count; i++ ){
			pos = ( pPath.GetAt( i ) - pStartPosition ).ToVector();
			
			if( i > 0 ){
				prevPos = ( pPath.GetAt( i - 1 ) - pStartPosition ).ToVector();
			}
			
			if( i < count - 1 ){
				nextPos = ( pPath.GetAt( i + 1 ) - pStartPosition ).ToVector();
				
			}else{
				nextPos = pos;
			}
			
			if( i == 0 ){
				view = pos - prevPos;
				len = view.Length();
				if( len < 0.001f ){
					view.Set( 0.0f, 0.0f, 1.0f );
					
				}else{
					view /= len;
				}
				
				matrix.SetWorld( decVector(), view, up );
				for( j=0; j<circlePointCount; j++ ){
					lastPoints[ j ] = matrix.Transform( radius * sinf( circleStep * ( float )j ), radius * cosf( circleStep * ( float )j ), 0.0f );
				}
			}
			
			view = nextPos - prevPos;
			len = view.Length();
			if( len < 0.001f ){
				view.Set( 0.0f, 0.0f, 1.0f );
				
			}else{
				view /= len;
			}
			
			matrix.SetWorld( pos, view, up );
			for( j=0; j<circlePointCount; j++ ){
				points[ j ] = matrix.Transform( radius * sinf( circleStep * ( float )j ), radius * cosf( circleStep * ( float )j ), 0.0f );
			}
			
			for( j=0; j<circlePointCount; j++ ){
				ddsFace = new deDebugDrawerShapeFace;
				ddsFace->AddVertex( lastPoints[ j ] );
				ddsFace->AddVertex( points[ j ] );
				ddsFace->AddVertex( points[ ( j + 1 ) % circlePointCount ] );
				ddsFace->AddVertex( lastPoints[ ( j + 1 ) % circlePointCount ] );
				pDDSPath->AddFace( ddsFace );
				ddsFace = NULL;
			}
			
			for( j=0; j<circlePointCount; j++ ){
				lastPoints[ j ] = points[ j ];
			}
		}
		
	}catch( const deException & ){
		if( ddsFace ){
			delete ddsFace;
		}
		throw;
	}
}

void mePathFindTest::pUpdateTypes(){
	const int count = pTypeList.GetCount();
	int i;
	
	pEngNavigator->RemoveAllTypes();
	
	for( i=0; i<count; i++ ){
		const mePathFindTestType &type = *pTypeList.GetAt( i );
		deNavigatorType &engType = *pEngNavigator->AddType( type.GetTypeNumber() );
		
		engType.SetFixCost( type.GetFixCost() );
		engType.SetCostPerMeter( type.GetCostPerMeter() );
	}
	
	pEngNavigator->NotifyTypesChanged();
}
