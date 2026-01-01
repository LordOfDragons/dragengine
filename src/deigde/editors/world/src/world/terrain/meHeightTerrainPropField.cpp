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
pHTSector(nullptr),
pEngine(engine),
pDirtyVInstances(false),
pDirty(false),
pKeepClean(true),
pListener(nullptr)
{
	DEASSERT_NOTNULL(engine)
	
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
	if(!pEngPF){
		return;
	}
	
	pEngPF->RemoveAllTypes();
	
	if(pKeepClean){
		return;
	}
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	
	decLayerMask cfCategory;
	cfCategory.SetBit(meWorld::eclmPropFields);
	
	decLayerMask cfFilter;
	cfFilter.SetBit(meWorld::eclmEditing);
	cfFilter.SetBit(meWorld::eclmForceField);
	cfFilter.SetBit(meWorld::eclmParticles);
	
	const decCollisionFilter collisionFilter(cfCategory, cfFilter);
	
	dePropFieldType *engPFType = nullptr;
	
	try{
		pHTSector->GetHeightTerrain()->GetVLayers().Visit([&](const meHTVegetationLayer &vlayer){
			vlayer.GetVariations().Visit([&](meHTVVariation &variation){
				engPFType = new dePropFieldType;
				engPFType->SetModel(variation.GetModel());
				engPFType->SetSkin(variation.GetSkin());
				engPFType->SetRotationPerForce(variation.GetRotationPerForce() * DEG2RAD);
				engPFType->SetRestitution(variation.GetRestitution());
				engPFType->SetCollisionFilter(collisionFilter);
				
				pEngPF->AddType(engPFType);
				engPFType = nullptr;
			});
		});
		
	}catch(const deException &){
		if(engPFType){
			delete engPFType;
		}
		throw;
	}
}



const meHTVInstance::Ref &meHeightTerrainPropField::AddVInstance(){
	pVInstances.Add(meHTVInstance::Ref::New());
	return pVInstances.Last();
}

void meHeightTerrainPropField::RemoveAllVInstances(){
	pVInstances.RemoveAll();
}

void meHeightTerrainPropField::UpdateVInstances(){
	if(!pDirtyVInstances || !pHTSector || !pHTSector->GetHeightTerrain()){
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
	SetHTSector(nullptr);
	if(pListener){
		delete pListener;
	}
}
