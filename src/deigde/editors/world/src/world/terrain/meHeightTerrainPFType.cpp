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

#include "meHeightTerrainPFLayer.h"
#include "meHeightTerrainPFType.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrain.h"
#include "../meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainPFType
////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainPFType::meHeightTerrainPFType(deEngine *engine) :
pPFLayer(NULL),
pEngine(engine),
pRotationPerForce(5.0f),
pRestitution(0.5f),
pCoverageDensity(1), //10; // 10 props per 1m^2
pRandomScaleMin(0.5f),
pRandomScaleMax(1.5f),
pRandomRotationMin(0.0f, -30.0f, 0.0f),
pRandomRotationMax(0.0f, 30.0f, 0.0f)
{
	if(!engine){
		DETHROW(deeInvalidParam);
	}
}

meHeightTerrainPFType::~meHeightTerrainPFType(){
}



// Management
///////////////

void meHeightTerrainPFType::SetPFLayer(meHeightTerrainPFLayer *pflayer){
	pPFLayer = pflayer;
}

void meHeightTerrainPFType::NotifyTypeChanged(){
	if(!pPFLayer){
		return;
	}
	
	meHeightTerrainSector * const htsector = pPFLayer->GetHTSector();
	if(htsector == NULL){
		return;
	}
	
	if(htsector->GetHeightTerrain()){
		htsector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		htsector->GetHeightTerrain()->SetChanged(true);
	}
}



void meHeightTerrainPFType::SetPathModel(const char *path){
	if(!path){
		DETHROW(deeInvalidParam);
	}
	
	if(pPathModel.Equals(path)){
		return;
	}
	
	pPathModel = path;
	
	if(pModel){
		pModel = NULL;
	}
	
	if(!pPathModel.IsEmpty()){
		try{
			pModel = pEngine->GetModelManager()->LoadModel(pPathModel, "/");
			
		}catch(const deException &e){
			if(pPFLayer && pPFLayer->GetHTSector() && pPFLayer->GetHTSector()->GetHeightTerrain()){
				pPFLayer->GetHTSector()->GetHeightTerrain()->GetWorld().GetLogger()->LogException(LOGSOURCE, e);
			}
		}
	}
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetPathSkin(const char *path){
	if(!path){
		DETHROW(deeInvalidParam);
	}
	
	if(pPathSkin.Equals(path)){
		return;
	}
	
	pPathSkin = path;
	
	if(pSkin){
		pSkin = NULL;
	}
	
	if(!pPathSkin.IsEmpty()){
		try{
			pSkin = pEngine->GetSkinManager()->LoadSkin(pPathSkin.GetString(), "/");
			
		}catch(const deException &){
			if(pPFLayer && pPFLayer->GetHTSector() && pPFLayer->GetHTSector()->GetHeightTerrain()){
				pSkin = pPFLayer->GetHTSector()->GetHeightTerrain()->GetWorld().
					GetEnvironment()->GetStockSkin(igdeEnvironment::essError);
			}
		}
	}
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetRotationPerForce(float rotation){
	rotation = decMath::max(rotation, 0.0f);
	
	if(fabsf(rotation - pRotationPerForce) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRotationPerForce = rotation;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetRestitution(float restitution){
	restitution = decMath::max(restitution, 0.0f);
	
	if(fabsf(restitution - pRestitution) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRestitution = restitution;
	
	NotifyTypeChanged();
}



void meHeightTerrainPFType::SetCoverageDensity(int density){
	if(density == pCoverageDensity){
		return;
	}
	
	pCoverageDensity = density;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMinimumRandomScaling(float scaling){
	if(fabsf(scaling - pRandomScaleMin) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRandomScaleMin = scaling;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMaximumRandomScaling(float scaling){
	if(fabsf(scaling - pRandomScaleMax) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRandomScaleMax = scaling;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMinimumRandomRotation(const decVector &rotation){
	if(rotation.IsEqualTo(pRandomRotationMin)){
		return;
	}
	
	pRandomRotationMin = rotation;
	
	NotifyTypeChanged();
}

void meHeightTerrainPFType::SetMaximumRandomRotation(const decVector &rotation){
	if(rotation.IsEqualTo(pRandomRotationMax)){
		return;
	}
	
	pRandomRotationMax = rotation;
	
	NotifyTypeChanged();
}
