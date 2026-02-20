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

#include "deHeightTerrain.h"
#include "deHeightTerrainSector.h"
#include "deHeightTerrainTexture.h"
#include "deHeightTerrainNavSpace.h"
#include "../../decal/deDecal.h"
#include "../../image/deImage.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../systems/modules/ai/deBaseAIHeightTerrain.h"
#include "../../../systems/modules/graphic/deBaseGraphicHeightTerrain.h"
#include "../../../systems/modules/physics/deBasePhysicsHeightTerrain.h"



// Class deHeightTerrainSector
////////////////////////////////

// NOTE:
// the current visible faces system uses a simple bit-per-face array
// to store the visibility. a better system would be to use an RLE
// based system with the following encoding:
// X Y X Y ...
// where X indicates the number of 1s and Y the number of 0s. this
// encoding does not require to store the value to repeat since it
// alternates between 1 and 0 starting with 1 for each line. The value
// 255 is special indicating that 254 of 0 or 1 are written and that
// the next value is of the same type. Therefore the data
// 200 times 1, 500 times 0, 200 times 1
// would be encoded like this ( in paranthesis the bit used for this
// run as well as the number of encoded elements ):
// 200(1/200), 255(0/254), 246(0/246), 200(1/200)

// Constructor, destructor
////////////////////////////

deHeightTerrainSector::deHeightTerrainSector(const decPoint &sector) :
pSector(sector),
pParentHeightTerrain(nullptr),
pIndex(-1){
}

deHeightTerrainSector::~deHeightTerrainSector(){
	SetHeightImage(nullptr);
	RemoveAllNavSpaces();
	RemoveAllDecals();
	RemoveAllTextures();
}



// Management
///////////////

void deHeightTerrainSector::SetParentHeightTerrain(deHeightTerrain *heightTerrain){
	pParentHeightTerrain = heightTerrain;
}

void deHeightTerrainSector::SetIndex(int index){
	pIndex = index;
}



void deHeightTerrainSector::SetHeightImage(deImage *heightImage){
	if(heightImage && heightImage->GetComponentCount() != 1){
		DETHROW(deeInvalidParam);
	}
	if(pHeightImage == heightImage){
		return;
	}
	
	// note: calculate extends requires image data to be retained. thing is
	//       deprecated anyways and should be done by physics module
	if(pHeightImage){
		pHeightImage->ReleaseImageData();
	}
	
	pHeightImage = heightImage;
	
	if(heightImage){
		heightImage->RetainImageData();
	}
	
	if(pParentHeightTerrain){
		const int last = pParentHeightTerrain->GetSectorResolution() - 1;
		pParentHeightTerrain->NotifyHeightChanged(pSector, pSector, decPoint(), decPoint(last, last));
	}
}

void deHeightTerrainSector::CalculateHeightExtends(float &minHeight, float &maxHeight){
	minHeight = 0.0f;
	maxHeight = 0.0f;
	
	if(!pHeightImage || !pParentHeightTerrain){
		return;
	}
	
	if(!pHeightImage->GetData()){
		DETHROW(deeInvalidParam); // data not retained. user has to use SetForceRetainImageData
		// note: this entire calculate extends thing is deprecated anyways and should be done by physics module
	}
	
	const int width = pHeightImage->GetWidth();
	const int height = pHeightImage->GetHeight();
	const int size = width * height;
	int i;
	
	if(pHeightImage->GetBitCount() == 8){
		const sGrayscale8 * const imageData = pHeightImage->GetDataGrayscale8();
		const float scaling = pParentHeightTerrain->GetHeightScaling() * HT_8BIT_PTOH;
		
		for(i=0; i<size; i++){
			const float curHeight = (imageData[i].value - HT_8BIT_BASE) * scaling;
			
			if(curHeight < minHeight){
				minHeight = curHeight;
				
			}else if(curHeight > maxHeight){
				maxHeight = curHeight;
			}
		}
		
	}else if(pHeightImage->GetBitCount() == 16){
		const sGrayscale16 * const imageData = pHeightImage->GetDataGrayscale16();
		const float scaling = pParentHeightTerrain->GetHeightScaling() * HT_16BIT_PTOH;
		
		for(i=0; i<size; i++){
			const float curHeight = (imageData[i].value - HT_16BIT_BASE) * scaling;
			
			if(curHeight < minHeight){
				minHeight = curHeight;
				
			}else if(curHeight > maxHeight){
				maxHeight = curHeight;
			}
		}
		
	}else{
		const sGrayscale32 * const imageData = pHeightImage->GetDataGrayscale32();
		
		for(i=0; i<size; i++){
			const float curHeight = imageData[i].value * pParentHeightTerrain->GetHeightScaling();
			
			if(curHeight < minHeight){
				minHeight = curHeight;
				
			}else if(curHeight > maxHeight){
				maxHeight = curHeight;
			}
		}
	}
	
	minHeight += pParentHeightTerrain->GetBaseHeight();
	maxHeight += pParentHeightTerrain->GetBaseHeight();
}



// Face Visibility
////////////////////

bool deHeightTerrainSector::GetFaceVisibleAt(int x, int y) const{
	if(!pParentHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	const int pointCount = pParentHeightTerrain->GetSectorResolution();
	if(x < 0 || x >= pointCount || y < 0 || y >= pointCount){
		DETHROW(deeInvalidParam);
	}
	
	if(pVisibleFaces.IsEmpty()){
		return true;
	}
	
	const int bitOffset = pointCount * y + x;
	return (pVisibleFaces[bitOffset >> 3] & (1 << (bitOffset & 0x7))) != 0;
}

void deHeightTerrainSector::SetFaceVisibleAt(int x, int y, bool visible){
	if(!pParentHeightTerrain){
		DETHROW(deeInvalidParam);
	}
	const int pointCount = pParentHeightTerrain->GetSectorResolution();
	if(x < 0 || x >= pointCount || y < 0 || y >= pointCount){
		DETHROW(deeInvalidParam);
	}
	
	pCreateVisibleFaces();
	
	const int bitOffset = pointCount * y + x;
	
	if(visible){
		pVisibleFaces[bitOffset >> 3] |= (unsigned char)(1 << (bitOffset & 0x7));
		
	}else{
		pVisibleFaces[bitOffset >> 3] &= ~((unsigned char)(1 << (bitOffset & 0x7)));
	}
}

void deHeightTerrainSector::SetAllFacesVisible(bool visible){
	if(visible){
		pVisibleFaces.SetCountDiscard(0);
		
	}else{
		pCreateVisibleFaces();
		pVisibleFaces.SetRangeAt(0, pVisibleFaces.GetCount(), 0);
	}
}



// Textures
/////////////

void deHeightTerrainSector::AddTexture(deHeightTerrainTexture *texture){
	DEASSERT_NOTNULL(texture)
	pTextures.AddOrThrow(texture);
	
	if(pParentHeightTerrain){
		pParentHeightTerrain->NotifySectorChanged(pIndex);
	}
}

void deHeightTerrainSector::RemoveTexture(deHeightTerrainTexture *texture){
	pTextures.RemoveOrThrow(texture);
	delete texture;
	
	if(pParentHeightTerrain){
		pParentHeightTerrain->NotifySectorChanged(pIndex);
	}
}

void deHeightTerrainSector::RemoveAllTextures(){
	pTextures.Visit([](deHeightTerrainTexture *texture){
		delete texture;
	});
	pTextures.RemoveAll();
	
	if(pParentHeightTerrain){
		pParentHeightTerrain->NotifySectorChanged(pIndex);
	}
}



// Decals
///////////

void deHeightTerrainSector::AddDecal(deDecal *decal){
	if(!decal || decal->GetParentComponent() || decal->GetParentHeightTerrainSector()){
		DETHROW(deeInvalidParam);
	}
	
	pDecals.Add(&decal->GetLLHeightTerrainSector());
	decal->SetParentHeightTerrainSector(this);
	
	if(pParentHeightTerrain){
		if(pParentHeightTerrain->GetPeerGraphic()){
			pParentHeightTerrain->GetPeerGraphic()->DecalAdded(pIndex, decal);
		}
		if(pParentHeightTerrain->GetPeerPhysics()){
			pParentHeightTerrain->GetPeerPhysics()->DecalAdded(pIndex, decal);
		}
	}
}

void deHeightTerrainSector::RemoveDecal(deDecal *decal){
	if(!decal || decal->GetParentHeightTerrainSector() != this){
		DETHROW(deeInvalidParam);
	}
	
	decal->SetParentHeightTerrainSector(nullptr);
	pDecals.Remove(&decal->GetLLHeightTerrainSector());
	
	if(pParentHeightTerrain){
		if(pParentHeightTerrain->GetPeerGraphic()){
			pParentHeightTerrain->GetPeerGraphic()->DecalRemoved(pIndex, decal);
		}
		if(pParentHeightTerrain->GetPeerPhysics()){
			pParentHeightTerrain->GetPeerPhysics()->DecalRemoved(pIndex, decal);
		}
	}
}

void deHeightTerrainSector::RemoveAllDecals(){
	pDecals.Visit([](deDecal *decal){
		decal->SetParentHeightTerrainSector(nullptr);
	});
	pDecals.RemoveAll();
	
	if(pParentHeightTerrain){
		if(pParentHeightTerrain->GetPeerGraphic()){
			pParentHeightTerrain->GetPeerGraphic()->AllDecalsRemoved(pIndex);
		}
		if(pParentHeightTerrain->GetPeerPhysics()){
			pParentHeightTerrain->GetPeerPhysics()->AllDecalsRemoved(pIndex);
		}
	}
}



// Navigation spaces
//////////////////////

void deHeightTerrainSector::AddNavSpace(deHeightTerrainNavSpace *navspace){
	DEASSERT_NOTNULL(navspace)
	pNavSpaces.AddOrThrow(navspace);
	
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->NavSpaceAdded(pIndex, navspace);
	}
}

void deHeightTerrainSector::RemoveNavSpace(deHeightTerrainNavSpace *navspace){
	const int index = pNavSpaces.IndexOf(navspace);
	DEASSERT_TRUE(index != -1)
	
	pNavSpaces.RemoveFrom(index);
	
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->NavSpaceRemoved(pIndex, index);
	}
	
	delete navspace;
}

void deHeightTerrainSector::RemoveAllNavSpaces(){
	if(pNavSpaces.IsEmpty()){
		return;
	}
	
	pNavSpaces.Visit([](deHeightTerrainNavSpace *navspace){
		delete navspace;
	});
	pNavSpaces.RemoveAll();
	
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->AllNavSpacesRemoved(pIndex);
	}
}

void deHeightTerrainSector::NotifyNavSpaceLayerChanged(int index){
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->NavSpaceLayerChanged(pIndex, index);
	}
}

void deHeightTerrainSector::NotifyNavSpaceTypeChanged(int index){
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->NavSpaceTypeChanged(pIndex, index);
	}
}

void deHeightTerrainSector::NotifyNavSpaceSnappingChanged(int index){
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->NavSpaceSnappingChanged(pIndex, index);
	}
}

void deHeightTerrainSector::NotifyNavSpaceLayoutChanged(int index){
	if(pParentHeightTerrain && pParentHeightTerrain->GetPeerAI()){
		pParentHeightTerrain->GetPeerAI()->NavSpaceLayoutChanged(pIndex, index);
	}
}



// Private Functions
//////////////////////

void deHeightTerrainSector::pCreateVisibleFaces(){
	if(pVisibleFaces.IsNotEmpty()){
		return;
	}
	
	DEASSERT_NOTNULL(pParentHeightTerrain)
	
	const int pointCount = pParentHeightTerrain->GetSectorResolution();
	const int byteCount = ((pointCount * pointCount - 1) >> 3) + 1;
	
	pVisibleFaces.SetAll(byteCount, 255);
}
