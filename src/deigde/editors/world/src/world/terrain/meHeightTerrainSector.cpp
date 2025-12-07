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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainTexture.h"
#include "meHeightTerrainPropField.h"
#include "meHeightTerrainPFLayer.h"
#include "meHeightTerrainPFType.h"
#include "meTerrainHeightImage.h"
#include "meTerrainMaskImage.h"
#include "meHeightTerrainNavSpace.h"
#include "../meWorld.h"
#include "../meWorldGuiParameters.h"
#include "../../utils/meBitArray.h"
#include "../../world/pathfinding/mePathFindTest.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainTexture.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <deigde/gui/wrapper/debugdrawer/igdeWDebugDrawerShape.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// NOTE the image is allowed to be incorrect in that the size differs from the
// set height image size in the world object. this is not going to work in the
// game itself but has to be kept as incorrect state in the editor so mappers
// can either replace the image or scale it to the proper size. the editor is
// supposed to provide this scaling support to help fix incorrect height images.
// therefore this class has to cope with the situation of incorrect height
// images. if the height image does not match it should be not set in the
// height terrain sector.

// Class meHeightTerrainSector
////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainSector::meHeightTerrainSector(deEngine *engine, const decPoint &coordinates) :
pActiveNavSpace(NULL),
pDDSelNavPoints(NULL)
{
	if(!engine) DETHROW(deeInvalidParam);
	
	pHeightTerrain = NULL;
	
	pEngine = engine;
	pEngSector = NULL;
	
	pDDPointsPerCluster = 64;
	pDDClusterCount = 16;
	pDDClusters = NULL;
	
	pCoordinates = coordinates;
	
	pActive = false;
	
	pRadiusOfInfluence = 5.0f; // 0.0f
	
	pDataType = edtInt16;
	pHeightImageChanged = false;
	pHeightImageSaved = false;
	
	pVisFaces = NULL;
	pVisImageChanged = false;
	pVisImageSaved = false;
	
	pTextures = NULL;
	pTextureCount = 0;
	pTextureSize = 0;
	pActiveTexture = NULL;
	
	pPropFields = NULL;
	pPropFieldCount = 0;
	pPropFieldCellCount = 0;
	pPFCacheChanged = false;
	pPFCacheSaved = false;
	
	pPFLayers = NULL;
	pPFLayerCount = 0;
	pPFLayerSize = 0;
	pActivePFLayer = NULL;
	
	try{
		pSetPropFieldCount(10);
		
		pCreateDDEdges();
		pCreateDDSelNavPoints();
		pRepositionDebugDrawers();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meHeightTerrainSector::~meHeightTerrainSector(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainSector::RebuildEngineSector(){
	DestroyEngineSector();
	CreateEngineSector();
	NotifySectorChanged();
}

void meHeightTerrainSector::SetHeightTerrain(meHeightTerrain *heightTerrain){
	if(heightTerrain == pHeightTerrain){
		return;
	}
	
	DestroyEngineSector();
	
	if(pHeightTerrain){
		deWorld * const world = pHeightTerrain->GetWorld().GetEngineWorld();
		
		if(pDDSelNavPoints){
			if(world){
				world->RemoveDebugDrawer(pDDSelNavPoints);
			}
			pDDSelNavPoints->FreeReference();
		}
		
		if(world){
			pRemoveDDEdgesFromWorld(*world);
			
			int i;
			for(i=0; i<pPropFieldCount; i++){
				if(pPropFields[i]){
					world->RemovePropField(pPropFields[i]->GetEnginePropField());
				}
			}
		}
	}
	
	pHeightTerrain = heightTerrain;
	
	pUpdatePropFieldPositions();
	
	if(heightTerrain){
		deWorld * const world = heightTerrain->GetWorld().GetEngineWorld();
		
		if(world){
			pAddDDEdgesToWorld(*world);
			
			if(pDDSelNavPoints){
				world->AddDebugDrawer(pDDSelNavPoints);
			}
			
			int i;
			for(i=0; i<pPropFieldCount; i++){
				if(pPropFields[i]){
					world->AddPropField(pPropFields[i]->GetEnginePropField());
				}
			}
		}
	}
	
	pCreateHeightImage();
	pCreateVisibility();
	
	CreateEngineSector();
}

void meHeightTerrainSector::CreateEngineSector(){
	if(!pHeightTerrain){
		return;
	}
	
	try{
		pEngSector = new deHeightTerrainSector(pCoordinates);
		pHeightTerrain->GetEngineHeightTerrain()->AddSector(pEngSector);
		
	}catch(const deException &){
		if(pEngSector){
			delete pEngSector;
		}
		throw;
	}
	
	decPoint pointCount(pHeightTerrain->GetSectorResolution(), pHeightTerrain->GetSectorResolution());
	const int navSpaceCount = pNavSpaces.GetCount();
	deHeightTerrainNavSpace *engNavSpace = NULL;
	deHeightTerrainTexture *engTexture = NULL;
	int i;
	
	try{
		pEngSector->SetHeightImage(pHeightImage);
		UpdateVisibilitySector(pEngSector);
		
		// create textures
		for(i=0; i<pTextureCount; i++){
			engTexture = pTextures[i]->CreateEngineTexture();
			pEngSector->AddTexture(engTexture);
			pTextures[i]->SetEngineTexture(engTexture);
			engTexture = NULL;
		}
		
		// add navigation spaces
		for(i=0; i<navSpaceCount; i++){
			meHeightTerrainNavSpace &navspace = *((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i));
			engNavSpace = navspace.CreateEngineNavSpace();
			pEngSector->AddNavSpace(engNavSpace);
			navspace.SetEngineNavSpace(engNavSpace);
			engNavSpace = NULL;
		}
		
	}catch(const deException &){
		if(engNavSpace){
			delete engNavSpace;
		}
		if(engTexture){
			delete engTexture;
		}
		throw;
	}
}

void meHeightTerrainSector::DestroyEngineSector(){
	if(!pEngSector){
		return;
	}
	
	const int navSpaceCount = pNavSpaces.GetCount();
	int i;
	
	for(i=0; i<navSpaceCount; i++){
		((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->SetEngineNavSpace(NULL);
	}
	for(i=0; i<pTextureCount; i++){
		pTextures[i]->SetEngineTexture(NULL);
	}
	pEngSector->RemoveAllTextures();
	
	if(pHeightTerrain){
		pHeightTerrain->GetEngineHeightTerrain()->RemoveSector(pEngSector);
		
	}else{
		delete pEngSector; // should never happen
	}
	pEngSector = NULL;
}



void meHeightTerrainSector::SetCoordinates(const decPoint &coordinates){
	if(coordinates == pCoordinates){
		return;
	}
	
	pCoordinates = coordinates;
	
	pRepositionDebugDrawers();
}

void meHeightTerrainSector::SectorSizeOrResChanged(){
	pCreateHeightImage();
	pCreateVisibility();
	pUpdatePropFieldPositions();
	
	pFreeDDEdges();
	pCreateDDEdges();
	pRepositionDebugDrawers();
	
	const int navSpaceCount = pNavSpaces.GetCount();
	int i;
	for(i=0; i<navSpaceCount; i++){
		((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->SectorSizeOrResChanged();
	}
	
	for(i=0; i<pPFLayerCount; i++){
		pPFLayers[i]->SectorSizeOrResChanged();
	}
}


void meHeightTerrainSector::SetActive(bool active){
	if(active == pActive){
		return;
	}
	
	pActive = true;
	
	pUpdateDDEdgesColors();
}



void meHeightTerrainSector::NotifySectorChanged(){
	if(pHeightTerrain){
		if(pHeightTerrain->GetEngineHeightTerrain()){
			pHeightTerrain->GetEngineHeightTerrain()->NotifySectorChanged(pHeightTerrain->IndexOfSector(this));
		}
		pHeightTerrain->SetChanged(true);
	}
}



void meHeightTerrainSector::InitDelegates(igdeEnvironment *environment){
	int p;
	
	for(p=0; p<pPropFieldCount; p++){
		pPropFields[p]->InitDelegates(environment);
	}
}



decVector2 meHeightTerrainSector::GetGridPointAt(const decDVector &position) const{
	if(!pHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	
	const double invSectorDim = 1.0 / (double)pHeightTerrain->GetSectorSize();
	const int resolution = pHeightTerrain->GetSectorResolution();
	const double divX = position.x * invSectorDim + 0.5;
	const double divZ = 0.5 - position.z * invSectorDim;
	
	return decVector2(
		(float)((divX - (float)pCoordinates.x) * (float)(resolution - 1)),
		(float)((divZ - (float)pCoordinates.y) * (float)(resolution - 1)));
}

float meHeightTerrainSector::GetHeightAtPoint(int x, int z) const{
	if(!pHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	if(x < 0 || x >= resolution || z < 0 || z >= resolution){
		DETHROW(deeInvalidParam);
	}
	return pHeightTerrain->GetBaseHeight() + pHeightImage->GetDataGrayscale32()
		[resolution * z + x].value * pHeightTerrain->GetHeightScaling();
}

void meHeightTerrainSector::GetHeightLocalAt(float x, float z, int &imgX, int &imgZ,
float &imgFacX, float &imgFacZ) const{
	if(!pHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	float invSectorDim = 1.0f / pHeightTerrain->GetSectorSize();
	
	imgFacX = (x * invSectorDim + 0.5f) * (float)resolution;
	imgFacZ = (0.5f - z * invSectorDim) * (float)resolution;
	
	imgX = decMath::clamp((int)imgFacX, 0, resolution - 2);
	imgFacX -= (float)imgX;
	
	imgZ = decMath::clamp((int)imgFacZ, 0, resolution - 2);
	imgFacZ -= (float)imgZ;
}

bool meHeightTerrainSector::GetHeightAt(float x, float z, float &height) const{
	if(!pHeightTerrain || !pVisFaces){
		DETHROW(deeInvalidParam);
	}
	
	sGrayscale32 *heights = pHeightImage->GetDataGrayscale32();
	float invSectorDim = 1.0f / pHeightTerrain->GetSectorSize();
	float qh1, qh2, qh3, qhiz;
	int qx, qz, qbase;
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	const float heightScaling = pHeightTerrain->GetHeightScaling();
	const float baseHeight = pHeightTerrain->GetBaseHeight();
	float lpx = (x * invSectorDim + 0.5f) * (float)resolution;
	float lpz = (0.5f - z * invSectorDim) * (float)resolution;
	
	qx = decMath::clamp((int)lpx, 0, resolution - 2);
	qz = decMath::clamp((int)lpz, 0, resolution - 2);
	
	lpx -= (float)qx;
	lpz -= (float)qz;
	
	qbase = qz * resolution + qx;
	qh1 = resolution + heights[qbase].value * heightScaling;
	
	if(lpx > lpz){ // triangle (1,2,4)
		qh2 = baseHeight + heights[qbase + 1].value * heightScaling;
		qh3 = baseHeight + heights[qbase + resolution + 1].value * heightScaling;
		
		if(lpx < 0.001f){ // avoid div by zero as we are right in the upper left corner
			height = qh1;
			
		}else{
			qhiz = lpz / lpx;
			height = qh1 * (1.0f - lpx) + (qh2 * (1.0f - qhiz) + qh3 * qhiz) * lpx;
		}
		
	}else{ // triangle (1,4,3)
		qh2 = baseHeight + heights[qbase + resolution].value * heightScaling;
		qh3 = baseHeight + heights[qbase + resolution + 1].value * heightScaling;
		
		if(lpz < 0.001f){ // avoid div by zero as we are right in the upper left corner
			height = qh1;
			
		}else{
			qhiz = lpx / lpz;
			height = qh1 * (1.0f - lpz) + (qh2 * (1.0f - qhiz) + qh3 * qhiz) * lpz;
		}
	}
	
	return pVisFaces->GetValueAt(qx, qz);
}

bool meHeightTerrainSector::GetHeightNormalAt(float x, float z, float &height, decVector &normal) const{
	if(!pHeightTerrain || !pVisFaces){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	const float sectorSize = pHeightTerrain->GetSectorSize();
	const float heightScaling = pHeightTerrain->GetHeightScaling();
	const float baseHeight = pHeightTerrain->GetBaseHeight();
	
	sGrayscale32 *heights = pHeightImage->GetDataGrayscale32();
	float divXZ = sectorSize / (float)resolution;
	float invSectorDim = 1.0f / sectorSize;
	float qh1, qh2, qh3, qhiz;
	int qx, qz, qbase;
	
	float lpx = (x * invSectorDim + 0.5f) * (float)resolution;
	float lpz = (0.5f - z * invSectorDim) * (float)resolution;
	
	qx = (int)lpx;
	if(qx < 0){
		qx = 0;
		
	}else if(qx >= resolution - 1){
		qx = resolution - 2;
	}
	
	qz = (int)lpz;
	if(qz < 0){
		qz = 0;
		
	}else if(qz >= resolution - 1){
		qz = resolution - 2;
	}
	
	lpx -= (float)qx;
	lpz -= (float)qz;
	
	qbase = qz * resolution + qx;
	qh1 = baseHeight + heights[qbase].value * heightScaling;
	
	if(lpx > lpz){ // triangle (1,2,4)
		qh2 = baseHeight + heights[qbase + 1].value * heightScaling;
		qh3 = baseHeight + heights[qbase + resolution + 1].value * heightScaling;
		
		if(lpx < 0.001f){ // avoid div by zero as we are right in the upper left corner
			height = qh1;
			
		}else{
			qhiz = lpz / lpx;
			height = qh1 * (1.0f - lpx) + (qh2 * (1.0f - qhiz) + qh3 * qhiz) * lpx;
		}
		
		normal.x = qh1 - qh2;
		normal.y = divXZ;
		normal.z = qh3 - qh2;
		
	}else{ // triangle (1,4,3)
		qh2 = baseHeight + heights[qbase + resolution].value * heightScaling;
		qh3 = baseHeight + heights[qbase + resolution + 1].value * heightScaling;
		
		if(lpz < 0.001f){ // avoid div by zero as we are right in the upper left corner
			height = qh1;
			
		}else{
			qhiz = lpx / lpz;
			height = qh1 * (1.0f - lpz) + (qh2 * (1.0f - qhiz) + qh3 * qhiz) * lpz;
		}
		
		normal.x = qh2 - qh3;
		normal.y = divXZ;
		normal.z = qh3 - qh1;
	}
	
	normal.Normalize();
	
	return pVisFaces->GetValueAt(qx, qz);
}

decDVector meHeightTerrainSector::GetWorldPosition() const{
	if(!pHeightTerrain){
		return decDVector();
	}
	
	const double sectorSize = (double)pHeightTerrain->GetSectorSize();
	return decDVector(sectorSize * pCoordinates.x, 0.0, sectorSize * pCoordinates.y);
}



void meHeightTerrainSector::Update(){
	int i;
	for(i=0; i<pPropFieldCount; i++){
		pPropFields[i]->Update();
	}
}

void meHeightTerrainSector::UpdateVInstances(){
	int i;
	for(i=0; i<pPropFieldCount; i++){
		pPropFields[i]->UpdateVInstances();
	}
}

void meHeightTerrainSector::ClearVegetation(){
	if(!pHeightTerrain || pHeightTerrain->GetVLayerCount() == 0){
		return;
	}
	
	int i;
	for(i=0; i<pPropFieldCount; i++){
		pPropFields[i]->Clear();
	}
	SetPFCacheChanged(true);
}

void meHeightTerrainSector::RebuildVegetationPropFieldTypes(){
	int i;
	for(i=0; i<pPropFieldCount; i++){
		pPropFields[i]->RebuildVegetationPropFieldTypes();
	}
}



void meHeightTerrainSector::SetRadiusOfInfluence(float radius){
	if(radius < 0.0f){
		pRadiusOfInfluence = 0.0f;
		
	}else{
		pRadiusOfInfluence = radius;
	}
}



void meHeightTerrainSector::ShowStateChanged(){
	pUpdateDDEdgesColors();
	pUpdateDDSelNavPointsColors();
	pUpdateNavSpaceDDColors();
}



// Height Image
/////////////////

void meHeightTerrainSector::SetDataType(int dataType){
	if(dataType < edtInt8 || dataType > edtFloat) DETHROW(deeInvalidParam);
	
	if(dataType != pDataType){
		pDataType = dataType;
		
		SetHeightImageChanged(true);
		if(pHeightTerrain){
			pHeightTerrain->GetWorld().NotifyHTSChanged(this);
		}
	}
}

bool meHeightTerrainSector::DoClampValues() const{
	return pDataType != edtFloat;
}

void meHeightTerrainSector::SetPathHeightImage(const char *path, bool loadFile){
	if(!path){
		DETHROW(deeInvalidParam);
	}
	
	if(pPathHeightImage.Equals(path)){
		return;
	}
	
	pPathHeightImage = path;
	
	if(loadFile){
		pHeightImageChanged = true;
		pHeightImageSaved = false;
		
		pLoadHeightImage();
		
	}else{
		pHeightImageChanged = true;
		pHeightImageSaved = true;
	}
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->SetDepChanged(true);
		pHeightTerrain->GetWorld().NotifyHTSChanged(this);
		pHeightTerrain->GetWorld().NotifyHTStateChanged();
	}
}

void meHeightTerrainSector::SetHeightImageChanged(bool changed){
	if(changed == pHeightImageChanged){
		return;
	}
	
	pHeightImageChanged = changed;
	
	if(pHeightTerrain){
		pHeightTerrain->SetDepChanged(true);
		
		const int resolution = pHeightTerrain->GetSectorResolution();
		InvalidatePropFields(decPoint(), decPoint(resolution, resolution));
	}
}

void meHeightTerrainSector::SetHeightImageSaved(bool saved){
	pHeightImageSaved = saved;
}



// Visibility
///////////////

void meHeightTerrainSector::SetPathVisibilityImage(const char *path){
	if(pPathVisImage.Equals(path)){
		return;
	}
	
	pPathVisImage = path;
	
	pVisImageChanged = true;
	pVisImageSaved = true;
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->SetDepChanged(true);
		pHeightTerrain->GetWorld().NotifyHTSChanged(this);
		pHeightTerrain->GetWorld().NotifyHTStateChanged();
	}
}

void meHeightTerrainSector::SetVisibilityChanged(bool changed){
	if(changed == pVisImageChanged){
		return;
	}
	
	pVisImageChanged = changed;
	
	if(pHeightTerrain){
		pHeightTerrain->SetDepChanged(true);
	}
}

void meHeightTerrainSector::SetVisibilitySaved(bool saved){
	pVisImageSaved = saved;
}



void meHeightTerrainSector::InitVisibilityFromSector(deHeightTerrainSector *sector) const{
	if(!sector || !pHeightTerrain || !pVisFaces){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	int x, y;
	for(y=0; y<resolution; y++){
		for(x=0; x<resolution; x++){
			pVisFaces->SetValueAt(x, y, sector->GetFaceVisibleAt(x, y));
		}
	}
}

void meHeightTerrainSector::UpdateVisibilitySector(deHeightTerrainSector *sector) const{
	if(!sector || !pHeightTerrain || !pVisFaces){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	int x, y;
	for(y=0; y<resolution; y++){
		for(x=0; x<resolution; x++){
			sector->SetFaceVisibleAt(x, y, pVisFaces->GetValueAt(x, y));
		}
	}
}

void meHeightTerrainSector::LoadVisibilityFromImage(){
	if(!pHeightTerrain || !pVisFaces){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	deImage::Ref image;
	decPath path;
	
	if(!pPathVisImage.IsEmpty()){
		const decString baseDir(pHeightTerrain->GetBaseDirectory());
		if(pEngine->GetVirtualFileSystem()->ExistsFile(decPath::AbsolutePathUnix(pPathVisImage, baseDir))){
			try{
				image.TakeOver(pEngine->GetImageManager()->LoadImage(pPathVisImage, baseDir));
				if(image->GetComponentCount() != 1 || image->GetWidth() != resolution || image->GetHeight() != resolution){
					DETHROW(deeInvalidParam);
					// TODO tell the user this is not going to work
				}
				
				image->RetainImageData(); // keep image data in memory... needs a better solution to avoid double loading
				
				int x, y, i;
				if(image->GetBitCount() == 8){
					sGrayscale8 *data = image->GetDataGrayscale8();
					i = 0;
					for(y=0; y<resolution; y++){
						for(x=0; x<resolution; x++){
							pVisFaces->SetValueAt(x, y, data[i++].value > HT_8BIT_BASE);
						}
					}
					
				}else if(image->GetBitCount() == 16){
					sGrayscale16 *data = image->GetDataGrayscale16();
					
					i = 0;
					for(y=0; y<resolution; y++){
						for(x=0; x<resolution; x++){
							pVisFaces->SetValueAt(x, y, data[i++].value > HT_16BIT_BASE);
						}
					}
					
				}else if(image->GetBitCount() == 32){
					sGrayscale32 *data = image->GetDataGrayscale32();
					
					i = 0;
					for(y=0; y<resolution; y++){
						for(x=0; x<resolution; x++){
							pVisFaces->SetValueAt(x, y, data[i++].value > 0.0f);
						}
					}
				}
				image->ReleaseImageData();
				
			}catch(const deException &e){
				if(image){
					image->ReleaseImageData();
				}
				if(pHeightTerrain){
					pHeightTerrain->GetWorld().GetLogger()->LogException(LOGSOURCE, e);
				}
			}
		}
	}
	
	SetVisibilitySaved(image);
	SetVisibilityChanged(true);
	
	if(pEngSector){
		UpdateVisibilitySector(pEngSector);
	}
	
	RebuildEngineSector();
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTSVisibilityChanged(this);
	}
}



// Textures
/////////////

meHeightTerrainTexture *meHeightTerrainSector::GetTextureAt(int index) const{
	if(index < 0 || index >= pTextureCount) DETHROW(deeInvalidParam);
	
	return pTextures[index];
}

meHeightTerrainTexture *meHeightTerrainSector::GetTextureNamed(const char *name) const{
	int i;
	for(i=0; i<pTextureCount; i++){
		if(pTextures[i]->GetName() == name){
			return pTextures[i];
		}
	}
	return NULL;
}

bool meHeightTerrainSector::HasTextureNamed(const char *name) const{
	int i;
	for(i=0; i<pTextureCount; i++){
		if(pTextures[i]->GetName() == name){
			return true;
		}
	}
	return false;
}

int meHeightTerrainSector::IndexOfTexture(meHeightTerrainTexture *texture) const{
	int i;
	for(i=0; i<pTextureCount; i++){
		if(texture == pTextures[i]){
			return i;
		}
	}
	return -1;
}

int meHeightTerrainSector::IndexOfTextureNamed(const char *name) const{
	int i;
	for(i=0; i<pTextureCount; i++){
		if(pTextures[i]->GetName() == name){
			return i;
		}
	}
	return -1;
}

bool meHeightTerrainSector::HasTexture(meHeightTerrainTexture *texture) const{
	int i;
	for(i=0; i<pTextureCount; i++){
		if(texture == pTextures[i]){
			return true;
		}
	}
	return false;
}

void meHeightTerrainSector::AddTexture(meHeightTerrainTexture *texture){
	if(!texture || HasTextureNamed(texture->GetName())) DETHROW(deeInvalidParam);
	
	deHeightTerrainTexture *engTexture = NULL;
	
	if(pTextureCount == pTextureSize){
		int newSize = pTextureCount * 3 / 2 + 1;
		meHeightTerrainTexture **newArray = new meHeightTerrainTexture*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pTextures){
			memcpy(newArray, pTextures, sizeof(meHeightTerrainTexture*) * pTextureSize);
			delete [] pTextures;
		}
		pTextures = newArray;
		pTextureSize = newSize;
	}
	
	pTextures[pTextureCount] = texture;
	pTextureCount++;
	
	texture->AddReference();
	texture->SetSector(this);
	
	if(pEngSector){
		try{
			engTexture = texture->CreateEngineTexture();
			pEngSector->AddTexture(engTexture);
			
		}catch(const deException &){
			if(engTexture) delete engTexture;
			throw;
		}
		
		texture->SetEngineTexture(engTexture);
	}
	NotifySectorChanged();
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->GetWorld().NotifyHTSTextureCountChanged(this);
	}
	
	if(!pActiveTexture) SetActiveTexture(texture);
}

void meHeightTerrainSector::RemoveTexture(meHeightTerrainTexture *texture){
	int i, index = IndexOfTexture(texture);
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(texture->GetEngineTexture()){
		if(pEngSector){
			pEngSector->RemoveTexture(texture->GetEngineTexture());
		}
		texture->SetEngineTexture(NULL);
	}
	
	if(texture == pActiveTexture) SetActiveTexture(NULL);
	
	for(i=index+1; i<pTextureCount; i++){
		pTextures[i - 1] = pTextures[i];
	}
	pTextureCount--;
	
	texture->SetSector(NULL);
	texture->SetEngineTexture(NULL);
	texture->FreeReference();
	
	NotifySectorChanged();
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->GetWorld().NotifyHTSTextureCountChanged(this);
	}
}

void meHeightTerrainSector::RemoveAllTextures(){
	int i;
	
	for(i=0; i<pTextureCount; i++){
		pTextures[i]->SetEngineTexture(NULL);
	}
	if(pEngSector){
		pEngSector->RemoveAllTextures();
	}
	
	SetActiveTexture(NULL);
	
	while(pTextureCount > 0){
		pTextureCount--;
		
		pTextures[pTextureCount]->SetSector(NULL);
		pTextures[pTextureCount]->SetEngineTexture(NULL);
		pTextures[pTextureCount]->FreeReference();
	}
	
	NotifySectorChanged();
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->GetWorld().NotifyHTSTextureCountChanged(this);
	}
}

void meHeightTerrainSector::SetActiveTexture(meHeightTerrainTexture *texture){
	if(texture != pActiveTexture){
		pActiveTexture = texture;
		
		if(pHeightTerrain){
			pHeightTerrain->GetWorld().NotifyHTSActiveTextureChanged(this);
		}
	}
}

meHeightTerrainTexture *meHeightTerrainSector::GetTextureUnder(float x, float y) const{
	if(!pHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	
	int gx = (int)x;
	int gy = (int)y;
	float bx = x - (float)gx;
	float by = y - (float)gy;
	float s1, s2, s3, s4, s5, s6, s7;
	deImage *mask;
	int t;
	
	if(gx < 0){
		gx = 0;
		bx = 0.0f;
		
	}else if(gx >= resolution){
		gx = resolution - 1;
		bx = 0.0f;
	}
	
	if(gy < 0){
		gy = 0;
		by = 0.0f;
		
	}else if(gy >= resolution){
		gy = resolution - 1;
		by = 0.0f;
	}
	
	for(t=pTextureCount-1; t>=0; t--){
		mask = pTextures[t]->GetMaskImage();
		
		if(mask){
			meTerrainMaskImage tmi(mask);
			
			s1 = tmi.GetMaskValueAt(gx, gy);
			s2 = tmi.GetMaskValueAt(gx + 1, gy);
			s3 = tmi.GetMaskValueAt(gx, gy + 1);
			s4 = tmi.GetMaskValueAt(gx + 1, gy + 1);
			
			s5 = s1 * (1.0f - bx) + s2 * bx;
			s6 = s3 * (1.0f - bx) + s4 * bx;
			
			s7 = s5 * (1.0f - by) + s6 * by;
			
			if(s7 > 0.5f){
				return pTextures[t];
			}
			
		}else{
			return pTextures[t];
		}
	}
	
	if(pTextureCount > 0){
		return pTextures[0];
	}
	
	return NULL;
}



// PropFields
///////////////

meHeightTerrainPFLayer *meHeightTerrainSector::GetPFLayerAt(int index) const{
	if(index < 0 || index >= pPFLayerCount) DETHROW(deeInvalidParam);
	
	return pPFLayers[index];
}

int meHeightTerrainSector::IndexOfPFLayer(meHeightTerrainPFLayer *pflayer) const{
	if(!pflayer) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPFLayerCount; i++){
		if(pflayer == pPFLayers[i]) return i;
	}
	
	return -1;
}

bool meHeightTerrainSector::HasPFLayer(meHeightTerrainPFLayer *pflayer) const{
	if(!pflayer) DETHROW(deeInvalidParam);
	int i;
	
	for(i=0; i<pPFLayerCount; i++){
		if(pflayer == pPFLayers[i]) return true;
	}
	
	return false;
}

void meHeightTerrainSector::AddPFLayer(meHeightTerrainPFLayer *pflayer){
	if(!pflayer) DETHROW(deeInvalidParam);
	
	if(pPFLayerCount == pPFLayerSize){
		int newSize = pPFLayerCount * 3 / 2 + 1;
		meHeightTerrainPFLayer **newArray = new meHeightTerrainPFLayer*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pPFLayers){
			memcpy(newArray, pPFLayers, sizeof(meHeightTerrainPFLayer*) * pPFLayerSize);
			delete [] pPFLayers;
		}
		pPFLayers = newArray;
		pPFLayerSize = newSize;
	}
	
	pPFLayers[pPFLayerCount] = pflayer;
	pPFLayerCount++;
	
	pflayer->AddReference();
	pflayer->SetHTSector(this);
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
//		pHeightTerrain->GetWorld().NotifyHTSPFLayerCountChanged( this );
	}
	
	if(!pActivePFLayer) SetActivePFLayer(pflayer);
}

void meHeightTerrainSector::RemovePFLayer(meHeightTerrainPFLayer *pflayer){
	int i, index = IndexOfPFLayer(pflayer);
	if(index == -1) DETHROW(deeInvalidParam);
	
	if(pflayer == pActivePFLayer) SetActivePFLayer(NULL);
	
	for(i=index+1; i<pPFLayerCount; i++){
		pPFLayers[i - 1] = pPFLayers[i];
	}
	pPFLayerCount--;
	
	pflayer->SetHTSector(NULL);
	pflayer->FreeReference();
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
//		pHeightTerrain->GetWorld().NotifyHTSPFLayerCountChanged( this );
	}
}

void meHeightTerrainSector::RemoveAllPFLayers(){
	SetActivePFLayer(NULL);
	
	while(pPFLayerCount > 0){
		pPFLayerCount--;
		
		pPFLayers[pPFLayerCount]->SetHTSector(NULL);
		pPFLayers[pPFLayerCount]->FreeReference();
	}
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
//		pHeightTerrain->GetWorld().NotifyHTSPFLayerCountChanged( this );
	}
}

void meHeightTerrainSector::SetActivePFLayer(meHeightTerrainPFLayer *pflayer){
	if(pflayer != pActivePFLayer){
		pActivePFLayer = pflayer;
		
		if(pHeightTerrain){
//			pHeightTerrain->GetWorld().NotifyHTSActivePFLayerChanged( this );
		}
	}
}



meHeightTerrainPropField *meHeightTerrainSector::GetPropFieldAt(int index) const{
	if(index < 0 || index >= pPropFieldCount) DETHROW(deeInvalidParam);
	
	return pPropFields[index];
}

void meHeightTerrainSector::InvalidateHeights(const decPoint &fromGrid, const decPoint &toGrid){
	pUpdateDDEdgesHeights(fromGrid, toGrid);
	pUpdateDDSelNavPointHeights();
	
	const int navSpaceCount = pNavSpaces.GetCount();
	int i;
	for(i=0; i<navSpaceCount; i++){
		((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->InvalidateHeights();
	}
	
	pRepositionDebugDrawers();
}

void meHeightTerrainSector::InvalidatePropFields(const decPoint &fromGrid, const decPoint &toGrid){
	if(!pHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	
	if(pPropFieldCount == 0 || pHeightTerrain->GetVLayerCount() == 0){
		return;
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	const float sectorSize = pHeightTerrain->GetSectorSize();
	
	float areaFactor = sectorSize / (float)resolution;
	float areaOffset = sectorSize * 0.5f;
	float areaFromX = ((float)fromGrid.x * areaFactor - areaOffset) - pRadiusOfInfluence;
	float areaFromY = (areaOffset - (float)toGrid.y * areaFactor) - pRadiusOfInfluence;
	float areaToX = ((float)toGrid.x * areaFactor - areaOffset) + pRadiusOfInfluence;
	float areaToY = (areaOffset - (float)fromGrid.y * areaFactor) + pRadiusOfInfluence;
	bool dirty = false;
	int p;
	
	for(p=0; p<pPropFieldCount; p++){
		const decVector2 &minExtend = pPropFields[p]->GetMinimumExtend();
		const decVector2 &maxExtend = pPropFields[p]->GetMaximumExtend();
		
		if(minExtend.x < areaToX && maxExtend.x > areaFromX && minExtend.y < areaToY && maxExtend.y > areaFromY){
			dirty = true;
			pPropFields[p]->Invalidate();
		}
	}
	
	if(dirty){
		SetPFCacheChanged(true);
	}
}

void meHeightTerrainSector::InvalidateAllPropFields(){
	if(!pHeightTerrain || pHeightTerrain->GetVLayerCount() == 0){
		return;
	}
	
	int i;
	for(i=0; i<pPropFieldCount; i++){
		pPropFields[i]->Invalidate();
	}
	SetPFCacheChanged(true);
}

void meHeightTerrainSector::ValidateAllPropFields(){
	int i;
	for(i=0; i<pPropFieldCount; i++){
		pPropFields[i]->MarkVInstancesValid();
		pPropFields[i]->Validate();
	}
}

void meHeightTerrainSector::SetPathPFCache(const char *path){
	if(pPathPFCache.Equals(path)){
		return;
	}
	
	pPathPFCache = path;
	
	pPFCacheChanged = true;
	pPFCacheSaved = true;
	
	if(pHeightTerrain){
		pHeightTerrain->SetChanged(true);
		pHeightTerrain->SetDepChanged(true);
		pHeightTerrain->GetWorld().NotifyHTSChanged(this);
		pHeightTerrain->GetWorld().NotifyHTStateChanged();
	}
}

void meHeightTerrainSector::SetPFCacheChanged(bool changed){
	if(changed == pPFCacheChanged){
		return;
	}
	
	pPFCacheChanged = changed;
	
	if(pHeightTerrain){
		if(changed){
			pHeightTerrain->SetDepChanged(true);
		}
		pHeightTerrain->GetWorld().NotifyHTStateChanged();
	}
}

void meHeightTerrainSector::SetPFCacheSaved(bool saved){
	if(pPFCacheSaved == saved){
		return;
	}
	
	pPFCacheSaved = saved;
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTStateChanged();
	}
}



// Navigation spaces
//////////////////////

int meHeightTerrainSector::GetNavSpaceCount() const{
	return pNavSpaces.GetCount();
}

meHeightTerrainNavSpace *meHeightTerrainSector::GetNavSpaceAt(int index) const{
	return (meHeightTerrainNavSpace*)pNavSpaces.GetAt(index);
}

meHeightTerrainNavSpace *meHeightTerrainSector::GetNavSpaceNamed(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pNavSpaces.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		meHeightTerrainNavSpace * const navspace = (meHeightTerrainNavSpace*)pNavSpaces.GetAt(i);
		if(navspace->GetName() == name){
			return navspace;
		}
	}
	
	return NULL;
}

bool meHeightTerrainSector::HasNavSpaceNamed(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pNavSpaces.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->GetName() == name){
			return true;
		}
	}
	
	return false;
}

int meHeightTerrainSector::IndexOfNavSpace(meHeightTerrainNavSpace *navspace) const{
	return pNavSpaces.IndexOf(navspace);
}

int meHeightTerrainSector::IndexOfNavSpaceNamed(const char *name) const{
	if(!name){
		DETHROW(deeInvalidParam);
	}
	
	const int count = pNavSpaces.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		if(((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->GetName() == name){
			return i;
		}
	}
	
	return -1;
}

bool meHeightTerrainSector::HasNavSpace(meHeightTerrainNavSpace *navspace) const{
	return pNavSpaces.Has(navspace);
}

void meHeightTerrainSector::AddNavSpace(meHeightTerrainNavSpace *navspace){
	if(!navspace || HasNavSpaceNamed(navspace->GetName())){
		DETHROW(deeInvalidParam);
	}
	
	pNavSpaces.Add(navspace);
	navspace->SetHTSector(this);
	
	deHeightTerrainNavSpace *engNavSpace = NULL;
	
	try{
		engNavSpace = navspace->CreateEngineNavSpace();
		pEngSector->AddNavSpace(engNavSpace);
		
	}catch(const deException &){
		if(engNavSpace){
			delete engNavSpace;
		}
		throw;
	}
	
	navspace->SetEngineNavSpace(engNavSpace);
	
	if(pHeightTerrain){
		pHeightTerrain->SetWorldChanged();
		pHeightTerrain->GetWorld().NotifyHTNavSpaceCountChanged();
	}
	
	if(!pActiveNavSpace){
		SetActiveNavSpace(navspace);
	}
}

void meHeightTerrainSector::RemoveNavSpace(meHeightTerrainNavSpace *navspace){
	const int index = pNavSpaces.IndexOf(navspace);
	if(index == -1){
		DETHROW(deeInvalidParam);
	}
	
	if(navspace->GetEngineNavSpace()){
		pEngSector->RemoveNavSpace(navspace->GetEngineNavSpace());
		navspace->SetEngineNavSpace(NULL);
	}
	
	if(navspace == pActiveNavSpace){
		SetActiveNavSpace(NULL);
	}
	
	navspace->SetHTSector(NULL);
	navspace->SetEngineNavSpace(NULL);
	
	pNavSpaces.RemoveFrom(index);
	
	if(pHeightTerrain){
		pHeightTerrain->SetWorldChanged();
		pHeightTerrain->GetWorld().GetPathFindTest()->Invalidate();
		pHeightTerrain->GetWorld().NotifyHTNavSpaceCountChanged();
	}
}

void meHeightTerrainSector::RemoveAllNavSpaces(){
	const int count = pNavSpaces.GetCount();
	int i;
	for(i=0; i<count; i++){
		meHeightTerrainNavSpace &navspace = *((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i));
		navspace.SetEngineNavSpace(NULL);
		navspace.SetHTSector(NULL);
	}
	if(pEngSector){ // in case called from destructor
		pEngSector->RemoveAllNavSpaces();
	}
	
	SetActiveNavSpace(NULL);
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().GetPathFindTest()->Invalidate();
	}
	pNavSpaces.RemoveAll();
	
	if(pHeightTerrain){
		pHeightTerrain->SetWorldChanged();
		pHeightTerrain->GetWorld().NotifyHTNavSpaceCountChanged();
	}
}

void meHeightTerrainSector::SetActiveNavSpace(meHeightTerrainNavSpace *navspace){
	if(navspace == pActiveNavSpace){
		return;
	}
	
	if(pActiveNavSpace){
		pActiveNavSpace->SetActive(false);
	}
	
	pActiveNavSpace = navspace;
	
	if(navspace){
		navspace->SetActive(true);
	}
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTActiveNavSpaceChanged();
	}
}

void meHeightTerrainSector::SetSelectedNavPoints(const decIntList &points){
	if(points == pSelectedNavPoints){
		return;
	}
	
	pSelectedNavPoints = points;
	
	pUpdateDDSelNavPointsShapes();
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().NotifyHTNavSpaceSelectedPointsChanged();
	}
}



// Private Functions
//////////////////////

void meHeightTerrainSector::pCleanUp(){
	SetHeightTerrain(NULL);
	
	RemoveAllNavSpaces();
	
	pSetPropFieldCount(0);
	
	RemoveAllPFLayers();
	if(pPFLayers){
		delete [] pPFLayers;
	}
	
	if(pVisFaces){
		delete pVisFaces;
	}
	
	RemoveAllTextures();
	if(pTextures){
		delete [] pTextures;
	}
	
	pFreeDDEdges();
}

void meHeightTerrainSector::pLoadHeightImage(){
	if(!pHeightTerrain || !pHeightImage){
		return;
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	
	// load image if the path is a valid file path
	if(!pPathHeightImage.IsEmpty()){
		const decString baseDir(pHeightTerrain->GetBaseDirectory());
		if(pEngine->GetVirtualFileSystem()->ExistsFile(decPath::AbsolutePathUnix(pPathHeightImage, baseDir))){
			// load the image
			deImage::Ref image;
			try{
				image.TakeOver(pEngine->GetImageManager()->LoadImage(pPathHeightImage, baseDir));
				image->RetainImageData(); // keep image data in memory... needs a better solution to avoid double loading
				
				// TODO this is not a nice solution here. it is possible that the user specifies
				// an incorrect image. the editor should provide the user with dialog to choose
				// what to do. most probably this entire code has to be split up to separate
				// into a loading and a copy part whereas the copy part results in an error if
				// the requirements are not met
				if(image->GetComponentCount() != 1 || image->GetWidth() != resolution
				|| image->GetHeight() != resolution){
					DETHROW(deeInvalidParam);
				}
				
				// copy the height values
				sGrayscale32 *destData = pHeightImage->GetDataGrayscale32();
				int p, pixelCount = resolution * resolution;
				
				if(image->GetBitCount() == 8){
					sGrayscale8 *srcData = image->GetDataGrayscale8();
					for(p=0; p<pixelCount; p++){
						destData[p].value = (float)(srcData[p].value - HT_8BIT_BASE) * HT_8BIT_PTOH;
					}
					
				}else if(image->GetBitCount() == 16){
					sGrayscale16 *srcData = image->GetDataGrayscale16();
					for(p=0; p<pixelCount; p++){
						destData[p].value = (float)(srcData[p].value - HT_16BIT_BASE) * HT_16BIT_PTOH;
					}
					
				}else{ // image->GetBitCount() == 32
					memcpy(destData, image->GetDataGrayscale32(), sizeof(sGrayscale32) * pixelCount);
				}
				
				// the loaded image is no more needed
				image->ReleaseImageData();
				
			}catch(const deException &e){
				if(image){
					image->ReleaseImageData();
				}
				if(pHeightTerrain){
					pHeightTerrain->GetWorld().GetLogger()->LogException(LOGSOURCE, e);
				}
			}
		}
	}
	
	// update the image
	NotifySectorChanged();
	pUpdateDDEdgesHeights(decPoint(), decPoint(resolution, resolution));
	pUpdateDDSelNavPointHeights();
}

void meHeightTerrainSector::pCreateHeightImage(){
	if(pHeightTerrain){
		const int resolution = pHeightTerrain->GetSectorResolution();
		pHeightImage.TakeOver(pEngine->GetImageManager()->CreateImage(resolution, resolution, 1, 1, 32));
		
		int p, pixelCount = resolution * resolution;
		sGrayscale32 * const imageData = pHeightImage->GetDataGrayscale32();
		for(p=0; p<pixelCount; p++){
			imageData[p].value = 0.0f;
		}
		
	}else{
		pHeightImage = NULL;
	}
	
	if(pEngSector){
		pEngSector->SetHeightImage(pHeightImage);
		NotifySectorChanged();
	}
}

void meHeightTerrainSector::pCreateVisibility(){
	if(pVisFaces){
		delete pVisFaces;
		pVisFaces = NULL;
	}
	
	if(!pHeightTerrain){
		return;
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	pVisFaces = new meBitArray(resolution, resolution);
}

void meHeightTerrainSector::pSetPropFieldCount(int cellCount){
	int pfcount = cellCount * cellCount;
	
	// free the old
	if(pPropFields){
		while(pPropFieldCount > 0){
			pPropFieldCount--;
			if(pPropFields[pPropFieldCount]){
				pPropFields[pPropFieldCount]->SetHTSector(NULL);
				delete pPropFields[pPropFieldCount];
			}
		}
		
		delete [] pPropFields;
		pPropFields = NULL;
		
		pPropFieldCellCount = 0;
		pPropFieldCount = 0;
	}
	
	// create the new array
	if(pfcount > 0){
		meHeightTerrainPropField *propField = NULL;
		int p;
		
		try{
			pPropFields = new meHeightTerrainPropField*[pfcount];
			if(!pPropFields) DETHROW(deeOutOfMemory);
			
			for(p=0; p<pfcount; p++) pPropFields[p] = NULL;
			
			while(pPropFieldCount < pfcount){
				propField = new meHeightTerrainPropField(pEngine);
				if(!propField) DETHROW(deeOutOfMemory);
				
				propField->SetHTSector(this);
				
				pPropFields[pPropFieldCount++] = propField;
				propField = NULL;
			}
			
			pPropFieldCellCount = cellCount;
			
		}catch(const deException &){
			if(propField) delete propField;
			throw;
		}
	}
	
	pUpdatePropFieldPositions();
}

void meHeightTerrainSector::pUpdatePropFieldPositions(){
	if(pPropFieldCellCount == 0 || !pHeightTerrain){
		return;
	}
	
	const float sectorSize = pHeightTerrain->GetSectorSize();
	double cellSize = (double)sectorSize / (double)pPropFieldCellCount;
	double cellOffset = ((double)sectorSize - cellSize) * 0.5;
	double sectorOffsetX = (double)sectorSize * (double)pCoordinates.x;
	double sectorOffsetZ = (double)sectorSize * (double)pCoordinates.y;
	float halfCellSize = (float)(cellSize * 0.5);
	decVector2 minExtend, maxExtend;
	decDVector position, size;
	dePropField *engPF;
	int x, y, p;
	
	size.x = cellSize;
	size.y = cellSize; // depends on the content and the terrain underneath
	size.z = cellSize;
	
	position.y = 0.0; // depends on the content and the terrain underneath
	
	for(y=0, p=0; y<pPropFieldCellCount; y++){
		position.z = sectorOffsetZ + cellOffset - cellSize * (double)y;
		minExtend.y = (float)position.z - halfCellSize;
		maxExtend.y = (float)position.z + halfCellSize;
		
		for(x=0; x<pPropFieldCellCount; x++, p++){
			engPF = pPropFields[p]->GetEnginePropField();
			
			position.x = sectorOffsetX - cellOffset + cellSize * (double)x;
			minExtend.x = (float)position.x - halfCellSize;
			maxExtend.x = (float)position.x + halfCellSize;
			
			engPF->SetPosition(position);
			//engPF->SetSize( size );
			
			pPropFields[p]->SetExtend(minExtend, maxExtend);
		}
	}
}



void meHeightTerrainSector::pRepositionDebugDrawers(){
	if(!pHeightTerrain){
		return;
	}
	
	const double aliasingOffset = 0.001; // small offset to make edges win over faces underneath them
	const float sectorSize = pHeightTerrain->GetSectorSize();
	const int count = pDDEdges.GetCount();
	int i;
	
	const decDVector position(
		(double)sectorSize * (double)pCoordinates.x,
		(double)pHeightTerrain->GetBaseHeight() + aliasingOffset,
		(double)sectorSize * (double)pCoordinates.y);
	
	for(i=0; i<count; i++){
		((deDebugDrawer*)pDDEdges.GetAt(i))->SetPosition(position);
	}
	
	if(pDDSelNavPoints){
		pDDSelNavPoints->SetPosition(position);
	}
}



// #include <dragengine/common/utils/decTimer.h>
void meHeightTerrainSector::pCreateDDEdges(){
// 	printf( "pCreateDDEdges BEGIN\n" );
// 	decTimer timer; timer.Reset();
	pFreeDDEdges();
	
	if(!pHeightTerrain){
		return;
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	
	const sGrayscale32 * const heights = pHeightImage->GetDataGrayscale32();
	deDebugDrawerManager &ddmgr = *pEngine->GetDebugDrawerManager();
	const float stepPoint = 1.0f / (float)(resolution - 1);
	const decVector normal(0.0f, 1.0f, 0.0f);
	deDebugDrawerShapeFace *face = NULL;
	deDebugDrawerShape *ddshape = NULL;
	deDebugDrawer *debugDrawer = NULL;
	int pcx, pcz, curx, curz;
	decVector corners[4];
	int cx, cz, x, z;
	
	pDDPointsPerCluster = resolution / 16; // 64 for 1024. ensures the number of clusters does not explode for large images
	pDDClusterCount = ((resolution - 2) / (pDDPointsPerCluster - 1)) + 1;
	
	pDDClusters = new sDDCluster[pDDClusterCount * pDDClusterCount];
	
	try{
		for(curz=0, cz=0; cz<pDDClusterCount; cz++){
			if(cz < pDDClusterCount - 1){
				pcz = pDDPointsPerCluster;
				
			}else{
				pcz = resolution - curz;
			}
			
			for(curx=0, cx=0; cx<pDDClusterCount; cx++){
				// cluster information
				if(cx < pDDClusterCount - 1){
					pcx = pDDPointsPerCluster;
					
				}else{
					pcx = resolution - curx;
				}
				
				sDDCluster &cluster = pDDClusters[pDDClusterCount * cz + cx];
				cluster.pointFrom.Set(curx, curz);
				cluster.pointTo = cluster.pointFrom + decPoint(pcx - 1, pcz - 1);
				cluster.faceCount.Set(pcx - 1, pcz - 1);
				
				curx += pDDPointsPerCluster - 1;
				
				// debug drawer
				debugDrawer = ddmgr.CreateDebugDrawer();
				debugDrawer->SetXRay(false); //true);
				
				ddshape = new deDebugDrawerShape;
				ddshape->SetFillColor(decColor(0.0f, 0.0f, 0.0f, 0.0f));
				
				for(z=cluster.pointFrom.y; z<cluster.pointTo.y; z++){
					const sGrayscale32 * const heightsRow1 = heights + resolution * z;
					const sGrayscale32 * const heightsRow2 = heights + resolution * (z + 1);
					
					corners[0].z = corners[1].z = 0.5f - stepPoint * (float)z;
					corners[2].z = corners[3].z = 0.5f - stepPoint * (float)(z + 1);
					
					for(x=cluster.pointFrom.x; x<cluster.pointTo.x; x++){
						corners[0].x = corners[3].x = stepPoint * (float)x - 0.5f;
						corners[1].x = corners[2].x = stepPoint * (float)(x + 1) - 0.5f;
						
						corners[0].y = heightsRow1[x].value;
						corners[1].y = heightsRow1[x + 1].value;
						corners[2].y = heightsRow2[x + 1].value;
						corners[3].y = heightsRow2[x].value;
						
						face = new deDebugDrawerShapeFace;
						face->AddVertex(corners[0]);
						face->AddVertex(corners[1]);
						face->AddVertex(corners[2]);
						face->AddVertex(corners[3]);
						face->SetNormal(normal); //(corners[1] - corners[0]) % (corners[2] - corners[1]));
						
						ddshape->AddFace(face);
						face = NULL;
					}
				}
				
				debugDrawer->AddShape(ddshape);
				debugDrawer->NotifyShapeLayoutChanged();
				
				pDDEdges.Add(debugDrawer);
				debugDrawer->FreeReference();
			}
			
			curz += pDDPointsPerCluster - 1;
		}
		
	}catch(const deException &){
		if(face){
			delete face;
		}
		if(ddshape){
			delete ddshape;
		}
		if(debugDrawer){
			debugDrawer->FreeReference();
		}
		throw;
	}
	
	pUpdateDDEdgesColors();
	
// 	printf( "pCreateDDEdges END (%iys)\n", ( int )( timer.GetElapsedTime() * 1e6f ) );
}

void meHeightTerrainSector::pFreeDDEdges(){
	pDDEdges.RemoveAll();
	
	if(pDDClusters){
		delete [] pDDClusters;
		pDDClusters = NULL;
	}
}

void meHeightTerrainSector::pAddDDEdgesToWorld(deWorld &world){
	const int count = pDDEdges.GetCount();
	int i;
	for(i=0; i<count; i++){
		world.AddDebugDrawer((deDebugDrawer*)pDDEdges.GetAt(i));
	}
}

void meHeightTerrainSector::pRemoveDDEdgesFromWorld(deWorld &world){
	const int count = pDDEdges.GetCount();
	int i;
	for(i=0; i<count; i++){
		world.RemoveDebugDrawer((deDebugDrawer*)pDDEdges.GetAt(i));
	}
}

void meHeightTerrainSector::pUpdateDDEdgesColors(){
	decColor color(0.0f, 0.25f, 1.0f, 1.0f);
	bool visible = false;
	
	if(pActive){
		color.Set(1.0f, 0.5f, 0.0f, 1.0f);
		//color.Set( 1.0f, 0.5f, 0.0f, 1.0f )
	}
	
	if(pHeightTerrain){
		switch(pHeightTerrain->GetWorld().GetGuiParameters().GetWorkMode()){
		case meWorldGuiParameters::ewmHeightPaint:
		case meWorldGuiParameters::ewmMaskPaint:
		case meWorldGuiParameters::ewmVisibilityPaint:
		case meWorldGuiParameters::ewmNavSpaceEdit:
			visible = true;
			break;
			
		default:
			break;
		}
	}
	
	const int count = pDDEdges.GetCount();
	int i;
	
	for(i=0; i<count; i++){
		deDebugDrawer &debugDrawer = *((deDebugDrawer*)pDDEdges.GetAt(i));
		debugDrawer.GetShapeAt(0)->SetEdgeColor(color);
// 		debugDrawer.GetShapeAt( 0 )->SetEdgeColor( decColor(
// 			( float )( ((i*12)%256) % pDDEdgesClusterCount ) / ( float )pDDEdgesClusterCount, 0.0f,
// 			( float )( ((i*12)%256) / pDDEdgesClusterCount ) / ( float )pDDEdgesClusterCount, 1.0f ) );
		debugDrawer.NotifyShapeColorChanged();
		debugDrawer.SetVisible(visible);
	}
}

void meHeightTerrainSector::pUpdateDDEdgesHeights(const decPoint &fromGrid, const decPoint &toGrid){
// 	printf( "pUpdateDDSEdgesHeights BEGIN\n" );
// 	decTimer timer; timer.Reset();
	if(!pHeightTerrain || !pDDClusters){
		return;
	}
	
	const int resolution = pHeightTerrain->GetSectorResolution();
	
	const decPoint clusterFrom(decPoint().Largest((fromGrid - decPoint(1, 1)) / (pDDPointsPerCluster - 1)));
	const decPoint clusterTo(decPoint(pDDClusterCount - 1, pDDClusterCount - 1)
		.Smallest(toGrid / (pDDPointsPerCluster - 1)));
	const sGrayscale32 * const heights = pHeightImage->GetDataGrayscale32();
	int cx, cz, x, z;
	
	for(cz=clusterFrom.y; cz<=clusterTo.y; cz++){
		for(cx=clusterFrom.x; cx<=clusterTo.x; cx++){
			const sDDCluster &cluster = pDDClusters[pDDClusterCount * cz + cx];
			const decPoint faceFrom(cluster.pointFrom.Largest(fromGrid - decPoint(1, 1)));
			const decPoint faceTo(cluster.pointTo.Smallest(toGrid));
			deDebugDrawer &debugDrawer = *((deDebugDrawer*)pDDEdges.GetAt(pDDClusterCount * cz + cx));
			deDebugDrawerShape &shape = *debugDrawer.GetShapeAt(0);
			
			for(z=faceFrom.y; z<faceTo.y; z++){
				const sGrayscale32 * const heightsRow1 = heights + resolution * z;
				const sGrayscale32 * const heightsRow2 = heights + resolution * (z + 1);
				const int faceRowIndex = cluster.faceCount.x * (z - cluster.pointFrom.y) - cluster.pointFrom.x;
				
				for(x=faceFrom.x; x<faceTo.x; x++){
					deDebugDrawerShapeFace &face = *shape.GetFaceAt(faceRowIndex + x);
					
					decVector corner1 = face.GetVertexAt(0);
					corner1.y = heightsRow1[x].value;
					face.SetVertexAt(0, corner1);
					
					decVector corner2 = face.GetVertexAt(1);
					corner2.y = heightsRow1[x + 1].value;
					face.SetVertexAt(1, corner2);
					
					decVector corner3 = face.GetVertexAt(2);
					corner3.y = heightsRow2[x + 1].value;
					face.SetVertexAt(2, corner3);
					
					decVector corner4 = face.GetVertexAt(3);
					corner4.y = heightsRow2[x].value;
					face.SetVertexAt(3, corner4);
					
					//face.SetNormal( ( corner2 - corner1 ) % ( corner3 - corner2 ) );
				}
			}
			
			debugDrawer.NotifyShapeContentChanged();
		}
	}
	
// 	printf( "pUpdateDDSEdgeHeights END (%iys)\n", ( int )( timer.GetElapsedTime() * 1e6f ) );
}



void meHeightTerrainSector::pCreateDDSelNavPoints(){
	pDDSelNavPoints = pEngine->GetDebugDrawerManager()->CreateDebugDrawer();
	pDDSelNavPoints->SetXRay(false);
	
	deDebugDrawerShape *ddshape = NULL;
	
	try{
		ddshape = new deDebugDrawerShape;
		ddshape->SetEdgeColor(decColor(1.0f, 0.0f, 0.0f, 1.0f));
		ddshape->SetFillColor(decColor(1.0f, 0.0f, 0.0f, 0.2f));
		pDDSelNavPoints->AddShape(ddshape);
		
	}catch(const deException &){
		if(ddshape){
			delete ddshape;
		}
		throw;
	}
	
	if(pHeightTerrain){
		pHeightTerrain->GetWorld().GetEngineWorld()->AddDebugDrawer(pDDSelNavPoints);
	}
}

void meHeightTerrainSector::pUpdateDDSelNavPointsColors(){
	bool visible = false;
	
	if(pHeightTerrain){
		switch(pHeightTerrain->GetWorld().GetGuiParameters().GetWorkMode()){
		case meWorldGuiParameters::ewmNavSpaceEdit:
			visible = true;
			break;
			
		default:
			break;
		}
	}
	
	pDDSelNavPoints->SetVisible(visible);
}

void meHeightTerrainSector::pUpdateDDSelNavPointsShapes(){
	deDebugDrawerShape &ddshape = *pDDSelNavPoints->GetShapeAt(0);
	decShapeList &shapes = ddshape.GetShapeList();
	int i;
	
	shapes.RemoveAll();
	
	if(pHeightImage && pHeightTerrain){
		const sGrayscale32 * const heights = pHeightImage->GetDataGrayscale32();
		const float sectorSize = pHeightTerrain->GetSectorSize();
		const int sectorResolution = pHeightTerrain->GetSectorResolution();
		const float heightScale = pHeightTerrain->GetHeightScaling();
		const float radius = 0.25f; // / (float)sectorResolution;
		const int count = pSelectedNavPoints.GetCount();
		const float invPointLast = 1.0f / (float)(sectorResolution - 1);
		
		for(i=0; i<count; i++){
			const int navpoint = pSelectedNavPoints.GetAt(i);
			const decVector center(
				sectorSize * ((float)(navpoint % sectorResolution) * invPointLast - 0.5f),
				heightScale * heights[navpoint].value,
				sectorSize * (0.5f - (float)(navpoint / sectorResolution) * invPointLast));
			shapes.Add(new decShapeSphere(radius, center));
		}
	}
	
	pDDSelNavPoints->NotifyShapeContentChanged();
}

void meHeightTerrainSector::pUpdateDDSelNavPointHeights(){
	const int count = pSelectedNavPoints.GetCount();
	if(count == 0 || !pHeightTerrain || !pHeightImage){
		return;
	}
	
	const sGrayscale32 * const heights = pHeightImage->GetDataGrayscale32();
	deDebugDrawerShape &ddshape = *pDDSelNavPoints->GetShapeAt(0);
	const float heightScale = pHeightTerrain->GetHeightScaling();
	decShapeList &shapes = ddshape.GetShapeList();
	int i;
	
	for(i=0; i<count; i++){
		const int navpoint = pSelectedNavPoints.GetAt(i);
		decShape &shape = *shapes.GetAt(i);
		
		decVector position(shape.GetPosition());
		position.y = heightScale * heights[navpoint].value;
		shape.SetPosition(position);
	}
	
	pDDSelNavPoints->NotifyShapeGeometryChanged();
}

void meHeightTerrainSector::pUpdateNavSpaceDDColors(){
	const int count = pNavSpaces.GetCount();
	int i;
	for(i=0; i<count; i++){
		((meHeightTerrainNavSpace*)pNavSpaces.GetAt(i))->UpdateDDColors();
	}
}
