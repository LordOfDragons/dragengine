/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include <string.h>

#include "deoglCollideList.h"
#include "deoglCollideListComponent.h"
#include "deoglCollideListLight.h"
#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "deoglCollideListPropFieldCluster.h"
#include "deoglCollideListHTSector.h"
#include "deoglCollideListHTSCluster.h"
#include "deoglCLVisitorElements.h"

#include "../component/deoglRComponent.h"
#include "../billboard/deoglRBillboard.h"
#include "../light/deoglRLight.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../propfield/deoglPropFieldCluster.h"
#include "../propfield/deoglRPropField.h"
#include "../propfield/deoglRPropFieldType.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../skin/deoglRSkin.h"
#include "../terrain/heightmap/deoglHTSCluster.h"
#include "../terrain/heightmap/deoglHTView.h"
#include "../terrain/heightmap/deoglHTViewSector.h"
#include "../terrain/heightmap/deoglRHTSector.h"
#include "../visitors/deoglTransformVolume.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
#include "../utils/collision/deoglDCollisionVolume.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionFrustum.h"

#include <dragengine/common/exceptions.h>



// Class deoglCollideList
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglCollideList::deoglCollideList(){
	pComponents = NULL;
	pComponentCount = 0;
	pComponentSize = 0;
	
	pLights = NULL;
	pLightCount = 0;
	pLightSize = 0;
	
	pBillboards = NULL;
	pBillboardCount = 0;
	pBillboardSize = 0;
	
	pHTSectors = NULL;
	pHTSectorCount = 0;
	pHTSectorSize = 0;
	
	pHTSClusters = nullptr;
	pHTSClusterCount = 0;
	pHTSClusterSize = 0;
	
	pPropFields = NULL;
	pPropFieldCount = 0;
	pPropFieldSize = 0;
	
	pPropFieldClusters = NULL;
	pPropFieldClusterCount = 0;
	pPropFieldClusterSize = 0;
	
	pTransformVolume = NULL;
	
	try{
		pTransformVolume = new deoglTransformVolume;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglCollideList::~deoglCollideList(){
	pCleanUp();
}



// Management
///////////////

void deoglCollideList::Clear(){
	RemoveAllPropFieldClusters();
	RemoveAllPropFields();
	RemoveAllHTSClusters();
	RemoveAllHTSectors();
	RemoveAllBillboards();
	RemoveAllLights();
	RemoveAllComponents();
	pParticleEmitterList.RemoveAll();
	pEnvMapList.RemoveAll();
}

void deoglCollideList::SortLinear( const decVector &pos, const decVector &view ){
}

void deoglCollideList::SortRadial( const decVector &pos ){
}

void deoglCollideList::FlagAllLit( bool lit ){
	int i;
	for( i=0; i<pComponentCount; i++ ) pComponents[ i ]->GetComponent()->SetLit( lit );
}

void deoglCollideList::AddElementsColliding( deoglWorldOctree *octree, deoglDCollisionVolume *volume ){
	if( ! octree || ! volume ) DETHROW( deeInvalidParam );
	deoglCLVisitorElements visitor( this, volume );
	decDVector minExtend, maxExtend;
	deoglDCollisionBox box;
	
	volume->GetEnclosingBox( &box );
	minExtend = box.GetCenter() - box.GetHalfSize();
	maxExtend = box.GetCenter() + box.GetHalfSize();
	
	visitor.SetVisitAll( true );
	
	octree->VisitNodesColliding( &visitor, minExtend, maxExtend );
}

void deoglCollideList::AddElementsColliding( deoglWorldOctree *octree, deoglDCollisionFrustum *volume ){
	if( ! octree || ! volume ) DETHROW( deeInvalidParam );
	// HACK: frustum does not support GetEnclosingBox yet so we cheat in this particular case
	// TODO: fix the enclosing box to improve speed
	deoglCLVisitorElements visitor( this, volume );
//	decVector minExtend, maxExtend;
//	deoglDCollisionBox box;
	
//	volume->GetEnclosingBox( &box );
//	minExtend = box.GetCenter() - box.GetHalfSize();
//	maxExtend = box.GetCenter() + box.GetHalfSize();
	
	visitor.SetVisitAll( true );
	
//	octree->VisitNodesColliding( &visitor, minExtend, maxExtend );
	octree->VisitNodesCollidingVolume( &visitor, volume );
}

void deoglCollideList::RemoveCulledElements(){
	RemoveCulledComponents();
	RemoveCulledLights();
	RemoveCulledBillboards();
}

void deoglCollideList::UpdateCubeFaceMasks( const decDVector &position ) const{
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		deoglCollideListComponent &component = *pComponents[ i ];
		component.UpdateCubeFaceMask( position );
		component.SetSpecialFlags( component.GetCubeFaceMask() );
	}
	
	// TODO same for billboard
}

void deoglCollideList::UpdateOccMeshCubeFaceMasks( const decDVector &position ) const{
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		deoglCollideListComponent &component = *pComponents[ i ];
		if( component.GetComponent()->GetOcclusionMesh() ){
			component.UpdateCubeFaceMask( position );
			component.SetSpecialFlags( component.GetCubeFaceMask() );
		}
	}
}



void deoglCollideList::AddParticleEmittersColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume ){
	if( ! volume ){
		DETHROW( deeInvalidParam );
	}
	
	deoglCLVisitorElements visitor( this, volume );
	
	visitor.SetVisitAll( false );
	visitor.SetVisitParticleEmitters( true );
	
	octree.VisitNodesCollidingVolume( &visitor, volume );
}



void deoglCollideList::AddEnvironmentMapsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume ){
}

void deoglCollideList::DebugSingleLine( deoglRTLogger &logger, bool sorted ) const{
	decStringList pointers;
	decString string, temp;
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		temp.Format( "%p", pComponents[ i ]->GetComponent() );
		pointers.Add( temp );
	}
	if( sorted ) pointers.SortAscending();
	string.AppendFormat( "c[%s]", pointers.Join( ", " ).GetString() );
	
	for( i=0; i<pBillboardCount; i++ ){
		temp.Format( "%p", pBillboards[ i ] );
		pointers.Add( temp );
	}
	if( sorted ) pointers.SortAscending();
	string.AppendFormat( " b[%s]", pointers.Join( ", " ).GetString() );
	
	logger.LogInfoFormat( "CollideList %p: %s", this, string.GetString() );
}



// Components
///////////////

deoglCollideListComponent *deoglCollideList::GetComponentAt( int index ) const{
	if( index < 0 || index >= pComponentCount ){
		DETHROW( deeInvalidParam );
	}
	return pComponents[ index ];
}

int deoglCollideList::IndexOfComponent( deoglRComponent *component ) const{
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pComponentCount; i++ ){
		if( component == pComponents[ i ]->GetComponent() ){
			return i;
		}
	}
	
	return -1;
}

bool deoglCollideList::HasComponent( deoglRComponent *component ) const{
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pComponentCount; i++ ){
		if( component == pComponents[ i ]->GetComponent() ){
			return true;
		}
	}
	
	return false;
}

deoglCollideListComponent *deoglCollideList::AddComponent( deoglRComponent *component ){
	if( ! component ){
		DETHROW( deeInvalidParam );
	}
	
	if( pComponentCount == pComponentSize ){
		int i, newSize = pComponentCount * 3 / 2 + 1;
		deoglCollideListComponent **newArray = new deoglCollideListComponent*[ newSize ];
		for( i=pComponentSize; i<newSize; i++ ){
			newArray[ i ] = NULL;
		}
		if( pComponents ){
			for( i=0; i<pComponentSize; i++ ){
				newArray[ i ] = pComponents[ i ];
			}
			delete [] pComponents;
		}
		pComponents = newArray;
		pComponentSize = newSize;
	}
	
	if( ! pComponents[ pComponentCount ] ){
		pComponents[ pComponentCount ] = new deoglCollideListComponent;
	}
	
	deoglCollideListComponent * const clcomponent = pComponents[ pComponentCount++ ];
	clcomponent->SetComponent( component );
	return clcomponent;
}

void deoglCollideList::RemoveComponent( deoglRComponent *component ){
	RemoveComponentFrom( IndexOfComponent( component ) );
}

void deoglCollideList::RemoveComponentFrom( int index ){
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	deoglCollideListComponent * const clcomp = pComponents[ index ];
	clcomp->Clear();
	
	int i;
	for( i=index+1; i<pComponentCount; i++ ){
		pComponents[ i - 1 ] = pComponents[ i ];
	}
	pComponents[ --pComponentCount ] = clcomp;
}

void deoglCollideList::RemoveAllComponents(){
	while( pComponentCount > 0 ){
		pComponents[ --pComponentCount ]->Clear();
	}
}



void deoglCollideList::AddComponentsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume ){
	if( ! volume ){
		DETHROW( deeInvalidParam );
	}
	
	deoglCLVisitorElements visitor( this, volume );
	visitor.SetVisitAll( false );
	visitor.SetVisitComponents( true );
	
	octree.VisitNodesCollidingVolume( &visitor, volume );
}

void deoglCollideList::RemoveCulledComponents(){
	int i, last = 0;
	for( i=0; i<pComponentCount; i++ ){
		if( pComponents[ i ]->GetCulled() ){
			pComponents[ i ]->Clear();
			continue;
		}
		
		if( i != last ){
			deoglCollideListComponent * const exchange = pComponents[ last ];
			pComponents[ last ] = pComponents[ i ];
			pComponents[ i ] = exchange;
		}
		last++;
	}
	pComponentCount = last;
}

void deoglCollideList::RemoveSolidComponents(){
	int i, last = 0;
	for( i=0; i<pComponentCount; i++ ){
		deoglRSkin * const skin = pComponents[ i ]->GetComponent()->GetSkin();
		if( skin && ! skin->GetIsSolid() ){
			pComponents[ i ]->Clear();
			continue;
		}
		
		if( i != last ){
			deoglCollideListComponent * const exchange = pComponents[ last ];
			pComponents[ last ] = pComponents[ i ];
			pComponents[ i ] = exchange;
		}
		last++;
	}
	pComponentCount = last;
}

void deoglCollideList::SortComponentsByModels(){
	int i = 0, j;
	while( i < pComponentCount ){
		deoglRModel * const referenceModel = pComponents[ i ]->GetComponent()->GetModel();
		i++;
		
		for( j=i; j<pComponentCount; j++ ){
			if( pComponents[ j ]->GetComponent()->GetModel() == referenceModel ){
				if( j > i ){
					deoglCollideListComponent * const exchange = pComponents[ i ];
					pComponents[ i ] = pComponents[ j ];
					pComponents[ j ] = exchange;
				}
				i++;
			}
		}
	}
}

void deoglCollideList::SortComponentsByDistance( const decVector &pos, const decVector &view ){
	decVector normView = view;
	decVector compDir;
	int c;
	
	if( pComponentCount > 0 ){
		normView.Normalize();
		
		// calculate distance to reference point using view direction as projection axis
		for( c=0; c<pComponentCount; c++ ){
			compDir = pComponents[ c ]->GetComponent()->GetMatrix().GetPosition().ToVector() - pos;
			pComponents[ c ]->GetComponent()->SetSortDistance( compDir * normView );
		}
		
		// sort by distance
		pSortCompByDist( 0, pComponentCount - 1 );
	}
}

void deoglCollideList::MarkComponents( bool marked ) const{
	int i;
	for( i=0; i<pComponentCount; i++ ){
		pComponents[ i ]->GetComponent()->SetMarked( marked );
	}
}



void deoglCollideList::LogComponents() const{
	if( pComponentCount == 0 ){
		return;
	}
	
	deoglRenderThread &renderThread = pComponents[ 0 ]->GetComponent()->GetRenderThread();
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		const deoglRComponent * const component = pComponents[ i ]->GetComponent();
		const deoglRModel * const model = component->GetModel();
		const decDVector pos( component->GetMatrix().GetPosition() );
		
		if( model ){
			const deoglModelLOD &modelLOD = model->GetLODAt( pComponents[ i ]->GetLODLevel() );
			renderThread.GetLogger().LogInfoFormat( "%i: comp=%p pos=(%.3f,%.3f,%.3f) model=%s tex=%i faces=%i",
				i, component, pos.x, pos.y, pos.z, model->GetFilename().GetString(),
				modelLOD.GetTextureCount(), modelLOD.GetFaceCount() );
			
		}else{
			renderThread.GetLogger().LogInfoFormat( "%i: comp=%p pos=(%.3f,%.3f,%.3f) model=-",
				i, component, pos.x, pos.y, pos.z );
		}
	}
}



// Lights
///////////

deoglCollideListLight *deoglCollideList::GetLightAt( int index ) const{
	if( index < 0 || index >= pLightCount ){
		DETHROW( deeInvalidParam );
	}
	return pLights[ index ];
}

int deoglCollideList::IndexOfLight( deoglRLight *light ) const{
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pLightCount; i++ ){
		if( light == pLights[ i ]->GetLight() ){
			return i;
		}
	}
	
	return -1;
}

bool deoglCollideList::HasLight( deoglRLight *light ) const{
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=0; i<pLightCount; i++ ){
		if( light == pLights[ i ]->GetLight() ){
			return true;
		}
	}
	
	return false;
}

deoglCollideListLight *deoglCollideList::AddLight( deoglRLight *light ){
	if( ! light ){
		DETHROW( deeInvalidParam );
	}
	
	if( pLightCount == pLightSize ){
		int i, newSize = pLightCount * 3 / 2 + 1;
		deoglCollideListLight **newArray = new deoglCollideListLight*[ newSize ];
		for( i=pLightSize; i<newSize; i++ ){
			newArray[ i ] = NULL;
		}
		if( pLights ){
			for( i=0; i<pLightSize; i++ ){
				newArray[ i ] = pLights[ i ];
			}
			delete [] pLights;
		}
		pLights = newArray;
		pLightSize = newSize;
	}
	
	if( ! pLights[ pLightCount ] ){
		pLights[ pLightCount ] = new deoglCollideListLight;
	}
	
	deoglCollideListLight * const cllight = pLights[ pLightCount++ ];
	cllight->SetLight( light );
	return cllight;
}

void deoglCollideList::RemoveLight( deoglRLight *light ){
	RemoveLightFrom( IndexOfLight( light ) );
}

void deoglCollideList::RemoveLightFrom( int index ){
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	deoglCollideListLight * const cllight = pLights[ index ];
	cllight->Clear();
	
	int i;
	for( i=index+1; i<pLightCount; i++ ){
		pLights[ i - 1 ] = pLights[ i ];
	}
	pLights[ --pLightCount ] = cllight;
}

void deoglCollideList::RemoveAllLights(){
	while( pLightCount > 0 ){
		pLights[ --pLightCount ]->Clear();
	}
}

void deoglCollideList::AddLightsColliding( deoglWorldOctree *octree, deoglDCollisionVolume *colVol ){
	if( ! octree || ! colVol ){
		DETHROW( deeInvalidParam );
	}
	
	deoglCLVisitorElements visitor( this, colVol );
	visitor.SetVisitAll( false );
	visitor.SetVisitLights( true );
	
	octree->VisitNodesCollidingVolume( &visitor, colVol );
}

void deoglCollideList::MarkLightsCulled( bool culled ){
	int i;
	for( i=0; i<pLightCount; i++ ){
		pLights[ i ]->SetCulled( culled );
	}
}

void deoglCollideList::RemoveCulledLights(){
	int i, last = 0;
	for( i=0; i<pLightCount; i++ ){
		if( pLights[ i ]->GetCulled() ){
			pLights[ i ]->Clear();
			continue;
		}
		
		if( i != last ){
			deoglCollideListLight * const exchange = pLights[ last ];
			pLights[ last ] = pLights[ i ];
			pLights[ i ] = exchange;
		}
		last++;
	}
	pLightCount = last;
}



// Billboards
///////////////

deoglRBillboard *deoglCollideList::GetBillboardAt( int index ) const{
	if( index < 0 || index >= pBillboardCount ){
		DETHROW( deeInvalidParam );
	}
	return pBillboards[ index ];
}

void deoglCollideList::AddBillboard( deoglRBillboard *billboard ){
	if( ! billboard ){
		DETHROW( deeInvalidParam );
	}
	
	if( pBillboardCount == pBillboardSize ){
		int i, newSize = pBillboardCount * 3 / 2 + 1;
		deoglRBillboard **newArray = new deoglRBillboard*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pBillboards ){
			for( i=0; i<pBillboardCount; i++ ) newArray[ i ] = pBillboards[ i ];
			delete [] pBillboards;
		}
		pBillboards = newArray;
		pBillboardSize = newSize;
	}
	
	pBillboards[ pBillboardCount ] = billboard;
	pBillboardCount++;
}

void deoglCollideList::RemoveAllBillboards(){
	pBillboardCount = 0;
}

void deoglCollideList::AddBillboardsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *colVol ){
	if( ! colVol ){
		DETHROW( deeInvalidParam );
	}
	deoglCLVisitorElements visitor( this, colVol );
	
	visitor.SetVisitAll( false );
	visitor.SetVisitBillboards( true );
	
	octree.VisitNodesCollidingVolume( &visitor, colVol );
}

void deoglCollideList::RemoveCulledBillboards(){
	int i, last = 0;
	for( i=0; i<pBillboardCount; i++ ){
		//if( pBillboards[ i ]->GetCulled() ){
		if( ! pBillboards[ i ]->GetVisible() ){
			//pBillboards[ i ]->Clear();
			continue;
		}
		
		if( i != last ){
			pBillboards[ last ] = pBillboards[ i ];
		}
		last++;
	}
	pBillboardCount = last;
}



// Height Terrain Sectors
///////////////////////////

deoglCollideListHTSector *deoglCollideList::GetHTSectorAt( int index ) const{
	if( index < 0 || index >= pHTSectorCount ) DETHROW( deeInvalidParam );
	
	return pHTSectors[ index ];
}

deoglCollideListHTSector *deoglCollideList::AddHTSector( deoglHTViewSector *sector ){
	if( ! sector ){
		DETHROW( deeInvalidParam );
	}
	
	if( pHTSectorCount == pHTSectorSize ){
		const int newSize = pHTSectorCount * 3 / 2 + 1;
		deoglCollideListHTSector ** const newArray = new deoglCollideListHTSector*[ newSize ];
		if( newSize > pHTSectorSize ){
			memset( newArray + pHTSectorSize, 0,
				sizeof( deoglCollideListHTSector* ) * ( newSize - pHTSectorSize ) );
		}
		if( pHTSectors ){
			memcpy( newArray, pHTSectors, sizeof( deoglCollideListHTSector* ) * pHTSectorSize );
			delete [] pHTSectors;
		}
		pHTSectors = newArray;
		pHTSectorSize = newSize;
	}
	
	if( ! pHTSectors[ pHTSectorCount ] ){
		pHTSectors[ pHTSectorCount ] = new deoglCollideListHTSector;
	}
	pHTSectors[ pHTSectorCount ]->SetSector( sector );
	return pHTSectors[ pHTSectorCount++ ];
}

void deoglCollideList::AddHTSector( deoglHTViewSector *sector, deoglDCollisionVolume *volume ){
	deoglCollideListHTSector &clsector = *AddHTSector( sector );
	bool empty = true;
	
	if( volume ){
		const deoglRHTSector &htsector = sector->GetSector();
		const int count = htsector.GetClusterCount();
		const deoglHTSCluster *cluster = htsector.GetClusters();
		deoglDCollisionBox box;
		decPoint i;
		
		for( i.y=0; i.y<count; i.y++ ){
			for( i.x=0; i.x<count; i.x++, cluster++ ){
				box.SetCenter( cluster->GetCenter() );
				box.SetHalfSize( cluster->GetHalfExtends() );
				
				if( volume->BoxHitsVolume( &box ) ){
					clsector.AddCluster( i );
					empty = false;
				}
			}
		}
	}
	
	if( empty ){
		clsector.Clear();
		pHTSectorCount--;
	}
}

void deoglCollideList::RemoveAllHTSectors(){
	while( pHTSectorCount > 0 ){
		pHTSectorCount--;
		pHTSectors[ pHTSectorCount ]->Clear();
	}
}

void deoglCollideList::AddHTSectorsColliding( deoglHTView *htview, deoglDCollisionVolume *volume ){
	if( ! htview || ! volume ){
		DETHROW( deeInvalidParam );
	}
	
	//htview->DetermineVisibilityUsing( volume );
	
	const int count = htview->GetSectorCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglHTViewSector * const sector = htview->GetSectorAt( i );
		
		// TODO test if the sector is visible using a bounding box test
		
		AddHTSector( sector, volume );
	}
}



// Height Terrain Sector Clusters
///////////////////////////////////

deoglCollideListHTSCluster *deoglCollideList::GetHTSClusterAt( int index ) const{
	if( index < 0 || index >= pHTSClusterCount ) DETHROW( deeInvalidParam );
	return pHTSClusters[ index ];
}

deoglCollideListHTSCluster *deoglCollideList::AddHTSCluster( deoglHTViewSectorCluster *cluster ){
	DEASSERT_NOTNULL( cluster )
	
	if( pHTSClusterCount == pHTSClusterSize ){
		const int newSize = pHTSClusterCount * 3 / 2 + 1;
		deoglCollideListHTSCluster ** const newArray = new deoglCollideListHTSCluster*[ newSize ];
		if( newSize > pHTSClusterSize ){
			memset( newArray + pHTSClusterSize, 0, sizeof( deoglCollideListHTSCluster* ) * ( newSize - pHTSClusterSize ) );
		}
		if( pHTSClusters ){
			memcpy( newArray, pHTSClusters, sizeof( deoglCollideListHTSCluster* ) * pHTSClusterSize );
			delete [] pHTSClusters;
		}
		pHTSClusters = newArray;
		pHTSClusterSize = newSize;
	}
	
	if( ! pHTSClusters[ pHTSClusterCount ] ){
		pHTSClusters[ pHTSClusterCount ] = new deoglCollideListHTSCluster;
	}
	pHTSClusters[ pHTSClusterCount ]->SetCluster( cluster );
	return pHTSClusters[ pHTSClusterCount++ ];
}

void deoglCollideList::RemoveAllHTSClusters(){
	while( pHTSClusterCount > 0 ){
		pHTSClusterCount--;
		pHTSClusters[ pHTSClusterCount ]->Clear();
	}
}



// Prop Fields
////////////////

deoglCollideListPropField *deoglCollideList::GetPropFieldAt( int index ) const{
	if( index < 0 || index >= pPropFieldCount ){
		DETHROW( deeInvalidParam );
	}
	return pPropFields[ index ];
}

deoglCollideListPropField *deoglCollideList::AddPropField( deoglRPropField *propField ){
	if( ! propField ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPropFieldCount == pPropFieldSize ){
		int newSize = pPropFieldCount * 3 / 2 + 1;
		deoglCollideListPropField **newArray = new deoglCollideListPropField*[ newSize ];
		if( newSize > pPropFieldSize ){
			memset( newArray + pPropFieldSize, '\0',
				sizeof( deoglCollideListPropField* ) * ( newSize - pPropFieldSize ) );
		}
		if( pPropFields ){
			memcpy( newArray, pPropFields, sizeof( deoglCollideListPropField* ) * pPropFieldSize );
			delete [] pPropFields;
		}
		pPropFields = newArray;
		pPropFieldSize = newSize;
	}
	
	if( ! pPropFields[ pPropFieldCount ] ){
		pPropFields[ pPropFieldCount ] = new deoglCollideListPropField;
	}
	pPropFields[ pPropFieldCount ]->SetPropField( propField );
	return pPropFields[ pPropFieldCount++ ];
}

void deoglCollideList::AddPropField( deoglRPropField *propField, deoglDCollisionVolume &volume ){
	const decDVector &pfpos = propField->GetPosition();
	const int typeCount = propField->GetTypeCount();
	deoglDCollisionBox box;
	int i, j;
	
	deoglCollideListPropField *clpropfield = NULL;
	
	for( i=0; i<typeCount; i++ ){
		deoglRPropFieldType &type = propField->GetTypeAt( i );
		const int clusterCount = type.GetClusterCount();
		
		deoglCollideListPropFieldType *cltype = NULL;
		
		for( j=0; j<clusterCount; j++ ){
			deoglPropFieldCluster &cluster = *type.GetClusterAt( j );
			
			const decDVector clusterMinExtend( pfpos + cluster.GetMinimumExtend() );
			const decDVector clusterMaxExtend( pfpos + cluster.GetMaximumExtend() );
			box.SetFromExtends( clusterMinExtend, clusterMaxExtend );
			
			if( ! volume.BoxHitsVolume( &box ) ){
				continue;
			}
			
			if( ! clpropfield ){
				clpropfield = AddPropField( propField );
			}
			if( ! cltype ){
				cltype = clpropfield->AddType( &type );
			}
			cltype->AddCluster( &cluster );
		}
	}
}

void deoglCollideList::RemoveAllPropFields(){
	while( pPropFieldCount > 0 ){
		pPropFieldCount--;
		pPropFields[ pPropFieldCount ]->Clear();
	}
}

void deoglCollideList::AddPropFieldsColliding( deoglRWorld &world, deoglDCollisionVolume &volume ){
	const int count = world.GetPropFieldCount();
	deoglDCollisionBox box;
	int i;
	
	for( i=0; i<count; i++ ){
		deoglRPropField &propField = *world.GetPropFieldAt( i );
		const decDVector &minExtend = propField.GetMinimumExtend();
		const decDVector &maxExtend = propField.GetMaximumExtend();
		
		box.SetFromExtends( minExtend, maxExtend );
		if( volume.BoxHitsVolume( &box ) ){
			AddPropField( &propField, volume );
		}
	}
}



// Prop Field Clusters
////////////////////////

deoglCollideListPropFieldCluster *deoglCollideList::GetPropFieldClusterAt( int index ) const{
	if( index < 0 || index >= pPropFieldClusterCount ){
		DETHROW( deeInvalidParam );
	}
	return pPropFieldClusters[ index ];
}

deoglCollideListPropFieldCluster *deoglCollideList::AddPropFieldCluster( deoglPropFieldCluster *cluster ){
	DEASSERT_NOTNULL( cluster )
	
	if( pPropFieldClusterCount == pPropFieldClusterSize ){
		const int newSize = pPropFieldClusterCount * 3 / 2 + 1;
		deoglCollideListPropFieldCluster ** const newArray = new deoglCollideListPropFieldCluster*[ newSize ];
		if( newSize > pPropFieldClusterSize ){
			memset( newArray + pPropFieldClusterSize, '\0',
				sizeof( deoglCollideListPropFieldCluster* ) * ( newSize - pPropFieldClusterSize ) );
		}
		if( pPropFieldClusters ){
			memcpy( newArray, pPropFieldClusters, sizeof( deoglCollideListPropFieldCluster* ) * pPropFieldClusterSize );
			delete [] pPropFieldClusters;
		}
		pPropFieldClusters = newArray;
		pPropFieldClusterSize = newSize;
	}
	
	if( ! pPropFieldClusters[ pPropFieldClusterCount ] ){
		pPropFieldClusters[ pPropFieldClusterCount ] = new deoglCollideListPropFieldCluster;
	}
	pPropFieldClusters[ pPropFieldClusterCount ]->SetCluster( cluster );
	return pPropFieldClusters[ pPropFieldClusterCount++ ];
}

void deoglCollideList::RemoveAllPropFieldClusters(){
	while( pPropFieldClusterCount > 0 ){
		pPropFieldClusterCount--;
		pPropFieldClusters[ pPropFieldClusterCount ]->Clear();
	}
}



#if 0
void deoglCollideList::AddParticlesColliding( deoglRWorld *world, deoglDCollisionVolume *volume ){
	if( ! world || ! volume ) DETHROW( deeInvalidParam );
	
	deWorld *engWorld = world->GetWorld();
	int p, psysCount = engWorld->GetParticleEmitterCount();
	deoglParticleEmitter *particleSystem;
	deParticleEmitter *engParticleEmitter;
	deParticleInstance *engInstances;
	deoglDCollisionSphere sphere;
	int i, instanceCount;
	
	for( p=0; p<psysCount; p++ ){
		engParticleEmitter = engWorld->GetParticleEmitterAt( p );
		particleSystem = ( deoglParticleEmitter* )engParticleEmitter->GetPeerGraphic();
		
		if( particleSystem ){
			instanceCount = engParticleEmitter->GetInstanceCount();
			engInstances = engParticleEmitter->GetInstances();
			
			for( i=0; i<instanceCount; i++ ){
				sphere.SetCenter( engParticleEmitter->GetEmitterPosition() + decDVector( engInstances[ i ].position ) );
				sphere.SetRadius( ( double )engInstances[ i ].size );
				
				if( volume->SphereHitsVolume( &sphere ) ){
					AddParticle( particleSystem, i );
				}
			}
		}
	}
}
#endif



// Private functions
//////////////////////

void deoglCollideList::pCleanUp(){
	Clear();
	
	if( pPropFieldClusters ){
		while( pPropFieldClusterSize > 0 ){
			if( pPropFieldClusters[ pPropFieldClusterSize - 1 ] ) delete pPropFieldClusters[ pPropFieldClusterSize - 1 ];
			pPropFieldClusterSize--;
		}
		delete [] pPropFieldClusters;
	}
	
	if( pPropFields ){
		while( pPropFieldSize > 0 ){
			if( pPropFields[ pPropFieldSize - 1 ] ) delete pPropFields[ pPropFieldSize - 1 ];
			pPropFieldSize--;
		}
		delete [] pPropFields;
	}
	
	if( pHTSClusters ){
		while( pHTSClusterSize > 0 ){
			if( pHTSClusters[ pHTSClusterSize - 1 ] ){
				delete pHTSClusters[ pHTSClusterSize - 1 ];
			}
			pHTSClusterSize--;
		}
		delete [] pHTSClusters;
	}
	
	if( pHTSectors ){
		while( pHTSectorSize > 0 ){
			if( pHTSectors[ pHTSectorSize - 1 ] ){
				delete pHTSectors[ pHTSectorSize - 1 ];
			}
			pHTSectorSize--;
		}
		delete [] pHTSectors;
	}
	
	if( pBillboards ){
		delete [] pBillboards;
	}
	
	if( pLights ){
		while( pLightSize > 0 ){
			pLightSize--;
			if( pLights[ pLightSize ] ){
				delete pLights[ pLightSize ];
			}
		}
		delete [] pLights;
	}
	
	if( pComponents ){
		while( pComponentSize > 0 ){
			pComponentSize--;
			if( pComponents[ pComponentSize ] ){
				delete pComponents[ pComponentSize ];
			}
		}
		delete [] pComponents;
	}
	
	if( pTransformVolume ) delete pTransformVolume;
}

void deoglCollideList::pSortCompByDist( int left, int right ){
	deoglCollideListComponent *pivotComp = pComponents[ left ];
	float pivot = pivotComp->GetComponent()->GetSortDistance();
	int r_hold = right;
	int l_hold = left;
	
	while( left < right ){
		while( ( pComponents[ right ]->GetComponent()->GetSortDistance() >= pivot ) && ( left < right ) ){
			right--;
		}
		if( left != right ){
			pComponents[ left ] = pComponents[ right ];
			left++;
		}
		while( ( pComponents[ left ]->GetComponent()->GetSortDistance() <= pivot ) && ( left < right ) ){
			left++;
		}
		if( left != right ){
			pComponents[ right ] = pComponents[ left ];
			right--;
		}
	}
	
	pComponents[ left ] = pivotComp;
	if( l_hold < left ){
		pSortCompByDist( l_hold, left - 1 );
	}
	if( r_hold > left ){
		pSortCompByDist( left + 1, r_hold );
	}
}
