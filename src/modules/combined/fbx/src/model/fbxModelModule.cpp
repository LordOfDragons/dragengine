/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "fbxModelModule.h"
#include "../shared/fbxConnection.h"
#include "../shared/fbxNode.h"
#include "../shared/fbxProperty.h"
#include "../shared/fbxScene.h"
#include "../shared/managed/fbxRig.h"
#include "../shared/managed/fbxRigBone.h"
#include "../shared/managed/fbxModel.h"
#include "../shared/managed/fbxModelCluster.h"
#include "../shared/property/fbxPropertyArrayDouble.h"
#include "../shared/property/fbxPropertyArrayFloat.h"
#include "../shared/property/fbxPropertyString.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decPointerList.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/common/file/decMemoryFile.h>
#include <dragengine/common/file/decMemoryFileReader.h>
#include <dragengine/common/file/decMemoryFileWriter.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/filesystem/deCacheHelper.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelBone.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *FBXModelCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif

deBaseModule *FBXModelCreateModule( deLoadableModule *loadableModule ){
	try{
		return new fbxModelModule( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
}



// Class fbxModelModule
/////////////////////////

// Constructor, destructor
////////////////////////////

fbxModelModule::fbxModelModule( deLoadableModule &loadableModule ) :
deBaseModelModule( loadableModule ),
pCacheTCSorter( NULL )
{
	//pCacheTCSorter = new deCacheHelper( &GetVFS(), decPath::CreatePathUnix( "/cache/local/tcsorter" ) );
	
	//
	// TODO
	// 
	// - face normal and tangent indices. right now the same indices as for vertices are used.
	//   fbx stored layer normals and layer tangents. in IndexToDirect mode the index can be
	//   directly stored for normals and tangents. in Direct mode we have to figure out the
	//   indices ourself most probably grouping by (vertex.position, face.normal). just grouping
	//   by normals is not going to work since different vertices can have similar normal.
	//   so both vertex position and normal have to be unique to assign a new index to it.
}

fbxModelModule::~fbxModelModule(){
	if( pCacheTCSorter ){
		delete pCacheTCSorter;
	}
}



// Loading and Saving
///////////////////////

void fbxModelModule::LoadModel( decBaseFileReader &reader, deModel &model ){
	try{
		fbxScene scene( reader );
		scene.Prepare( *this );
		//scene.DebugPrintStructure( *this, true );
		
		pLoadModel( model, scene );
		
	}catch( const deException & ){
		LogErrorFormat( "Failed reading file '%s' at file position %d",
			reader.GetFilename(), reader.GetPosition() );
		throw;
	}
}

void fbxModelModule::SaveModel(decBaseFileWriter &writer, const deModel &model ){
	// nothing yet
}



// Private Functions
//////////////////////

void fbxModelModule::pLoadModel( deModel &model, fbxScene &scene ){
	fbxNode &nodeGeometry = *scene.FirstNodeNamed( "Geometry" );
	fbxNode * const nodePose = scene.FirstNodeNamedOrNull( "Pose" );
	
	deObjectReference refLoadModel;
	refLoadModel.TakeOver( new fbxModel( scene, nodeGeometry ) );
	fbxModel &loadModel = ( fbxModel& )( deObject& )refLoadModel;
	
	deObjectReference refLoadRig;
	fbxRig *loadRig = NULL;
	if( nodePose ){
		refLoadRig.TakeOver( new fbxRig( scene, nodePose ) );
		loadRig = ( fbxRig* )( deObject* )refLoadRig;
		//loadRig->DebugPrintStructure( *this, "LoadModel ", true );
		loadModel.MatchClusters( *loadRig );
		loadModel.BuildWeights();
	}
	
	// loadModel.DebugPrintStructure( *this, "LoadSkin ", true );
	
	// load textures
	pLoadModelTextures( model, loadModel );
	model.GetTextureCoordinatesSetList().Add( "default" );
	
	// load bones
	if( loadRig ){
		pLoadModelBones( model, *loadRig );
	}
	
	// create and add lod
	deModelLOD *modelLod = NULL;
	
	try{
		modelLod = new deModelLOD;
		pLoadModelLod( model, *modelLod, loadModel, loadRig );
		model.AddLOD( modelLod );
		
	}catch( const deException & ){
		if( modelLod ){
			delete modelLod;
		}
		throw;
	}
}

void fbxModelModule::pLoadModelBones( deModel &model, const fbxRig &rig ){
	const int boneCount = rig.GetBoneCount();
	int i;
	
	for( i=0; i<boneCount; i++ ){
		pLoadModelBone( model, *rig.GetBoneAt( i ) );
	}
}

void fbxModelModule::pLoadModelBone( deModel &model, const fbxRigBone &rigBone ){
	deModelBone *bone = NULL;
	try{
		bone = new deModelBone( rigBone.GetName() );
		bone->SetPosition( rigBone.GetPosition() );
		bone->SetOrientation( rigBone.GetOrientation() );
		if( rigBone.GetParent() ){
			bone->SetParent( rigBone.GetParent()->GetIndex() );
		}
		model.AddBone( bone );
		
	}catch( const deException & ){
		if( bone ){
			delete bone;
		}
		throw;
	}
}

void fbxModelModule::pLoadModelTextures( deModel &model, const fbxModel &loadModel ){
	// find connections involving model node
	const int64_t idModel = loadModel.GetModelID();
	decPointerList cons;
	loadModel.GetScene().FindConnections( idModel, cons );
	
	// add material nodes connected to model node
	const int conCount = cons.GetCount();
	int i;
	
	for( i=0; i<conCount; i++ ){
		const fbxConnection &connection = *( ( fbxConnection* )cons.GetAt( i ) );
		if( connection.GetTarget() != idModel ){
			continue;
		}
		
		const fbxNode &node = *loadModel.GetScene().NodeWithID( connection.OtherID( idModel ) );
		if( node.GetName() == "Material" ){
			pLoadModelTexture( model, loadModel, node );
		}
	}
}

void fbxModelModule::pLoadModelTexture( deModel &model, const fbxModel &loadModel, const fbxNode &nodeMaterial ){
	const decString &name = nodeMaterial.GetPropertyAt( 1 )->CastString().GetValue();
	const int width = 1024;
	const int height = 1024;
	
	deModelTexture *texture = NULL;
	try{
		texture = new deModelTexture( name, width, height );
		texture->SetDecalOffset( model.GetTextureCount() );
		texture->SetDoubleSided( ! loadModel.GetCulling());
		model.AddTexture( texture );
		
	}catch( const deException & ){
		if( texture ){
			delete texture;
		}
		throw;
	}
}

void fbxModelModule::pLoadModelLod( deModel &model, deModelLOD &lod,
const fbxModel &loadModel, const fbxRig *loadRig ){
	pLoadModelVertices( model, lod, loadModel, loadRig );
	pLoadModelFaces( model, lod, loadModel, loadRig );
}

void fbxModelModule::pLoadModelVertices( deModel &model, deModelLOD &lod,
const fbxModel &loadModel, const fbxRig *loadRig ){
	// weight sets
	const decIntList &wsWeights = loadModel.GetWeightSetWeights();
	const decIntList &wsFirstWeight = loadModel.GetWeightSetsFirstWeight();
	const decIntList &wgSetCount = loadModel.GetWeightGroupsSetCount();
	const int wgCount = wgSetCount.GetCount();
	int wsNextWeight = 0;
	int wsSetIndex = 0;
	int i, j, k;
	
	lod.SetWeightCount( wsWeights.GetCount() );
	deModelWeight * const weights = lod.GetWeights();
	
	lod.SetWeightGroupCount( wgCount );
	
	for( i=0; i<wgCount; i++ ){
		const int setCount = wgSetCount.GetAt( i );
		const int wgWeightCount = i + 1;
		
		lod.SetWeightGroupAt( i, setCount );
		
		for( j=0; j<setCount; j++ ){
			const int firstWeight = wsFirstWeight.GetAt( wsSetIndex++ );
			for( k=0; k<wgWeightCount; k++ ){
				const deModelWeight &inWeight = loadModel.GetWeightAt( wsWeights.GetAt( firstWeight + k ) );
				deModelWeight &outWeight = weights[ wsNextWeight++ ];
				outWeight.SetBone( inWeight.GetBone() );
				outWeight.SetWeight( inWeight.GetWeight() );
			}
		}
	}
	
	// vertices
	const int count = loadModel.GetVertexCount();
	
	lod.SetVertexCount( count );
	lod.SetNormalCount( count );
	lod.SetTangentCount( count );
	
	deModelVertex * const vertices = lod.GetVertices(); // only valid after SetVertexCount()
	
	for( i=0; i<count; i++ ){
		vertices[ i ].SetWeightSet( loadModel.GetVertexWeightSetAt( i ) );
		vertices[ i ].SetPosition( loadModel.GetVertexPositionAt( i ) );
	}
}

void fbxModelModule::pLoadModelFaces( deModel &model, deModelLOD &lod,
const fbxModel &loadModel, const fbxRig *loadRig ){
	const fbxNode &nodeGeometry = loadModel.GetNodeGeometry();
	const fbxProperty &propPolygonVertexIndex =
		*nodeGeometry.FirstNodeNamed( "PolygonVertexIndex" )->GetPropertyAt( 0 );
	
	const fbxNode * const nodeLayerMaterials = nodeGeometry.FirstNodeNamedOrNull( "LayerElementMaterial" );
	const fbxProperty * const propMaterials = nodeLayerMaterials ?
		nodeLayerMaterials->FirstNodeNamed( "Materials" )->GetPropertyAt( 0 ) : NULL;
	bool allSameMaterial = true;
	if( nodeLayerMaterials ){
		const fbxScene::eMappingInformationType matMit = fbxScene::ConvMappingInformationType( *nodeLayerMaterials );
		switch( matMit ){
		case fbxScene::emitAllSame:
			break;
			
		case fbxScene::emitByPolygon:
			allSameMaterial = false;
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "unsupported mapping information type for LayerElementMaterial" );
		}
	}
	
	const fbxNode * const nodeLayerNormals = nodeGeometry.FirstNodeNamedOrNull( "LayerElementNormal" );
	const fbxProperty * const propNormals = nodeLayerNormals ?
		nodeLayerNormals->FirstNodeNamed( "Normals" )->GetPropertyAt( 0 ) : NULL;
	
	const fbxNode * const nodeLayerTangets = nodeGeometry.FirstNodeNamedOrNull( "LayerElementTangent" );
	const fbxProperty * const propTangents = nodeLayerTangets ?
		nodeLayerTangets->FirstNodeNamed( "Tangents" )->GetPropertyAt( 0 ) : NULL;
	
	const fbxNode * const nodeLayerUVs = nodeGeometry.FirstNodeNamedOrNull( "LayerElementUV" );
	fbxScene::eMappingInformationType uvMit = fbxScene::emitByVertex;
	fbxScene::eReferenceInformationType uvRit = fbxScene::eritDirect;
	const fbxProperty *propUVIndex = NULL;
	const fbxProperty *propUV = NULL;
	
	if( nodeLayerUVs ){
		uvMit = fbxScene::ConvMappingInformationType( *nodeLayerUVs );
		switch( uvMit ){
		case fbxScene::emitByVertex:
		case fbxScene::emitByPolygonVertex:
			break;
			
		default:
			DETHROW_INFO( deeInvalidParam, "unsupported mapping information type on LayerElementUV" );
		}
		
		uvRit = fbxScene::ConvReferenceInformationType( *nodeLayerUVs );
		propUV = nodeLayerUVs->FirstNodeNamed( "UV" )->GetPropertyAt( 0 );
		if( uvRit == fbxScene::eritIndexToDirect ){
			propUVIndex = nodeLayerUVs->FirstNodeNamed( "UVIndex" )->GetPropertyAt( 0 );
		}
	}
	
	const int polyVertIndexCount = propPolygonVertexIndex.GetValueCount();
	int i;
	
	// we have to first determine the number of required triangles. polygons can have any
	// number of vertices. for this reason the last vertex in the loop is negative with
	// all others positive
	int faceCount = 0;
	
	for( i=2; i<polyVertIndexCount; i++ ){
		faceCount++;
		if( propPolygonVertexIndex.GetValueAtAsInt( i ) < 0 ){
			i += 2;
		}
	}
	
	// set up texture coordinates
	lod.SetTextureCoordinatesSetCount( 1 );
	deModelTextureCoordinatesSet &tcs = lod.GetTextureCoordinatesSetAt( 0 );
	
	if( propUV && ( propUVIndex || uvMit == fbxScene::emitByVertex ) ){
		const int count = propUV->GetValueCount() / 2;
		lod.SetTextureCoordinatesCount( count );
		tcs.SetTextureCoordinatesCount( count );
		for( i=0; i<count; i++ ){
			tcs.SetTextureCoordinatesAt( i, fbxScene::ConvUVFbxToDe( propUV->GetValueAtAsVector2( i ) ) );
		}
		
	}else{
		lod.SetTextureCoordinatesCount( faceCount * 3 );
		tcs.SetTextureCoordinatesCount( faceCount * 3 );
	}
	
	// now build the faces
	int faceIndex0 = propPolygonVertexIndex.GetValueAtAsInt( 0 );
	int faceIndexLast = propPolygonVertexIndex.GetValueAtAsInt( 1 );
	int texCoordIndex = 0;
	int polygonIndex = 0;
	int indexFace = 0;
	
	decVector2 faceUV0, faceUVLast, uv;
	int faceUVIndex0 = 0, faceUVIndexLast = 0, uvIndex = 0;
	if( uvMit == fbxScene::emitByPolygonVertex ){
		faceUV0 = fbxScene::ConvUVFbxToDe( propUV->GetValueAtAsVector2( 0 ) );
		faceUVLast = fbxScene::ConvUVFbxToDe( propUV->GetValueAtAsVector2( 1 ) );
	}
	
	lod.SetFaceCount( faceCount );
	
	for( i=2; i<polyVertIndexCount; i++ ){
		int texture = allSameMaterial ? 0 : propMaterials->GetValueAtAsInt( polygonIndex );
		
		// some FBX define no materials or not enough materials which is bad.
		// fix this by adding default textures until the index is valid
		pEnsureTextureIndex( model, texture );
		
		int index = propPolygonVertexIndex.GetValueAtAsInt( i );
		
		if( propUV ){
			if( propUVIndex ){
				uvIndex = propUVIndex->GetValueAtAsInt( i );
				
			}else{
				if( uvMit == fbxScene::emitByPolygonVertex ){
					uv = fbxScene::ConvUVFbxToDe( propUV->GetValueAtAsVector2( i ) );
				}
				
				uvIndex = index;
				if( uvIndex < 0 ){
					uvIndex ^= -1;
				}
			}
		}
		
		bool initNextPolygon = false;
		if( index < 0 ){
			index ^= -1;
			initNextPolygon = i + 2 < polyVertIndexCount;
			polygonIndex++;
		}
		
		// continue polygon. each new vertex forms a new face with base and last vertex.
		// right handed so flip direction
		deModelFace &face = lod.GetFaceAt( indexFace++ );
		
		face.SetVertex1( index );
		face.SetVertex2( faceIndexLast );
		face.SetVertex3( faceIndex0 );
		
		(void)propNormals;
		face.SetNormal1( index );
		face.SetNormal2( faceIndexLast );
		face.SetNormal3( faceIndex0 );
		
		(void)propTangents;
		face.SetTangent1( index );
		face.SetTangent2( faceIndexLast );
		face.SetTangent3( faceIndex0 );
		
		face.SetTexture( texture );
		
		if( propUV ){
			if( uvMit == fbxScene::emitByPolygonVertex && uvRit == fbxScene::eritDirect ){
				tcs.SetTextureCoordinatesAt( texCoordIndex, uv );
				face.SetTextureCoordinates1( texCoordIndex++ );
				tcs.SetTextureCoordinatesAt( texCoordIndex, faceUVLast );
				face.SetTextureCoordinates2( texCoordIndex++ );
				tcs.SetTextureCoordinatesAt( texCoordIndex, faceUV0 );
				face.SetTextureCoordinates3( texCoordIndex++ );
				faceUVLast = uv;
				
			}else{
				face.SetTextureCoordinates1( uvIndex );
				face.SetTextureCoordinates2( faceUVIndexLast );
				face.SetTextureCoordinates3( faceUVIndex0 );
			}
			
		}else{
			face.SetTextureCoordinates1( texCoordIndex++ );
			face.SetTextureCoordinates2( texCoordIndex++ );
			face.SetTextureCoordinates3( texCoordIndex++ );
		}
		
		faceIndexLast = index;
		faceUVIndexLast = uvIndex;
		
		// if this is the end of the polygon initialize the next one if possible
		if( initNextPolygon ){
			faceIndex0 = propPolygonVertexIndex.GetValueAtAsInt( i + 1 );
			faceIndexLast = propPolygonVertexIndex.GetValueAtAsInt( i + 2 );
			
			if( propUV ){
				if( propUVIndex ){
					faceUVIndex0 = propUVIndex->GetValueAtAsInt( i + 1 );
					faceUVIndexLast = propUVIndex->GetValueAtAsInt( i + 2 );
					
				}else{
					if( uvMit == fbxScene::emitByPolygonVertex ){
						faceUV0 = fbxScene::ConvUVFbxToDe( propUV->GetValueAtAsVector2( i + 1 ) );
						faceUVLast = fbxScene::ConvUVFbxToDe( propUV->GetValueAtAsVector2( i + 2 ) );
					}
					
					faceUVIndex0 = faceIndex0;
					faceUVIndexLast = faceIndexLast;
				}
			}
			
			i += 2;
		}
	}
	
	if( indexFace != faceCount ){
		DETHROW( deeInvalidAction );
	}
	
// 	lod.SetNormalCount( count );
// 	lod.SetTangentCount( count );
}

void fbxModelModule::pEnsureTextureIndex( deModel &model, int count ){
	while( count >= model.GetTextureCount() ){
		deModelTexture *texture = NULL;
		decString baseName( "material" );
		decString name( baseName );
		int number = 1;
		
		while( model.HasTextureNamed( name ) ){
			name.Format( "%s%02d", baseName.GetString(), number++ );
		}
		
		try{
			texture = new deModelTexture( name, 1024, 1024 );
			model.AddTexture( texture );
			
		}catch( const deException & ){
			if( texture ){
				delete texture;
			}
			throw;
		}
	}
}

#ifdef WITH_INTERNAL_MODULE
#include <dragengine/systems/modules/deInternalModule.h>

class fbxModelModuleInternal : public deInternalModule{
public:
	fbxModelModuleInternal(deModuleSystem *system) : deInternalModule(system){
		SetName("FBXModel");
		SetDescription("Handles models in the binary FBX format.");
		SetAuthor("Plüss Roland (roland@rptd.ch)");
		SetVersion(MODULE_VERSION);
		SetType(deModuleSystem::emtModel);
		SetDirectoryName("fbxmodel");
		GetPatternList().Add(".fbx");
		SetDefaultExtension(".fbx");
		SetPriority(1);
		SetNoSaving(true);
	}
	
	void CreateModule() override{
		SetModule(FBXModelCreateModule(this));
		if(!GetModule()){
			SetErrorCode(eecCreateModuleFailed);
		}
	}
};

deInternalModule *fbxModelRegisterInternalModule(deModuleSystem *system){
	return new fbxModelModuleInternal(system);
}
#endif
