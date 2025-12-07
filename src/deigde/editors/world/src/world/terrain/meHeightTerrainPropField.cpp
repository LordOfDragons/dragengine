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

#include "meHeightTerrain.h"
#include "meHeightTerrainSector.h"
#include "meHeightTerrainPropField.h"
#include "meHeightTerrainPFType.h"
#include "meUpdateHeightTerrainPropField.h"
#include "../meWorld.h"
#include "../heightterrain/meHTVInstance.h"
#include "../heightterrain/meUpdateHTVInstances.h"
#include "../heightterrain/meHTVegetationLayer.h"
#include "../heightterrain/meHTVVariation.h"
#include "../../utils/meByteArray.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/common/exceptions.h>



// Class meHeightTerrainPropField
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainPropField::meHeightTerrainPropField(deEngine *engine) :
pHTSector(NULL),
pEngine(engine),
pEngPF(NULL),
pVInstances(NULL),
pVInstanceCount(0),
pVInstanceSize(0),
pDirtyVInstances(false),
pDirty(false),
pKeepClean(true),
pListener(NULL)
{
	if(! engine){
		DETHROW(deeInvalidParam);
	}
	
	try{
		pListener = new meUpdateHeightTerrainPropField(this);
		pEngPF = engine->GetPropFieldManager()->CreatePropField();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meHeightTerrainPropField::~meHeightTerrainPropField(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainPropField::SetHTSector(meHeightTerrainSector *htsector){
	if(htsector == pHTSector){
		return;
	}
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->GetWorld().GetEngineWorld()->RemovePropField(pEngPF);
	}
	
	pHTSector = htsector;
	
	if(htsector && htsector->GetHeightTerrain()){
		htsector->GetHeightTerrain()->GetWorld().GetEngineWorld()->AddPropField(pEngPF);
	}
}



void meHeightTerrainPropField::SetExtend(const decVector2 &minimum, const decVector2 &maximum){
	pMinExtend = minimum;
	pMaxExtend = maximum;
}

void meHeightTerrainPropField::InitDelegates(igdeEnvironment *environment){
	environment->SetPropFieldDelegee(pEngPF, pListener);
}

void meHeightTerrainPropField::Invalidate(){
	pDirty = true;
}

void meHeightTerrainPropField::Validate(){
	pDirty = false;
}



void meHeightTerrainPropField::Update(){
	pKeepClean = false;
	
	if(pDirty){
		MarkVInstancesDirty();
		pDirty = false;
	}
	
	RebuildVegetationPropFieldTypes();
}

void meHeightTerrainPropField::Clear(){
	pKeepClean = true;
	
	if(pEngPF){
		pEngPF->RemoveAllTypes();
	}
}



void meHeightTerrainPropField::RebuildVegetationPropFieldTypes(){
	if(! pEngPF){
		return;
	}
	
	pEngPF->RemoveAllTypes();
	
	if(pKeepClean){
		return;
	}
	if(! pHTSector || ! pHTSector->GetHeightTerrain()){
		return;
	}
	
	decLayerMask cfCategory;
	cfCategory.SetBit(meWorld::eclmPropFields);
	
	decLayerMask cfFilter;
	cfFilter.SetBit(meWorld::eclmEditing);
	cfFilter.SetBit(meWorld::eclmForceField);
	cfFilter.SetBit(meWorld::eclmParticles);
	
	const decCollisionFilter collisionFilter(cfCategory, cfFilter);
	
	const int layerCount = pHTSector->GetHeightTerrain()->GetVLayerCount();
	dePropFieldType *engPFType = NULL;
	int i, j;
	
	try{
		for(i=0; i<layerCount; i++){
			meHTVegetationLayer &vlayer = *pHTSector->GetHeightTerrain()->GetVLayerAt(i);
			const int variationCount = vlayer.GetVariationCount();
			
			for(j=0; j<variationCount; j++){
				meHTVVariation &variation = *vlayer.GetVariationAt(j);
				
				engPFType = new dePropFieldType;
				engPFType->SetModel(variation.GetModel());
				engPFType->SetSkin(variation.GetSkin());
				engPFType->SetRotationPerForce(variation.GetRotationPerForce() * DEG2RAD);
				engPFType->SetRestitution(variation.GetRestitution());
				engPFType->SetCollisionFilter(collisionFilter);
				
				pEngPF->AddType(engPFType);
				engPFType = NULL;
			}
		}
		
	}catch(const deException &){
		if(engPFType){
			delete engPFType;
		}
		throw;
	}
}



meHTVInstance &meHeightTerrainPropField::GetVInstanceAt(int index) const{
	if(index < 0 || index >= pVInstanceCount){
		DETHROW(deeInvalidParam);
	}
	return pVInstances[index];
}

meHTVInstance &meHeightTerrainPropField::AddVInstance(){
	if(pVInstanceCount == pVInstanceSize){
		const int newSize = pVInstanceSize + 500;
		meHTVInstance * const newArray = new meHTVInstance[newSize];
		if(pVInstances){
			memcpy(newArray, pVInstances, sizeof(meHTVInstance) * pVInstanceSize);
			delete [] pVInstances;
		}
		pVInstances = newArray;
		pVInstanceSize = newSize;
	}
	
	pVInstanceCount++;
	return pVInstances[pVInstanceCount - 1];
}

void meHeightTerrainPropField::RemoveAllVInstances(){
	pVInstanceCount = 0;
}

void meHeightTerrainPropField::UpdateVInstances(){
	if(! pDirtyVInstances || ! pHTSector || ! pHTSector->GetHeightTerrain()){
		return;
	}
	
	meUpdateHTVInstances updater(*this);
	updater.UpdateInstances();
	
	pDirtyVInstances = false;
	pHTSector->SetPFCacheChanged(true);
}

void meHeightTerrainPropField::MarkVInstancesDirty(){
	if(pDirtyVInstances){
		return;
	}
	
	RemoveAllVInstances();
	pDirtyVInstances = true;
}

void meHeightTerrainPropField::MarkVInstancesValid(){
	pDirtyVInstances = false;
}



// Private Functions
//////////////////////

void meHeightTerrainPropField::pCleanUp(){
	SetHTSector(NULL);
	
	if(pVInstances){
		delete [] pVInstances;
	}
	
	if(pEngPF){
		pEngPF->FreeReference();
	}
	
	if(pListener){
		delete pListener;
	}
}
