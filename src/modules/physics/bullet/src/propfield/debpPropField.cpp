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

#include "debpPropField.h"
#include "debpPropFieldType.h"

#include "../debpCommon.h"
#include "../dePhysicsBullet.h"
#include "../coldet/debpCollisionDetection.h"
#include "../component/debpComponent.h"
#include "../terrain/heightmap/debpHeightTerrain.h"
#include "../terrain/heightmap/debpHTSector.h"
#include "../world/debpWorld.h"

#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldGround.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldBendState.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainSector.h>
#include <dragengine/common/exceptions.h>



// Class debpPropField
////////////////////////

// Constructor, destructor
////////////////////////////

debpPropField::debpPropField(dePhysicsBullet *bullet, dePropField *propField){
	if(!bullet || !propField) DETHROW(deeInvalidParam);
	
	int t, typeCount = propField->GetTypeCount();
	
	pBullet = bullet;
	pPropField = propField;
	
	pTypes = NULL;
	pTypeCount = 0;
	pTypeSize = 0;
	
	pDirty = false;
	
	try{
		for(t=0; t<typeCount; t++){
			TypeAdded(t, propField->GetTypeAt(t));
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

debpPropField::~debpPropField(){
	pCleanUp();
}



// Management
///////////////

debpPropFieldType *debpPropField::GetTypeAt(int index) const{
	if(index < 0 || index >= pTypeCount) DETHROW(deeInvalidParam);
	
	return pTypes[index];
}



void debpPropField::Update(float elapsed){
//	if( pDirty ){
		int t;
		
		for(t=0; t<pTypeCount; t++){
			pTypes[t]->Update(elapsed);
		}
		
//		pDirty = false;
//	}
}



void debpPropField::PositionChanged(){
}

void debpPropField::TypeAdded(int index, dePropFieldType *type){
	if(pTypeCount == pTypeSize){
		int newSize = pTypeSize * 3 / 2 + 1;
		debpPropFieldType **newArray = new debpPropFieldType*[newSize];
		if(!newArray) DETHROW(deeOutOfMemory);
		if(pTypes){
			memcpy(newArray, pTypes, sizeof(debpPropFieldType*) * pTypeSize);
			delete [] pTypes;
		}
		pTypes = newArray;
		pTypeSize = newSize;
	}
	
	pTypes[pTypeCount] = new debpPropFieldType(this, type);
	if(!pTypes[pTypeCount]) DETHROW(deeOutOfMemory);
	pTypeCount++;
	
	pDirty = true;
}

void debpPropField::TypeRemoved(int index, dePropFieldType *type){
	int t;
	
	delete pTypes[index];
	
	for(t=index+1; t<pTypeCount; t++){
		pTypes[t - 1] = pTypes[t];
	}
	pTypeCount--;
}

void debpPropField::AllTypesRemoved(){
	while(pTypeCount > 0){
		pTypeCount--;
		delete pTypes[pTypeCount];
	}
}

void debpPropField::TypeChanged(int index, dePropFieldType *type){
	pTypes[index]->MarkDirty();
	pDirty = true;
}

void debpPropField::InstancesChanged(int index, dePropFieldType *type){
	pTypes[index]->MarkDirty();
	pDirty = true;
}

void debpPropField::ProjectInstances(const dePropFieldGround &ground, const decVector &direction){
	pProjectInstancesDown(ground);
	
	int i;
	for(i=0; i<pTypeCount; i++){
		pTypes[i]->MarkDirty();
	}
	
	pDirty = true;
}



// Private functions
//////////////////////

void debpPropField::pCleanUp(){
	AllTypesRemoved();
	if(pTypes) delete [] pTypes;
}

void debpPropField::pProjectInstancesDown(const dePropFieldGround &ground){
	// this is an optimized version for the case that the projection direction
	// is exactly downwards ( matching (0,-1,0) )
	
	const decDVector &fieldPosition = pPropField->GetPosition();
	deHeightTerrain *hterrain = ground.GetHeightTerrain();
	dePropFieldInstance *pftInstances;
	decVector instancePosition;
	debpCollisionResult result;
	int pfti, pftInstanceCount;
	dePropFieldType *pfType;
	int pft, pfTypeCount;
	
	decVector bestNormal;
	float bestHeight;
	bool hasHit;
	
	debpHeightTerrain *bpHTerrain = NULL;
	int imageDim = hterrain->GetSectorResolution();
	float sectorDim = hterrain->GetSectorSize();
	double invSectorDim = 1.0 / (double)sectorDim;
	int sx, sz, qx, qz, qbase;
	debpHTSector *bpHTSector;
	float qpiz, qpheight;
	decVector qpnormal;
	double wpx, wpz;
	double gpx, gpz;
	float lpx, lpz;
	
	/*
	float tmboxMinX, tmboxMaxX, tmboxMinZ, tmboxMaxZ;
	const debpTriangle *tmfaces;
	const debpVertex *tmverts;
	int f, faceCount;
	*/
	
	if(hterrain) bpHTerrain = (debpHeightTerrain*)hterrain->GetPeerPhysics();
	
	pfTypeCount = pPropField->GetTypeCount();
	
	if(bpHTerrain) bpHTerrain->Update();
	
	for(pft=0; pft<pfTypeCount; pft++){
		pfType = pPropField->GetTypeAt(pft);
		pftInstanceCount = pfType->GetInstanceCount();
		pftInstances = pfType->GetInstances();
		
		for(pfti=0; pfti<pftInstanceCount; pfti++){
			instancePosition = pftInstances[pfti].GetPosition();
			
			wpx = fieldPosition.x + (double)instancePosition.x;
			wpz = fieldPosition.z + (double)instancePosition.z;
			
			hasHit = false;
			
			// test against height terrain
			if(bpHTerrain){
				gpx = wpx * invSectorDim + 0.5;
				gpz = wpz * invSectorDim + 0.5;
				
				sx = (int)gpx;
				sz = (int)gpz;
				bpHTSector = bpHTerrain->GetSectorWith(sx, sz);
				
				if(bpHTSector){
					decVector *points = bpHTSector->GetPoints();
					
					lpx = (float)(gpx - (double)sx) * (float)imageDim;
					lpz = (1.0f - (float)(gpz - (double)sz)) * (float)imageDim;
					
					qx = (int)lpx;
					if(qx < 0){
						qx = 0;
						
					}else if(qx >= imageDim - 1){
						qx = imageDim - 2;
					}
					
					qz = (int)lpz;
					if(qz < 0){
						qz = 0;
						
					}else if(qz >= imageDim - 1){
						qz = imageDim - 2;
					}
					
					if(bpHTSector->GetSector()->GetFaceVisibleAt(qx, qz)){
						lpx -= (float)qx;
						lpz -= (float)qz;
						
						qbase = qz * imageDim + qx;
						const decVector &qp1 = points[qbase];
						
						if(lpx > lpz){ // triangle (1,2,4)
							const decVector &qp2 = points[qbase + 1];
							const decVector &qp3 = points[qbase + imageDim + 1];
							
							if(lpx < 0.001f){ // avoid div by zero as we are right in the upper left corner
								qpheight = qp1.y;
								
							}else{
								qpiz = lpz / lpx;
								qpheight = qp1.y * (1.0f - lpx) + (qp2.y * (1.0f - qpiz) + qp3.y * qpiz) * lpx;
							}
							
							qpnormal.x = (qp2.y - qp1.y) * (qp3.z - qp2.z) - (qp2.z - qp1.z) * (qp3.y - qp2.y);
							qpnormal.y = (qp2.z - qp1.z) * (qp3.x - qp2.x) - (qp2.x - qp1.x) * (qp3.z - qp2.z);
							qpnormal.z = (qp2.x - qp1.x) * (qp3.y - qp2.y) - (qp2.y - qp1.y) * (qp3.x - qp2.x);
							
						}else{ // triangle (1,4,3)
							const decVector &qp2 = points[qbase + imageDim];
							const decVector &qp3 = points[qbase + imageDim + 1];
							
							if(lpz < 0.001f){ // avoid div by zero as we are right in the upper left corner
								qpheight = qp1.y;
								
							}else{
								qpiz = lpx / lpz;
								qpheight = qp1.y * (1.0f - lpz) + (qp2.y * (1.0f - qpiz) + qp3.y * qpiz) * lpz;
							}
							
							qpnormal.x = (qp3.y - qp1.y) * (qp2.z - qp3.z) - (qp3.z - qp1.z) * (qp2.y - qp3.y);
							qpnormal.y = (qp3.z - qp1.z) * (qp2.x - qp3.x) - (qp3.x - qp1.x) * (qp2.z - qp3.z);
							qpnormal.z = (qp3.x - qp1.x) * (qp2.y - qp3.y) - (qp3.y - qp1.y) * (qp2.x - qp3.x);
						}
						
						if(!hasHit || qpheight > bestHeight){
							hasHit = true;
							bestHeight = qpheight;
							bestNormal = qpnormal;
						}
					}
				}
			}
			
			// if a hit has been found adjust the instance
			if(hasHit){
				instancePosition.y = bestHeight;
				pftInstances[pfti].SetPosition(instancePosition);
				bestNormal.Normalize();
				if(bestNormal.y < 0.99f){
					pftInstances[pfti].SetRotation((
						decMatrix::CreateRotation(pftInstances[pfti].GetRotation()) *
						decMatrix::CreateVU(
							decVector(-bestNormal.x * bestNormal.y, bestNormal.x * bestNormal.x + bestNormal.z * bestNormal.z, -bestNormal.z * bestNormal.y),
							bestNormal)).GetEulerAngles());
				}
				
			// otherwise we block it out ( todo: make this better )
			}else{
				pftInstances[pfti].SetScaling(0.0f);
			}
		}
	}
}
