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
#include "deoglCollideListPropField.h"
#include "deoglCollideListPropFieldType.h"
#include "deoglCollideListHTSector.h"
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
#include "../utils/octree/deoglDefaultDOctree.h"

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
	
	pPropFields = NULL;
	pPropFieldCount = 0;
	pPropFieldSize = 0;
	
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
	RemoveAllPropFields();
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

void deoglCollideList::MarkElementsVisible( bool visible ){
	MarkComponentsVisible( visible );
	MarkLightsVisible( visible );
	MarkBillboardsVisible( visible );
}

void deoglCollideList::RemoveVisibleElements( bool visible ){
	RemoveVisibleComponents( visible );
	RemoveVisibleLights( visible );
	RemoveVisibleBillboards( visible );
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



// Components
///////////////

deoglCollideListComponent *deoglCollideList::GetComponentAt( int index ) const{
	if( index < 0 || index >= pComponentCount ) DETHROW( deeInvalidParam );
	
	return pComponents[ index ];
}

int deoglCollideList::IndexOfComponent( deoglRComponent *component ) const{
	if( ! component ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		if( component == pComponents[ i ]->GetComponent() ){
			return i;
		}
	}
	
	return -1;
}

bool deoglCollideList::HasComponent( deoglRComponent *component ) const{
	if( ! component ) DETHROW( deeInvalidParam );
	int i;
	
	for( i=0; i<pComponentCount; i++ ){
		if( component == pComponents[ i ]->GetComponent() ){
			return true;
		}
	}
	
	return false;
}

void deoglCollideList::AddComponent( deoglRComponent *component ){
	if( ! component ) DETHROW( deeInvalidParam );
	
	if( pComponentCount == pComponentSize ){
		int i, newSize = pComponentCount * 3 / 2 + 1;
		deoglCollideListComponent **newArray = new deoglCollideListComponent*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
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
		if( ! pComponents[ pComponentCount ] ) DETHROW( deeOutOfMemory );
	}
	pComponents[ pComponentCount ]->SetComponent( component );
	pComponentCount++;
}

void deoglCollideList::RemoveComponent( deoglRComponent *component ){
	RemoveComponentFrom( IndexOfComponent( component ) );
}

void deoglCollideList::RemoveComponentFrom( int index ){
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	deoglCollideListComponent *clcomp = pComponents[ index ];
	int i;
	
	clcomp->Clear();
	
	for( i=index+1; i<pComponentCount; i++ ){
		pComponents[ i - 1 ] = pComponents[ i ];
	}
	pComponentCount--;
	pComponents[ pComponentCount ] = clcomp;
}

void deoglCollideList::RemoveAllComponents(){
	pComponentCount = 0;
}



void deoglCollideList::AddComponentsColliding( deoglWorldOctree &octree, deoglDCollisionVolume *volume ){
	if( ! volume ) DETHROW( deeInvalidParam );
	deoglCLVisitorElements visitor( this, volume );
	
	visitor.SetVisitAll( false );
	visitor.SetVisitComponents( true );
	
	octree.VisitNodesCollidingVolume( &visitor, volume );
}

void deoglCollideList::MarkComponentsVisible( bool visible ){
	int c;
	
	for( c=0; c<pComponentCount; c++ ){
		pComponents[ c ]->GetComponent()->SetRenderVisible( visible );
	}
}

void deoglCollideList::RemoveVisibleComponents( bool visible ){
	deoglCollideListComponent *exchange;
	int c, last = 0;
	
	for( c=0; c<pComponentCount; c++ ){
		if( pComponents[ c ]->GetComponent()->GetRenderVisible() != visible ){
			if( c != last ){
				pComponents[ last ]->Clear();
				exchange = pComponents[ last ];
				pComponents[ last ] = pComponents[ c ];
				pComponents[ c ] = exchange;
			}
			last++;
		}
	}
	pComponentCount = last;
}

void deoglCollideList::RemoveSolidComponents(){
	deoglCollideListComponent *exchange;
	int c, last = 0;
	
	for( c=0; c<pComponentCount; c++ ){
		deoglRSkin * const skin = pComponents[ c ]->GetComponent()->GetSkin();
		if( skin && ! skin->GetIsSolid() ){
			continue;
		}
		if( c != last ){
			pComponents[ last ]->Clear();
			exchange = pComponents[ last ];
			pComponents[ last ] = pComponents[ c ];
			pComponents[ c ] = exchange;
		}
		last++;
	}
	pComponentCount = last;
}

void deoglCollideList::SortComponentsByModels(){
	deoglCollideListComponent *exchange;
	deoglRModel *referenceModel;
	int i = 0, j;
	
	while( i < pComponentCount ){
		referenceModel = pComponents[ i ]->GetComponent()->GetModel();
		i++;
		
		for( j=i; j<pComponentCount; j++ ){
			if( pComponents[ j ]->GetComponent()->GetModel() == referenceModel ){
				if( j > i ){
					exchange = pComponents[ i ];
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

deoglRLight *deoglCollideList::GetLightAt( int index ) const{
	if( index < 0 || index >= pLightCount ) DETHROW( deeInvalidParam );
	return pLights[ index ];
}

void deoglCollideList::AddLight( deoglRLight *light ){
	if( ! light ) DETHROW( deeInvalidParam );
	
	if( pLightCount == pLightSize ){
		int i, newSize = pLightCount * 3 / 2 + 1;
		deoglRLight **newArray = new deoglRLight*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( pLights ){
			for( i=0; i<pLightCount; i++ ) newArray[ i ] = pLights[ i ];
			delete [] pLights;
		}
		pLights = newArray;
		pLightSize = newSize;
	}
	
	pLights[ pLightCount ] = light;
	pLightCount++;
}

void deoglCollideList::RemoveAllLights(){
	pLightCount = 0;
}

void deoglCollideList::AddLightsColliding( deoglWorldOctree *octree, deoglDCollisionVolume *colVol ){
	if( ! octree || ! colVol ) DETHROW( deeInvalidParam );
	deoglCLVisitorElements visitor( this, colVol );
	
	visitor.SetVisitAll( false );
	visitor.SetVisitLights( true );
	
	octree->VisitNodesCollidingVolume( &visitor, colVol );
}

void deoglCollideList::MarkLightsVisible( bool visible ){
	int l;
	
	for( l=0; l<pLightCount; l++ ){
		pLights[ l ]->SetVisible( visible );
	}
}

void deoglCollideList::RemoveVisibleLights( bool visible ){
	int l, last = 0;
	
	for( l=0; l<pLightCount; l++ ){
		if( pLights[ l ]->GetVisible() != visible ){
			if( l != last ){
				pLights[ last ] = pLights[ l ];
			}
			last++;
		}
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

void deoglCollideList::MarkBillboardsVisible( bool visible ){
	int l;
	
	for( l=0; l<pBillboardCount; l++ ){
		pBillboards[ l ]->SetRenderVisible( visible );
	}
}

void deoglCollideList::RemoveVisibleBillboards( bool visible ){
	int l, last = 0;
	
	for( l=0; l<pBillboardCount; l++ ){
		if( pBillboards[ l ]->GetVisible() != visible ){
			if( l != last ){
				pBillboards[ last ] = pBillboards[ l ];
			}
			last++;
		}
	}
	pBillboardCount = last;
}



// Height Terrain Sectors
///////////////////////////

deoglCollideListHTSector *deoglCollideList::GetHTSectorAt( int index ) const{
	if( index < 0 || index >= pHTSectorCount ) DETHROW( deeInvalidParam );
	
	return pHTSectors[ index ];
}

void deoglCollideList::AddHTSector( deoglHTViewSector *sector, deoglDCollisionVolume *volume ){
	if( ! sector ) DETHROW( deeInvalidParam );
	
	deoglCollideListHTSector *clsector;
	bool empty = true;
	
	if( pHTSectorCount == pHTSectorSize ){
		int newSize = pHTSectorCount * 3 / 2 + 1;
		deoglCollideListHTSector **newArray = new deoglCollideListHTSector*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( newSize > pHTSectorSize ){
			memset( newArray + pHTSectorSize, '\0', sizeof( deoglCollideListHTSector* ) * ( newSize - pHTSectorSize ) );
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
		if( ! pHTSectors[ pHTSectorCount ] ) DETHROW( deeOutOfMemory );
	}
	pHTSectors[ pHTSectorCount ]->SetSector( sector );
	clsector = pHTSectors[ pHTSectorCount ];
	pHTSectorCount++;
	
	if( volume ){
		deoglRHTSector &htsector = sector->GetSector();
		int c, clusterCount = htsector.GetClusterCount() * htsector.GetClusterCount();
		deoglHTSCluster * const clusters = htsector.GetClusters();
		deoglDCollisionBox box;
		
		for( c=0; c<clusterCount; c++ ){
			box.SetCenter( clusters[ c ].GetCenter() );
			box.SetHalfSize( clusters[ c ].GetHalfExtends() );
			
			if( volume->BoxHitsVolume( &box ) ){
				clsector->AddCluster( c );
				empty = false;
			}
		}
	}
	
	if( empty ){
		clsector->Clear();
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
	if( ! htview || ! volume ) DETHROW( deeInvalidParam );
	
	//htview->DetermineVisibilityUsing( volume );
	
	int s, sectorCount = htview->GetSectorCount();
	deoglHTViewSector *sector;
	
	for( s=0; s<sectorCount; s++ ){
		sector = htview->GetSectorAt( s );
		
		// TODO test if the sector is visible using a bounding box test
		
		//if( sector->GetVisible() ){
			AddHTSector( sector, volume );
		//}
	}
}



// Prop Fields
////////////////

deoglCollideListPropField *deoglCollideList::GetPropFieldAt( int index ) const{
	if( index < 0 || index >= pPropFieldCount ) DETHROW( deeInvalidParam );
	
	return pPropFields[ index ];
}

void deoglCollideList::AddPropField( deoglRPropField *propField, deoglDCollisionVolume *volume ){
	if( ! propField ) DETHROW( deeInvalidParam );
	
	deoglCollideListPropField *clpf;
	
	if( pPropFieldCount == pPropFieldSize ){
		int newSize = pPropFieldCount * 3 / 2 + 1;
		deoglCollideListPropField **newArray = new deoglCollideListPropField*[ newSize ];
		if( ! newArray ) DETHROW( deeOutOfMemory );
		if( newSize > pPropFieldSize ){
			memset( newArray + pPropFieldSize, '\0', sizeof( deoglCollideListPropField* ) * ( newSize - pPropFieldSize ) );
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
		if( ! pPropFields[ pPropFieldCount ] ) DETHROW( deeOutOfMemory );
	}
	pPropFields[ pPropFieldCount ]->SetPropField( propField );
	clpf = pPropFields[ pPropFieldCount++ ];
	
	if( volume ){
		const decDVector &pfpos = propField->GetPosition();
		decDVector clusterMinExtend, clusterMaxExtend;
		int t, typeCount = clpf->GetTypeCount();
		deoglCollideListPropFieldType *cltype;
		deoglPropFieldCluster *cluster;
		deoglDCollisionBox box;
		int c, clusterCount;
		bool isEmpty = true;
		
		for( t=0; t<typeCount; t++ ){
			cltype = clpf->GetTypeAt( t );
			deoglRPropFieldType &type = propField->GetTypeAt( t );
			clusterCount = type.GetClusterCount();
			
			cltype->RemoveAllClusters();
			
			for( c=0; c<clusterCount; c++ ){
				cluster = type.GetClusterAt( c );
				
				clusterMinExtend = pfpos + cluster->GetMinimumExtend();
				clusterMaxExtend = pfpos + cluster->GetMaximumExtend();
				
				box.SetFromExtends( clusterMinExtend, clusterMaxExtend );
				
				if( volume->BoxHitsVolume( &box ) ){
					cltype->AddCluster( cluster );
					isEmpty = false;
				}
			}
			//propField->GetOpenGL()->LogInfo( "CollideList: propField=%p t=%i(%p;%p) c=%i", propField, t, cltype, type, cltype->GetClusterCount() );
		}
		
		if( isEmpty ){
			clpf->Clear();
			pPropFieldCount--;
		}
	}
}

void deoglCollideList::RemoveAllPropFields(){
	while( pPropFieldCount > 0 ){
		pPropFieldCount--;
		pPropFields[ pPropFieldCount ]->Clear();
	}
}

void deoglCollideList::AddPropFieldsColliding( deoglRWorld &world, deoglDCollisionVolume *volume ){
	if( ! volume ){
		DETHROW( deeInvalidParam );
	}
	
	int p, propFieldCount = world.GetPropFieldCount();
	deoglDCollisionBox box;
	
	for( p=0; p<propFieldCount; p++ ){
		deoglRPropField * const propField = world.GetPropFieldAt( p );
		
		if( propField ){
			const decDVector &minExtend = propField->GetMinimumExtend();
			const decDVector &maxExtend = propField->GetMaximumExtend();
			
			box.SetFromExtends( minExtend, maxExtend );
			if( volume->BoxHitsVolume( &box ) ){
				AddPropField( propField, volume );
			}
		}
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
	
	if( pPropFields ){
		while( pPropFieldSize > 0 ){
			if( pPropFields[ pPropFieldSize - 1 ] ) delete pPropFields[ pPropFieldSize - 1 ];
			pPropFieldSize--;
		}
		delete [] pPropFields;
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
	
	if( pLights ) delete [] pLights;
	
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
