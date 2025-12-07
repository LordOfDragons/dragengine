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

#include "mePropField.h"
#include "mePropFieldType.h"
#include "../meWorld.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>



// Class mePropFieldType
//////////////////////////

// Constructor, destructor
////////////////////////////

mePropFieldType::mePropFieldType(deEngine *engine){
	if(! engine) DETHROW(deeInvalidParam);
	
	pPropField = NULL;
	
	pEngine = engine;
	pEngPFType = NULL;
	
	pModel = NULL;
	pSkin = NULL;
	pRotPerForceX = 5.0f;
	pRotPerForceZ = 5.0f;
	
	pInstances = NULL;
	pInstanceCount = 0;
}

mePropFieldType::~mePropFieldType(){
	if(pInstances) delete [] pInstances;
	
	if(pSkin) pSkin->FreeReference();
	if(pModel) pModel->FreeReference();
}



// Management
///////////////

void mePropFieldType::SetPropField(mePropField *propField){
	pPropField = propField;
	pEngPFType = NULL;
}

void mePropFieldType::SetEnginePFType(dePropFieldType *engPFType){
	if(engPFType && ! pPropField) DETHROW(deeInvalidParam);
	
	pEngPFType = engPFType;
}



dePropFieldType *mePropFieldType::CreateEnginePFType(){
	if(! pPropField) DETHROW(deeInvalidParam);
	
	dePropFieldInstance *engPFTInstances = NULL;
	dePropFieldType *engPFType = NULL;
	int i;
	
	if(pInstanceCount > 0){
		try{
			engPFType = new dePropFieldType();
			
			engPFType->SetModel(pModel);
			engPFType->SetSkin(pSkin);
			
			decLayerMask collisionCategory;
			collisionCategory.SetBit(meWorld::eclmPropFields);
			
			decLayerMask collisionFilter;
			collisionFilter.SetBit(meWorld::eclmEditing);
			collisionFilter.SetBit(meWorld::eclmForceField);
			collisionFilter.SetBit(meWorld::eclmParticles);
			
			engPFType->SetCollisionFilter(decCollisionFilter(collisionCategory, collisionFilter));
			
			engPFType->SetRotationPerForce(pRotPerForceX * DEG2RAD);
			
			engPFType->SetInstanceCount(pInstanceCount);
			engPFTInstances = engPFType->GetInstances();
			for(i=0; i<pInstanceCount; i++){
				engPFTInstances[i].SetPosition(pInstances[i].GetPosition());
				engPFTInstances[i].SetRotation(pInstances[i].GetRotation());
				engPFTInstances[i].SetScaling(pInstances[i].GetScaling());
				// others
			}
			
		}catch(const deException &){
			if(engPFTInstances) delete engPFTInstances;
			throw;
		}
	}
	
	return engPFType;
}

void mePropFieldType::UpdateEnginePFType(){
	dePropField *engPropField = NULL;
	int engPFTInstanceCount = 0;
	
	if(pPropField){
		engPropField = pPropField->GetEnginePropField();
	}
	
	if(pEngPFType){
		engPFTInstanceCount = pEngPFType->GetInstanceCount();
	}
	
	if(engPFTInstanceCount == pInstanceCount){
		if(pEngPFType){
			dePropFieldInstance *engPFTInstances = pEngPFType->GetInstances();
			int i;
			
			for(i=0; i<pInstanceCount; i++){
				engPFTInstances[i].SetPosition(pInstances[i].GetPosition());
				engPFTInstances[i].SetRotation(pInstances[i].GetRotation());
				engPFTInstances[i].SetScaling(pInstances[i].GetScaling());
				// and others
			}
		}
		
	}else{
		dePropFieldType *engPFType = NULL;
		
		try{
			engPFType = CreateEnginePFType();
			
			if(pEngPFType && engPropField){
				engPropField->RemoveType(pEngPFType);
				pEngPFType = NULL;
			}
			
			if(engPFType){
				engPropField->AddType(engPFType);
				pEngPFType = engPFType;
			}
			
		}catch(const deException &){
			if(engPFType) delete engPFType;
			throw;
		}
	}
	
	NotifyTypeChanged();
}

void mePropFieldType::NotifyTypeChanged(){
	if(pPropField){
		pPropField->SetWorldChanged();
	}
}



void mePropFieldType::SetPathModel(const char *path){
	if(pPathModel.Equals(path)){
		return;
	}
	
	pPathModel = path;
	
	if(pModel){
		pModel->FreeReference();
		pModel = NULL;
	}
	
	if(! pPathModel.IsEmpty()){
		pModel = pEngine->GetModelManager()->LoadModel(pPathModel.GetString(), "/");
	}
	
	UpdateEnginePFType();
	if(pPropField && pPropField->GetWorld()){
		pPropField->GetWorld()->NotifyPFTypeChanged(pPropField, this);
	}
}

void mePropFieldType::SetPathSkin(const char *path){
	if(pPathSkin.Equals(path)){
		return;
	}
	
	pPathSkin = path;
	
	if(pSkin){
		pSkin->FreeReference();
		pSkin = NULL;
	}
	
	if(! pPathSkin.IsEmpty()){
		try{
			pSkin = pEngine->GetSkinManager()->LoadSkin(pPathSkin.GetString(), "/");
			
		}catch(const deException &){
			if(pPropField && pPropField->GetWorld()){
				pSkin = pPropField->GetWorld()->GetEnvironment()->GetStockSkin(igdeEnvironment::essError);
				pSkin->AddReference();
			}
		}
	}
	
	UpdateEnginePFType();
	if(pPropField && pPropField->GetWorld()){
		pPropField->GetWorld()->NotifyPFTypeChanged(pPropField, this);
	}
}

void mePropFieldType::SetRotationPerForceX(float rotPerForce){
	rotPerForce = decMath::max(rotPerForce, 0.0f);
	if(fabsf(rotPerForce - pRotPerForceX) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRotPerForceX = rotPerForce;
	
	UpdateEnginePFType();
	if(pPropField && pPropField->GetWorld()){
		pPropField->GetWorld()->NotifyPFTypeChanged(pPropField, this);
	}
}

void mePropFieldType::SetRotationPerForceZ(float rotPerForce){
	rotPerForce = decMath::max(rotPerForce, 0.0f);
	if(fabsf(rotPerForce - pRotPerForceZ) < FLOAT_SAFE_EPSILON){
		return;
	}
	
	pRotPerForceZ = rotPerForce;
	
	UpdateEnginePFType();
	if(pPropField && pPropField->GetWorld()){
		pPropField->GetWorld()->NotifyPFTypeChanged(pPropField, this);
	}
}



void mePropFieldType::RebuildInstances(){
	// TODO build some default instances for testing
}
