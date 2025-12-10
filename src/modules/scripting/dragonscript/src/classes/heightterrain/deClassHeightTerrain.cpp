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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "deClassHeightTerrain.h"
#include "../graphics/deClassImage.h"
#include "../world/deClassSkin.h"
#include "../math/deClassVector.h"
#include "../math/deClassVector2.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../math/deClassPoint.h"
#include "../world/deClassDecal.h"
#include "../physics/deClassCollisionFilter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include "dragengine/deEngine.h"
#include "dragengine/resources/decal/deDecal.h"
#include "dragengine/resources/image/deImage.h"
#include "dragengine/resources/image/deImageManager.h"
#include "dragengine/resources/skin/deSkin.h"
#include "dragengine/resources/skin/deSkinManager.h"
#include "dragengine/resources/terrain/heightmap/deHeightTerrain.h"
#include "dragengine/resources/terrain/heightmap/deHeightTerrainManager.h"
#include "dragengine/resources/terrain/heightmap/deHeightTerrainSector.h"
#include "dragengine/resources/terrain/heightmap/deHeightTerrainTexture.h"
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sHTNatDat{
	deHeightTerrain::Ref hterrain;
};



// Constructors, Destructors
//////////////////////////////

// public func new( float sectorSize, int sectorResolution )
deClassHeightTerrain::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsHT,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // sectorSize
	p_AddParameter(init.clsInt); // sectorResolution
}
void deClassHeightTerrain::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sHTNatDat * const nd = new (p_GetNativeData(myself)) sHTNatDat;
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const float sectorSize = rt->GetValue(0)->GetFloat();
	const int sectorResolution = rt->GetValue(1)->GetInt();
	nd->hterrain = ds.GetGameEngine()->GetHeightTerrainManager()->
		CreateHeightTerrain(sectorSize, sectorResolution);
}

// public func destructor()
deClassHeightTerrain::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsHT,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassHeightTerrain::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sHTNatDat*>(p_GetNativeData(myself))->~sHTNatDat();
}



// Management
///////////////

// public func float getSectorSize()
deClassHeightTerrain::nfGetSectorSize::nfGetSectorSize(const sInitData &init) : dsFunction(init.clsHT,
"getSectorSize", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassHeightTerrain::nfGetSectorSize::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	rt->PushFloat(hterrain.GetSectorSize());
}

// public func int getSectorResolution()
deClassHeightTerrain::nfGetSectorResolution::nfGetSectorResolution(const sInitData &init) : dsFunction(init.clsHT,
"getSectorResolution", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassHeightTerrain::nfGetSectorResolution::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	rt->PushInt(hterrain.GetSectorResolution());
}



// public func float getBaseHeight()
deClassHeightTerrain::nfGetBaseHeight::nfGetBaseHeight(const sInitData &init) : dsFunction(init.clsHT,
"getBaseHeight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassHeightTerrain::nfGetBaseHeight::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	rt->PushFloat(hterrain.GetBaseHeight());
}

// public func void setBaseHeight( float height )
deClassHeightTerrain::nfSetBaseHeight::nfSetBaseHeight(const sInitData &init) : dsFunction(init.clsHT,
"setBaseHeight", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // height
}
void deClassHeightTerrain::nfSetBaseHeight::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	hterrain.SetBaseHeight(rt->GetValue(0)->GetFloat());
}

// public func float getHeightScaling()
deClassHeightTerrain::nfGetHeightScaling::nfGetHeightScaling(const sInitData &init) : dsFunction(init.clsHT,
"getHeightScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassHeightTerrain::nfGetHeightScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	rt->PushFloat(hterrain.GetHeightScaling());
}

// public func void setHeightScaling( float scaling )
deClassHeightTerrain::nfSetHeightScaling::nfSetHeightScaling(const sInitData &init) : dsFunction(init.clsHT,
"setHeightScaling", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // scaling
}
void deClassHeightTerrain::nfSetHeightScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	hterrain.SetHeightScaling(rt->GetValue(0)->GetFloat());
}

// public func CollisionFilter getCollisionFilter()
deClassHeightTerrain::nfGetCollisionFilter::nfGetCollisionFilter(const sInitData &init) : dsFunction(init.clsHT,
"getCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsCF){
}
void deClassHeightTerrain::nfGetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deClassCollisionFilter &clsCF = *(((deClassHeightTerrain*)GetOwnerClass())->GetDS().GetClassCollisionFilter());
	
	clsCF.PushCollisionFilter(rt, hterrain.GetCollisionFilter());
}

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassHeightTerrain::nfSetCollisionFilter::nfSetCollisionFilter(const sInitData &init) : dsFunction(init.clsHT,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
}
void deClassHeightTerrain::nfSetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deClassCollisionFilter &clsCF = *(((deClassHeightTerrain*)GetOwnerClass())->GetDS().GetClassCollisionFilter());
	
	const decCollisionFilter &collisionFilter = clsCF.GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	
	hterrain.SetCollisionFilter(collisionFilter);
}



// Sectors
////////////

// public func int getSectorCount()
deClassHeightTerrain::nfGetSectorCount::nfGetSectorCount(const sInitData &init) :
dsFunction(init.clsHT, "getSectorCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassHeightTerrain::nfGetSectorCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	rt->PushInt(hterrain.GetSectorCount());
}

// public func bool hasSector( Point coordinates )
deClassHeightTerrain::nfHasSector::nfHasSector(const sInitData &init) :
dsFunction(init.clsHT, "hasSector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // coordinates
}
void deClassHeightTerrain::nfHasSector::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const decPoint &scoord = ds.GetClassPoint()->GetPoint(rt->GetValue(0)->GetRealObject());
	rt->PushBool(hterrain.GetSectorWith(scoord));
}

// public func int indexOfSector( Point coordinates )
deClassHeightTerrain::nfIndexOfSector::nfIndexOfSector(const sInitData &init) :
dsFunction(init.clsHT, "indexOfSector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsPt); // coordinates
}
void deClassHeightTerrain::nfIndexOfSector::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const decPoint &scoord = ds.GetClassPoint()->GetPoint(rt->GetValue(0)->GetRealObject());
	deHeightTerrainSector * const sector = hterrain.GetSectorWith(scoord);
	rt->PushInt(sector ? sector->GetIndex() : -1);
}

// public func void addSector( Point coordinates )
deClassHeightTerrain::nfAddSector::nfAddSector(const sInitData &init) :
dsFunction(init.clsHT, "addSector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPt); // coordinates
}
void deClassHeightTerrain::nfAddSector::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const decPoint &scoord = ds.GetClassPoint()->GetPoint(rt->GetValue(0)->GetRealObject());
	if(hterrain.GetSectorWith(scoord)){
		DSTHROW(dueInvalidParam);
	}
	
	// create sector and add it
	deHeightTerrainSector *sector = NULL;
	try{
		sector = new deHeightTerrainSector(scoord);
		hterrain.AddSector(sector);
		
	}catch(...){
		if(sector){
			delete sector;
		}
		throw;
	}
}

// public func void removeSector( int sector )
deClassHeightTerrain::nfRemoveSector::nfRemoveSector(const sInitData &init) :
dsFunction(init.clsHT, "removeSector", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfRemoveSector::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	hterrain.RemoveSector(hterrain.GetSectorAt(rt->GetValue(0)->GetInt()));
}

// public func void removeAllSectors()
deClassHeightTerrain::nfRemoveAllSectors::nfRemoveAllSectors(const sInitData &init) :
dsFunction(init.clsHT, "removeAllSectors", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassHeightTerrain::nfRemoveAllSectors::RunFunction(dsRunTime*, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	hterrain.RemoveAllSectors();
}

// public func Image getHeightImage( int sector )
deClassHeightTerrain::nfGetHeightImage::nfGetHeightImage(const sInitData &init) :
dsFunction(init.clsHT, "getHeightImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImg){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfGetHeightImage::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	ds.GetClassImage()->PushImage(rt, hterrain.GetSectorAt(rt->GetValue(0)->GetInt())->GetHeightImage());
}

// public func void setHeightImage( int sector, Image image )
deClassHeightTerrain::nfSetHeightImage::nfSetHeightImage(const sInitData &init) :
dsFunction(init.clsHT, "setHeightImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsImg); // image
}
void deClassHeightTerrain::nfSetHeightImage::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	hterrain.GetSectorAt(rt->GetValue(0)->GetInt())->SetHeightImage(
		ds.GetClassImage()->GetImage(rt->GetValue(1)->GetRealObject()));
}

// public func void setVisibilityFromImage( int sector, Image image )
deClassHeightTerrain::nfSetVisibilityFromImage::nfSetVisibilityFromImage(const sInitData &init) :
dsFunction(init.clsHT, "setVisibilityFromImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsImg); // image
}
void deClassHeightTerrain::nfSetVisibilityFromImage::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	
	deImage * const image = ds.GetClassImage()->GetImage(rt->GetValue(1)->GetRealObject());
	const int imageDim = hterrain.GetSectorResolution();
	int x, y;
	
	if(image){
		if(image->GetWidth() != imageDim || image->GetHeight() != imageDim){
			DSTHROW_INFO(dueInvalidParam, "image size not matching");
		}
		if(image->GetComponentCount() != 1){
			DSTHROW_INFO(dueInvalidParam, "image component count is not 1");
		}
		
		switch(image->GetBitCount()){
		case 8:{
			const sGrayscale8 *data = image->GetDataGrayscale8();
			for(y=0; y<imageDim; y++){
				for(x=0; x<imageDim; x++){
					sector.SetFaceVisibleAt(x, y, data->value > HT_8BIT_BASE);
					data++;
				}
			}
			}break;
			
		case 16:{
			const sGrayscale16 *data = image->GetDataGrayscale16();
			for(y=0; y<imageDim; y++){
				for(x=0; x<imageDim; x++){
					sector.SetFaceVisibleAt(x, y, data->value > HT_16BIT_BASE);
					data++;
				}
			}
			}break;
			
		case 32:{
			const sGrayscale32 *data = image->GetDataGrayscale32();
			for(y=0; y<imageDim; y++){
				for(x=0; x<imageDim; x++){
					sector.SetFaceVisibleAt(x, y, data->value > 0.0f);
					data++;
				}
			}
			}break;
			
		default:
			DSTHROW_INFO(dueInvalidParam, "unsupported image format");
		}
		
	}else{
		for(y=0; y<imageDim; y++){
			for(x=0; x<imageDim; x++){
				sector.SetFaceVisibleAt(x, y, true);
			}
		}
	}
	
	hterrain.NotifySectorChanged(sector.GetIndex());
}



// Textures
/////////////

// public func int getTextureCount( int sector )
deClassHeightTerrain::nfGetTextureCount::nfGetTextureCount(const sInitData &init) :
dsFunction(init.clsHT, "getTextureCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfGetTextureCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	rt->PushInt(hterrain.GetSectorAt(rt->GetValue(0)->GetInt())->GetTextureCount());
}

// public func void addTexture( int sector )
deClassHeightTerrain::nfAddTexture::nfAddTexture(const sInitData &init) :
dsFunction(init.clsHT, "addTexture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfAddTexture::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	
	deHeightTerrainTexture *texture = NULL;
	try{
		texture = new deHeightTerrainTexture;
		sector.AddTexture(texture);
		
	}catch(...){
		if(texture){
			delete texture;
		}
		throw;
	}
}

// public func Skin textureGetSkin( int sector, int texture )
deClassHeightTerrain::nfTextureGetSkin::nfTextureGetSkin(const sInitData &init) :
dsFunction(init.clsHT, "textureGetSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSkin){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
}
void deClassHeightTerrain::nfTextureGetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	ds.GetClassSkin()->PushSkin(rt, texture.GetSkin());
}

// public func void textureSetSkin( int sector, int texture, Skin skin )
deClassHeightTerrain::nfTextureSetSkin::nfTextureSetSkin(const sInitData &init) :
dsFunction(init.clsHT, "textureSetSkin", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
	p_AddParameter(init.clsSkin); // skin
}
void deClassHeightTerrain::nfTextureSetSkin::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	texture.SetSkin(ds.GetClassSkin()->GetSkin(rt->GetValue(2)->GetRealObject()));
	hterrain.NotifySectorChanged(sector.GetIndex());
}

// public func Vector2 textureGetUVOffset( int sector, int texture )
deClassHeightTerrain::nfTextureGetUVOffset::nfTextureGetUVOffset(const sInitData &init) :
dsFunction(init.clsHT, "textureGetUVOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
}
void deClassHeightTerrain::nfTextureGetUVOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	
	ds.GetClassVector2()->PushVector2(rt, texture.GetProjectionOffset());
}

// public func void textureSetUVOffset( int sector, int texture, Vector2 offset )
deClassHeightTerrain::nfTextureSetUVOffset::nfTextureSetUVOffset(const sInitData &init) :
dsFunction(init.clsHT, "textureSetUVOffset", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
	p_AddParameter(init.clsVec2); // offset
}
void deClassHeightTerrain::nfTextureSetUVOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	texture.SetProjectionOffset(ds.GetClassVector2()->GetVector2(rt->GetValue(2)->GetRealObject()));
	hterrain.NotifySectorChanged(sector.GetIndex());
}

// public func Vector2 textureGetUVScaling( int sector, int texture )
deClassHeightTerrain::nfTextureGetUVScaling::nfTextureGetUVScaling(const sInitData &init) :
dsFunction(init.clsHT, "textureGetUVScaling", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVec2){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
}
void deClassHeightTerrain::nfTextureGetUVScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	
	ds.GetClassVector2()->PushVector2(rt, texture.GetProjectionScaling());
}

// public func void textureSetUVScaling( int sector, int texture, Vector2 scaling )
deClassHeightTerrain::nfTextureSetUVScaling::nfTextureSetUVScaling(const sInitData &init) :
dsFunction(init.clsHT, "textureSetUVScaling", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
	p_AddParameter(init.clsVec2); // scaling
}
void deClassHeightTerrain::nfTextureSetUVScaling::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	texture.SetProjectionScaling(ds.GetClassVector2()->GetVector2(rt->GetValue(2)->GetRealObject()));
	hterrain.NotifySectorChanged(sector.GetIndex());
}

// public func float textureGetUVRotation( int sector, int texture )
deClassHeightTerrain::nfTextureGetUVRotation::nfTextureGetUVRotation(const sInitData &init) :
dsFunction(init.clsHT, "textureGetUVRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
}
void deClassHeightTerrain::nfTextureGetUVRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	rt->PushFloat(texture.GetProjectionRotation() * RAD2DEG);
}

// public func void textureSetUVRotation( int sector, int texture, float rotation )
deClassHeightTerrain::nfTextureSetUVRotation::nfTextureSetUVRotation(const sInitData &init) :
dsFunction(init.clsHT, "textureSetUVRotation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
	p_AddParameter(init.clsFlt); // rotation
}
void deClassHeightTerrain::nfTextureSetUVRotation::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	texture.SetProjectionRotation(rt->GetValue(2)->GetFloat() * DEG2RAD);
	hterrain.NotifySectorChanged(sector.GetIndex());
}

// public func Image textureGetMask( int sector, int texture )
deClassHeightTerrain::nfTextureGetMask::nfTextureGetMask(const sInitData &init) :
dsFunction(init.clsHT, "textureGetMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImg){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
}
void deClassHeightTerrain::nfTextureGetMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	ds.GetClassImage()->PushImage(rt, texture.GetMaskImage());
}

// public func void textureSetMask( int sector, int texture, Image mask )
deClassHeightTerrain::nfTextureSetMask::nfTextureSetMask(const sInitData &init) :
dsFunction(init.clsHT, "textureSetMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // texture
	p_AddParameter(init.clsImg); // mask
}
void deClassHeightTerrain::nfTextureSetMask::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainTexture &texture = *sector.GetTextureAt(rt->GetValue(1)->GetInt());
	texture.SetMaskImage(ds.GetClassImage()->GetImage(rt->GetValue(2)->GetRealObject()));
	hterrain.NotifySectorChanged(sector.GetIndex());
}



// Decals
///////////

// public func int getDecalCount( int sector )
deClassHeightTerrain::nfGetDecalCount::nfGetDecalCount(const sInitData &init) :
dsFunction(init.clsHT,"getDecalCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfGetDecalCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	rt->PushInt(sector.GetDecalCount());
}

// public func void addDecal( int sector, Decal decal )
deClassHeightTerrain::nfAddDecal::nfAddDecal(const sInitData &init) :
dsFunction(init.clsHT, "addDecal", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsDec); // decal
}
void deClassHeightTerrain::nfAddDecal::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	sector.AddDecal(ds.GetClassDecal()->GetDecal(rt->GetValue(1)->GetRealObject()));
}

// public func void removeDecal( int sector, Decal decal )
deClassHeightTerrain::nfRemoveDecal::nfRemoveDecal(const sInitData &init) :
dsFunction(init.clsHT, "removeDecal", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsDec); // decal
}
void deClassHeightTerrain::nfRemoveDecal::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deScriptingDragonScript &ds = ((deClassHeightTerrain*)GetOwnerClass())->GetDS();
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	sector.RemoveDecal(ds.GetClassDecal()->GetDecal(rt->GetValue(1)->GetRealObject()));
}

// public func void removeAllDecals( int sector )
deClassHeightTerrain::nfRemoveAllDecals::nfRemoveAllDecals(const sInitData &init) :
dsFunction(init.clsHT, "removeAllDecals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfRemoveAllDecals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	sector.RemoveAllDecals();
}



// Navigation spaces
//////////////////////

// public func int getNavSpaceCount( int sector )
deClassHeightTerrain::nfGetNavSpaceCount::nfGetNavSpaceCount(const sInitData &init) :
dsFunction(init.clsHT, "getNavSpaceCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfGetNavSpaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	rt->PushInt(sector.GetNavSpaceCount());
}

// public func void addNavSpace( int sector )
deClassHeightTerrain::nfAddNavSpace::nfAddNavSpace(const sInitData &init) :
dsFunction(init.clsHT, "addNavSpace", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfAddNavSpace::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	
	deHeightTerrainNavSpace *navspace = NULL;
	try{
		navspace = new deHeightTerrainNavSpace;
		sector.AddNavSpace(navspace);
		
	}catch(...){
		DSTHROW(dueInvalidParam);
	}
}

// public func void removeNavSpaceAt( int sector, int navspace )
deClassHeightTerrain::nfRemoveNavSpaceAt::nfRemoveNavSpaceAt(const sInitData &init) :
dsFunction(init.clsHT, "removeNavSpaceAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfRemoveNavSpaceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	sector.RemoveNavSpace(sector.GetNavSpaceAt(rt->GetValue(1)->GetInt()));
}

// public func void removeAllNavSpaces( int sector )
deClassHeightTerrain::nfRemoveAllNavSpaces::nfRemoveAllNavSpaces(const sInitData &init) :
dsFunction(init.clsHT, "removeAllNavSpaces", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
}
void deClassHeightTerrain::nfRemoveAllNavSpaces::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	sector.RemoveAllNavSpaces();
}



// public func NavigationSpaceType navspaceGetType( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetType::nfNavSpaceGetType(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsNavigationSpaceType); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	
	rt->PushValue(((deClassHeightTerrain*)GetOwnerClass())->GetClassNavigationSpaceType()
		->GetVariable(navspace.GetType())->GetStaticValue());
}

// public func void navspaceSetType( int sector, int navspace, NavigationSpaceType type )
deClassHeightTerrain::nfNavSpaceSetType::nfNavSpaceSetType(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsNavigationSpaceType); // type
}
void deClassHeightTerrain::nfNavSpaceSetType::RunFunction(dsRunTime *rt, dsValue *myself){
	deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const int index = rt->GetValue(1)->GetInt();
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(index);
	
	if(!rt->GetValue(2)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	navspace.SetType((deNavigationSpace::eSpaceTypes)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 2 )->GetRealObject() ) );
	sector.NotifyNavSpaceTypeChanged(index);
}

// public func int navspaceGetLayer( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetLayer::nfNavSpaceGetLayer(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetLayer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetLayer::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetLayer());
}

// public func void navspaceSetLayer( int sector, int navspace, int layer )
deClassHeightTerrain::nfNavSpaceSetLayer::nfNavSpaceSetLayer(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetLayer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // layer
}
void deClassHeightTerrain::nfNavSpaceSetLayer::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const int index = rt->GetValue(1)->GetInt();
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(index);
	
	navspace.SetLayer(rt->GetValue(2)->GetInt());
	sector.NotifyNavSpaceLayerChanged(index);
}

// public func float navspaceGetSnapDistance( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetSnapDistance::nfNavSpaceGetSnapDistance(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetSnapDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetSnapDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushFloat(navspace.GetSnapDistance());
}

// public func void navspaceSetSnapDistance( int sector, int navspace, float distance )
deClassHeightTerrain::nfNavSpaceSetSnapDistance::nfNavSpaceSetSnapDistance(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetSnapDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsFlt); // distance
}
void deClassHeightTerrain::nfNavSpaceSetSnapDistance::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const int index = rt->GetValue(1)->GetInt();
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(index);
	
	navspace.SetSnapDistance(rt->GetValue(2)->GetFloat());
	sector.NotifyNavSpaceSnappingChanged(index);
}

// public func float navspaceGetSnapAngle( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetSnapAngle::nfNavSpaceGetSnapAngle(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetSnapAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetSnapAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushFloat(navspace.GetSnapAngle() * RAD2DEG);
}

// public func void navspaceSetSnapAngle( int sector, int navspace, float angle )
deClassHeightTerrain::nfNavSpaceSetSnapAngle::nfNavSpaceSetSnapAngle(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetSnapAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsFlt); // angle
}
void deClassHeightTerrain::nfNavSpaceSetSnapAngle::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const int index = rt->GetValue(1)->GetInt();
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(index);
	
	navspace.SetSnapAngle(rt->GetValue(2)->GetFloat() * DEG2RAD);
	sector.NotifyNavSpaceSnappingChanged(index);
}



// public func int navspaceGetCornerCount( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetCornerCount::nfNavSpaceGetCornerCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetCornerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetCornerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetCornerCount());
}

// public func void navspaceSetCornerCount( int sector, int navspace, int count )
deClassHeightTerrain::nfNavSpaceSetCornerCount::nfNavSpaceSetCornerCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetCornerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // count
}
void deClassHeightTerrain::nfNavSpaceSetCornerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	navspace.SetCornerCount(rt->GetValue(2)->GetInt());
}

// public func int navspaceGetCornerAt( int sector, int navspace, int corner )
deClassHeightTerrain::nfNavSpaceGetCornerAt::nfNavSpaceGetCornerAt(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetCornerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // index
}
void deClassHeightTerrain::nfNavSpaceGetCornerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetCornerAt(rt->GetValue(2)->GetInt()));
}

// public func void navspaceSetCornerAt( int sector, int navspace, int corner, int navpoint )
deClassHeightTerrain::nfNavSpaceSetCornerAt::nfNavSpaceSetCornerAt(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetCornerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // corner
	p_AddParameter(init.clsInt); // navpoint
}
void deClassHeightTerrain::nfNavSpaceSetCornerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	
	const int corner = rt->GetValue(2)->GetInt();
	const unsigned int navpoint = (unsigned int)rt->GetValue(3)->GetInt();
	navspace.SetCornerAt(corner, navpoint);
}



// public func int navspaceGetEdgeCount( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetEdgeCount::nfNavSpaceGetEdgeCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetEdgeCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetEdgeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetEdgeCount());
}

// public func void navspaceSetEdgeCount( int sector, int navspace, int count )
deClassHeightTerrain::nfNavSpaceSetEdgeCount::nfNavSpaceSetEdgeCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetEdgeCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // count
}
void deClassHeightTerrain::nfNavSpaceSetEdgeCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	navspace.SetEdgeCount(rt->GetValue(2)->GetInt());
}

// public func int navspaceEdgeGetPoint1( int sector, int navspace, int edge )
deClassHeightTerrain::nfNavSpaceEdgeGetPoint1::nfNavSpaceEdgeGetPoint1(const sInitData &init) :
dsFunction(init.clsHT, "navspaceEdgeGetPoint1", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // edge
}
void deClassHeightTerrain::nfNavSpaceEdgeGetPoint1::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetEdgeAt(rt->GetValue(2)->GetInt()).GetPoint1());
}

// public func int navspaceEdgeGetPoint2( int sector, int navspace, int edge )
deClassHeightTerrain::nfNavSpaceEdgeGetPoint2::nfNavSpaceEdgeGetPoint2(const sInitData &init) :
dsFunction(init.clsHT, "navspaceEdgeGetPoint2", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // edge
}
void deClassHeightTerrain::nfNavSpaceEdgeGetPoint2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetEdgeAt(rt->GetValue(2)->GetInt()).GetPoint2());
}

// public func int navspaceEdgeGetType1( int sector, int navspace, int edge )
deClassHeightTerrain::nfNavSpaceEdgeGetType1::nfNavSpaceEdgeGetType1(const sInitData &init) :
dsFunction(init.clsHT, "navspaceEdgeGetType1", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // edge
}
void deClassHeightTerrain::nfNavSpaceEdgeGetType1::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetEdgeAt(rt->GetValue(2)->GetInt()).GetType1());
}

// public func int navspaceEdgeGetType2( int sector, int navspace, int edge )
deClassHeightTerrain::nfNavSpaceEdgeGetType2::nfNavSpaceEdgeGetType2(const sInitData &init) :
dsFunction(init.clsHT, "navspaceEdgeGetType2", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // edge
}
void deClassHeightTerrain::nfNavSpaceEdgeGetType2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetEdgeAt(rt->GetValue(2)->GetInt()).GetType2());
}

// public func void navspaceSetEdgeAt( int sector, int navspace, int edge, int point1, int point2, int type1, int type2 )
deClassHeightTerrain::nfNavSpaceSetEdgeAt::nfNavSpaceSetEdgeAt(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetEdgeAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // edge
	p_AddParameter(init.clsInt); // point1
	p_AddParameter(init.clsInt); // point2
	p_AddParameter(init.clsInt); // type1
	p_AddParameter(init.clsInt); // type2
}
void deClassHeightTerrain::nfNavSpaceSetEdgeAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	
	const int edgeIndex = rt->GetValue(2)->GetInt();
	const unsigned int point1 = (unsigned int)rt->GetValue(3)->GetInt();
	const unsigned int point2 = (unsigned int)rt->GetValue(4)->GetInt();
	const unsigned short type1 = (unsigned short)rt->GetValue(5)->GetInt();
	const unsigned short type2 = (unsigned short)rt->GetValue(6)->GetInt();
	
	deHeightTerrainNavSpaceEdge &edge = navspace.GetEdgeAt(edgeIndex);
	edge.SetPoint1(point1);
	edge.SetPoint2(point2);
	edge.SetType1(type1);
	edge.SetType2(type2);
}



// public func int navspaceGetFaceCount( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceGetFaceCount::nfNavSpaceGetFaceCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceGetFaceCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceGetFaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetFaceCount());
}

// public func void navspaceSetFaceCount( int sector, int navspace, int count )
deClassHeightTerrain::nfNavSpaceSetFaceCount::nfNavSpaceSetFaceCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetFaceCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // count
}
void deClassHeightTerrain::nfNavSpaceSetFaceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	navspace.SetFaceCount(rt->GetValue(2)->GetInt());
}

// public func int navspaceFaceGetCornerCount( int sector, int navspace, int face )
deClassHeightTerrain::nfNavSpaceFaceGetCornerCount::nfNavSpaceFaceGetCornerCount(const sInitData &init) :
dsFunction(init.clsHT, "navspaceFaceGetCornerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // face
}
void deClassHeightTerrain::nfNavSpaceFaceGetCornerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	const deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetFaceAt(rt->GetValue(2)->GetInt()).GetCornerCount());
}

// public func int navspaceFaceGetType( int sector, int navspace, int face )
deClassHeightTerrain::nfNavSpaceFaceGetType::nfNavSpaceFaceGetType(const sInitData &init) :
dsFunction(init.clsHT, "navspaceFaceGetType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // face
}
void deClassHeightTerrain::nfNavSpaceFaceGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	const deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	rt->PushInt(navspace.GetFaceAt(rt->GetValue(2)->GetInt()).GetType());
}

// public func void navspaceSetFaceAt( int sector, int navspace, int face, int cornerCount, int type )
deClassHeightTerrain::nfNavSpaceSetFaceAt::nfNavSpaceSetFaceAt(const sInitData &init) :
dsFunction(init.clsHT, "navspaceSetFaceAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
	p_AddParameter(init.clsInt); // face
	p_AddParameter(init.clsInt); // cornerCount
	p_AddParameter(init.clsInt); // type
}
void deClassHeightTerrain::nfNavSpaceSetFaceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	deHeightTerrainNavSpace &navspace = *sector.GetNavSpaceAt(rt->GetValue(1)->GetInt());
	
	const int faceIndex = rt->GetValue(2)->GetInt();
	const unsigned short cornerCount = (unsigned short)rt->GetValue(3)->GetInt();
	const unsigned short type = (unsigned short)rt->GetValue(4)->GetInt();
	
	deNavigationSpaceFace &face = navspace.GetFaceAt(faceIndex);
	face.SetCornerCount(cornerCount);
	face.SetType(type);
}

// public func void navspaceLayoutChanged( int sector, int navspace )
deClassHeightTerrain::nfNavSpaceLayoutChanged::nfNavSpaceLayoutChanged(const sInitData &init) :
dsFunction(init.clsHT, "navspaceLayoutChanged", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sector
	p_AddParameter(init.clsInt); // navspace
}
void deClassHeightTerrain::nfNavSpaceLayoutChanged::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain &hterrain = *(static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain);
	deHeightTerrainSector &sector = *hterrain.GetSectorAt(rt->GetValue(0)->GetInt());
	sector.NotifyNavSpaceLayoutChanged(rt->GetValue(1)->GetInt());
}



// Special
////////////

// public func int hashCode()
deClassHeightTerrain::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsHT, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassHeightTerrain::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain * const hterrain = static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain;
	rt->PushInt((int)(intptr_t)hterrain);
}

// public func bool equals( Object object )
deClassHeightTerrain::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsHT, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassHeightTerrain::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deHeightTerrain * const hterrain = static_cast<sHTNatDat*>(p_GetNativeData(myself))->hterrain;
	deClassHeightTerrain * const clsHT = (deClassHeightTerrain*)GetOwnerClass();
	dsValue *object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsHT)){
		rt->PushBool(false);
		
	}else{
		const deHeightTerrain * const otherHT = static_cast<sHTNatDat*>(p_GetNativeData(object))->hterrain;
		rt->PushBool(hterrain == otherHT);
	}
}



// Class deClassHeightTerrain
///////////////////////////////

// Constructor
////////////////

deClassHeightTerrain::deClassHeightTerrain(deScriptingDragonScript &ds) :
dsClass("HeightTerrain", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	p_SetNativeDataSize(sizeof(sHTNatDat));
}

deClassHeightTerrain::~deClassHeightTerrain(){
}



// Management
///////////////

void deClassHeightTerrain::CreateClassMembers(dsEngine *engine){
	pClsNavigationSpaceType = engine->GetClass("Dragengine.Scenery.NavigationSpaceType");
	
	sInitData init;
	init.clsHT = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsDec = pDS.GetClassDecal();
	init.clsDVec = pDS.GetClassDVector();
	init.clsImg = pDS.GetClassImage();
	init.clsQuat = pDS.GetClassQuaternion();
	init.clsPt = pDS.GetClassPoint();
	init.clsSkin = pDS.GetClassSkin();
	init.clsVec = pDS.GetClassVector();
	init.clsVec2 = pDS.GetClassVector2();
	init.clsCF = pDS.GetClassCollisionFilter();
	init.clsNavigationSpaceType = pClsNavigationSpaceType;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSectorResolution(init));
	AddFunction(new nfGetSectorSize(init));
	
	AddFunction(new nfGetBaseHeight(init));
	AddFunction(new nfSetBaseHeight(init));
	AddFunction(new nfGetHeightScaling(init));
	AddFunction(new nfSetHeightScaling(init));
	AddFunction(new nfGetCollisionFilter(init));
	AddFunction(new nfSetCollisionFilter(init));
	
	// sectors
	AddFunction(new nfGetSectorCount(init));
	AddFunction(new nfHasSector(init));
	AddFunction(new nfIndexOfSector(init));
	AddFunction(new nfAddSector(init));
	AddFunction(new nfRemoveSector(init));
	AddFunction(new nfRemoveAllSectors(init));
	
	AddFunction(new nfGetHeightImage(init));
	AddFunction(new nfSetHeightImage(init));
	
	AddFunction(new nfSetVisibilityFromImage(init));
	
	// textures
	AddFunction(new nfGetTextureCount(init));
	AddFunction(new nfAddTexture(init));
	AddFunction(new nfTextureGetSkin(init));
	AddFunction(new nfTextureSetSkin(init));
	AddFunction(new nfTextureGetUVOffset(init));
	AddFunction(new nfTextureSetUVOffset(init));
	AddFunction(new nfTextureGetUVScaling(init));
	AddFunction(new nfTextureSetUVScaling(init));
	AddFunction(new nfTextureGetUVRotation(init));
	AddFunction(new nfTextureSetUVRotation(init));
	AddFunction(new nfTextureGetMask(init));
	AddFunction(new nfTextureSetMask(init));
	
	// decals
	AddFunction(new nfGetDecalCount(init));
	AddFunction(new nfAddDecal(init));
	AddFunction(new nfRemoveDecal(init));
	AddFunction(new nfRemoveAllDecals(init));
	
	// navspaces
	AddFunction(new nfGetNavSpaceCount(init));
	AddFunction(new nfAddNavSpace(init));
	AddFunction(new nfRemoveNavSpaceAt(init));
	AddFunction(new nfRemoveAllNavSpaces(init));
	
	AddFunction(new nfNavSpaceGetType(init));
	AddFunction(new nfNavSpaceSetType(init));
	AddFunction(new nfNavSpaceGetLayer(init));
	AddFunction(new nfNavSpaceSetLayer(init));
	AddFunction(new nfNavSpaceGetSnapDistance(init));
	AddFunction(new nfNavSpaceSetSnapDistance(init));
	AddFunction(new nfNavSpaceGetSnapAngle(init));
	AddFunction(new nfNavSpaceSetSnapAngle(init));
	
	AddFunction(new nfNavSpaceGetCornerCount(init));
	AddFunction(new nfNavSpaceSetCornerCount(init));
	AddFunction(new nfNavSpaceGetCornerAt(init));
	AddFunction(new nfNavSpaceSetCornerAt(init));
	
	AddFunction(new nfNavSpaceGetEdgeCount(init));
	AddFunction(new nfNavSpaceSetEdgeCount(init));
	AddFunction(new nfNavSpaceEdgeGetPoint1(init));
	AddFunction(new nfNavSpaceEdgeGetPoint2(init));
	AddFunction(new nfNavSpaceEdgeGetType1(init));
	AddFunction(new nfNavSpaceEdgeGetType2(init));
	AddFunction(new nfNavSpaceSetEdgeAt(init));
	
	AddFunction(new nfNavSpaceGetFaceCount(init));
	AddFunction(new nfNavSpaceSetFaceCount(init));
	AddFunction(new nfNavSpaceFaceGetCornerCount(init));
	AddFunction(new nfNavSpaceFaceGetType(init));
	AddFunction(new nfNavSpaceSetFaceAt(init));
	AddFunction(new nfNavSpaceLayoutChanged(init));
	
	// special
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deHeightTerrain *deClassHeightTerrain::GetHeightTerrain(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sHTNatDat*>(p_GetNativeData(myself->GetBuffer()))->hterrain;
}

void deClassHeightTerrain::PushHeightTerrain(dsRunTime *rt, deHeightTerrain *hterrain){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!hterrain){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	static_cast<sHTNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->hterrain = hterrain;
	hterrain->AddReference();
}
