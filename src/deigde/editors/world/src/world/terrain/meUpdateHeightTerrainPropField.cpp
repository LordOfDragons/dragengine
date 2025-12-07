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
#include "meHeightTerrainPFLayer.h"
#include "meUpdateHeightTerrainPropField.h"
#include "../heightterrain/meHTVegetationLayer.h"
#include "../heightterrain/meHTVInstance.h"
#include "../heightterrain/meHTVVariation.h"
#include "../meWorld.h"
#include "../../utils/meByteArray.h"

#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldGround.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/common/exceptions.h>



// Class meUpdateHeightTerrainPropField
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUpdateHeightTerrainPropField::meUpdateHeightTerrainPropField(meHeightTerrainPropField *propField){
	if(! propField) DETHROW(deeInvalidParam);
	
	pPropField = propField;
}

meUpdateHeightTerrainPropField::~meUpdateHeightTerrainPropField(){
}



// Management
///////////////



// Notifications
//////////////////

void meUpdateHeightTerrainPropField::CreateInstances(float density){
	meHeightTerrainSector * const htsector = pPropField->GetHTSector();
	dePropField * const engPF = pPropField->GetEnginePropField();
	
	if(! engPF || ! htsector || ! htsector->GetHeightTerrain()){
		// this should never fail but to avoid troubles we do nothing if so
		const int count = engPF->GetTypeCount();
		int i;
		for(i=0; i<count; i++){
			engPF->GetTypeAt(i)->SetInstanceCount(0);
			engPF->NotifyInstancesChanged(i);
		}
		return;
	}
	
	if(engPF->GetTypeCount() == 0){
		// not updated properly. do nothing
		const int count = engPF->GetTypeCount();
		int i;
		for(i=0; i<count; i++){
			engPF->GetTypeAt(i)->SetInstanceCount(0);
			engPF->NotifyInstancesChanged(i);
		}
		return;
	}
	
	meHeightTerrainPropField &htpf = *pPropField;
	htpf.UpdateVInstances();
	int i;
	
	if(pPropField->GetKeepClean()){
		const int count = engPF->GetTypeCount();
		for(i=0; i<count; i++){
			engPF->GetTypeAt(i)->SetInstanceCount(0);
			engPF->NotifyInstancesChanged(i);
		}
		return;
	}
	
	// clear and add instances from the vegetation instances inside this prop field
	const int layerCount = htsector->GetHeightTerrain()->GetVLayerCount();
	const int instanceCount = htpf.GetVInstanceCount();
	decVector relIPos;
	int typeIndex = 0;
	int j, k;
	
	for(i=0; i<layerCount; i++){
		const meHTVegetationLayer &vlayer = *htsector->GetHeightTerrain()->GetVLayerAt(i);
		const int variationCount = vlayer.GetVariationCount();
		
		for(j=0; j<variationCount; j++){
			dePropFieldType * const engPFType = engPF->GetTypeAt(typeIndex++);
			
			// determine how many instances match this layer and variation
			int pficount = 0;
			
			for(k=0; k<instanceCount; k++){
				const meHTVInstance &vinstance = htpf.GetVInstanceAt(k);
				
				if(vinstance.GetVLayer() == i && vinstance.GetVariation() == j){
					pficount++;
				}
			}
			
			// pick only density times many instances from the pool. currently the instances
			// are random so we can simply take only density times instances but in upcoming
			// implementations this might no more be the case.
			int upficount = pficount; // (int)(ceilf((float)pficount * density));
			//if( upficount < 0 ) upficount = 0;
			//if( upficount > pficount ) upficount = pficount;
			
			// only add instances if there are any
			if(upficount == 0){
				continue;
			}
			
			engPFType->SetInstanceCount(upficount);
			dePropFieldInstance * const pfinstances = engPFType->GetInstances();
			
			// add vegetation instances matching this layer
			int pfi = 0;
			for(k=0; k<instanceCount; k++){
				const meHTVInstance &vinstance = htpf.GetVInstanceAt(k);
				if(vinstance.GetVLayer() != i || vinstance.GetVariation() != j){
					continue;
				}
				
				if(pfi == upficount){
					break;
				}
				
				dePropFieldInstance &pfinstance = pfinstances[pfi++];
				pfinstance.SetPosition(vinstance.GetPosition());
				pfinstance.SetRotation(vinstance.GetRotation());
				pfinstance.SetScaling(vinstance.GetScaling());
			}
			
			if(pfi < upficount){
				DETHROW(deeInvalidAction);
			}
		}
	}
	
	const int engTypeCount = engPF->GetTypeCount();
	for(i=0; i<engTypeCount; i++){
		engPF->NotifyInstancesChanged(i);
	}
}



// Private Functions
//////////////////////

void meUpdateHeightTerrainPropField::pProjectToGround(){
	dePropField *engPropField = pPropField->GetEnginePropField();
	
	if(engPropField){
		meHeightTerrainSector *htsector = pPropField->GetHTSector();
		dePropFieldGround ground;
		bool hasGround = false;
		
		if(htsector){
			meHeightTerrain *hterrain = htsector->GetHeightTerrain();
			
			if(hterrain){
				ground.SetHeightTerrain(hterrain->GetEngineHeightTerrain());
				hasGround = ground.GetHeightTerrain() != NULL;
				/*
				meWorld *world = hterrain->GetWorld();
				if(world){
					const decPoint3 &worldSize = world->GetWorldSize();
					deTerrain *engTerrain;
					meWorldSector *sector;
					int t, terrainCount;
					meTerrain *terrain;
					decPoint3 s;
					
					for(s.z=0; s.z<worldSize.z; s.z++){
						for(s.y=0; s.y<worldSize.y; s.y++){
							for(s.x=0; s.x<worldSize.x; s.x++){
								sector = world->GetSectorAt(s);
								
								terrainCount = sector->GetTerrainCount();
								for(t=0; t<terrainCount; t++){
									terrain = sector->GetTerrainAt(t);
									engTerrain = terrain->GetDETerrain();
									if(engTerrain && engTerrain->GetTerrainMesh()->GetGeometryType() == deTerrainMesh::egtTerrainGround){
										ground.AddTerrain(engTerrain);
										hasGround = true;
									}
								}
							}
						}
					}
				}
				*/
			}
		}
		
		if(hasGround){
			engPropField->NotifyProjectInstances(ground, decVector(0.0f, -1.0f, 0.0f));
		}
	}
}

float meUpdateHeightTerrainPropField::pGetMaskDensity(const meByteArray *mask, float positionX, float positionY) const{
	int colons = mask->GetColons();
	int rows = mask->GetRows();
	int mx, my;
	float d1, d2, d3, d4;
	float fx, fy, invfx, invfy;
	bool canGoX, canGoY;
	float maskFactor = 1.0f / 255.0f;
	
	mx = (int)positionX;
	my = (int)positionY;
	if(mx < 0) mx = 0;
	if(mx >= colons) mx = colons - 1;
	if(my < 0) my = 0;
	if(my >= rows) my = rows - 1;
	
	fx = positionX - (float)mx;
	fy = positionY - (float)my;
	invfx = 1.0f - fx;
	invfy = 1.0f - fy;
	
	canGoX = mx < colons - 1;
	canGoY = my < rows - 1;
	
	d1 = (float)mask->GetValueAt(mx, my) * maskFactor;
	
	if(canGoX){
		d2 = (float)mask->GetValueAt(mx + 1, my) * maskFactor;
		
	}else{
		d2 = d1;
	}
	
	if(canGoY){
		d3 = (float)mask->GetValueAt(mx, my + 1) * maskFactor;
		
	}else{
		d3 = d1;
	}
	
	if(canGoX && canGoY){
		d4 = (float)mask->GetValueAt(mx + 1, my + 1) * maskFactor;
		
	}else{
		d4 = d1;
	}
	
	return ((float)d1 * invfx + (float)d2 * fx) * invfy + ((float)d3 * invfx + (float)d4 * fx) * fy;
}
