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

#include "deoglHTSCluster.h"
#include "deoglHTSTexture.h"
#include "deoglRHeightTerrain.h"
#include "deoglRHTSector.h"
#include "deoglTerrainMaskImage.h"
#include "../../deoglBasics.h"
#include "../../skin/deoglSkin.h"
#include "../../skin/deoglSkinTexture.h"
#include "../../texture/texture2d/deoglTexture.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../utils/collision/deoglDCollisionBox.h"
#include "../../world/deoglRWorld.h"

#include <dragengine/deTUniqueReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainTexture.h>



// Class deoglRHTSector
/////////////////////////

// Constructor, destructor
////////////////////////////

deoglRHTSector::deoglRHTSector(deoglRHeightTerrain &heightTerrain, const deHeightTerrainSector &sector) :
pHeightTerrain(heightTerrain),
pIndex(-1),
pCoordinates(sector.GetSector()),

pBaseHeight(sector.GetParentHeightTerrain()->GetBaseHeight()),
pScaling(sector.GetParentHeightTerrain()->GetHeightScaling()),

pValidTextures(false),
pDirtyMaskTextures(true),
pTexturesRequirePrepareForRender(true),

pMinHeight(0.0f),
pMaxHeight(0.0f),

pDirtyPoints(true),

pValid(false)
{
	int i;
	for(i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++){
		pMasks[i] = nullptr;
	}
	
	try{
		pCreateArrays(sector);
		
	}catch(const deException &){
// 		heightTerrain->GetOpenGL()->SetErrorTrace( e );
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE(heightTerrain.GetRenderThread(), HTSector);
}

deoglRHTSector::~deoglRHTSector(){
	LEAK_CHECK_FREE(pHeightTerrain.GetRenderThread(), HTSector);
	pCleanUp();
}



// Management
///////////////

void deoglRHTSector::SetIndex(int index){
	pIndex = index;
}

decDMatrix deoglRHTSector::CalcWorldMatrix() const{
	return CalcWorldMatrix(pHeightTerrain.GetParentWorld()->GetReferencePosition());
}

decDMatrix deoglRHTSector::CalcWorldMatrix(const decDVector &referencePosition) const{
	return decDMatrix::CreateTranslation(CalcWorldPosition(referencePosition));
}

decDVector deoglRHTSector::CalcWorldPosition() const{
	DEASSERT_NOTNULL(pHeightTerrain.GetParentWorld())
	return CalcWorldPosition(pHeightTerrain.GetParentWorld()->GetReferencePosition());
}

decDVector deoglRHTSector::CalcWorldPosition(const decDVector &referencePosition) const{
	return decDVector(pHeightTerrain.GetSectorSize() * (double)pCoordinates.x, 0.0,
		pHeightTerrain.GetSectorSize() * (double)pCoordinates.y) - referencePosition;
}

void deoglRHTSector::AddToWorldCompute(deoglWorldCompute &worldCompute){
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.AddToWorldCompute(worldCompute);
	});
}

void deoglRHTSector::UpdateWorldCompute(){
	if(!pHeightTerrain.GetParentWorld()){
		return;
	}
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.UpdateWorldCompute();
	});
}

void deoglRHTSector::RemoveFromWorldCompute(){
	if(!pHeightTerrain.GetParentWorld()){
		return;
	}
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.RemoveFromWorldCompute();
	});
}

void deoglRHTSector::PrepareForRender(){
	if(!pValidTextures){
		return;
	}
	
	pCreateVBODataPoints1();
	pCreateVBODataFaces();
	pUpdateMaskTextures();
	pUpdateHeightMap();
	
	if(pTexturesRequirePrepareForRender){
		pTexturesRequirePrepareForRender = false;
		
		pTextures.Visit([&](deoglHTSTexture &t){
			t.PrepareForRender();
		});
	}
}



deoglHTSTexture &deoglRHTSector::GetTextureAt(int index) const{
	return pTextures.GetAt(index);
}

void deoglRHTSector::TextureRequirePrepareForRender(){
	pTexturesRequirePrepareForRender = true;
	pHeightTerrain.SectorRequirePrepareForRender();
}



void deoglRHTSector::HeightChanged(const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to){
	pDirtyPoints = true;
	pHeightTerrain.SectorRequirePrepareForRender();
	
	pSyncHeightMap(sector, from, to);
}

void deoglRHTSector::SectorChanged(const deHeightTerrainSector &sector){
	pDirtyPoints = true;
	pDirtyMaskTextures = true;
	pTexturesRequirePrepareForRender = true;
	pHeightTerrain.SectorRequirePrepareForRender();
	
	pSyncSector(sector);
}



// Clusters
/////////////

deoglHTSCluster &deoglRHTSector::GetClusterAt(int x, int z){
	if(x < 0 || x >= pClusterCount || z < 0 || z >= pClusterCount){
		DETHROW(deeInvalidParam);
	}
	
	return pClusters[pClusterCount * z + x];
}

const deoglHTSCluster &deoglRHTSector::GetClusterAt(int x, int z) const{
	if(x < 0 || x >= pClusterCount || z < 0 || z >= pClusterCount){
		DETHROW(deeInvalidParam);
	}
	
	return pClusters[pClusterCount * z + x];
}

deoglHTSCluster &deoglRHTSector::GetClusterAt(const decPoint &coordinate){
	return GetClusterAt(coordinate.x, coordinate.y);
}

const deoglHTSCluster &deoglRHTSector::GetClusterAt(const decPoint &coordinate) const{
	return GetClusterAt(coordinate.x, coordinate.y);
}

void deoglRHTSector::ClustersUpdateWorldComputeElementTextures(){
	if(!pHeightTerrain.GetParentWorld()){
		return;
	}
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.UpdateWorldComputeTextures();
	});
}



// Private functions
//////////////////////

void deoglRHTSector::pCleanUp(){
	deoglDelayedOperations &dops = pHeightTerrain.GetRenderThread().GetDelayedOperations();
	
	pDropMaskPixelBuffers();
	
	pTextures.RemoveAll();
	int i;
	for(i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++){
		if(pMasks[i]){
			delete pMasks[i];
		}
	}
	
	pVBODataFaces.Visit([&](GLuint &vbo){
		dops.DeleteOpenGLBuffer(vbo);
	});
	pVBODataPoints2.Visit([&](GLuint &vbo){
		dops.DeleteOpenGLBuffer(vbo);
	});
	pVBODataPoints1.Visit([&](GLuint &vbo){
		dops.DeleteOpenGLBuffer(vbo);
	});
}



void deoglRHTSector::pCreateArrays(const deHeightTerrainSector &sector){
	pCreateHeightMap(sector);
}

void deoglRHTSector::pCreateHeightMap(const deHeightTerrainSector &sector){
	const int imageDim = pHeightTerrain.GetSectorResolution();
//	float sectorDim = pHeightTerrain->GetSectorSize();
//	float sizeStep = sectorDim / ( float )imageDim;
//	float sizeOffset = sectorDim * 0.5f;
	int x, z;
	
	// create clusters
	int maxPointsPerCluster = 65;
	int pcx, pcz, curx, curz;
	
	pClusterCount = ((imageDim - 2) / (maxPointsPerCluster - 1)) + 1;
	
	pClusters.SetCountDiscard(pClusterCount * pClusterCount);
	
	curz = 0;
	for(z=0; z<pClusterCount; z++){
		if(z < pClusterCount - 1){
			pcz = maxPointsPerCluster;
			
		}else{
			pcz = imageDim - curz;
		}
		
		curx = 0;
		for(x=0; x<pClusterCount; x++){
			if(x < pClusterCount - 1){
				pcx = maxPointsPerCluster;
				
			}else{
				pcx = imageDim - curx;
			}
			
			const int index = pClusterCount * z + x;
			deoglHTSCluster &cluster = pClusters[index];
			cluster.SetHTSector(this);
			cluster.SetIndex(index);
			cluster.SetCoordinates(decPoint(x, z));
			cluster.SetSize(curx, curz, pcx, pcz);
			
			curx += maxPointsPerCluster - 1;
		}
		
		curz += maxPointsPerCluster - 1;
	}
	
	// init clusters
	// NOTE currently each cluster calculates the LOD chain. now the majority of
	// clusters shares the same configuration ( in terms of points per cluster ).
	// instead of calculating these identical LOD chains multiple times they
	// could be pushed into a LOD chain cache instead which reduces not only
	// memory consumption but also processing time during loading time.
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.InitFromHeightImage(sector);
	});
	
	// debug
	/*
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.DebugPrint();
	}
	*/
}

void deoglRHTSector::pSetTextureCount(int count){
	if(count == pTextures.GetCount()){
		return;
	}
	
	pTextures.RemoveAll();
	
	int i;
	for(i=0; i<count; i++){
		pTextures.Add(deTUniqueReference<deoglHTSTexture>::New(*this, i));
	}
}



//#include <dragengine/common/utils/decTimer.h>
//static decTimer timer;
//#define DEBUG_RESET_TIMERS				timer.Reset();
//#define DEBUG_PRINT_TIMER(what)			printf( "[OGL] Timer: %s = %iys\n", what, ( int )( timer.GetElapsedTime() * 1000000.0 ) )

void deoglRHTSector::pDropMaskPixelBuffers(){
	int i;
	for(i=0; i<OGLHTS_MAX_MASK_TEXTURES; i++){
		pPixBufMasks[i].Clear();
	}
}



void deoglRHTSector::pSyncSector(const deHeightTerrainSector &sector){
	const int imageDim = pHeightTerrain.GetSectorResolution();
	
	// store sector parameters
	pCoordinates = sector.GetSector();
	pBaseHeight = sector.GetParentHeightTerrain()->GetBaseHeight();
	pScaling = sector.GetParentHeightTerrain()->GetHeightScaling();
	
	// determine if the sector is valid
	pValid = true;
	
	deImage * const heightImage = sector.GetHeightImage();
	if(heightImage){
		if(heightImage->GetWidth() != imageDim){
			pValid = false;
		}
		if(heightImage->GetHeight() != imageDim){
			pValid = false;
		}
		
	}else{
		pValid = false;
	}
	
	if(!pValid){
		pSetTextureCount(0);
		return;
	}
	
	// sync textures
	pSyncTextures(sector);
}

void deoglRHTSector::pSyncTextures(const deHeightTerrainSector &sector){
	// determine if textures are valid and create them if not existing
	pValidTextures = false;
	
	if(sector.GetTextures().IsNotEmpty()){
		pSetTextureCount(sector.GetTextures().GetCount());
		pValidTextures = true;
	}
	
	// copy texture parameters for render thread part
	sector.GetTextures().VisitIndexed([&](int i, deHeightTerrainTexture *t){
		pTextures[i]->SetMatrix(
			decTexMatrix::CreateScale(t->GetProjectionScaling().x, t->GetProjectionScaling().y)
			* decTexMatrix::CreateTranslation(t->GetProjectionOffset().x, t->GetProjectionOffset().y)
			* decTexMatrix::CreateRotation(t->GetProjectionRotation()));
	});
	
	// update skins if dirty
	if(pValidTextures){
		sector.GetTextures().VisitIndexed([&](int i, deHeightTerrainTexture *t){
			const deSkin * const skin = t->GetSkin();
			
			if(skin){
				pTextures[i]->SetSkin(((deoglSkin*)skin->GetPeerGraphic())->GetRSkin());
				
			}else{
				pTextures[i]->SetSkin(nullptr);
			}
		});
	}
	
	// sync texture masks if dirty
	if(pValidTextures){
		pSyncMaskTextures(sector);
	}
}

void deoglRHTSector::pSyncMaskTextures(const deHeightTerrainSector &sector){
	const int textureCount = decMath::min(pTextures.GetCount(), 16);
	const int maskCount = ((textureCount - 1) >> 2) + 1;
	int m, t;
	
	int maskWidth = 0, maskHeight = 0;
	int p, x, y, offset;
	
	pDropMaskPixelBuffers();
	
	deImage *images[16];
	for(t=0; t<16; t++){
		images[t] = nullptr;
	}
	
	for(t=0; t<textureCount; t++){
		deImage * const image = sector.GetTextures().GetAt(t)->GetMaskImage();
		if(!image){
			continue;
		}
		
		const int imageWidth = image->GetWidth();
		const int imageHeight = image->GetHeight();
		
		if(maskWidth == 0){
			maskWidth = imageWidth;
			maskHeight = imageHeight;
		}
		
		if(imageWidth == maskWidth && imageHeight == maskHeight){
			images[t] = image;
		}
	}
	
	if(maskWidth == 0){
		return;
	}
	
	const int pixelCount = maskWidth * maskHeight;
	for(m=0; m<maskCount; m++){
		pPixBufMasks[m] = deoglPixelBuffer::Ref::New(deoglPixelBuffer::epfFloat4, maskWidth, maskHeight, 1);
		deoglPixelBuffer::sFloat4 * const pbdata = pPixBufMasks[m]->GetPointerFloat4();
		
		for(t=m*4; t<textureCount; t++){
			const int relT = t - m * 4;
			
			if(t == 0){
				for(p=0; p<pixelCount; p++){
					pbdata[p].r = 1.0f;
				}
				
			}else if(images[t]){
				const deoglTerrainMaskImage tmi(*images[t]);
				
				if(relT == 0){
					for(p=0, y=0; y<maskHeight; y++){
						offset = maskWidth * (maskHeight - 1 - y);
						for(x=0; x<maskWidth; x++){
							pbdata[p++].r = tmi.GetMaskValueAt(offset + x);
						}
					}
					
				}else if(relT == 1){
					for(p=0, y=0; y<maskHeight; y++){
						offset = maskWidth * (maskHeight - 1 - y);
						for(x=0; x<maskWidth; x++){
							pbdata[p++].g = tmi.GetMaskValueAt(offset + x);
						}
					}
					
				}else if(relT == 2){
					for(p=0, y=0; y<maskHeight; y++){
						offset = maskWidth * (maskHeight - 1 - y);
						for(x=0; x<maskWidth; x++){
							pbdata[p++].b = tmi.GetMaskValueAt(offset + x);
						}
					}
					
				}else if(relT == 3){
					for(p=0, y=0; y<maskHeight; y++){
						offset = maskWidth * (maskHeight - 1 - y);
						for(x=0; x<maskWidth; x++){
							pbdata[p++].a = tmi.GetMaskValueAt(offset + x);
						}
					}
				}
				
			}else{
				if(relT == 0){
					for(p=0; p<pixelCount; p++) pbdata[p].r = 1.0f;
					
				}else if(relT == 1){
					for(p=0; p<pixelCount; p++) pbdata[p].g = 1.0f;
					
				}else if(relT == 2){
					for(p=0; p<pixelCount; p++) pbdata[p].b = 1.0f;
					
				}else if(relT == 3){
					for(p=0; p<pixelCount; p++) pbdata[p].a = 1.0f;
				}
			}
		}
	}
}

void deoglRHTSector::pSyncHeightMap(const deHeightTerrainSector &sector, const decPoint &from, const decPoint &to){
	if(!pValid){
		return;
	}
	
	deImage * const heightImage = sector.GetHeightImage();
	const float baseHeight = sector.GetParentHeightTerrain()->GetBaseHeight();
	const int imageDim = pHeightTerrain.GetSectorResolution();
	const int pixelCount = imageDim * imageDim;
	int x, y;
	
	// create height values if not existing already
	pHeights.SetCountDiscard(pixelCount);
	
	// update height values
	if(heightImage->GetBitCount() == 8){
		const sGrayscale8 * const imageData = heightImage->GetDataGrayscale8();
		const float scaling = sector.GetParentHeightTerrain()->GetHeightScaling() * HT_8BIT_PTOH;
		
		for(y=from.y; y<=to.y; y++){
			const sGrayscale8 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights.GetArrayPointer() + imageDim * y;
			
			for(x=from.x; x<=to.x; x++){
				heightsRow[x] = baseHeight + (float)(imageDataRow[x].value - HT_8BIT_BASE) * scaling;
			}
		}
		
	}else if(heightImage->GetBitCount() == 16){
		sGrayscale16 * const imageData = heightImage->GetDataGrayscale16();
		const float scaling = sector.GetParentHeightTerrain()->GetHeightScaling() * HT_16BIT_PTOH;
		
		for(y=from.y; y<=to.y; y++){
			const sGrayscale16 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights.GetArrayPointer() + imageDim * y;
			
			for(x=from.x; x<=to.x; x++){
				heightsRow[x] = baseHeight + (float)(imageDataRow[x].value - HT_16BIT_BASE) * scaling;
			}
		}
		
	}else{
		sGrayscale32 * const imageData = heightImage->GetDataGrayscale32();
		const float scaling = sector.GetParentHeightTerrain()->GetHeightScaling();
		
		for(y=from.y; y<=to.y; y++){
			const sGrayscale32 * const imageDataRow = imageData + imageDim * y;
			float * const heightsRow = pHeights.GetArrayPointer() + imageDim * y;
			
			for(x=from.x; x<=to.x; x++){
				heightsRow[x] = baseHeight + imageDataRow[x].value * scaling;
			}
		}
	}
	
	UpdateWorldCompute();
}



void deoglRHTSector::pUpdateMaskTextures(){
	if(!pDirtyMaskTextures){
		return;
	}
	pDirtyMaskTextures = false;
	
	const int textureCount = decMath::min(pTextures.GetCount(), 16);
	int i;
	
	for(i=0; i<4; i++){
		const int relT = decMath::min(textureCount - i * 4, 4);
		
		if(pPixBufMasks[i] && relT > 0){
			if(!pMasks[i]){
				pMasks[i] = new deoglTexture(pHeightTerrain.GetRenderThread());
			}
			
			pMasks[i]->SetSize(pPixBufMasks[i]->GetWidth(), pPixBufMasks[i]->GetHeight());
			
			if(relT == 1){
				pMasks[i]->SetFormatMappingByNumber(deoglCapsFmtSupport::eutfR8);
				
			}else if(relT == 2){
				//pMasks[ m ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG4 ); // not supported anymore
				//pMasks[ m ]->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG8 ); // uses 2 bytes
				pMasks[i]->SetFormatMappingByNumber(deoglCapsFmtSupport::eutfR3G3B2); // uses only 1 byte but wastes 2 for unused component
				
			}else if(relT == 3){
				pMasks[i]->SetFormatMappingByNumber(deoglCapsFmtSupport::eutfR3G3B2); // GL_RGB4, GL_RGB5, GL_RGB8
				
			}else{
				pMasks[i]->SetFormatMappingByNumber(deoglCapsFmtSupport::eutfRGBA2); // GL_RGBA4, GL_RGBA8
			}
			
			pMasks[i]->SetPixels(pPixBufMasks[i]);
			
		}else{
			if(pMasks[i]){
				delete pMasks[i];
				pMasks[i] = nullptr;
			}
		}
	}
}

void deoglRHTSector::pCreateVBODataPoints1(){
	if(pVBODataPoints1.IsNotEmpty()){
		return;
	}
	
	OGL_IF_CHECK(deoglRenderThread &renderThread = pHeightTerrain.GetRenderThread();)
	const int vboCount = ((pClusters.GetCount() - 1) / 32) + 1;
	if(vboCount == 0){
		return;
	}
	
	int assignedClusters = 0;
	int vboOffset = 0;
	int v = 0;
	
	pVBODataPoints1.SetCountDiscard(vboCount);
	OGL_CHECK(renderThread, pglGenBuffers(vboCount, pVBODataPoints1.GetArrayPointer()));
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.SetOffsetVBODataPoints(vboOffset);
		cluster.SetVBODataPoints1(pVBODataPoints1[v]);
		vboOffset += cluster.GetCountVBODataPoints();
		
		assignedClusters++;
		if(assignedClusters == 32){
			OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBODataPoints1[v]));
			OGL_CHECK(renderThread, pglBufferData(GL_ARRAY_BUFFER,
				sizeof(deoglVBOHeightTerrain1) * vboOffset, nullptr, GL_STATIC_DRAW));
//renderThread.GetLogger().LogInfoFormat( "htsector data1: size=%i vbo=%u", sizeof(deoglVBOHeightTerrain1)*vboOffset, pVBODataPoints1[ v ] );
			
			v++;
			assignedClusters = 0;
			vboOffset = 0;
		}
	});
	
	if(v < vboCount && vboOffset > 0){
		OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBODataPoints1[v]));
		OGL_CHECK(renderThread, pglBufferData(GL_ARRAY_BUFFER,
			sizeof(deoglVBOHeightTerrain1) * vboOffset, nullptr, GL_STATIC_DRAW));
//renderThread.GetLogger().LogInfoFormat( "htsector data1: size=%i vbo=%u", sizeof(deoglVBOHeightTerrain1)*vboOffset, pVBODataPoints1[ v ] );
	}
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.UpdateVBOData1();
	});
}

void deoglRHTSector::pCreateVBODataFaces(){
	if(pVBODataFaces.IsNotEmpty()){
		return;
	}
	
	OGL_IF_CHECK(deoglRenderThread &renderThread = pHeightTerrain.GetRenderThread();)
	const int vboCount = ((pClusters.GetCount() - 1) / 32) + 1;
	if(vboCount == 0){
		return;
	}
	
	int assignedClusters = 0;
	int vboOffset = 0;
	int v = 0;
	
	pVBODataFaces.SetCountDiscard(vboCount);
	OGL_CHECK(renderThread, pglGenBuffers(vboCount, pVBODataFaces.GetArrayPointer()));
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.SetOffsetVBODataFaces(vboOffset);
		cluster.SetVBODataFaces(pVBODataFaces[v]);
		vboOffset += cluster.GetCountVBODataFaces();
		
		assignedClusters++;
		if(assignedClusters == 32){
			OGL_CHECK(renderThread, pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pVBODataFaces[v]));
			OGL_CHECK(renderThread, pglBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(GLushort) * vboOffset, nullptr, GL_STATIC_DRAW));
//renderThread.GetLogger().LogInfoFormat( "htsector index: size=%i vbo=%u", sizeof(GLushort)*vboOffset, pVBODataFaces[ v ] );
			
			v++;
			assignedClusters = 0;
			vboOffset = 0;
		}
	});
	
	if(v < vboCount && vboOffset > 0){
		OGL_CHECK(renderThread, pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pVBODataFaces[v]));
		OGL_CHECK(renderThread, pglBufferData(GL_ELEMENT_ARRAY_BUFFER,
			sizeof(GLushort) * vboOffset, nullptr, GL_STATIC_DRAW));
//renderThread.GetLogger().LogInfoFormat( "htsector index: size=%i vbo=%u", sizeof(GLushort)*vboOffset, pVBODataFaces[ v ] );
	}
	
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.UpdateVBODataFaces();
	});
	
	OGL_CHECK(renderThread, pglBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void deoglRHTSector::pUpdateHeightMap(){
	if(!pDirtyPoints){
		return;
	}
	pDirtyPoints = false;
	
	if(!pValid || pHeights.IsEmpty()){
		return;
	}
	
	OGL_IF_CHECK(deoglRenderThread &renderThread = pHeightTerrain.GetRenderThread();)
	int /*b, l, p,*/ clusterCount = pClusters.GetCount();
	const int imageDim = pHeightTerrain.GetSectorResolution();
//	float invCellSize = ( float )imageDim / engHT->GetSectorSize();
//	float invCellSize2 = invCellSize * 2.0f;
//	deoglVBOHeightTerrain2 *baseDataPoint;
//	oglHTFacePoint *baseFacePoint;
	int firstPointX, firstPointZ;
	int pointCountX, pointCountZ;
	//float nx, ny, nz;
	int x, z;
	
	// create vbos if not existing already
	if(pVBODataPoints2.IsEmpty()){
		int vboOffset = 0, assignedClusters = 0;
		int vboCount = ((clusterCount - 1) / 32) + 1;
		
		if(vboCount > 0){
			pVBODataPoints2.SetCountDiscard(vboCount);
			OGL_CHECK(renderThread, pglGenBuffers(vboCount, pVBODataPoints2.GetArrayPointer()));
			
			int v = 0;
			pClusters.Visit([&](deoglHTSCluster &cluster){
				cluster.SetVBODataPoints2(pVBODataPoints2[v]);
				vboOffset += cluster.GetCountVBODataPoints();
				
				assignedClusters++;
				if(assignedClusters == 32){
					OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBODataPoints2[v]));
					OGL_CHECK(renderThread, pglBufferData(GL_ARRAY_BUFFER,
						sizeof(deoglVBOHeightTerrain2) * vboOffset, nullptr, GL_STREAM_DRAW));
//renderThread.GetLogger().LogInfoFormat( "htsector data2: size=%i vbo=%u", sizeof( deoglVBOHeightTerrain2 )*vboOffset, pVBODataPoints2[ v ] );
					
					v++;
					assignedClusters = 0;
					vboOffset = 0;
				}
			});
			
			if(v < vboCount && vboOffset > 0){
				OGL_CHECK(renderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBODataPoints2[v]));
				OGL_CHECK(renderThread, pglBufferData(GL_ARRAY_BUFFER,
					sizeof(deoglVBOHeightTerrain2) * vboOffset, nullptr, GL_STREAM_DRAW));
//renderThread.GetLogger().LogInfoFormat( "htsector data2: size=%i vbo=%u", sizeof( deoglVBOHeightTerrain2 )*vboOffset, pVBODataPoints2[ v ] );
			}
		}
	}
	
	// update vbos, vaos and rts instances
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.UpdateVBOData2();
	});
	pClusters.Visit([&](deoglHTSCluster &cluster){
		cluster.UpdateVAO();
	});
	
	// update bounding boxes
	pMinHeight = 0.0f;
	pMaxHeight = 0.0f;
	
	float minHeight, maxHeight;
	int lastZ, firstX, lastX;
	
	pClusters.VisitIndexed([&](int c, deoglHTSCluster &cluster){
		firstPointX = cluster.GetFirstPointX();
		firstPointZ = cluster.GetFirstPointZ();
		pointCountX = cluster.GetPointCountX();
		pointCountZ = cluster.GetPointCountZ();
		
		lastZ = firstPointZ + pointCountZ;
		maxHeight = minHeight = pHeights[imageDim * firstPointZ + firstPointX];
		
		for(z=firstPointZ; z<lastZ; z++){
			firstX = imageDim * z + firstPointX;
			lastX = firstX + pointCountX;
			
			for(x=firstX; x<lastX; x++){
				if(pHeights[x] < minHeight){
					minHeight = pHeights[x];
					
				}else if(pHeights[x] > maxHeight){
					maxHeight = pHeights[x];
				}
			}
		}
		
		cluster.UpdateHeightExtends(minHeight, maxHeight);
		
		if(c == 0){
			pMinHeight = minHeight;
			pMaxHeight = maxHeight;
			
		}else{
			pMinHeight = decMath::min(pMinHeight, minHeight);
			pMaxHeight = decMath::max(pMaxHeight, maxHeight);
		}
	});
//DEBUG_PRINT_TIMER( "HTSector: Extends" );
}
