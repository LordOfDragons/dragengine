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
#include <stdlib.h>
#include <string.h>

#include "deoglRPropField.h"
#include "deoglRPropFieldType.h"
#include "deoglPropFieldCluster.h"
#include "deoglPointSieve.h"
#include "deoglPointSieveBucket.h"
#include "deoglPFClusterGenerator.h"
#include "../deGraphicOpenGl.h"
#include "../envmap/deoglEnvironmentMap.h"
#include "../imposter/deoglImposterBillboard.h"
#include "../model/deoglModelLOD.h"
#include "../model/deoglRModel.h"
#include "../model/texture/deoglModelTexture.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../skin/dynamic/deoglRDynamicSkin.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../skin/state/deoglSkinState.h"
#include "../sky/deoglSky.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texunitsconfig/deoglTexUnitConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../texture/texunitsconfig/deoglTexUnitsConfigList.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../world/deoglRWorld.h"
#include "../delayedoperation/deoglDelayedDeletion.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>



// Class deoglRPropFieldType
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRPropFieldType::deoglRPropFieldType( deoglRPropField &propField ) :
pPropField( propField ),

pModel( NULL ),
pSkin( NULL ),

pUseSkinTexture( NULL ),

pBendFactor( 1.0f ),

pParamBlock( NULL ),

pValidParamBlock( false ),
pDirtyParamBlock( true ),

pDirtyModel( true ){
	LEAK_CHECK_CREATE( propField.GetRenderThread(), PropFieldType );
}

class deoglRPropFieldDeletion : public deoglDelayedDeletion{
public:
	deoglSPBlockUBO *paramBlock;
	decPointerList clusters;
	
	deoglRPropFieldDeletion() :
	paramBlock( NULL ){
	}
	
	virtual ~deoglRPropFieldDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		const int clusterCount = clusters.GetCount();
		int i;
		for( i=0; i<clusterCount; i++ ){
			delete ( deoglPropFieldCluster* )clusters.GetAt( i );
		}
		if( paramBlock ){
			paramBlock->FreeReference();
		}
	}
};

class deoglRPropFieldRemoveAllClustersDeletion : public deoglDelayedDeletion{
public:
	decPointerList clusters;
	
	deoglRPropFieldRemoveAllClustersDeletion(){
	}
	
	virtual ~deoglRPropFieldRemoveAllClustersDeletion(){
	}
	
	virtual void DeleteObjects( deoglRenderThread &renderThread ){
		const int clusterCount = clusters.GetCount();
		int i;
		for( i=0; i<clusterCount; i++ ){
			delete ( deoglPropFieldCluster* )clusters.GetAt( i );
		}
	}
};

deoglRPropFieldType::~deoglRPropFieldType(){
	LEAK_CHECK_FREE( pPropField.GetRenderThread(), PropFieldType );
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	if( pModel ){
		pModel->FreeReference();
	}
	
	// delayed deletion of opengl containing objects
	deoglRPropFieldDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRPropFieldDeletion;
		delayedDeletion->paramBlock = pParamBlock;
		delayedDeletion->clusters = pClusters;
		pPropField.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pPropField.GetRenderThread().GetLogger().LogException( e );
		//throw; -> otherwise terminate
	}
}



// Management
///////////////

void deoglRPropFieldType::SetModel( deoglRModel *model ){
	if( model == pModel ){
		return;
	}
	
	if( pModel ){
		pModel->FreeReference();
	}
	
	pModel = model;
	
	if( model ){
		model->AddReference();
	}
	
	pDirtyModel = true;
}

void deoglRPropFieldType::SetSkin( deoglRSkin *skin ){
	if( skin == pSkin ){
		return;
	}
	
	if( pSkin ){
		pSkin->FreeReference();
	}
	
	pSkin = skin;
	
	if( skin ){
		skin->AddReference();
	}
	
	pUseSkinTexture = NULL;
	if( skin && skin->GetTextureCount() > 0 ){
		pUseSkinTexture = &skin->GetTextureAt( 0 );
	}
	
	InvalidateParamBlocks();
	MarkTUCsDirty();
	//pComponent->MarkTextureUseSkinDirty();
}



void deoglRPropFieldType::RebuildInstances( const dePropFieldType &type ){
	pBendFactor = 1.0f;
	
	RemoveAllClusters();
	
	if( ! pModel || ! pSkin ){
		return;
	}
	
	const dePropFieldInstance * const engInstances = type.GetInstances();
	const int instanceCount = type.GetInstanceCount();
	int i;
	
	// determine the extend of the instances in this type
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	for( i=0; i<instanceCount; i++ ){
		const decVector &position = engInstances[ i ].GetPosition();
		pMinExtend.SetSmallest( position );
		pMaxExtend.SetLargest( position );
	}
	
	// we only add clusters if there is something
	const float fieldSizeX = pMaxExtend.x - pMinExtend.x;
	const float fieldSizeZ = pMaxExtend.z - pMinExtend.z;
	
	if( fieldSizeX > 0.001f && fieldSizeZ > 0.001f ){
		AddClustersWithSieve( type );
		//AddClustersWithGenerator();
		//printf( "prop field %p, type %p, %i clusters\n", pPropField, this, pClusterCount );
	}
	
	// determine the bending factor
	const decVector &modelMaxExtend = pModel->GetExtends().maximum;
	const float largestValue = decMath::max( modelMaxExtend.x, modelMaxExtend.y, modelMaxExtend.z );
	if( largestValue > 0.001f ){
		pBendFactor = 1.0f / largestValue;
	}
}

void deoglRPropFieldType::AddClustersWithSieve( const dePropFieldType &type ){
	const float propRadius = pModel->GetExtends().maximum.Length();
	const dePropFieldInstance * const engInstances = type.GetInstances();
	const int instanceCount = type.GetInstanceCount();
	int i;
	
	// squeeze instances through a point sieve
	const float minClusterSize = 50.0f; // 25.0f, 50.0f, 100.0f
	
	const float fieldSizeX = pMaxExtend.x - pMinExtend.x;
	const float fieldSizeZ = pMaxExtend.z - pMinExtend.z;
	
	const int clusterCountX = decMath::max( ( int )ceilf( fieldSizeX / minClusterSize ), 10 );
	const int clusterCountZ = decMath::max( ( int )ceilf( fieldSizeZ / minClusterSize ), 10 );
	
	const int clusterCount = clusterCountX * clusterCountZ;
	deoglPointSieve sieve( clusterCountX, clusterCountZ, fieldSizeX, fieldSizeZ );
	const float sieveOffsetX = pMinExtend.x + fieldSizeX * 0.5f;
	const float sieveOffsetZ = pMinExtend.z + fieldSizeZ * 0.5f;
	
	for( i=0; i<instanceCount; i++ ){
		const decVector &position = engInstances[ i ].GetPosition();
		sieve.DropPoint( position.x - sieveOffsetX, position.z - sieveOffsetZ, i );
	}
	
	// add a cluster for each cluster which is not empty
	decVector clusterMinExtend, clusterMaxExtend;
	deoglPropFieldCluster *cluster = NULL;
	int c;
	
	try{
		for( c=0; c<clusterCount; c++ ){
			const deoglPointSieveBucket &bucket = sieve.GetBucketAt( c );
			
			const int indexCount = bucket.GetIndexCount();
			if( indexCount == 0 ){
				continue;
			}
			
			cluster = new deoglPropFieldCluster( *this );
			
			cluster->SetInstanceCount( indexCount );
			deoglPropFieldCluster::sInstance * const pfInstances = cluster->GetInstances();
			
			for( i=0; i< indexCount; i++ ){
				const int index = bucket.GetIndexAt( i );
				
				const decVector &ipos = engInstances[ index ].GetPosition();
				
				const decMatrix mrot( decMatrix::CreateRotation( engInstances[ index ].GetRotation() ) );
				
				const float iscale = engInstances[ index ].GetScaling();
				const float iradius = propRadius * iscale;
				
				const decVector instanceMinExtend( ipos.x - iradius, ipos.y - iradius, ipos.z - iradius );
				const decVector instanceMaxExtend( ipos.x + iradius, ipos.y + iradius, ipos.z + iradius );
				
				if( i == 0 ){
					clusterMinExtend = instanceMinExtend;
					clusterMaxExtend = instanceMaxExtend;
					
				}else{
					clusterMinExtend.SetSmallest( instanceMinExtend );
					clusterMaxExtend.SetLargest( instanceMaxExtend );
				}
				
				deoglPropFieldCluster::sInstance &pfinst = pfInstances[ i ];
				pfinst.instance = index;
				pfinst.rotation[ 0 ] = mrot.a11;
				pfinst.rotation[ 1 ] = mrot.a12;
				pfinst.rotation[ 2 ] = mrot.a13;
				pfinst.rotation[ 3 ] = mrot.a21;
				pfinst.rotation[ 4 ] = mrot.a22;
				pfinst.rotation[ 5 ] = mrot.a23;
				pfinst.rotation[ 6 ] = mrot.a31;
				pfinst.rotation[ 7 ] = mrot.a32;
				pfinst.rotation[ 8 ] = mrot.a33;
				pfinst.position[ 0 ] = ipos.x;
				pfinst.position[ 1 ] = ipos.y;
				pfinst.position[ 2 ] = ipos.z;
				pfinst.scaling = iscale;
				pfinst.bstate = engInstances[ index ].GetBendState();
			}
			
			cluster->SetExtends( clusterMinExtend, clusterMaxExtend );
			
			AddCluster( cluster );
			cluster = NULL;
		}
		
	}catch( const deException & ){
		if( cluster ){
			delete cluster;
		}
		throw;
	}
}

void deoglRPropFieldType::AddClustersWithGenerator( const dePropFieldType &type ){
	const decVector2 errorMargin = decVector2( 0.001f, 0.001f );
	const decVector2 fieldMinExtend = decVector2( pMinExtend.x, pMinExtend.z ) - errorMargin;
	const decVector2 fieldMaxExtend = decVector2( pMaxExtend.x, pMaxExtend.z ) + errorMargin;
	deoglPFClusterGenerator generator( fieldMinExtend, fieldMaxExtend, 250 );
	const dePropFieldInstance * const engInstances = type.GetInstances();
	const int instanceCount = type.GetInstanceCount();
	int i;
	
	for( i=0; i<instanceCount; i++ ){
		const decVector &position = engInstances[ i ].GetPosition();
		
		//printf( "* add entry to tree %i (%g,%g)\n", i, position.x, position.z );
		generator.AddEntryToTree( i, decVector2( position.x, position.z ) );
	}
	
	AddClustersFromGenerator( type, generator );
}

void deoglRPropFieldType::AddClustersFromGenerator( const dePropFieldType &type, const deoglPFClusterGenerator &generator ){
	const deoglPFClusterGenerator * const node1 = generator.GetNode1();
	const deoglPFClusterGenerator * const node2 = generator.GetNode2();
	const int entryCount = generator.GetEntryCount();
	
	if( entryCount > 0 ){
		const deoglPFClusterGenerator::sEntry * const entries = generator.GetEntries();
		const float propRadius = pModel->GetExtends().maximum.Length();
		const dePropFieldInstance * const engInstances = type.GetInstances();
		decVector clusterMinExtend, clusterMaxExtend;
		deoglPropFieldCluster *cluster = NULL;
		int i;
		
		try{
			cluster = new deoglPropFieldCluster( *this );
			cluster->SetInstanceCount( entryCount );
			deoglPropFieldCluster::sInstance * const instances = cluster->GetInstances();
			
			for( i=0; i<entryCount; i++ ){
				const int index = entries[ i ].index;
				const decVector &ipos = engInstances[ index ].GetPosition();
				deoglPropFieldCluster::sInstance &instance = instances[ i ];
				
				const decMatrix mrot( decMatrix::CreateRotation( engInstances[ index ].GetRotation() ) );
				
				const float iscale = engInstances[ index ].GetScaling();
				const float iradius = propRadius * iscale;
				
				const decVector instanceMinExtend( ipos.x - iradius, ipos.y - iradius, ipos.z - iradius );
				const decVector instanceMaxExtend( ipos.x + iradius, ipos.y + iradius, ipos.z + iradius );
				
				if( i == 0 ){
					clusterMinExtend = instanceMinExtend;
					clusterMaxExtend = instanceMaxExtend;
					
				}else{
					clusterMinExtend.SetSmallest( instanceMinExtend );
					clusterMaxExtend.SetLargest( instanceMaxExtend );
				}
				
				instance.instance = index;
				instance.rotation[ 0 ] = mrot.a11;
				instance.rotation[ 1 ] = mrot.a12;
				instance.rotation[ 2 ] = mrot.a13;
				instance.rotation[ 3 ] = mrot.a21;
				instance.rotation[ 4 ] = mrot.a22;
				instance.rotation[ 5 ] = mrot.a23;
				instance.rotation[ 6 ] = mrot.a31;
				instance.rotation[ 7 ] = mrot.a32;
				instance.rotation[ 8 ] = mrot.a33;
				instance.position[ 0 ] = ipos.x;
				instance.position[ 1 ] = ipos.y;
				instance.position[ 2 ] = ipos.z;
				instance.scaling = iscale;
				instance.bstate = engInstances[ index ].GetBendState();
			}
			
			cluster->SetExtends( clusterMinExtend, clusterMaxExtend );
			
			AddCluster( cluster );
			
		}catch( const deException & ){
			if( cluster ){
				delete cluster;
			}
			throw;
		}
	}
	
	if( node1 ){
		AddClustersFromGenerator( type, *node1 );
	}
	if( node2 ){
		AddClustersFromGenerator( type, *node2 );
	}
}



void deoglRPropFieldType::PrepareForRender(){
	if( pDirtyModel ){
		if( pModel ){
			pModel->PrepareImposterBillboard();
		}
		
		pDirtyModel = false;
	}
	
	// update tbos before they are used to avoid hick-ups
// 	const int count = pClusters.GetCount();
// 	int i;
// 	
// 	for( i=0; i<count; i++ ){
// 		( ( deoglPropFieldCluster* )pClusters.GetAt( i ) )->UpdateTBOs();
// 	}
}

void deoglRPropFieldType::UpdateInstances( const decDVector &cameraPosition, const decDMatrix &cameraMatrix ){
//	RebuildInstances();
}



int deoglRPropFieldType::GetClusterCount() const{
	return pClusters.GetCount();
}

deoglPropFieldCluster *deoglRPropFieldType::GetClusterAt( int index ) const{
	return ( deoglPropFieldCluster* )pClusters.GetAt( index );
}

void deoglRPropFieldType::AddCluster( deoglPropFieldCluster *cluster ){
	if( ! cluster ){
		DETHROW( deeInvalidParam );
	}
	pClusters.Add( cluster );
}

void deoglRPropFieldType::RemoveAllClusters(){
	// this is called during synchronization. clusters use though opengl objects a lot so
	// they have to be delayed deleted
	deoglRPropFieldRemoveAllClustersDeletion *delayedDeletion = NULL;
	
	try{
		delayedDeletion = new deoglRPropFieldRemoveAllClustersDeletion;
		delayedDeletion->clusters = pClusters;
		pPropField.GetRenderThread().GetDelayedOperations().AddDeletion( delayedDeletion );
		pClusters.RemoveAll();
		
	}catch( const deException &e ){
		if( delayedDeletion ){
			delete delayedDeletion;
		}
		pPropField.GetRenderThread().GetLogger().LogException( e );
		throw;
	}
	
	/*
	const int count = pClusters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglPropFieldCluster* )pClusters.GetAt( i );
	}
	pClusters.RemoveAll();
	*/
}



void deoglRPropFieldType::PrepareBendStateData( const dePropFieldType &type ){
	const int count = pClusters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglPropFieldCluster* )pClusters.GetAt( i ) )->PrepareBendStateData( type );
	}
}



deoglSPBlockUBO *deoglRPropFieldType::GetParamBlockFor( deoglSkinTexture::eShaderTypes shaderType ){
	if( shaderType < deoglSkinTexture::estPropFieldGeometry || shaderType > deoglSkinTexture::estPropFieldEnvMap ){
		DETHROW( deeInvalidParam );
	}
	
	return GetParamBlock();
}

deoglSPBlockUBO *deoglRPropFieldType::GetParamBlock(){
	if( ! pValidParamBlock ){
		if( pParamBlock ){
			pParamBlock->FreeReference();
			pParamBlock = NULL;
		}
		
		if( pUseSkinTexture ){
			deoglSkinShader &skinShader = *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estPropFieldGeometry );
			
			/*if( deoglSkinShader::USE_SHARED_SPB ){
				pParamBlock = new deoglSPBlockUBO( *pPropField.GetRenderThread()
					.GetBufferObject().GetLayoutSkinInstanceUBO() );
				
			}else{*/
				pParamBlock = skinShader.CreateSPBInstParam();
			//}
		}
		
		pValidParamBlock = true;
		pDirtyParamBlock = true;
	}
	
	if( pDirtyParamBlock ){
		if( pParamBlock ){
			UpdateInstanceParamBlock( *pParamBlock, *pUseSkinTexture->GetShaderFor( deoglSkinTexture::estPropFieldGeometry ) );
		}
		
		pDirtyParamBlock = false;
	}
	
	return pParamBlock;
}

void deoglRPropFieldType::InvalidateParamBlocks(){
	pValidParamBlock = false;
	MarkParamBlocksDirty();
}

void deoglRPropFieldType::MarkParamBlocksDirty(){
	pDirtyParamBlock = true;
}

void deoglRPropFieldType::MarkTUCsDirty(){
	const int count = pClusters.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( deoglPropFieldCluster* )pClusters.GetAt( i ) )->MarkTUCsDirty();
	}
}



void deoglRPropFieldType::UpdateInstanceParamBlock( deoglSPBlockUBO &paramBlock, deoglSkinShader &skinShader ){
	deoglRDynamicSkin *useDynamicSkin = NULL;
	deoglSkinState *useSkinState = NULL;
	
	if( ! pUseSkinTexture ){
		return;
	}
	
	paramBlock.MapBuffer();
	try{
		// prop field matrix
		const decDVector &referencePosition = pPropField.GetParentWorld()->GetReferencePosition();
		const decDVector &pfpos = pPropField.GetPosition();
		decDMatrix matrixModel = decDMatrix::CreateTranslation( pfpos - referencePosition );
		
		//printf( "propfield=%p type=%p pos=(%f,%f,%f)\n", pPropField->GetPropField(), pType, matrixModel.a14, matrixModel.a24, matrixModel.a34 );
		
		// update shader parameter block
		int target;
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixModel );
		if( target != -1 ){
			paramBlock.SetParameterDataMat4x3( target, matrixModel );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixNormal );
		if( target != -1 ){
			paramBlock.SetParameterDataMat3x3( target, decDMatrix() ); // inverse of 0-rotation
		}
		
		// per texture properties
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutMatrixTexCoord );
		if( target != -1 ){
			paramBlock.SetParameterDataMat3x2( target, decTexMatrix() );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutPropFieldParams );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, pBendFactor );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutDoubleSided );
		if( target != -1 ){
			if( pModel ){
				paramBlock.SetParameterDataBool( target, pModel->GetLODAt( 0 ).GetTextureAt( 0 ).GetDoubleSided() );
				
			}else{
				paramBlock.SetParameterDataBool( target, false );
			}
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutEnvMapFade );
		if( target != -1 ){
			paramBlock.SetParameterDataFloat( target, 0.0f );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutVariationSeed );
		if( target != -1 ){
			if( useSkinState ){
				paramBlock.SetParameterDataVec2( target, useSkinState->GetVariationSeed() );
				
			}else{
				paramBlock.SetParameterDataVec2( target, 0.0f, 0.0f );
			}
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBillboardPosTransform );
		if( target != -1 ){
			decVector2 scale = decVector2( 1.0f, 1.0f );
			decVector2 offset;
			
			if( pModel && pModel->GetImposterBillboard() ){
				const deoglImposterBillboard &billboard = *pModel->GetImposterBillboard();
				const decVector2 &minExtend = billboard.GetMinExtend();
				const decVector2 &maxExtend = billboard.GetMaxExtend();
				
				scale.x = ( maxExtend.x - minExtend.x ) * 0.5f;
				scale.y = ( maxExtend.y - minExtend.y ) * 0.5f;
				offset.x = ( maxExtend.x + minExtend.x ) * 0.5f;
				offset.y = ( maxExtend.y + minExtend.y ) * 0.5f;
			}
			
			paramBlock.SetParameterDataVec4( target, scale.x, scale.y, offset.x, offset.y );
		}
		
		target = skinShader.GetInstanceUniformTarget( deoglSkinShader::eiutBillboardParams );
		if( target != -1 ){
			paramBlock.SetParameterDataBVec3( target, false, false, false );
		}
		
		// per texture dynamic texture properties
		skinShader.SetDynTexParamsInInstParamSPB( paramBlock, *pUseSkinTexture, useSkinState, useDynamicSkin );
		
	}catch( const deException & ){
		paramBlock.UnmapBuffer();
		throw;
	}
	paramBlock.UnmapBuffer();
}



void deoglRPropFieldType::WorldReferencePointChanged(){
	pDirtyParamBlock = true;
}
