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

#include "meHTVVariation.h"
#include "meHTVegetationLayer.h"
#include "../meWorld.h"
#include "../terrain/meHeightTerrain.h"
#include "../terrain/meHeightTerrainSector.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/common/exceptions.h>



// Class meHTVegetationLayer
////////////////////////////////////

// Constructor, destructor
////////////////////////////

meHTVVariation::meHTVVariation(deEngine *engine){
	if(!engine) DETHROW(deeInvalidParam);
	
	pEngine = engine;
	pVLayer = nullptr;
	
	pModel = nullptr;
	pSkin = nullptr;
	pRotationPerForce = 5.0f;
	pRestitution = 0.5f;
	
	pRandomScaleMin = 0.5f;
	pRandomScaleMax = 1.5f;
	pRandomRotationMin = -30.0f;
	pRandomRotationMax = 30.0f;
}

meHTVVariation::~meHTVVariation(){
}



// Management
///////////////

void meHTVVariation::SetVLayer(meHTVegetationLayer *vlayer){
	pVLayer = vlayer;
}



void meHTVVariation::SetPathModel(const char *path){
	if(!path) DETHROW(deeInvalidParam);
	
	if(!pPathModel.Equals(path)){
		pPathModel = path;
		
		pModel = nullptr;
		
		if(!pPathModel.IsEmpty()){
			try{
				pModel = pEngine->GetModelManager()->LoadModel(pPathModel.GetString(), "/");
				
			}catch(const deException &){
			}
		}
		
		NotifyChanged();
	}
}

void meHTVVariation::SetPathSkin(const char *path){
	if(!path) DETHROW(deeInvalidParam);
	
	if(!pPathSkin.Equals(path)){
		pPathSkin = path;
		
		pSkin = nullptr;
		
		if(!pPathSkin.IsEmpty()){
			try{
				pSkin = pEngine->GetSkinManager()->LoadSkin(pPathSkin.GetString(), "/");
				
			}catch(const deException &){
				if(pVLayer && pVLayer->GetHeightTerrain()){
					pSkin = pVLayer->GetHeightTerrain()->GetWorld().GetEnvironment()->
						GetStockSkin(igdeEnvironment::essError);
				}
			}
		}
		
		NotifyChanged();
	}
}

void meHTVVariation::SetRotationPerForce(float rotation){
	if(rotation < 0.0f) rotation = 0.0f;
	
	if(fabsf(rotation - pRotationPerForce) > 1e-5f){
		pRotationPerForce = rotation;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetRestitution(float restitution){
	if(restitution < 0.0f) restitution = 0.0f;
	
	if(fabsf(restitution - pRestitution) > 1e-5f){
		pRestitution = restitution;
		
		NotifyChanged();
	}
}



void meHTVVariation::SetMinimumRandomScaling(float scaling){
	if(fabsf(scaling - pRandomScaleMin) > 1e-5f){
		pRandomScaleMin = scaling;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetMaximumRandomScaling(float scaling){
	if(fabsf(scaling - pRandomScaleMax) > 1e-5f){
		pRandomScaleMax = scaling;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetMinimumRandomRotation(float rotation){
	if(fabsf(rotation - pRandomRotationMin) > 1e-5f){
		pRandomRotationMin = rotation;
		
		NotifyChanged();
	}
}

void meHTVVariation::SetMaximumRandomRotation(float rotation){
	if(fabsf(rotation - pRandomRotationMax) > 1e-5f){
		pRandomRotationMax = rotation;
		
		NotifyChanged();
	}
}



void meHTVVariation::NotifyChanged(){
	if(pVLayer){
		meHeightTerrain *hterrain = pVLayer->GetHeightTerrain();
		if(hterrain){
			hterrain->GetWorld().NotifyHTVLVariationChanged(pVLayer, this);
			hterrain->SetChanged(true);
			hterrain->InvalidateAllPropFields();
		}
	}
}
