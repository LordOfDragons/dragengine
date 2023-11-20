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

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainNavSpace.h"
#include "meHeightTerrainNavSpaceType.h"
#include "meHeightTerrainNavSpaceFace.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"
#include "../pathfinding/mePathFindTest.h"
#include "../../utils/meByteArray.h"

#include <deigde/loadsave/igdeLoadSaveHTNavSpace.h>
#include <deigde/loadsave/igdeLoadSaveHTNavSpace.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>
#include <deigde/undo/igdeUndoSystem.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/world/deWorld.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainNavSpace
//////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainNavSpace::meHeightTerrainNavSpace( deEngine &engine, const char *name ) :
pEngine( engine ),
pEngNavSpace( NULL ),
    pHTSector( NULL ),

pName( name ),
pSpaceType( deNavigationSpace::estMesh ),
pLayer( 0 ),
pSnapDistance( 0.001f ),
pSnapAngle( 180.0f ),

pActiveType( NULL ),

pNavSpaceChanged( false ),
pNavSpaceSaved( false ),

pActive( false ),

pDDTypeFaces( NULL ),
pBulkUpdate( false )
{
	try{
		pDDTypeFaces = engine.GetDebugDrawerManager()->CreateDebugDrawer();
		pDDTypeFaces->SetXRay( true );
		pDDTypeFaces->SetVisible( false );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meHeightTerrainNavSpace::~meHeightTerrainNavSpace(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainNavSpace::SetHTSector( meHeightTerrainSector *sector ){
	if( sector == pHTSector ){
		return;
	}
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		RemoveDDFromWorld( *pHTSector->GetHeightTerrain()->GetWorld().GetEngineWorld() );
	}
	
	SetEngineNavSpace( NULL );
	pHTSector = sector;
	
	if( sector && sector->GetHeightTerrain() ){
		AddDDToWorld( *sector->GetHeightTerrain()->GetWorld().GetEngineWorld() );
	}
	
	pRepositionDD();
	UpdateDDColors();
}

deHeightTerrainNavSpace *meHeightTerrainNavSpace::CreateEngineNavSpace() const{
	deHeightTerrainNavSpace *navspace = NULL;
	const int typeCount = pTypes.GetCount();
	int navCornerCount = 0;
	int navFaceCount = 0;
	int i, j, k;
	
	for( i=0; i<typeCount; i++ ){
		const meHeightTerrainNavSpaceType &type = *( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) );
		const int faceCount = type.GetFaceCount();
		
		for( j=0; j<faceCount; j++ ){
			navCornerCount += type.GetFaceAt( j )->GetNavPoints().GetCount();
		}
		navFaceCount += faceCount;
	}
	
	try{
		navspace = new deHeightTerrainNavSpace;
		navspace->SetType( pSpaceType );
		navspace->SetLayer( pLayer );
		navspace->SetSnapDistance( pSnapDistance );
		navspace->SetSnapAngle( pSnapAngle * DEG2RAD );
		navspace->SetCornerCount( navCornerCount );
		navspace->SetEdgeCount( 0 ); // TODO
		navspace->SetFaceCount( navFaceCount );
		
		unsigned int *navCorners = navspace->GetCorners();
		deNavigationSpaceFace *navFaces = navspace->GetFaces();
		
		for( i=0; i<typeCount; i++ ){
			const meHeightTerrainNavSpaceType &type = *( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) );
			const int faceCount = type.GetFaceCount();
			
			for( j=0; j<faceCount; j++ ){
				const decIntList &points = type.GetFaceAt( j )->GetNavPoints();
				const int pointCount = points.GetCount();
				
				navFaces->SetType( ( unsigned short )type.GetType() );
				navFaces->SetCornerCount( pointCount );
				navFaces++;
				
				for( k=0; k<pointCount; k++ ){
					*navCorners++ = ( unsigned int )points.GetAt( k );
				}
			}
		}
		
	}catch( const deException & ){
		if( navspace ){
			delete navspace;
		}
		throw;
	}
	
	return navspace;
}

void meHeightTerrainNavSpace::SetEngineNavSpace( deHeightTerrainNavSpace *navspace ){
	pEngNavSpace = navspace;
}



void meHeightTerrainNavSpace::SetName( const char *name ){
	pName = name;
}

void meHeightTerrainNavSpace::SetSpaceType( deNavigationSpace::eSpaceTypes type ){
	if( type < deNavigationSpace::estGrid || type > deNavigationSpace::estVolume ){
		DETHROW( deeInvalidParam );
	}
	
	if( pSpaceType == type ){
		return;
	}
	
	pSpaceType = type;
	
	if( pEngNavSpace ){
		pEngNavSpace->SetType( type );
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceTypeChanged( engHTSector.IndexOfNavSpace( pEngNavSpace ) );
	}
	
	InvalidatePathTest();
	NotifyChanged();
}

void meHeightTerrainNavSpace::SetLayer( int layer ){
	if( pLayer == layer ){
		return;
	}
	
	pLayer = layer;
	
	if( pEngNavSpace ){
		pEngNavSpace->SetLayer( layer );
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceLayerChanged( engHTSector.IndexOfNavSpace( pEngNavSpace ) );
		InvalidatePathTest();
	}
	
	NotifyChanged();
}

void meHeightTerrainNavSpace::SetSnapDistance( float distance ){
	if( fabsf( distance - pSnapDistance ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSnapDistance = distance;
	
	if( pEngNavSpace ){
		pEngNavSpace->SetSnapDistance( distance );
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceSnappingChanged( engHTSector.IndexOfNavSpace( pEngNavSpace ) );
		InvalidatePathTest();
	}
	
	NotifyChanged();
}

void meHeightTerrainNavSpace::SetSnapAngle( float angle ){
	if( fabsf( angle - pSnapAngle ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pSnapAngle = angle;
	
	if( pEngNavSpace ){
		pEngNavSpace->SetSnapAngle( angle * DEG2RAD );
		deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
		engHTSector.NotifyNavSpaceSnappingChanged( engHTSector.IndexOfNavSpace( pEngNavSpace ) );
		InvalidatePathTest();
	}
	
	NotifyChanged();
}



void meHeightTerrainNavSpace::NotifyChanged(){
	if( ! pHTSector ){
		return;
	}
	
	pHTSector->NotifySectorChanged();
	if( pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceChanged( this );
	}
}

void meHeightTerrainNavSpace::InvalidatePathTest() {
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	pHTSector->GetHeightTerrain()->GetWorld().GetPathFindTest()->Invalidate();
}



void meHeightTerrainNavSpace::HeightTerrainSizeChanged(){
	pRepositionDD();
}

void meHeightTerrainNavSpace::SectorSizeOrResChanged(){
	if( ! pHTSector ){
		return;
	}
	
	const int typeCount = pTypes.GetCount();
	int i;
	
	for( i=0; i<typeCount; i++ ){
		( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->RemoveAllFaces();
	}
	
	UpdateNavSpaceFaces();
}

void meHeightTerrainNavSpace::HeightTerrainHeightParametersChanged(){
	pRepositionDD();
}

void meHeightTerrainNavSpace::AddDDToWorld( deWorld &world ){
	world.AddDebugDrawer( pDDTypeFaces );
}

void meHeightTerrainNavSpace::RemoveDDFromWorld( deWorld &world ){
	world.RemoveDebugDrawer( pDDTypeFaces );
}

void meHeightTerrainNavSpace::InvalidateHeights(){
	pUpdateDDHeights();
}

void meHeightTerrainNavSpace::UpdateDDColors(){
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	const meWorld &world = pHTSector->GetHeightTerrain()->GetWorld();
	bool visible = false;
	
	switch( world.GetGuiParameters().GetWorkMode() ){
	case meWorldGuiParameters::ewmNavSpaceEdit:
		visible = pActive;
		break;
		
	case meWorldGuiParameters::ewmHeightPaint:
	case meWorldGuiParameters::ewmMaskPaint:
	case meWorldGuiParameters::ewmVisibilityPaint:
		visible = pActive && world.GetGuiParameters().GetShowNavigationSpacesSelected();
		break;
		
	default:
		break;
	}
	
	if( world.GetGuiParameters().GetShowNavigationSpaces() ){
		visible = true;
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deDebugDrawerShape &shape = *pDDTypeFaces->GetShapeAt( i );
		decColor color( 0.5f, 0.5f, 0.5f, 1.0f );
		
		//if( pHeightTerrain->GetActive() ){
			color = ( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->GetColor();
		//}
		
		shape.SetEdgeColor( color );
		shape.SetFillColor( decColor( color, 0.1f ) );
	}
	
	pDDTypeFaces->NotifyShapeColorChanged();
	pDDTypeFaces->SetVisible( visible );
}



void meHeightTerrainNavSpace::SetActive( bool active ){
	if( active == pActive ){
		return;
	}
	
	pActive = active;
	UpdateDDColors();
}



// Types
//////////

int meHeightTerrainNavSpace::GetTypeCount() const{
	return pTypes.GetCount();
}

meHeightTerrainNavSpaceType *meHeightTerrainNavSpace::GetTypeAt( int index ) const{
	return ( meHeightTerrainNavSpaceType* )pTypes.GetAt( index );
}

meHeightTerrainNavSpaceType *meHeightTerrainNavSpace::GetTypeNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		meHeightTerrainNavSpaceType * const type = ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i );
		if( type->GetName() == name ){
			return type;
		}
	}
	
	return NULL;
}

bool meHeightTerrainNavSpace::HasTypeNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->GetName() == name ){
			return true;
		}
	}
	
	return false;
}

int meHeightTerrainNavSpace::IndexOfType( meHeightTerrainNavSpaceType *type ) const{
	return pTypes.IndexOf( type );
}

int meHeightTerrainNavSpace::IndexOfTypeNamed( const char *name ) const{
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		if( ( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->GetName() == name ){
			return i;
		}
	}
	
	return -1;
}

bool meHeightTerrainNavSpace::HasType( meHeightTerrainNavSpaceType *type ) const{
	return pTypes.Has( type );
}

void meHeightTerrainNavSpace::AddType( meHeightTerrainNavSpaceType *type ){
	if( ! type || HasTypeNamed( type->GetName() ) ){
		DETHROW( deeInvalidParam );
	}
	
	pTypes.Add( type );
	type->SetNavSpace( this );
	
	if( ! pBulkUpdate ){
		pUpdateDDTypeFaces();
	}
	
	NotifyTypeCountChanged();
	
	if( ! pActiveType ){
		SetActiveType( type );
	}
}

void meHeightTerrainNavSpace::RemoveType( meHeightTerrainNavSpaceType *type ){
	if( ! pTypes.Has( type ) ){
		DETHROW( deeInvalidParam );
	}
	
	if( type == pActiveType ){
		if( pTypes.GetCount() == 1 ){
			SetActiveType( NULL );
			
		}else if( pTypes.GetAt( 0 ) == type ){
			SetActiveType( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( 1 ) );
			
		}else{
			SetActiveType( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( 0 ) );
		}
	}
	
	type->SetNavSpace( NULL );
	
	pTypes.Remove( type );
	
	pUpdateDDTypeFaces();
	UpdateNavSpaceFaces();
	
	NotifyTypeCountChanged();
}

void meHeightTerrainNavSpace::RemoveAllTypes(){
	const int count = pTypes.GetCount();
	int i;
	
	SetActiveType( NULL );
	
	for( i=0; i<count; i++ ){
		( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->SetNavSpace( NULL );
	}
	pTypes.RemoveAll();
	
	pUpdateDDTypeFaces();
	UpdateNavSpaceFaces();
	
	NotifyTypeCountChanged();
}

void meHeightTerrainNavSpace::SetActiveType( meHeightTerrainNavSpaceType *type ){
	if( type == pActiveType ){
		return;
	}
	
	pActiveType = type;
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceActiveTypeChanged( this );
	}
}

void meHeightTerrainNavSpace::NotifyTypeCountChanged(){
	if( ! pHTSector ){
		return;
	}
	
	pHTSector->NotifySectorChanged();
	
	if( pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetDepChanged( true );
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceTypeCountChanged( this );
	}
}



// Nav-space file
///////////////////

void meHeightTerrainNavSpace::SetPathNavSpace( const char *path, bool load ){
	if( pPathNavSpace == path ){
		return;
	}
	
	pPathNavSpace = path;
	
	if( load ){
		LoadNavSpaceFromFile();
		
	}else{
		SetNavSpaceSaved( true );
	}
	SetNavSpaceChanged( true );
	
	NotifyTypeCountChanged();
}

void meHeightTerrainNavSpace::SetNavSpaceChanged( bool changed ){
	if( changed == pNavSpaceChanged ){
		return;
	}
	
	pNavSpaceChanged = changed;
	
	if( changed && pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->SetDepChanged( true );
	}
}

void meHeightTerrainNavSpace::SetNavSpaceSaved( bool saved ){
	pNavSpaceSaved = saved;
}

void meHeightTerrainNavSpace::UpdateNavSpaceFaces(){
	if( ! pEngNavSpace ){
		return;
	}
	
	const int typeCount = pTypes.GetCount();
	int navCornerCount = 0;
	int navFaceCount = 0;
	int i, j, k;
	
	for( i=0; i<typeCount; i++ ){
		const meHeightTerrainNavSpaceType &type = *( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) );
		const int faceCount = type.GetFaceCount();
		
		for( j=0; j<faceCount; j++ ){
			navCornerCount += type.GetFaceAt( j )->GetNavPoints().GetCount();
		}
		navFaceCount += faceCount;
	}
	
	pEngNavSpace->SetCornerCount( navCornerCount );
	pEngNavSpace->SetEdgeCount( 0 ); // TODO
	pEngNavSpace->SetFaceCount( navFaceCount );
	
	unsigned int *navCorners = pEngNavSpace->GetCorners();
	deNavigationSpaceFace *navFaces = pEngNavSpace->GetFaces();
	
	for( i=0; i<typeCount; i++ ){
		const meHeightTerrainNavSpaceType &type = *( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) );
		const int faceCount = type.GetFaceCount();
		
		for( j=0; j<faceCount; j++ ){
			const decIntList &points = type.GetFaceAt( j )->GetNavPoints();
			const int pointCount = points.GetCount();
			
			navFaces->SetType( ( unsigned short )type.GetType() );
			navFaces->SetCornerCount( pointCount );
			navFaces++;
			
			for( k=0; k<pointCount; k++ ){
				*navCorners++ = ( unsigned int )points.GetAt( k );
			}
		}
	}
	
	deHeightTerrainSector &engHTSector = *pHTSector->GetEngineSector();
	engHTSector.NotifyNavSpaceLayoutChanged( engHTSector.IndexOfNavSpace( pEngNavSpace ) );
	
	InvalidatePathTest();
	
	if( pHTSector && pHTSector->GetHeightTerrain() ){
		pHTSector->GetHeightTerrain()->GetWorld().NotifyHTNavSpaceFacesChanged( this );
	}
}

void meHeightTerrainNavSpace::LoadNavSpaceFromFile(){
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return; // this is a problem. we can not load the file without an environment
	}
	
	meWorld &world = pHTSector->GetHeightTerrain()->GetWorld();
	world.GetUndoSystem()->RemoveAll(); // sorry, no better way yet to keep this consistent... without using a complex undo action
	
	if( ! pPathNavSpace.IsEmpty() ){
		const decString baseDir( pHTSector->GetHeightTerrain()->GetBaseDirectory() );
		const decPath path( decPath::AbsolutePathUnix( pPathNavSpace, baseDir ) );
		
		if( pEngine.GetVirtualFileSystem()->ExistsFile( path ) ){
			igdeLoadSaveHTNavSpace loadNavSpace( *world.GetEnvironment(), LOGSOURCE );
			deHeightTerrainNavSpace *engNavSpace = NULL;
			int i, j;
			
			try{
				pBulkUpdate = true;
				
				engNavSpace = new deHeightTerrainNavSpace;
				loadNavSpace.Load( *engNavSpace, decBaseFileReader::Ref::New(
					pEngine.GetVirtualFileSystem()->OpenFileForReading( path ) ) );
				
				int typeCount = pTypes.GetCount();
				for( i=0; i<typeCount; i++ ){
					( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->RemoveAllFaces();
				}
				
				const int navFaceCount = engNavSpace->GetFaceCount();
				const deNavigationSpaceFace * const navFaces = engNavSpace->GetFaces();
				const unsigned int *navCorners = engNavSpace->GetCorners();
				
				for( i=0; i<navFaceCount; i++ ){
					const int cornerCount = navFaces[ i ].GetCornerCount();
					const int navType = ( int )navFaces[ i ].GetType();
					
					meHeightTerrainNavSpaceType *type = NULL;
					for( j=0; j<typeCount; j++ ){
						meHeightTerrainNavSpaceType * const checkType = ( meHeightTerrainNavSpaceType* )pTypes.GetAt( j );
						if( checkType->GetType() == navType ){
							type = checkType;
							break;
						}
					}
					
					if( ! type ){
						decString name( "Type " );
						name.AppendValue( navType );
						
						const meHeightTerrainNavSpaceType::Ref newType(
							meHeightTerrainNavSpaceType::Ref::New( new meHeightTerrainNavSpaceType ) );
						newType->SetName( name );
						newType->SetType( navType );
						AddType( newType );
						type = newType;
						typeCount = pTypes.GetCount();
					}
					
					const meHeightTerrainNavSpaceFace::Ref newFace(
						meHeightTerrainNavSpaceFace::Ref::New( new meHeightTerrainNavSpaceFace ) );
					decIntList &newFacePoints = newFace->GetNavPoints();
					for( j=0; j<cornerCount; j++ ){
						newFacePoints.Add( ( int )*navCorners++ );
					}
					type->AddFace( newFace );
				}
				
				pBulkUpdate = false;
				
			}catch( const deException &e ){
				pBulkUpdate = false;
				world.GetLogger()->LogException( LOGSOURCE, e );
			}
		}
	}
	
	const int typeCount = pTypes.GetCount();
	int i;
	for( i=0; i<typeCount; i++ ){
		( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->UpdateDDFaces();
	}
	
	pUpdateDDTypeFaces();
	
	SetNavSpaceSaved( true );
	SetNavSpaceChanged( true );
	
	NotifyTypeCountChanged();
	
	world.NotifyHTNavSpaceFacesChanged( this );
}



// Private Functions
//////////////////////

void meHeightTerrainNavSpace::pCleanUp(){
	SetHTSector( NULL );
	
	RemoveAllTypes();
	
	if( pDDTypeFaces ){
		pDDTypeFaces->FreeReference();
	}
}



void meHeightTerrainNavSpace::pRepositionDD(){
	if( ! pHTSector || ! pHTSector->GetHeightTerrain() ){
		return;
	}
	
	const float sectorSize = pHTSector->GetHeightTerrain()->GetSectorSize();
	const float heightScaling = pHTSector->GetHeightTerrain()->GetHeightScaling();
	const float baseHeight = pHTSector->GetHeightTerrain()->GetBaseHeight();
	const decPoint &coordinates = pHTSector->GetCoordinates();
	const decVector scaling( sectorSize, heightScaling, sectorSize );
	
	decDVector position;
	position.x = ( double )sectorSize * ( double )coordinates.x;
	position.y = ( double )baseHeight;
	position.z = ( double )sectorSize * ( double )coordinates.y;
	
	pDDTypeFaces->SetPosition( position );
	pDDTypeFaces->SetScale( scaling );
}

void meHeightTerrainNavSpace::pUpdateDDTypeFaces(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->SetDDShape( NULL );
	}
	pDDTypeFaces->RemoveAllShapes();
	
	deDebugDrawerShape *shape = NULL;
	deDebugDrawerShape *safeShape;
	
	try{
		for( i=0; i<count; i++ ){
			meHeightTerrainNavSpaceType &type = *( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) );
			
			shape = new deDebugDrawerShape;
			pDDTypeFaces->AddShape( shape );
			safeShape = shape;
			shape = NULL;
			
			type.SetDDShape( safeShape );
		}
		
	}catch( const deException & ){
		if( shape ){
			delete shape;
		}
		throw;
	}
	
	pDDTypeFaces->NotifyShapeLayoutChanged();
	
	UpdateDDColors();
	UpdateNavSpaceFaces();
}

void meHeightTerrainNavSpace::pUpdateDDHeights(){
	if( ! pHTSector ){
		return;
	}
	
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( meHeightTerrainNavSpaceType* )pTypes.GetAt( i ) )->UpdateHeights();
	}
	pDDTypeFaces->NotifyShapeContentChanged();
}
