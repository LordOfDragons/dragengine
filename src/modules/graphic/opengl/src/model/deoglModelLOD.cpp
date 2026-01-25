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

#include "deoglRModel.h"
#include "deoglModelLOD.h"
#include "deoglModelLODTexCoordSet.h"
#include "deoglModelLODVertPosSet.h"
#include "deoglModelLODVisitor.h"
#include "face/deoglModelFace.h"
#include "octree/deoglModelOctree.h"
#include "texture/deoglModelTexture.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../rendering/task/shared/deoglRenderTaskSharedInstance.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTBufferObject.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/paramblock/deoglSPBlockMemory.h"
#include "../skin/shader/deoglSkinShader.h"
#include "../utils/vcoptimizer/deoglVCOptimizer.h"
#include "../vao/deoglVAO.h"
#include "../vbo/deoglSharedVBOBlock.h"
#include "../vbo/deoglSharedVBO.h"
#include "../vbo/deoglSharedVBOList.h"
#include "../vbo/deoglSharedVBOListList.h"
#include "../vbo/deoglVBOAttribute.h"
#include "../vbo/deoglVBOLayout.h"
#include "../vbo/writer/deoglVBOWriterModel.h"
#include "../gi/deoglGIBVHLocal.h"
#include "../utils/bvh/deoglBVH.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/collection/decTList.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelLOD.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/model/deModelTextureCoordinatesSet.h>
#include <dragengine/resources/model/deModelVertexPositionSet.h>
#include <dragengine/resources/model/deModelLodVertexPositionSet.h>
#include <dragengine/resources/model/deModelLodVertexPositionSetPosition.h>
#include <dragengine/resources/model/deModelVertex.h>
#include <dragengine/resources/model/deModelWeight.h>



// Class deoglModelLOD
////////////////////////

// #define DO_TIMING 1

// Constructor, destructor
////////////////////////////

deoglModelLOD::deoglModelLOD(deoglRModel &model, int lodIndex, const deModel &engModel) :
pModel(model),
pLODIndex(lodIndex),
pIBO(0),
pIBOType(deoglVBOLayout::eitUnsignedShort),
pGIBVHLocal(nullptr)
{
	pVBOBlock = nullptr;
	pVBOBlockPositionWeight = nullptr;
	pVBOBlockCalcNormalTangent = nullptr;
	pVBOBlockWriteSkinnedVBO = nullptr;
	pVBOBlockWithWeight = nullptr;
	pVBOBlockVertPosSet = nullptr;
	
	pDoubleSided = false;
	pDecal = false;
	pVertPosSetPosCount = 0;
	
	pOctree = nullptr;
	
	pMaxError = 0.0f;
	pAvgError = 0.0f;
	
	try{
		#ifdef DO_TIMING
		deoglRTLogger &logger = model.GetRenderThread().GetLogger();
		logger.LogInfoFormat("loading %s lod %d\n", pModel.GetFilename().GetString(), pLODIndex);
		decTimer timer;
		#endif
		
		pBuildArrays(engModel);
		#ifdef DO_TIMING
		logger.LogInfoFormat("- build array: %f\n", timer.GetElapsedTime());
		#endif
		
		pCalcTangents();
		#ifdef DO_TIMING
		logger.LogInfoFormat("- build tangents: %f\n", timer.GetElapsedTime());
		#endif
		
		pCalcErrorMetrics(engModel);
		#ifdef DO_TIMING
		logger.LogInfoFormat("- build calc-error-metrics: %f\n", timer.GetElapsedTime());
		#endif
		
		pOptimizeVertexCache();
		#ifdef DO_TIMING
		logger.LogInfoFormat("- build optimize-vertex-cache: %f\n", timer.GetElapsedTime());
		logger.LogInfoFormat("=> max-error: %f\n", pMaxError);
		#endif
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoglModelLOD::deoglModelLOD(deoglRModel &model, int lodIndex, decBaseFileReader &cacheReader) :
pModel(model),
pLODIndex(lodIndex),
pIBO(0),
pIBOType(deoglVBOLayout::eitUnsignedShort),
pGIBVHLocal(nullptr)
{
	pVBOBlock = nullptr;
	pVBOBlockPositionWeight = nullptr;
	pVBOBlockCalcNormalTangent = nullptr;
	pVBOBlockWriteSkinnedVBO = nullptr;
	pVBOBlockWithWeight = nullptr;
	pVBOBlockVertPosSet = nullptr;
	
	pDoubleSided = false;
	pDecal = false;
	pVertPosSetPosCount = 0;
	
	pOctree = nullptr;
	
	pMaxError = 0.0f;
	pAvgError = 0.0f;
	
	try{
		LoadFromCache(cacheReader);
		
	}catch(...){
		pCleanUp();
		throw;
	}
}

deoglModelLOD::~deoglModelLOD(){
	pCleanUp();
}



// Management
///////////////


void deoglModelLOD::PrepareVBOBlock(){
	if(pVBOBlock || pFaces.GetCount() == 0){
		return;
	}
	
	deoglRTBufferObject::eSharedVBOLists listType = deoglRTBufferObject::esvbolStaticModel;
	
	if(pTexCoordSets.GetCount() == 1){
		listType = deoglRTBufferObject::esvbolStaticModelTCS1;
		
	}else if(pTexCoordSets.GetCount() >= 2){ // more than 2 not supported yet
		listType = deoglRTBufferObject::esvbolStaticModelTCS2;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(listType);
	
	if(pVertices.GetCount() > svbolist.GetMaxPointCount()){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared VBOs."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pVertices.GetCount());
	}
	
	pVBOBlock = svbolist.AddData(pVertices.GetCount(), pFaces.GetCount() * 3);
	pVBOBlock->GetVBO()->GetVAO()->EnsureRTSVAO();
	
	pWriteVBOData();
}

#if 0
void deoglModelLOD::PrepareVBOBlockPositionWeight(){
	if(pVBOBlockPositionWeight || pPositionCount == 0){
		return;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
		deoglRTBufferObject::esvbolModelPositionWeightIndices);
	
	if(pPositionCount > svbolist.GetMaxPointCount()){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared position weight VBO."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pPositionCount);
	}
	
	pVBOBlockPositionWeight = svbolist.AddData(pPositionCount);
	
	pWriteVBODataPositionWeight();
}
#endif

#if 0
void deoglModelLOD::PrepareVBOBlockCalcNormalTangent(){
	if(pVBOBlockCalcNormalTangent || pFaces.GetCount() == 0){
		return;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
		deoglRTBufferObject::esvbolModelCalcNormalTangent);
	
	if(pFaces.GetCount() > svbolist.GetMaxPointCount()){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared calc normal tangent VBO."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pFaces.GetCount());
	}
	
	pVBOBlockCalcNormalTangent = svbolist.AddData(pFaces.GetCount());
	
	pWriteVBODataCalcNormalTangent();
}
#endif

#if 0
void deoglModelLOD::PrepareVBOBlockWriteSkinnedVBO(){
	if(pVBOBlockWriteSkinnedVBO || pVertices.GetCount() == 0){
		return;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
		deoglRTBufferObject::esvbolModelWriteSkinnedVBO);
	
	if(pVertices.GetCount() > svbolist.GetMaxPointCount()){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared write skinned vbo VBO."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pVertices.GetCount());
	}
	
	pVBOBlockWriteSkinnedVBO = svbolist.AddData(pVertices.GetCount());
	
	pWriteVBODataWriteSkinnedVBO();
}
#endif

void deoglModelLOD::PrepareVBOBlockWithWeight(){
	if(pVBOBlockWithWeight || pVertices.GetCount() == 0){
		return;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
		deoglRTBufferObject::esvbolStaticModelWeight);
	
	if(pVertices.GetCount() > svbolist.GetMaxPointCount()){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared model with weight VBO."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pVertices.GetCount());
	}
	
	pVBOBlockWithWeight = svbolist.AddData(pVertices.GetCount());
	
	pWriteVBODataWithWeight();
}

void deoglModelLOD::PrepareVBOBlockVertPosSet(){
	if(pVBOBlockVertPosSet || pVertPosSetPosCount == 0){
		return;
	}
	
	deoglRenderThread &renderThread = pModel.GetRenderThread();
	deoglSharedVBOList &svbolist = renderThread.GetBufferObject().GetSharedVBOListForType(
		deoglRTBufferObject::esvbolModelVertexPositionSets);
	
	if(pVertPosSetPosCount > svbolist.GetMaxPointCount()){
		renderThread.GetLogger().LogInfoFormat(
			"Model(%s,%i): Too many points (%i) to fit into shared model with VPS VBO."
			" Using over-sized VBO (performance not optimal).",
			pModel.GetFilename().GetString(), pLODIndex, pVertPosSetPosCount);
	}
	
	pVBOBlockVertPosSet = svbolist.AddData(pVertPosSetPosCount);
	
	pWriteVBOBlockVertPosSet();
}

GLuint deoglModelLOD::GetIBO(){
	if(pIBO){
		return pIBO;
	}
	
	pIBOType = deoglVBOLayout::eitUnsignedInt;
	
	OGL_IF_CHECK(deoglRenderThread &renderThread = pModel.GetRenderThread();)
	GLuint *data = nullptr;
	
	try{
		OGL_CHECK(renderThread, pglGenBuffers(1, &pIBO));
		
		const int bufferSize = (pFaces.GetCount() * 3) * sizeof(GLuint);
		
		OGL_CHECK(renderThread, pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pIBO));
		OGL_CHECK(renderThread, pglBufferData(GL_ELEMENT_ARRAY_BUFFER,
			bufferSize, nullptr, GL_STATIC_DRAW));
		
		OGL_CHECK(renderThread, data = (GLuint*)pglMapBufferRange(GL_ELEMENT_ARRAY_BUFFER,
			0, bufferSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
		if(!data){
			DETHROW(deeInvalidParam);
		}
		
		int i;
		for(i=0; i<pFaces.GetCount(); i++){
			*(data++) = ( GLuint )pFaces[ i ].GetVertex3();
			*(data++) = ( GLuint )pFaces[ i ].GetVertex2();
			*(data++) = ( GLuint )pFaces[ i ].GetVertex1();
		}
		
	}catch(const deException &){
		OGL_CHECK(renderThread, pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
		pModel.GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer(pIBO);
		pIBO = 0;
		throw;
	}
	
	OGL_CHECK(renderThread, pglUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER));
	OGL_CHECK(renderThread, pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	
	return pIBO;
}



deoglModelTexture &deoglModelLOD::GetTextureAt(int index){
	return pTextures.GetAt(index);
}

const deoglModelTexture &deoglModelLOD::GetTextureAt(int index) const{
	return pTextures.GetAt(index);
}

deoglSharedSPBRTIGroupList &deoglModelLOD::GetSharedSPBRTIGroupListAt(int texture) const{
	return pTextures.GetAt(texture)->GetRTIGroups();
}



void deoglModelLOD::PrepareOctree(){
	if(pOctree){
		return;
	}
	
	const deoglRModel::sExtends &extends = pModel.GetExtends();
	const float threshold = 0.001f;
	const int maxDepth = 8;
	
	pOctree = new deoglModelOctree((extends.minimum + extends.maximum) * 0.5f,
		(extends.maximum - extends.minimum) * 0.5f);
	
	pFaces.Visit([&](deoglModelFace &face){
		const decVector &mv1 = pPositions[pVertices[face.GetVertex1()].position].position;
		const decVector &mv2 = pPositions[pVertices[face.GetVertex2()].position].position;
		const decVector &mv3 = pPositions[pVertices[face.GetVertex3()].position].position;
		
		decVector faceMinExtend = mv1.Smallest(mv2).Smallest(mv3);
		decVector faceMaxExtend = mv1.Largest(mv2).Largest(mv3);
		
		// it is possible that the extends get very small. make sure the extends
		// are at least of a small size to avoid missing faces
		const decVector diff(faceMaxExtend - faceMinExtend);
		
		if(diff.x < threshold){
			const float adjust = (threshold - diff.x) * 0.5f;
			faceMinExtend.x -= adjust;
			faceMaxExtend.x += adjust;
		}
		
		if(diff.y < threshold){
			const float adjust = (threshold - diff.y) * 0.5f;
			faceMinExtend.y -= adjust;
			faceMaxExtend.y += adjust;
		}
		
		if(diff.z < threshold){
			const float adjust = (threshold - diff.z) * 0.5f;
			faceMinExtend.z -= adjust;
			faceMaxExtend.z += adjust;
		}
		
		// store extends
		face.SetExtends(faceMinExtend, faceMaxExtend);
		face.SetCenter((faceMinExtend + faceMaxExtend) * 0.5f);
		
		// insert into tree
		pOctree->InsertFaceIntoTree(&face, maxDepth);
	});
}



void deoglModelLOD::pCalcErrorMetrics(const deModel &engModel){
	// TODO this is horribly slow for large models with many vertices due to octree visiting.
	//      needs a better solution
	
	pMaxError = pAvgError = 0.0f;
	if(pLODIndex == 0 || pFaces.GetCount() == 0){
		return;
	}
	
	const deModelLOD &engLod = engModel.GetLODAt(pLODIndex);
	if(engLod.GetHasLodError()){
		pAvgError = pMaxError = engLod.GetLodError();
		return;
	}
	
	const deoglModelLOD &lod0Mesh = pModel.GetLODAt(0);
	const int lod0PositionCount = lod0Mesh.GetPositions().GetCount();
	if(lod0PositionCount >= 5000){
		// TODO temporary hack. if the number of vertices is high calculation time sky-rockets.
		// for a large model with over 67k vertices (130k faces) this consumes over 30s.
		// the good thing on this calculation though is that it is not required to be run.
		// if skipped the result is sub-optimal but not wrong. if not calculated we need
		// to do this later on using gradual optimization. once the result is present the
		// cache file can be re-written with the optimized value.
		pAvgError = pMaxError = 0.025f * pLODIndex;
		return;
	}
	
// 		deoglRTLogger &logger = pModel.GetRenderThread().GetLogger();
// 		decTimer timer;
	PrepareOctree();
// 		logger.LogInfoFormat( "  - prepare octree: %f\n", timer.GetElapsedTime() );
	
	const oglModelPosition * const lod0Positions = lod0Mesh.GetPositions().GetArrayPointer();
	//const decVector boxExtends = decVector( 0.1f, 0.1f, 0.1f );
	const decVector boxExtends = decVector(0.2f, 0.2f, 0.2f);
	const float initLODDistance = boxExtends.Length(); //1.0f;
	deoglModelLODVisitor visitor(*this);
	int i;
	
	for(i=0; i<lod0PositionCount; i++){
		const decVector &lodPoint = lod0Positions[i].position;
		
		visitor.SetBoxExtends(lodPoint - boxExtends, lodPoint + boxExtends);
		visitor.SetLODPoint(lodPoint);
		visitor.SetMinLODDistance(initLODDistance);
		
		pOctree->VisitNodesColliding(&visitor, visitor.GetBoxMinExtend(), visitor.GetBoxMaxExtend());
		
		if(visitor.GetMinLODDistance() > pMaxError){
			pMaxError = visitor.GetMinLODDistance();
		}
		pAvgError += visitor.GetMinLODDistance();
	}
	
	if(lod0PositionCount > 0){
		pAvgError /= (float)lod0PositionCount;
	}
	
	// drop octree. saves CPU memory. will be created and kept if somebody really needs it
	delete pOctree;
	pOctree = nullptr;
	
	// sanity check
	if(pMaxError == initLODDistance){
		pModel.GetRenderThread().GetLogger().LogWarnFormat(
			"Model(%s,%d): Max error matches init LOD distance!", pModel.GetFilename().GetString(), pLODIndex);
	}
}



struct sCacheTexture{
	int firstFace;
	int faceCount;
	bool doubleSided;
	bool decal;
	int decalOffset;
};

void deoglModelLOD::LoadFromCache(decBaseFileReader &reader){
	int i, count;
	
	count = reader.ReadInt();
	if(count > 0){
		sCacheTexture cacheTextures;
		for(i=0; i<count; i++){
			reader.Read(&cacheTextures, sizeof(sCacheTexture));
			deoglModelTexture::Ref texture = deoglModelTexture::Ref::New(pModel.GetRenderThread());
			texture->SetFirstFace(cacheTextures.firstFace);
			texture->SetFaceCount(cacheTextures.faceCount);
			texture->SetDoubleSided(cacheTextures.doubleSided);
			texture->SetDecal(cacheTextures.decal);
			texture->SetDecalOffset(cacheTextures.decalOffset);
			pTextures.Add(std::move(texture));
		}
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pPositions.SetCountDiscard(count);
		reader.Read(pPositions.GetArrayPointer(), sizeof(oglModelPosition) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pTexCoords.SetCountDiscard(count);
		reader.Read(pTexCoords.GetArrayPointer(), sizeof(decVector2) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pNormals.SetCountDiscard(count);
		reader.Read(pNormals.GetArrayPointer(), sizeof(decVector) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pTangents.SetCountDiscard(count);
		pNegateTangents.SetCountDiscard(count);
		reader.Read(pTangents.GetArrayPointer(), sizeof(decVector) * count);
		reader.Read(pNegateTangents.GetArrayPointer(), sizeof(bool) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pWeightsEntries.SetCountDiscard(count);
		reader.Read(pWeightsEntries.GetArrayPointer(), sizeof(oglModelWeight) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pWeightsCounts.SetCountDiscard(count);
		reader.Read(pWeightsCounts.GetArrayPointer(), sizeof(int) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pVertices.SetCountDiscard(count);
		reader.Read(pVertices.GetArrayPointer(), sizeof(oglModelVertex) * count);
	}
	
	count = reader.ReadInt();
	if(count > 0){
		pFaces.SetCountDiscard(count);
		reader.Read(pFaces.GetArrayPointer(), sizeof(deoglModelFace) * count);
	}
	
	count = reader.ReadInt();
	if(count){
		pTexCoordSets.SetCountDiscard(count);
		
		for(i=0; i<count; i++){
			deoglModelLODTexCoordSet &tcs = pTexCoordSets[i];
			
			const int textureCoordinateCount = reader.ReadInt();
			tcs.GetTextureCoordinates().SetCountDiscard(textureCoordinateCount);
			if(textureCoordinateCount > 0){
				reader.Read(tcs.GetTextureCoordinates().GetArrayPointer(),
					sizeof(decVector2) * textureCoordinateCount);
			}
			
			const int tangentCount = reader.ReadInt();
			tcs.GetTangents().SetCountDiscard(tangentCount);
			tcs.GetNegateTangents().SetCountDiscard(tangentCount);
			if(tangentCount > 0){
				reader.Read(tcs.GetTangents().GetArrayPointer(), sizeof(decVector) * tangentCount);
				reader.Read(tcs.GetNegateTangents().GetArrayPointer(), sizeof(bool) * tangentCount);
			}
		}
	}
	
	count = reader.ReadInt();
	if(count){
		pVertPosSets.SetCountDiscard(count);
		
		for(i=0; i<count; i++){
			deoglModelLODVertPosSet &vps = pVertPosSets[i];
			
			vps.SetVBOOffset(reader.ReadInt());
			
			const int positionCount = reader.ReadInt();
			vps.GetPositions().SetCountDiscard(positionCount);
			vps.GetPositions().Visit([&](deoglModelLODVertPosSet::sPosition &position){
				position.index = reader.ReadInt();
				position.position = reader.ReadVector();
			});
		}
	}
	pVertPosSetPosCount = reader.ReadInt();
	
	pDoubleSided = (reader.ReadByte() == 1);
	pDecal = (reader.ReadByte() == 1);
	pMaxError = reader.ReadFloat();
	pAvgError = reader.ReadFloat();
}

void deoglModelLOD::SaveToCache(decBaseFileWriter &writer){
	writer.WriteInt(pTextures.GetCount());
	
	sCacheTexture cacheTextures;
	pTextures.Visit([&](const deoglModelTexture &texture){
		cacheTextures.firstFace = texture.GetFirstFace();
		cacheTextures.faceCount = texture.GetFaceCount();
		cacheTextures.doubleSided = texture.GetDoubleSided();
		cacheTextures.decal = texture.GetDecal();
		cacheTextures.decalOffset = texture.GetDecalOffset();
		writer.Write(&cacheTextures, sizeof(sCacheTexture));
	});
	
	writer.WriteInt(pPositions.GetCount());
	if(pPositions.IsNotEmpty()){
		writer.Write(pPositions.GetArrayPointer(), sizeof(oglModelPosition) * pPositions.GetCount());
	}
	
	writer.WriteInt(pTexCoords.GetCount());
	if(pTexCoords.IsNotEmpty()){
		writer.Write(pTexCoords.GetArrayPointer(), sizeof(decVector2) * pTexCoords.GetCount());
	}
	
	writer.WriteInt(pNormals.GetCount());
	if(pNormals.IsNotEmpty()){
		writer.Write(pNormals.GetArrayPointer(), sizeof(decVector) * pNormals.GetCount());
	}
	
	writer.WriteInt(pTangents.GetCount());
	if(pTangents.IsNotEmpty()){
		writer.Write(pTangents.GetArrayPointer(), sizeof(decVector) * pTangents.GetCount());
		writer.Write(pNegateTangents.GetArrayPointer(), sizeof(bool) * pTangents.GetCount());
	}
	
	writer.WriteInt(pWeightsEntries.GetCount());
	if(pWeightsEntries.IsNotEmpty()){
		writer.Write(pWeightsEntries.GetArrayPointer(), sizeof(oglModelWeight) * pWeightsEntries.GetCount());
	}
	
	writer.WriteInt(pWeightsCounts.GetCount());
	if(pWeightsCounts.IsNotEmpty()){
		writer.Write(pWeightsCounts.GetArrayPointer(), sizeof(int) * pWeightsCounts.GetCount());
	}
	
	writer.WriteInt(pVertices.GetCount());
	if(pVertices.IsNotEmpty()){
		writer.Write(pVertices.GetArrayPointer(), sizeof(oglModelVertex) * pVertices.GetCount());
	}
	
	writer.WriteInt(pFaces.GetCount());
	if(pFaces.IsNotEmpty()){
		writer.Write(pFaces.GetArrayPointer(), sizeof(deoglModelFace) * pFaces.GetCount());
	}
	
	writer.WriteInt(pTexCoordSets.GetCount());
	pTexCoordSets.Visit([&](const deoglModelLODTexCoordSet &tcs){
		const int textureCoordinateCount = tcs.GetTextureCoordinates().GetCount();
		writer.WriteInt(textureCoordinateCount);
		if(textureCoordinateCount > 0){
			writer.Write(tcs.GetTextureCoordinates().GetArrayPointer(),
				sizeof(decVector2) * textureCoordinateCount);
		}
		
		const int tangentCount = tcs.GetTangents().GetCount();
		writer.WriteInt(tangentCount);
		if(tangentCount > 0){
			writer.Write(tcs.GetTangents().GetArrayPointer(), sizeof(decVector) * tangentCount);
			writer.Write(tcs.GetNegateTangents().GetArrayPointer(), sizeof(bool) * tangentCount);
		}
	});
	
	writer.WriteInt(pVertPosSets.GetCount());
	pVertPosSets.Visit([&](const deoglModelLODVertPosSet &vps){
		writer.WriteInt(vps.GetVBOOffset());
		
		writer.WriteInt(vps.GetPositions().GetCount());
		vps.GetPositions().Visit([&](const deoglModelLODVertPosSet::sPosition &position){
			writer.WriteInt(position.index);
			writer.WriteVector(position.position);
		});
	});
	writer.WriteInt(pVertPosSetPosCount);
	
	writer.WriteByte(pDoubleSided ? 1 : 0);
	writer.WriteByte(pDecal ? 1 : 0);
	writer.WriteFloat(pMaxError);
	writer.WriteFloat(pAvgError);
}



void deoglModelLOD::PrepareGILocalBVH(){
	if(pGIBVHLocal){
		return;
	}
	
	decTList<deoglBVH::sBuildPrimitive> primitives;
	int primitiveCount = 0;
	bool disable = false;
	
	if(pFaces.GetCount() > 10000){
		pModel.GetRenderThread().GetLogger().LogWarnFormat(
			"Model(%s,%i): Very high face count (%d). Disable model to not slow down global illumination.",
			pModel.GetFilename().GetString(), pLODIndex, pFaces.GetCount());
		disable = true;
	}
	
	if(pFaces.GetCount() > 0 && !disable){
		primitives.SetCountDiscard(pFaces.GetCount());
		primitiveCount = pFaces.GetCount();
		int i;
		
		for(i=0; i<pFaces.GetCount(); i++){
			deoglBVH::sBuildPrimitive &primitive = primitives[i];
			
			/*
			// this works only if octree has been prepare yet. but the octree does
			// apply some enlaring we do not want to use here
			primitive.minExtend = pFaces[i].GetMinExtend();
			primitive.maxExtend = pFaces[i].GetMaxExtend();
			primitive.center = pFaces[i].GetCenter();
			*/
			
			const deoglModelFace &face = pFaces[i];
			const oglModelPosition &v1 = pPositions[pVertices[face.GetVertex1()].position];
			const oglModelPosition &v2 = pPositions[pVertices[face.GetVertex2()].position];
			const oglModelPosition &v3 = pPositions[pVertices[face.GetVertex3()].position];
			
			primitive.minExtend = v1.position.Smallest(v2.position).Smallest(v3.position);
			primitive.maxExtend = v1.position.Largest(v2.position).Largest(v3.position);
			primitive.center = (primitive.minExtend + primitive.maxExtend) * 0.5f;
		}
	}
	
	try{
		// models have variable face count. the highest lod level typically used for ray tracing
		// should have low face count. if models are not optimized this can result in higher
		// face counts which is bad for performance. in tests a max depth of 12 (more can not
		// be used in shader ray tracing) seems to be the best solution. if low face count can
		// be guaranteed (for example auto-decimation) then lower max depth can be better.
		// occlusion meshes use 6 here
		pGIBVHLocal = new deoglGIBVHLocal(pModel.GetRenderThread());
		pGIBVHLocal->BuildBVH(primitives, primitiveCount, 12);
		
		if(pGIBVHLocal->GetBVH().GetRootNode()){
			pGIBVHLocal->TBOAddVertices(pPositions.GetArrayPointer(), pPositions.GetCount());
			pGIBVHLocal->TBOAddFaces(pFaces.GetArrayPointer(),
				pVertices.GetArrayPointer(), pTexCoords.GetArrayPointer());
			pGIBVHLocal->TBOAddBVH();
		}
		
	}catch(const deException &){
		if(pGIBVHLocal){
			delete pGIBVHLocal;
			pGIBVHLocal = nullptr;
		}
		throw;
	}
	
	// check for suboptimal configurations and warn the developer
	if(pFaces.GetCount() > 300){
		pModel.GetRenderThread().GetLogger().LogInfoFormat(
			"Model(%s,%i): High face count slows down global illumination (%d)."
			" Consider adding highest LOD variation with less than 300 faces.",
			pModel.GetFilename().GetString(), pLODIndex, pFaces.GetCount());
	}
}



// Private functions
//////////////////////

void deoglModelLOD::pCleanUp(){
	if(pGIBVHLocal){
		delete pGIBVHLocal;
	}
	
	if(pVBOBlockWithWeight){
		pVBOBlockWithWeight->DelayedRemove();
	}
	if(pVBOBlockVertPosSet){
		pVBOBlockVertPosSet->DelayedRemove();
	}
	if(pVBOBlockWriteSkinnedVBO){
		pVBOBlockWriteSkinnedVBO->DelayedRemove();
	}
	if(pVBOBlockCalcNormalTangent){
		pVBOBlockCalcNormalTangent->DelayedRemove();
	}
	if(pVBOBlockPositionWeight){
		pVBOBlockPositionWeight->DelayedRemove();
	}
	if(pVBOBlock){
		pVBOBlock->DelayedRemove();
	}
	
	pModel.GetRenderThread().GetDelayedOperations().DeleteOpenGLBuffer(pIBO);
	
	if(pOctree){
		delete pOctree;
	}
}

void deoglModelLOD::pCalcTangents(){
	// reset tangents
	pPositions.Visit([&](oglModelPosition &pos){
		pos.normal.SetZero();
	});
	pNormals.SetRangeAt(0, pNormals.GetCount(), {});
	pTangents.SetRangeAt(0, pTangents.GetCount(), {});
	pNegateTangents.SetRangeAt(0, pNegateTangents.GetCount(), false);
	pTexCoordSets.Visit([&](deoglModelLODTexCoordSet &tcs){
		bool * const tcsNegateTangents = tcs.GetNegateTangents().GetArrayPointer();
		decVector * const tcsTangents = tcs.GetTangents().GetArrayPointer();
		const int tcsTangentCount = tcs.GetTangents().GetCount();
		
		int j;
		for(j=0; j<tcsTangentCount; j++){
			tcsTangents[j].SetZero();
			tcsNegateTangents[j] = false;
		}
	});
	
	// calculate normals and tangents
	pFaces.Visit([&](deoglModelFace &face){
		// get positions
		const oglModelVertex &v1 = pVertices[face.GetVertex1()];
		const oglModelVertex &v2 = pVertices[face.GetVertex2()];
		const oglModelVertex &v3 = pVertices[face.GetVertex3()];
		const decVector &p1 = pPositions[v1.position].position;
		const decVector &p2 = pPositions[v2.position].position;
		const decVector &p3 = pPositions[v3.position].position;
		const decVector2 &tc1 = pTexCoords[v1.texcoord];
		const decVector2 &tc2 = pTexCoords[v2.texcoord];
		const decVector2 &tc3 = pTexCoords[v3.texcoord];
		
		// calculate face normal
		decVector faceNormal((p2 - p1) % (p3 - p1));
		float len = faceNormal.Length();
		if(len != 0.0f){
			faceNormal /= len;
			
		}else{
			faceNormal.Set(0.0f, 1.0f, 0.0f);
		}
		
		// calculate face tangent
		decVector2 d1(tc2 - tc1);
		decVector2 d2(tc3 - tc1);
		
		decVector tangent((p2 - p1) * d2.y - (p3 - p1) * d1.y);
		len = tangent.Length();
		if(len != 0.0f){
			tangent /= len;
			
		}else{
			tangent.Set(1.0f, 0.0f, 0.0f);
		}
		
		decVector bitangent((p3 - p1) * d1.x - (p2 - p1) * d2.x);
		bool negateTangent = (faceNormal % tangent) * bitangent < 0.0f;
		
		// add face normal and tanget to vertices
		pPositions[v1.position].normal += faceNormal;
		pPositions[v2.position].normal += faceNormal;
		pPositions[v3.position].normal += faceNormal;
		pNormals[v1.normal] += faceNormal;
		pNormals[v2.normal] += faceNormal;
		pNormals[v3.normal] += faceNormal;
		pTangents[v1.tangent] += tangent;
		pTangents[v2.tangent] += tangent;
		pTangents[v3.tangent] += tangent;
		pNegateTangents[v1.tangent] = negateTangent;
		pNegateTangents[v2.tangent] = negateTangent;
		pNegateTangents[v3.tangent] = negateTangent;
		
		face.SetFaceNormal(faceNormal);
		
		// process texture coordinate sets if existing
		pTexCoordSets.Visit([&](deoglModelLODTexCoordSet &tcs){
			const decVector2 * const tcsTexCoords = tcs.GetTextureCoordinates().GetArrayPointer();
			bool * const tcsNegateTangents = tcs.GetNegateTangents().GetArrayPointer();
			decVector * const tcsTangents = tcs.GetTangents().GetArrayPointer();
			
			const decVector2 &tcstc1 = tcsTexCoords[v1.texcoord];
			const decVector2 &tcstc2 = tcsTexCoords[v2.texcoord];
			const decVector2 &tcstc3 = tcsTexCoords[v3.texcoord];
			
			// calculate face tangent
			d1 = tcstc2 - tcstc1;
			d2 = tcstc3 - tcstc1;
			
			tangent = (p2 - p1) * d2.y - (p3 - p1) * d1.y;
			len = tangent.Length();
			if(len != 0.0f){
				tangent /= len;
				
			}else{
				tangent.Set(1.0f, 0.0f, 0.0f);
			}
			
			bitangent = (p3 - p1) * d1.x - (p2 - p1) * d2.x;
			negateTangent = ((faceNormal % tangent) * bitangent < 0.0f);
			
			// add face normal and tanget to vertices
			tcsTangents[v1.tangent] += tangent;
			tcsTangents[v2.tangent] += tangent;
			tcsTangents[v3.tangent] += tangent;
			tcsNegateTangents[v1.tangent] = negateTangent;
			tcsNegateTangents[v2.tangent] = negateTangent;
			tcsNegateTangents[v3.tangent] = negateTangent;
		});
	});
	
	// normalize normals and tangents
	pPositions.Visit([&](oglModelPosition &pos){
		float len = pos.normal.Length();
		
		if(len != 0.0f){
			pos.normal /= len;
			
		}else{
			pos.normal.Set(0.0f, 1.0f, 0.0f);
		}
	});
	
	pNormals.Visit([&](decVector &normal){
		float len = normal.Length();
		
		if(len != 0.0f){
			normal /= len;
			
		}else{
			normal.Set(0.0f, 1.0f, 0.0f);
		}
	});
	
	pTangents.Visit([&](decVector &tangent){
		float len = tangent.Length();
		
		if(len != 0.0f){
			tangent /= len;
			
		}else{
			tangent.Set(1.0f, 0.0f, 0.0f);
		}
	});
	
	pTexCoordSets.Visit([&](deoglModelLODTexCoordSet &tcs){
		tcs.GetTangents().Visit([&](decVector &tangent){
			float len = tangent.Length();
			
			if(len != 0.0f){
				tangent /= len;
				
			}else{
				tangent.Set(1.0f, 0.0f, 0.0f);
			}
		});
	});
}

struct sGroupVertex{
	int vertex = 0;
	sGroupVertex *next = nullptr;
};

void deoglModelLOD::pBuildArrays(const deModel &engModel){
	const deModelLOD &engLod = engModel.GetLODAt(pLODIndex);
	const int modelTexCoordSetCount = engLod.GetTextureCoordinatesSets().GetCount();
	const int modelVertPosSetCount = engLod.GetVertexPositionSets().GetCount();
	const int modelTexCoordCount = engLod.GetTextureCoordinatesCount();
	const int modelWeightGroupCount = engLod.GetWeightGroups().GetCount();
	const int modelTangentCount = engLod.GetTangentCount();
	const int modelWeightCount = engLod.GetWeights().GetCount();
	const int modelVertexCount = engLod.GetVertices().GetCount();
	const int modelNormalCount = engLod.GetNormalCount();
	const int modelFaceCount = engLod.GetFaces().GetCount();
	const deModelLodVertexPositionSet * const modelVertPosSets = engLod.GetVertexPositionSets().GetArrayPointer();
	const deModelTextureCoordinatesSet * const modelTexCoordSets = engLod.GetTextureCoordinatesSets().GetArrayPointer();
	const deModelWeight * const modelWeights = engLod.GetWeights().GetArrayPointer();
	const deModelVertex * const modelVertices = engLod.GetVertices().GetArrayPointer();
	const deModelFace * const modelFaces = engLod.GetFaces().GetArrayPointer();
	const int * const modelWeightGroups = engLod.GetWeightGroups().GetArrayPointer();
	int i, j;
	
	// calculate the number of weight sets
	int weightsCount = 0;
	for(i=0; i<modelWeightGroupCount; i++){
		weightsCount += modelWeightGroups[i];
	}
	
	/*
	if(pWeightsCount > 0){
		ogl.LogInfoFormat("Model(%s,%i): %i weight groups, %i weight sets, %i total weights.",
			engModel.GetFilename(), pLODIndex, modelWeightGroupCount, pWeightsCount, modelWeightCount);
	}
	*/
	
	// create weights counts array
	pWeightsCounts.SetCountDiscard(weightsCount);
	if(weightsCount > 0){
		int *walker = pWeightsCounts.GetArrayPointer();
		for(i=0; i<modelWeightGroupCount; i++){
			const int count = modelWeightGroups[i];
			const int index = i + 1;
			
			for(j=0; j<count; j++){
				*(walker++) = index;
			}
		}
	}
	
	// create weights entries array and fill in the weights counts
	pWeightsEntries.SetCountDiscard(modelWeightCount);
	for(i=0; i<modelWeightCount; i++){
		const deModelWeight &in = modelWeights[i];
		oglModelWeight &out = pWeightsEntries[i];
		
		out.bone = in.GetBone();
		out.weight = in.GetWeight();
	}
	
	// create positions array and fill it with vertex positions matching vertex indices in the faces
	pPositions.SetCountDiscard(modelVertexCount);
	for(i=0; i<modelVertexCount; i++){
		const deModelVertex &in = modelVertices[i];
		oglModelPosition &out = pPositions[i];
		
		out.position = in.GetPosition();
		out.weights = in.GetWeightSet();
	}
	
	// create vertices array with the maximal number of possible vertices and set counter to 0.
	// then model vertices are grouped into "render vertices". this is required since model
	// vertices are grouped by position only whereas render vertices are grouped by position,
	// normals, tangent and texture coordinates. this grouping will produce a smaller number of
	// render vertices required. at the end the array will be trimmed down if the memory
	// difference is memory consumption has to be reduced as much as possible
	pVertices.SetCountDiscard(0);
	pVertices.EnlargeCapacity(modelFaceCount * 3);
	
	// add textures
	engModel.GetTextures().Visit([&](const deModelTexture &engTexture){
		deoglModelTexture::Ref texture = deoglModelTexture::Ref::New(pModel.GetRenderThread());
		texture->SetDoubleSided(engTexture.GetDoubleSided());
		texture->SetDecal(engTexture.GetDecal());
		texture->SetDecalOffset(engTexture.GetDecalOffset());
		pTextures.Add(std::move(texture));
		
		pDoubleSided |= engTexture.GetDoubleSided();
		pDecal |= engTexture.GetDecal();
	});
	
	// add texture coordinate sets and set the texture coordinate count
	pTexCoords.SetCountDiscard(modelTexCoordCount);
	for(j=0; j<modelTexCoordCount; j++){
		pTexCoords[j] = modelTexCoordSets[0].GetTextureCoordinates().GetAt(j);
	}
	
	pTexCoordSets.SetCountDiscard(decMath::max(modelTexCoordSetCount - 1, 0));
	for(i=0; i<pTexCoordSets.GetCount(); i++){
		deoglModelLODTexCoordSet &tcs = pTexCoordSets[i];
		
		tcs.GetTextureCoordinates().SetCountDiscard(pTexCoordSets.GetCount());
		for(j=0; j<pTexCoordSets.GetCount(); j++){
			tcs.GetTextureCoordinates()[j] = modelTexCoordSets[i + 1].GetTextureCoordinates()[j];
		}
	}
	
	// add faces sorted by textures and group vertices. this can be very slow with a naive
	// implementation and large models. sortVertices stores the first point an original model
	// vertex / has been involved with. each additional involvement is added to a forward
	// linked list.
	pFaces.SetCountDiscard(modelFaceCount);
	if(modelFaceCount > 0){
		int sortedVertexCount = 0;
		int f = 0;
		
		auto sortedVertices = decTList<sGroupVertex>(modelFaceCount * 3, sGroupVertex{});
		auto sortVertices = decTList<sGroupVertex*>(modelVertexCount, nullptr);
		
		pTextures.VisitIndexed([&](int ti, deoglModelTexture &texture){
			int texFaceCount = 0;
			
			texture.SetFirstFace(f);
			
			for(j=0; j<pFaces.GetCount(); j++){
				const deModelFace &modelFace = modelFaces[j];
				if(modelFace.GetTexture() != ti){
					continue;
				}
				
				/*if( f >= pFaces.GetCount() ){
					DETHROW(deeInvalidParam);
				}*/
				
				deoglModelFace &face = pFaces[f];
				face.SetIndex(f);
				face.SetTexture(ti);
				
				int k;
				for(k=0; k<3; k++){
					const int position = modelFace.GetVertexAt(k);
					const int texCoord = modelFace.GetTextureCoordinatesAt(k);
					const int normal = modelFace.GetNormalAt(k);
					const int tangent = modelFace.GetTangentAt(k);
					
					sGroupVertex *findVertex = sortVertices.GetAt(position);
					sGroupVertex *lastVertex = nullptr;
					while(findVertex){
						const oglModelVertex &vertex = pVertices[findVertex->vertex];
						if(texCoord == vertex.texcoord && normal == vertex.normal
						&& tangent == vertex.tangent){
							break;
						}
						lastVertex = findVertex;
						findVertex = findVertex->next;
					}
					
					if(findVertex){
						face.SetVertexAt(k, findVertex->vertex);
						
					}else{
						pVertices.Add({});
						oglModelVertex &vertex = pVertices.Last();
						vertex.position = position;
						vertex.texcoord = texCoord;
						vertex.normal = normal;
						vertex.tangent = tangent;
						
						sGroupVertex &nextVertex = sortedVertices[sortedVertexCount++];
						nextVertex.vertex = pVertices.GetCount() - 1;
						nextVertex.next = nullptr;
						
						if(lastVertex){
							lastVertex->next = &nextVertex;
							
						}else{
							sortVertices.SetAt(position, &nextVertex);
						}
						
						face.SetVertexAt(k, pVertices.GetCount() - 1);
					}
				}
				
				f++;
				texFaceCount++;
			}
			
			texture.SetFaceCount(texFaceCount);
		});
		
		DEASSERT_TRUE(f == pFaces.GetCount())
	}
	
	// optimize the memory consumption of the vertices array. required only if the difference
	// is large and on systems with low memory like mobile systems.
	pVertices.CompactCapacity();
	
	// create normal array
	pNormals.SetCountDiscard(modelNormalCount);
	
	// create tangent array
	pTangents.SetCountDiscard(modelTangentCount);
	pNegateTangents.SetCountDiscard(modelTangentCount);
	
	pTexCoordSets.Visit([&](deoglModelLODTexCoordSet &tcs){
		tcs.GetTangents().SetCountDiscard(modelTangentCount);
		tcs.GetNegateTangents().SetCountDiscard(modelTangentCount);
	});
	
	// add vertex position sets. since vertices stored for rendering are points grouping various
	// parameters a single vertex position set position typically maps to multiple points
	pVertPosSets.SetCountDiscard(modelVertPosSetCount);
	if(modelVertPosSetCount > 0){
		pVertPosSetPosCount = 0;
		
		for(i=0; i<modelVertPosSetCount; i++){
			const int baseSetIndex = engModel.GetVertexPositionSetAt(i)->GetBaseSet();
			const deModelLodVertexPositionSet &modelVps = modelVertPosSets[i];
			const deModelLodVertexPositionSet::PositionsList &modelVpsPos = modelVps.GetPositions();
			const int positionCount = modelVpsPos.GetCount();
			deoglModelLODVertPosSet &vps = pVertPosSets[i];
			
			// determine the count of points affected by all positions
			int pointCount = 0;
			for(j=0; j<positionCount; j++){
				const int vertex = modelVpsPos.GetAt(j).GetVertex();
				
				pVertices.Visit([&](const oglModelVertex &v){
					if(v.position == vertex){
						pointCount++;
					}
				});
			}
			
			// store positions for all affected points
			vps.SetVBOOffset(pVertPosSetPosCount);
			vps.GetPositions().SetCountDiscard(pointCount);
			
			deoglModelLODVertPosSet::sPosition *vpsPosition = vps.GetPositions().GetArrayPointer();
			const deModelLodVertexPositionSet::PositionsList *modelVpsBasePos = nullptr;
			int basePositionCount = 0;
			
			if(baseSetIndex != -1){
				const deModelLodVertexPositionSet &modelVpsBase = modelVertPosSets[baseSetIndex];
				modelVpsBasePos = &modelVpsBase.GetPositions();
				basePositionCount = modelVpsBasePos->GetCount();
			}
			
			for(j=0; j<positionCount; j++){
				const int vertex = modelVpsPos[j].GetVertex();
				
				// calculate position which is either relative to the base position or relative
				// to another vertex position set position
				decVector position(modelVpsPos[j].GetPosition());
				
				if(modelVpsBasePos){
					int k;
					for(k=0; k<basePositionCount; k++){
						const deModelLodVertexPositionSetPosition &basePos = modelVpsBasePos->GetAt(k);
						if(basePos.GetVertex() == vertex){
							position -= basePos.GetPosition();
							break;
						}
					}
					
					if(k == basePositionCount){
						position -= modelVertices[vertex].GetPosition();
					}
					
				}else{
					position -= modelVertices[vertex].GetPosition();
				}
				
				// add position for all affected points
				pVertices.VisitIndexed([&](int vi, const oglModelVertex &v){
					if(v.position == vertex){
						vpsPosition->index = vi;
						vpsPosition->position = position;
						vpsPosition++;
					}
				});
			}
			
			pVertPosSetPosCount += pointCount;
		}
	}
	
	// debug
#if 0
	pModel->GetOgl()->LogInfoFormat("Model(%s,%i): %i faces, %i points (%.1f%% less).", engModel.GetFilename(),
		pLODIndex, pFaces.GetCount(), pVertices.GetCount(), 100.0f * (1.0f - (float)pVertices.GetCount() / (float)(pFaces.GetCount() * 3)));
#endif
}

void deoglModelLOD::pOptimizeVertexCache(){
	deoglVCOptimizer optimizer;
	
	//decTimer timer;
	//timer.Reset();
	
	// calculate the optimal ordering of faces and vertices
	optimizer.SetVertexCount(pVertices.GetCount());
	
	optimizer.SetFaceCount(pFaces.GetCount());
	pFaces.VisitIndexed([&](int i, const deoglModelFace &f){
		optimizer.SetFaceAt(i, f.GetVertex1(), f.GetVertex2(), f.GetVertex3());
	});
	
	optimizer.SetGroupCount(pTextures.GetCount());
	pTextures.VisitIndexed([&](int i, const deoglModelTexture &t){
		optimizer.SetGroupAt(i, t.GetFirstFace(), t.GetFaceCount());
	});
	
	optimizer.Optimize();
	
	// apply the ordering. for this a new array is created for the vertices and faces and the
	// entries copied in the correct order. for the face in addition the vertex indices are
	// remapped to the new vertex order
	const deoglVCOptimizer::sVertex * const optimizerVertices = optimizer.GetVertices().GetArrayPointer();
	const int * const reorderedVertices = optimizer.GetReorderedVertices().GetArrayPointer();
	const int * const reorderedFaces = optimizer.GetReorderedFaces().GetArrayPointer();
	decTList<oglModelVertex> vertices;
	decTList<deoglModelFace> faces;
	
	vertices.SetCountDiscard(pVertices.GetCount());
	vertices.VisitIndexed([&](int i, oglModelVertex &vertex){
		vertex = pVertices[reorderedVertices[i]];
	});
	
	if(pFaces.IsNotEmpty()){
		faces.SetCountDiscard(pFaces.GetCount());
		faces.VisitIndexed([&](int i, deoglModelFace &face){
			face = pFaces[reorderedFaces[i]];
			face.SetIndex(i);
			face.SetVertex1(optimizerVertices[face.GetVertex1()].reorderedIndex);
			face.SetVertex2(optimizerVertices[face.GetVertex2()].reorderedIndex);
			face.SetVertex3(optimizerVertices[face.GetVertex3()].reorderedIndex);
		});
	}
	
	pVertPosSets.Visit([&](deoglModelLODVertPosSet &vps){
		vps.GetPositions().Visit([&](deoglModelLODVertPosSet::sPosition &position){
			position.index = optimizerVertices[position.index].reorderedIndex;
		});
	});
	
	pFaces = std::move(faces);
	pVertices = std::move(vertices);
	
	//pModel->GetOgl()->LogInfoFormat("model '%s' lod %i optimize in %ims", pModel.GetFilename().GetString(),
	//	pLODIndex, (int)(timer.GetElapsedTime() * 1000.0f));
}

void deoglModelLOD::pWriteVBOData(){
	DEASSERT_NOTNULL(pVBOBlock)
	
	deoglVBOWriterModel writerVBO(pModel.GetRenderThread());
	int tcsCount;
	
	tcsCount = pTexCoordSets.GetCount();
	if(tcsCount > 2){
		tcsCount = 2;
	}
	
	writerVBO.Reset(pVBOBlock);
	
	pVertices.Visit([&](const oglModelVertex &vertex){
		writerVBO.WritePoint(pPositions[vertex.position].position,
			pNormals[vertex.normal], pTangents[vertex.tangent],
			pNegateTangents[vertex.tangent], pTexCoords[vertex.texcoord],
			pPositions[vertex.position].normal);
		
		pTexCoordSets.Visit([&](const deoglModelLODTexCoordSet &tcs){
			writerVBO.WriteTexCoordSetPoint(tcs.GetTangents()[vertex.tangent],
				tcs.GetNegateTangents()[vertex.tangent],
				tcs.GetTextureCoordinates()[vertex.texcoord]);
		});
	});
	
	pFaces.Visit([&](const deoglModelFace &face){
		writerVBO.WriteIndices(face.GetVertex3(), face.GetVertex2(), face.GetVertex1());
	});
}

#if 0
void deoglModelLOD::pWriteVBODataPositionWeight(){
	if(!pVBOBlockPositionWeight){
		DETHROW(deeInvalidParam);
	}
	
	char * const vboData = reinterpret_cast<char*>(pVBOBlockPositionWeight->GetData());
	int i;
	
	for(i=0; i<pPositionCount; i++){
		const int offset = 16 * i;
		GLfloat * const dataPosition = (GLfloat*)(vboData + offset);
		GLint * const dataWeight = (GLint*)(vboData + offset + 12);
		
		dataPosition[0] = (GLfloat)pPositions[i].position.x;
		dataPosition[1] = (GLfloat)pPositions[i].position.y;
		dataPosition[2] = (GLfloat)pPositions[i].position.z;
		*dataWeight = ( GLint )pPositions[ i ].weights;
	}
}
#endif

#if 0
void deoglModelLOD::pWriteVBODataCalcNormalTangent(){
	if(!pVBOBlockCalcNormalTangent){
		DETHROW(deeInvalidParam);
	}
	
	char * const vboData = reinterpret_cast<char*>(pVBOBlockCalcNormalTangent->GetData());
	
	pFaces.VisitIndexed([&](int i, const deoglModelFace &face){
		const int offset = 44 * i;
		GLint * const dataPositionIndex = (GLint*)(vboData + offset);
		GLint * const dataNormalIndex = (GLint*)(vboData + offset + 12);
		GLint * const dataTangentIndex = (GLint*)(vboData + offset + 24);
		GLfloat * const dataTangentFactor = (GLfloat*)(vboData + offset + 36);
		
		const oglModelVertex &v1 = pVertices[face.GetVertex1()];
		const oglModelVertex &v2 = pVertices[face.GetVertex2()];
		const oglModelVertex &v3 = pVertices[face.GetVertex3()];
		
		dataPositionIndex[0] = (GLint)v1.position;
		dataPositionIndex[1] = (GLint)v2.position;
		dataPositionIndex[2] = (GLint)v3.position;
		
		dataNormalIndex[0] = (GLint)v1.normal;
		dataNormalIndex[1] = (GLint)v2.normal;
		dataNormalIndex[2] = (GLint)v3.normal;
		
		dataTangentIndex[0] = (GLint)v1.tangent;
		dataTangentIndex[1] = (GLint)v2.tangent;
		dataTangentIndex[2] = (GLint)v3.tangent;
		
		const decVector2 &tc1 = pTexCoords[v1.texcoord];
		const decVector2 &tc2 = pTexCoords[v2.texcoord];
		const decVector2 &tc3 = pTexCoords[v3.texcoord];
		
		dataTangentFactor[0] = tc3.y - tc1.y;
		dataTangentFactor[1] = tc2.y - tc1.y;
	});
}
#endif

#if 0
void deoglModelLOD::pWriteVBODataWriteSkinnedVBO(){
	if(!pVBOBlockWriteSkinnedVBO){
		DETHROW(deeInvalidParam);
	}
	
	char * const vboData = reinterpret_cast<char*>(pVBOBlockWriteSkinnedVBO->GetData());
	
	pVertices.VisitIndexed([&](int i, const oglModelVertex &vertex){
		char * const vboDataVertex = vboData + (i * 16);
		
		*( ( GLint* )( vboDataVertex + 0 ) ) = ( GLint )vertex.position;
		*( ( GLint* )( vboDataVertex + 4 ) ) = ( GLint )vertex.normal;
		*( ( GLint* )( vboDataVertex + 8 ) ) = ( GLint )vertex.tangent;
		*( ( GLfloat* )( vboDataVertex + 12 ) ) = ( GLfloat )( pNegateTangents[ vertex.tangent ] ? -1.0f : 1.0f );
	});
}
#endif

void deoglModelLOD::pWriteVBODataWithWeight(){
	DEASSERT_NOTNULL(pVBOBlockWithWeight)
	
	deoglVBOWriterModel writerVBO(pModel.GetRenderThread());
	
	writerVBO.Reset(pVBOBlockWithWeight);
	
	pVertices.Visit([&](const oglModelVertex &vertex){
		writerVBO.WritePoint(pPositions[vertex.position].position, pNormals[vertex.normal],
			pTangents[vertex.tangent], pNegateTangents[vertex.tangent],
			pTexCoords[vertex.texcoord], pPositions[vertex.position].normal,
			pPositions[vertex.position].weights);
	});
	
// 	int maxCount = 0;
// 	for( i=0; i<pWeightsCount; i++ ){
// 		if( pWeightsCounts[ i ] > maxCount ) maxCount = pWeightsCounts[ i ];
// 	}
// 	pModel->GetOgl()->LogInfoFormat( "model %s bones=%i weights=%i(max %i) vertices=%i normals=%i tangents=%i",
// 		pModel.GetFilename().GetString(), pModel->GetModel()->GetBoneCount(), pWeightsCount, maxCount,
// 		pVertices.GetCount(), pNormalCount, pTangentCount );
}

void deoglModelLOD::pWriteVBOBlockVertPosSet(){
	DEASSERT_NOTNULL(pVBOBlockVertPosSet)
	
	struct sPoint{
		GLfloat x, y, z;
		GLuint index;
	};
	
	sPoint * const vboData = (sPoint*)pVBOBlockVertPosSet->GetData();
	
	pVertPosSets.Visit([&](const deoglModelLODVertPosSet &vps){
		sPoint * const points = vboData + vps.GetVBOOffset();
		vps.GetPositions().VisitIndexed([&](int i, const deoglModelLODVertPosSet::sPosition &position){
			points[i].x = (GLfloat)position.position.x;
			points[i].y = (GLfloat)position.position.y;
			points[i].z = (GLfloat)position.position.z;
			points[i].index = (GLuint)position.index;
		});
	});
}
