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

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "deoglRParticleEmitterType.h"
#include "deoglRParticleEmitterInstance.h"
#include "deoglRParticleEmitter.h"
#include "deoglRParticleEmitterInstanceType.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../sky/deoglRSkyInstance.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglVBOLayout.h"
#include "../visitors/deoglFindBestEnvMap.h"
#include "../world/deoglRWorld.h"
#include "../world/deoglWorldOctree.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/resources/particle/deParticleEmitterInstance.h>
#include <dragengine/resources/particle/deParticleEmitterInstanceType.h>
#include <dragengine/resources/particle/deParticleEmitterOffsets.h>
#include <dragengine/resources/particle/deParticleEmitterType.h>



// Class deoglRParticleEmitterInstance::WorldComputeElement
/////////////////////////////////////////////////////////////

deoglRParticleEmitterInstance::WorldComputeElement::WorldComputeElement( deoglRParticleEmitterInstance &emitter ) :
deoglWorldComputeElement( eetParticleEmitter, &emitter ),
pEmitter( emitter ){
}

void deoglRParticleEmitterInstance::WorldComputeElement::UpdateData( sDataElement &data ) const{
	const decDVector &refpos = GetReferencePosition();
	data.SetExtends( pEmitter.GetMinExtend() - refpos, pEmitter.GetMaxExtend() - refpos );
	data.SetLayerMask( pEmitter.GetLayerMask() );
	data.flags = ( uint32_t )( deoglWorldCompute::eefParticleEmitter
		| deoglWorldCompute::eefDynamic | deoglWorldCompute::eefGIDynamic );
	data.geometryCount = 0; //1;
	data.highestLod = 0;
}

void deoglRParticleEmitterInstance::WorldComputeElement::UpdateDataGeometries( sDataElementGeometry *data ) const{
}



// Class deoglRParticleEmitterInstance
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRParticleEmitterInstance::deoglRParticleEmitterInstance( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pEmitter( NULL ),
pParentWorld( NULL ),
pOctreeNode( NULL ),
pWorldComputeElement( deoglWorldComputeElement::Ref::New( new WorldComputeElement( *this ) ) ),

pBurstTime( 0.0f ),

pParticles( NULL ),
pParticleCount( 0 ),
pParticleSize( 0 ),

pLocalVBOData( NULL ),
pSharedVBOData( NULL ),

pIndices( NULL ),
pIndexCount( 0 ),
pIndexSize( 0 ),
pIndexUsedCount( 0 ),
pDirtyIBO( false ),

pRenderEnvMap( NULL ),
pDirtyRenderEnvMap( true ),

pVBOShared( 0 ),
pVBOLocal( 0 ),
pIBO( 0 ),
pVAO( NULL ),

pDirtyParticles( true ),

pCSOctreeIndex( 0 ),

pWorldMarkedRemove( false ){
	LEAK_CHECK_CREATE( renderThread, ParticleEmitterInstance );
}

deoglRParticleEmitterInstance::~deoglRParticleEmitterInstance(){
	LEAK_CHECK_FREE( pRenderThread, ParticleEmitterInstance );
	SetParentWorld( NULL );
	
	ReleaseParticles();
	SetEmitter( NULL );
	
	if( pIndices ){
		delete [] pIndices;
	}
	if( pRenderEnvMap ){
		pRenderEnvMap->FreeReference();
	}
	if( pVAO ){
		delete pVAO;
	}
	
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer( pIBO );
	dops.DeleteOpenGLBuffer( pVBOLocal );
	dops.DeleteOpenGLBuffer( pVBOShared );
}



// Management
///////////////

void deoglRParticleEmitterInstance::SetEmitter( deoglRParticleEmitter *emitter ){
	if( emitter == pEmitter ){
		return;
	}
	
	if( pEmitter ){
		pEmitter->FreeReference();
	}
	
	pEmitter = emitter;
	
	if( emitter ){
		emitter->AddReference();
	}
}

void deoglRParticleEmitterInstance::SetParentWorld( deoglRWorld *world ){
	if( world == pParentWorld ){
		return;
	}
	
	if( pRenderEnvMap ){
		pRenderEnvMap->GetParticleEmitterInstanceList().RemoveIfExisting( this );
		pRenderEnvMap->FreeReference();
		pRenderEnvMap = NULL;
	}
	pWorldComputeElement->RemoveFromCompute();
	if( pOctreeNode ){
		pOctreeNode->RemoveParticleEmitter( this );
		pOctreeNode = NULL;
	}
	
	pParentWorld = world;
	
	pDirtyRenderEnvMap = true;
}

void deoglRParticleEmitterInstance::SetOctreeNode( deoglWorldOctree *node ){
	pOctreeNode = node;
}

void deoglRParticleEmitterInstance::UpdateOctreeNode(){
	if( pParentWorld ){
		//if( pParticleEmitter->GetVisible() ){
			pParentWorld->GetOctree().InsertParticleEmitterIntoTree( this );
			
			if( pWorldComputeElement->GetWorldCompute() ){
				pWorldComputeElement->ComputeUpdateElement();
				
			}else{
				pParentWorld->GetCompute().AddElement( pWorldComputeElement );
			}
			
		/*}else{
			if( pWorldComputeElement->GetIndex() != -1 ){
				pParentWorld->GetCompute().RemoveElement( pWorldComputeElement );
			}
			if( pOctreeNode ){
				pOctreeNode->GetParticleEmittersList().Remove( this );
				pOctreeNode = NULL;
			}
		}*/
	}
}



void deoglRParticleEmitterInstance::SetBurstTime( float burstTime ){
	pBurstTime = burstTime;
	InvalidateAllTypesParamBlocks();
}

void deoglRParticleEmitterInstance::SetPosition( const decDVector &position ){
	pPosition = position;
	MarkAllTypesParamBlocksDirty();
}

void deoglRParticleEmitterInstance::SetReferencePosition( const decDVector &position ){
	pReferencePosition = position;
}

void deoglRParticleEmitterInstance::SetLayerMask( const decLayerMask &layerMask ){
	pLayerMask = layerMask;
}



void deoglRParticleEmitterInstance::PrepareForRender(){
	if( pEmitter ){
		pEmitter->PrepareForRender();
	}
	
	UpdateParticlesVBO();
	UpdateRenderEnvMap();
}



void deoglRParticleEmitterInstance::UpdateExtends( const deParticleEmitterInstance &instance ){
	const int typeCount = pTypes.GetCount();
	decVector minExtend, maxExtend;
	int t, p;
	
	for( t=0; t<typeCount; t++ ){
		const deParticleEmitterInstanceType &type = instance.GetTypeAt( t );
		const deParticleEmitterInstanceType::sParticle * const particles = type.GetParticleArray();
		const int particleCount = type.GetParticleCount();
		
		for( p=0; p<particleCount; p++ ){
			const deParticleEmitterInstanceType::sParticle &particle = particles[ p ];
			const decVector ppos( particle.positionX, particle.positionY, particle.positionZ );
			
			minExtend.SetSmallest( ppos );
			maxExtend.SetLargest( ppos );
		}
	}
	
	pMinExtend = pReferencePosition + decDVector( minExtend ) - decDVector( 0.1, 0.1, 0.1 );
	pMaxExtend = pReferencePosition + decDVector( maxExtend ) + decDVector( 0.1, 0.1, 0.1 );
}



void deoglRParticleEmitterInstance::SetRenderEnvMap( deoglEnvironmentMap *envmap ){
	if( envmap == pRenderEnvMap ){
		return;
	}
	
	if( pRenderEnvMap ){
		pRenderEnvMap->GetParticleEmitterInstanceList().RemoveIfExisting( this );
		pRenderEnvMap->FreeReference();
	}
	
	pRenderEnvMap = envmap;
	
	if( envmap ){
		envmap->AddReference();
		envmap->GetParticleEmitterInstanceList().Add( this );
	}
	
	MarkAllTypesTUCsDirty();
}

void deoglRParticleEmitterInstance::WorldEnvMapLayoutChanged(){
	pDirtyRenderEnvMap = true;
}

void deoglRParticleEmitterInstance::UpdateRenderEnvMap(){
	if( ! pDirtyRenderEnvMap ){
		return;
	}
	
	// for the time being we simply pick the environment map that is closest to the component position.
	// this can lead to wrong picks and harshly switching environment maps but this is enough for the
	// first test.
	// 
	// for the time being we choose no region but visit all environment maps existing in the world.
	// a more optimal solution would be to search in an area somewhat larger than the distance to the
	// currently used environment map. the new environment map has to be closer than the current one
	// to be better. if no environment map exists yet a full search could be done.
	// 
	// better solutions could be using grid like connection between environment maps. this way the next
	// best environment map can be searched by following the grid.
	// 
	// for the time being the center of the bounding box is used as the reference point. for large
	// components we've got the problem of what environment map to use. this though has to be solved
	// later on most probably by splitting up the component into smaller components.
	deoglFindBestEnvMap visitor;
	decDVector position;
	
	position = pPosition; // ( GetMinExtend() + GetMaxExtend() ) * 0.5;
	
	visitor.SetPosition( position );
	//pParentWorld->VisitRegion( GetMinimumExtend(), GetMaximumExtend(), visitor );
	visitor.VisitList( pParentWorld->GetEnvMapList() );
	
	if( visitor.GetEnvMap() ){
		SetRenderEnvMap( visitor.GetEnvMap() );
		
	}else if( pParentWorld->GetSkyEnvironmentMap() ){
		SetRenderEnvMap( pParentWorld->GetSkyEnvironmentMap() );
		
	}else{
		SetRenderEnvMap( NULL );
	}
	//pOgl->LogInfoFormat( "update particle emitter instance %p render env map %p\n", pInstance, pRenderEnvMap );
	
	pDirtyRenderEnvMap = false;
}

void deoglRParticleEmitterInstance::InvalidateRenderEnvMap(){
	if( ! pRenderEnvMap ){
		return;
	}
	
	SetRenderEnvMap( NULL );
	pDirtyRenderEnvMap = true;
}

void deoglRParticleEmitterInstance::InvalidateRenderEnvMapIf( deoglEnvironmentMap *envmap ){
	if( pRenderEnvMap == envmap ){
		InvalidateRenderEnvMap();
	}
}



void deoglRParticleEmitterInstance::UpdateParticles( const deParticleEmitterInstance &instance ){
	const int typeCount = pTypes.GetCount();
	int t, p, particleCount = 0;
	
	// determine the total number of particles and indices required
	pIndexCount = 0;
	pIndexUsedCount = 0;
	pDirtyIBO = true;
	
	if( pEmitter ){
		for( t=0; t<typeCount; t++ ){
			deoglRParticleEmitterInstanceType &itype = *( ( deoglRParticleEmitterInstanceType* )pTypes.GetAt( t ) );
			const deoglRParticleEmitterType &etype = pEmitter->GetTypeAt( t );
			const int typeParticleCount = instance.GetTypeAt( t ).GetParticleCount();
			
			itype.SetFirstParticle( particleCount );
			itype.SetParticleCount( typeParticleCount );
			particleCount += typeParticleCount;
			
			itype.SetFirstIndex( pIndexCount );
			
			switch( etype.GetSimulationType() ){
			case deParticleEmitterType::estParticle:
				itype.SetIndexCount( typeParticleCount );
				pIndexCount += typeParticleCount;
				break;
				
			case deParticleEmitterType::estBeam:
			case deParticleEmitterType::estRibbon:
				itype.SetIndexCount( typeParticleCount * 4 );
				pIndexCount += typeParticleCount * 4;
				break;
			}
		}
	}
	
	// ensure a large enough array exists. the data does not have to be conservated.
	if( particleCount > pParticleSize ){
		// enlarge particle array
		if( pParticles ){
			delete [] pParticles;
			pParticles = NULL;
		}
		pParticles = new sParticle[ particleCount ];
		pParticleSize = particleCount;
		
		// enlarge local vbo data array
		if( pLocalVBOData ){
			delete [] pLocalVBOData;
			pLocalVBOData = NULL;
		}
		pLocalVBOData = new sLocalVBOData[ particleCount ];
		
		// enlarge shared vbo data array
		if( pSharedVBOData ){
			delete [] pSharedVBOData;
			pSharedVBOData = NULL;
		}
		pSharedVBOData = new char[ DEPE_SPARTICLE_SIZE * particleCount ];
	}
	pParticleCount = particleCount;
	
	if( pIndexCount > pIndexSize ){
		if( pIndices ){
			delete [] pIndices;
			pIndices = NULL;
		}
		pIndices = new GLushort[ pIndexCount ];
		pIndexSize = pIndexCount;
	}
	
	// only if there is an emitter
	if( pEmitter ){
		// populate particle, local and shared vbo data array with the current particle states
		for( t=0; t<typeCount; t++ ){
			const deoglRParticleEmitterInstanceType &itype =
				*( ( deoglRParticleEmitterInstanceType* )pTypes.GetAt( t ) );
			const deoglRParticleEmitterType &etype = pEmitter->GetTypeAt( t );
			const int firstParticle = itype.GetFirstParticle();
			const int typeParticleCount = itype.GetParticleCount();
			
			// particle state
			for( p=0; p<typeParticleCount; p++ ){
				sParticle &particle = pParticles[ firstParticle + p ];
				
				particle.emitterInstance = this;
				particle.type = t;
				particle.particle = firstParticle + p;
				particle.renderType = ( unsigned char )etype.GetSimulationType();
				
				if( p < typeParticleCount - 1 ){
					particle.ribbonLine[ 2 ] = 1;
					
				}else{
					particle.ribbonLine[ 2 ] = 0;
				}
				if( p > 0 ){
					particle.ribbonLine[ 1 ] = -1;
					
				}else{
					particle.ribbonLine[ 1 ] = 0;
				}
				if( p > 1 ){
					particle.ribbonLine[ 0 ] = -2;
					
				}else{
					particle.ribbonLine[ 0 ] = particle.ribbonLine[ 1 ];
				}
			}
			
			// local vbo data. if not used let it be undefined to not loose time for nothing
			/*
			if( engType.GetSimulationType() == deParticleEmitterType::estBeam ){
				float beamFactor = 1.0f;
				
				if( typeParticleCount > 1 ){
					beamFactor /= ( float )( typeParticleCount - 1 );
				}
				
				for( p=0; p<typeParticleCount; p++ ){
					sLocalVBOData &localVBOData = pLocalVBOData[ firstParticle + p ];
					
					localVBOData.beamLocation = beamFactor * ( float )p;
				}
			}
			*/
			
			// shared vbo data.
			if( typeParticleCount > 0 ){
				memcpy( pSharedVBOData + DEPE_SPARTICLE_SIZE * firstParticle,
					instance.GetTypeAt( t ).GetParticleArray(), DEPE_SPARTICLE_SIZE * typeParticleCount );
			}
		}
	}
	pDirtyParticles = true;
}

void deoglRParticleEmitterInstance::UpdateParticlesVBO(){
	if( ! pDirtyParticles ){
		return;
	}
	
	if( pParticleCount == 0 ){
		pDirtyParticles = false;
		return;
	}
	
	// shared vbo
	const deoglVBOLayout &vboLayoutShared = *pEmitter->GetVBOLayoutShared();
	
	if( ! pVBOShared ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOShared ) );
		if( ! pVBOShared ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOShared ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, DEPE_SPARTICLE_SIZE * pParticleCount, NULL, GL_STREAM_DRAW ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, DEPE_SPARTICLE_SIZE * pParticleCount, pSharedVBOData, GL_STREAM_DRAW ) );
	
	// local vbo
	/*
	const deoglVBOLayout &vboLayoutLocal = *pEmitter->GetVBOLayoutLocal();
	const int vboLocalStride = vboLayoutLocal.GetStride();
	
	if( ! pVBOLocal ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pVBOLocal ) );
		if( ! pVBOLocal ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOLocal ) );
	OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, vboLocalStride * pParticleCount, NULL, GL_STREAM_DRAW ) );
	
	for( t=0; t<pTypeCount; t++ ){
		const deParticleEmitterType &engType = pEmitter->GetParticleEmitter()->GetTypeAt( t );
		
		if( engType.GetSimulationType() == deParticleEmitterType::estBeam ){
			OGL_CHECK( pRenderThread, pglBufferSubData( GL_ARRAY_BUFFER, vboLocalStride * pTypes[ t ].GetFirstParticle(),
				vboLocalStride * pTypes[ t ].GetParticleCount(), pLocalVBOData ) );
		}
	}
	*/
	
	// ibo
	if( ! pIBO ){
		OGL_CHECK( pRenderThread, pglGenBuffers( 1, &pIBO ) );
		if( ! pIBO ){
			DETHROW( deeOutOfMemory );
		}
	}
	
	// vao
	if( ! pVAO ){
		pVAO = new deoglVAO( pRenderThread );
		pVAO->SetIndexType( vboLayoutShared.GetIndexType() );
		OGL_CHECK( pRenderThread, pglBindVertexArray( pVAO->GetVAO() ) );
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOShared ) );
		vboLayoutShared.SetVAOAttributeAt( pRenderThread, 0, 0 );
		vboLayoutShared.SetVAOAttributeAt( pRenderThread, 1, 1 );
		vboLayoutShared.SetVAOAttributeAt( pRenderThread, 2, 2 );
		vboLayoutShared.SetVAOAttributeAt( pRenderThread, 3, 3 );
		
		/*
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pVBOLocal ) );
		vboLayoutLocal.SetVAOAttributeAt( *pOgl, 0, 4 );
		*/
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ELEMENT_ARRAY_BUFFER, pIBO ) );
		
		OGL_CHECK( pRenderThread, pglBindVertexArray( 0 ) );
		
		pVAO->EnsureRTSVAO();
	}
	
	OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, 0 ) );
	
	pDirtyParticles = false;
}

void deoglRParticleEmitterInstance::ReleaseParticles(){
	pIndexCount = 0;
	pIndexUsedCount = 0;
	pDirtyIBO = true;
	
	if( pSharedVBOData ){
		delete [] pSharedVBOData;
		pSharedVBOData = NULL;
	}
	
	if( pLocalVBOData ){
		delete [] pLocalVBOData;
		pLocalVBOData = NULL;
	}
	
	if( pParticles ){
		delete [] pParticles;
		pParticles = NULL;
		pParticleCount = 0;
		pParticleSize = 0;
	}
}

decVector deoglRParticleEmitterInstance::GetParticlePositionAt( int index ) const{
	if( index < 0 || index >= pParticleCount ){
		DETHROW( deeInvalidParam );
	}
	
	const deParticleEmitterInstanceType::sParticle * const data = ( deParticleEmitterInstanceType::sParticle* )pSharedVBOData;
	return decVector( data[ index ].positionX, data[ index ].positionY, data[ index ].positionZ );
}



void deoglRParticleEmitterInstance::WorldReferencePointChanged(){
	MarkAllTypesParamBlocksDirty();
}



void deoglRParticleEmitterInstance::PrepareQuickDispose(){
	pParentWorld = NULL;
	pOctreeNode = NULL;
}



// Types
//////////

int deoglRParticleEmitterInstance::GetTypeCount() const{
	return pTypes.GetCount();
}

deoglRParticleEmitterInstanceType &deoglRParticleEmitterInstance::GetTypeAt( int index ) const{
	return *( ( deoglRParticleEmitterInstanceType* )pTypes.GetAt( index ) );
}

void deoglRParticleEmitterInstance::RemoveAllTypes(){
	pTypes.RemoveAll();
}

void deoglRParticleEmitterInstance::AddType( deoglRParticleEmitterInstanceType *type ){
	pTypes.Add( type );
}

void deoglRParticleEmitterInstance::InvalidateAllTypesParamBlocks(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterInstanceType* )pTypes.GetAt( i ) )->InvalidateParamBlocks();
	}
}

void deoglRParticleEmitterInstance::MarkAllTypesParamBlocksDirty(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterInstanceType* )pTypes.GetAt( i ) )->MarkParamBlocksDirty();
	}
}

void deoglRParticleEmitterInstance::MarkAllTypesTUCsDirty(){
	const int count = pTypes.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglRParticleEmitterInstanceType* )pTypes.GetAt( i ) )->MarkTUCsDirty();
	}
}



// Index Buffer
/////////////////

void deoglRParticleEmitterInstance::ClearIBO(){
	pIndexUsedCount = 0;
	pDirtyIBO = true;
}

void deoglRParticleEmitterInstance::AddIBOEntry( int index ){
	if( pIndexUsedCount == pIndexCount ){
		DETHROW( deeInvalidParam );
	}
	
	pIndices[ pIndexUsedCount++ ] = ( GLushort )index;
	
	pDirtyIBO = true;
}

void deoglRParticleEmitterInstance::AddIBOEntries( int index1, int index2, int index3, int index4 ){
	if( pIndexUsedCount + 4 >= pIndexCount ){
		DETHROW( deeInvalidParam );
	}
	
	pIndices[ pIndexUsedCount++ ] = ( GLushort )index1;
	pIndices[ pIndexUsedCount++ ] = ( GLushort )index2;
	pIndices[ pIndexUsedCount++ ] = ( GLushort )index3;
	pIndices[ pIndexUsedCount++ ] = ( GLushort )index4;
	
	pDirtyIBO = true;
}

void deoglRParticleEmitterInstance::UpdateIBO(){
	if( ! pDirtyIBO ){
		return;
	}
	
	if( pIndexUsedCount > 0 ){
		const deoglVBOLayout &vboLayout = *pEmitter->GetVBOLayoutShared();
		const int size = vboLayout.GetIndexSize() * pIndexUsedCount;
		
		OGL_CHECK( pRenderThread, pglBindBuffer( GL_ARRAY_BUFFER, pIBO ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, size, NULL, GL_STREAM_DRAW ) );
		OGL_CHECK( pRenderThread, pglBufferData( GL_ARRAY_BUFFER, size, pIndices, GL_STREAM_DRAW ) );
	}
	
	pDirtyIBO = false;
}



// Render world usage
///////////////////////

void deoglRParticleEmitterInstance::SetWorldMarkedRemove( bool marked ){
	pWorldMarkedRemove = marked;
}
