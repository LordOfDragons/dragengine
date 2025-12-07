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
#include <stdint.h>
#include <string.h>

#include "deClassCachedVegetation.h"
#include "../math/deClassPoint.h"
#include "../physics/deClassCollisionFilter.h"
#include "../world/deClassWorld.h"
#include "../../deClassPathes.h"
#include "../../deScriptingDragonScript.h"
#include "../../peers/dedsPropField.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/resources/propfield/dePropFieldManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <libdscript/exceptions.h>



// Local structs and classes
//////////////////////////////

struct dedsCachedVegetationInstance{
	short type;
	short positionX;
	float positionY;
	short positionZ;
	unsigned char rotationX;
	unsigned char rotationY;
	unsigned char rotationZ;
	unsigned char scaling;
};

class dedsCachedVegetationPropField : public deBaseScriptingPropField{
private:
	float pHeight;
	dePropField *pEngPF;
	dedsCachedVegetationInstance *pInstances;
	int pInstanceCount;
	decVector pScalePosition;
	
public:
	dedsCachedVegetationPropField(){
		pEngPF = NULL;
		pHeight = 0.0f;
		pInstances = NULL;
		pInstanceCount = 0;
	}
	
	~dedsCachedVegetationPropField(){
		if(pInstances) delete [] pInstances;
		if(pEngPF){
			dedsPropField * const peer = (dedsPropField*)pEngPF->GetPeerScripting();
			if(peer){
				peer->SetDelegee(NULL);
			}
			pEngPF->FreeReference();
		}
	}
	
	inline dePropField *GetEnginePF() const{ return pEngPF; }
	
	void CreatePF(deEngine *engine){
		if(!pEngPF){
			pEngPF = engine->GetPropFieldManager()->CreatePropField();
			if(!pEngPF) DSTHROW(dueOutOfMemory);
			
			dedsPropField *peer = (dedsPropField*)pEngPF->GetPeerScripting();
			peer->SetDelegee(this);
		}
	}
	
	inline float GetHeight() const{ return pHeight; }
	
	void SetHeight(float height){
		pHeight = height;
	}
	
	inline int GetInstanceCount() const{ return pInstanceCount; }
	inline dedsCachedVegetationInstance *GetInstances() const{ return pInstances; }
	
	void SetInstanceCount(int count){
		if(count < 0) DSTHROW(dueInvalidParam);
		
		if(pInstances){
			delete [] pInstances;
			pInstances = NULL;
			pInstanceCount = 0;
		}
		
		if(count > 0){
			pInstances = new dedsCachedVegetationInstance[count];
			if(!pInstances) DSTHROW(dueOutOfMemory);
			pInstanceCount = count;
		}
	}
	
	void SetCollisionFilter(const decCollisionFilter &filter){
		int t, typeCount = pEngPF->GetTypeCount();
		dePropFieldType *type;
		
		for(t=0; t<typeCount; t++){
			type = pEngPF->GetTypeAt(t);
			type->SetCollisionFilter(filter);
		}
	}
	
	void SetScalePosition(const decVector &scalePosition){
		pScalePosition = scalePosition;
	}
	
	virtual void CreateInstances(float density){
		decVector scaleRotation((DEG2RAD * 360.0f) / 255.0f, (DEG2RAD * 360.0f) / 255.0f, (DEG2RAD * 360.0f) / 255.0f);
		float scaleScaling = 5.0f / 255.0f;
		
		int t, typeCount = pEngPF->GetTypeCount();
		dePropFieldInstance *pfinstances;
		int pfi, pficount, upficount;
		dePropFieldType *engPFType;
		decVector ipos, irot;
		float iscale;
		int i;
		
		for(t=0; t<typeCount; t++){
			// determine how many instances match this type
			pficount = 0;
			for(i=0; i<pInstanceCount; i++){
				if(t == pInstances[i].type) pficount++;
			}
			
			// pick only density times many instances from the pool. currently the instances
			// are random so we can simply take only density times instances but in upcoming
			// implementations this might no more be the case.
			upficount = pficount; // (int)(ceilf((float)pficount * density));
			//if( upficount < 0 ) upficount = 0;
			//if( upficount > pficount ) upficount = pficount;
			
			engPFType = pEngPF->GetTypeAt(t);
			engPFType->SetInstanceCount(upficount);
			
			// add vegetation instances with matching this type
			if(upficount > 0){
				pfinstances = engPFType->GetInstances();
				
				for(pfi=0, i=0; i<pInstanceCount; i++){
					if(t == pInstances[i].type){
						if(pfi == upficount) break;
						
						dePropFieldInstance &pfinstance = pfinstances[pfi];
						
						ipos.x = (float)pInstances[i].positionX * pScalePosition.x;
						ipos.y = pInstances[i].positionY;
						ipos.z = (float)pInstances[i].positionZ * pScalePosition.z;
						irot.x = (float)pInstances[i].rotationX * scaleRotation.x;
						irot.y = (float)pInstances[i].rotationY * scaleRotation.y;
						irot.z = (float)pInstances[i].rotationZ * scaleRotation.z;
						iscale = (float)pInstances[i].scaling * scaleScaling;
						
						pfinstance.SetPosition(ipos);
						pfinstance.SetRotation(irot);
						pfinstance.SetScaling(iscale);
						
						pfi++;
					}
				}
				
				if(pfi < upficount) DSTHROW(dueInvalidAction);
			}
			
			// notify the engine that the instances are ready
			pEngPF->NotifyInstancesChanged(t);
		}
	}
};

class dedsCachedVegetationSector{
private:
	dedsCachedVegetationPropField *pPFs;
	int pPFCount;
	int pPFCellCount;
	float pSectorDim;
	deWorld *pWorld;
	decPoint pCoordinates;
	decCollisionFilter pCollisionFilter;
	
public:
	dedsCachedVegetationSector(deEngine *engine, float sectorDim, int pfCellCount, const decPoint &coordinates){
		if(!engine || sectorDim < 10.0f || pfCellCount < 1) DSTHROW(dueInvalidParam);
		
		int p, pfCount = pfCellCount * pfCellCount;
		
		pPFs = NULL;
		pPFCount = 0;
		pPFCellCount = pfCellCount;
		pSectorDim = sectorDim;
		pCoordinates = coordinates;
		pWorld = NULL;
		
		pPFs = new dedsCachedVegetationPropField[pfCount];
		if(!pPFs) DSTHROW(dueOutOfMemory);
		pPFCount = pfCount;
		
		for(p=0; p<pfCount; p++){
			pPFs[p].CreatePF(engine);
		}
		UpdatePFPositions();
	}
	
	~dedsCachedVegetationSector(){
		SetWorld(NULL);
		
		if(pPFs) delete [] pPFs;
	}
	
	void SetWorld(deWorld *world){
		if(world != pWorld){
			int p;
			
			if(pWorld){
				pWorld->FreeReference();
				
				for(p=0; p<pPFCount; p++){
					pWorld->RemovePropField(pPFs[p].GetEnginePF());
				}
			}
			
			pWorld = world;
			
			if(world){
				world->AddReference();
				
				for(p=0; p<pPFCount; p++){
					world->AddPropField(pPFs[p].GetEnginePF());
				}
			}
		}
	}
	
	inline const decPoint &GetCoordinates() const{ return pCoordinates; }
	
	inline int GetPFCount() const{ return pPFCount; }
	inline dedsCachedVegetationPropField *GetPFs() const{ return pPFs; }
	
	void SetCollisionFilter(const decCollisionFilter &filter){
		int p;
		
		pCollisionFilter = filter;
		
		for(p=0; p<pPFCount; p++){
			pPFs[p].SetCollisionFilter(filter);
		}
	}
	
	void Clear(){
		int p;
		
		for(p=0; p<pPFCount; p++){
			pPFs[p].GetEnginePF()->RemoveAllTypes();
		}
	}
	
	void UpdatePFPositions(){
		double cellSize = (double)pSectorDim / (double)pPFCellCount;
		double cellOffset = ((double)pSectorDim - cellSize) * 0.5;
		double sectorOffsetX = (double)pSectorDim * (double)pCoordinates.x;
		double sectorOffsetZ = (double)pSectorDim * (double)pCoordinates.y;
		decDVector position;
		dePropField *engPF;
		int x, y, p;
		
		for(y=0, p=0; y<pPFCellCount; y++){
			position.z = sectorOffsetZ + cellOffset - cellSize * (double)y;
			
			for(x=0; x<pPFCellCount; x++, p++){
				engPF = pPFs[p].GetEnginePF();
				
				position.x = sectorOffsetX - cellOffset + cellSize * (double)x;
				position.y = pPFs[p].GetHeight();
				
				engPF->SetPosition(position);
			}
		}
	}
	
	void LoadCacheFile(deEngine *engine, decBaseFileReader &reader){
		deModelManager *mdlmgr = engine->GetModelManager();
		deSkinManager *skinmgr = engine->GetSkinManager();
		dedsCachedVegetationInstance *instances;
		dePropFieldType *engPFType = NULL;
		decDVector sectorPosition;
		decVector scalePosition;
		int i, instanceCount;
		int t, typeCount;
		int p;
		
		Clear();
		
		// determine sector position
		sectorPosition.x = (double)pSectorDim * (double)pCoordinates.x;
		sectorPosition.y = 0.0f;
		sectorPosition.z = (double)pSectorDim * (double)pCoordinates.y;
		
		// read the header
		char header[10];
		reader.Read((char*)&header, 10); // signature {char[10]}
		if(strncmp(header, "DEPFCache ", 10) != 0) DSTHROW(dueInvalidParam); // deeInvalidFormat
		
		int version = reader.ReadByte(); // version {byte}
		if(version != 1) DSTHROW(dueInvalidParam); // deeInvalidFormat
		
		scalePosition.x = reader.ReadFloat(); // scalePositionX {float}
		scalePosition.y = reader.ReadFloat(); // scalePositionY {float}
		scalePosition.z = reader.ReadFloat(); // scalePositionZ {float}
		for(p=0; p<pPFCount; p++){
			pPFs[p].SetScalePosition(scalePosition);
		}
		
		// read the list of types and add them to each prop field
		typeCount = reader.ReadShort(); // number of types {short}
		
		float rotPerForce, restitution;
		
		for(t=0; t<typeCount; t++){
			// model path { string8 }
			deModel::Ref model(deModel::Ref::New(mdlmgr->LoadModel(reader.ReadString8(), "/")));
			
			// skin path { string8 }
			deSkin::Ref skin(deSkin::Ref::New(skinmgr->LoadSkin(reader.ReadString8(), "/")));
			
			rotPerForce = reader.ReadFloat() * DEG2RAD; // rotation per force {float}
			restitution = reader.ReadFloat(); // restitution {float}
			
			try{
				for(p=0; p<pPFCount; p++){
					engPFType = new dePropFieldType;
					engPFType->SetModel(model);
					engPFType->SetSkin(skin);
					engPFType->SetRotationPerForce(rotPerForce);
					engPFType->SetRestitution(restitution);
					engPFType->SetCollisionFilter(pCollisionFilter);
					
					pPFs[p].GetEnginePF()->AddType(engPFType);
					engPFType = NULL;
				}
				
			}catch(...){
				if(engPFType) delete engPFType;
				throw;
			}
		}
		
		// read the prop fields
		int propfieldCount = reader.ReadShort(); // prop field count {short}
		if(propfieldCount != pPFCount) DSTHROW(dueInvalidAction);
		
		for(p=0; p<pPFCount; p++){
			reader.ReadFloat(); // prop field position X {float}
			reader.ReadFloat(); // prop field position Y {float}
			reader.ReadFloat(); // prop field position Z {float}
			instanceCount = reader.ReadInt(); // instance count {int}
			
			pPFs[p].SetInstanceCount(instanceCount);
			instances = pPFs[p].GetInstances();
			
			for(i=0; i<instanceCount; i++){
				instances[i].type = reader.ReadShort(); // instance type {short}
				instances[i].positionX = reader.ReadShort(); // instance position X {short, encoded}
				instances[i].positionY = reader.ReadFloat(); // instance position Y {float}
				instances[i].positionZ = reader.ReadShort(); // instance position Z {short, encoded}
				instances[i].rotationX = reader.ReadByte(); // instance rotation X {byte, encoded}
				instances[i].rotationY = reader.ReadByte(); // instance rotation Y {byte, encoded}
				instances[i].rotationZ = reader.ReadByte(); // instance rotation Z {byte, encoded}
				instances[i].scaling = reader.ReadByte(); // instance scaling {byte, encoded}
			}
			
			pPFs[p].GetEnginePF()->NotifyGroundChanged();
		}
	}
};

class dedsCachedVegetation{
private:
	deEngine *pEngine;
	dedsCachedVegetationSector **pSectors;
	int pSectorCount;
	int pSectorSize;
	float pSectorDim;
	int pPFCellCount;
	deWorld *pWorld;
	decCollisionFilter pCollisionFilter;
	
public:
	dedsCachedVegetation(deEngine *engine, float sectorDim, int pfCellCount){
		if(!engine || sectorDim < 10.0f || pfCellCount < 1) DSTHROW(dueInvalidParam);
		
		pEngine = engine;
		pSectors = NULL;
		pSectorCount = 0;
		pSectorSize = 0;
		pSectorDim = sectorDim;
		pPFCellCount = pfCellCount;
		pWorld = NULL;
	}
	
	~dedsCachedVegetation(){
		RemoveAllSectors();
		if(pSectors) delete [] pSectors;
	}
	
	void SetWorld(deWorld *world){
		if(world != pWorld){
			int s;
			
			if(pWorld){
				pWorld->FreeReference();
				
				for(s=0; s<pSectorCount; s++){
					pSectors[s]->SetWorld(NULL);
				}
			}
			
			pWorld = world;
			
			if(world){
				world->AddReference();
				
				for(s=0; s<pSectorCount; s++){
					pSectors[s]->SetWorld(world);
				}
			}
		}
	}
	
	inline int GetSectorCount() const{ return pSectorCount; }
	
	dedsCachedVegetationSector *GetSectorAt(int index) const{
		if(index < 0 || index >= pSectorCount) DSTHROW(dueInvalidParam);
		
		return pSectors[index];
	}
	
	dedsCachedVegetationSector *GetSectorWith(const decPoint &coordinates) const{
		int s;
		
		for(s=0; s<pSectorCount; s++){
			if(pSectors[s]->GetCoordinates() == coordinates){
				return pSectors[s];
			}
		}
		
		return NULL;
	}
	
	void AddSectorWith(const decPoint &coordinates, const char *cacheFile){
		deVirtualFileSystem *vfs = pEngine->GetVirtualFileSystem();
		dedsCachedVegetationSector *sector = NULL;
		decPath path;
		
		if(GetSectorWith(coordinates)) DSTHROW(dueInvalidParam);
		
		if(pSectorCount == pSectorSize){
			int newSize = pSectorSize * 3 / 2 + 1;
			dedsCachedVegetationSector **newArray = new dedsCachedVegetationSector*[newSize];
			if(!newArray) DSTHROW(dueOutOfMemory);
			if(pSectors){
				memcpy(newArray, pSectors, sizeof(dedsCachedVegetationSector*) * pSectorSize);
				delete [] pSectors;
			}
			pSectors = newArray;
			pSectorSize = newSize;
		}
		
		pSectors[pSectorCount] = new dedsCachedVegetationSector(pEngine, pSectorDim, pPFCellCount, coordinates);
		sector = pSectors[pSectorCount];
		pSectorCount++;
		
		sector->SetCollisionFilter(pCollisionFilter);
		sector->SetWorld(pWorld);
		
		try{
			path.SetFromUnix(cacheFile);
			
			sector->LoadCacheFile(pEngine, decBaseFileReader::Ref::New(vfs->OpenFileForReading(path)));
			
		}catch(...){
			sector->Clear();
			throw;
		}
	}
	
	void RemoveSectorWith(const decPoint &coordinates){
		int s, index = -1;
		
		for(s=0; s<pSectorCount; s++){
			if(pSectors[s]->GetCoordinates() == coordinates){
				index = s;
				break;
			}
		}
		
		if(index != -1){
			pSectors[index]->SetWorld(NULL);
			delete pSectors[index];
			
			for(s=index+1; s<pSectorCount; s++){
				pSectors[s - 1] = pSectors[s];
			}
			pSectorCount--;
		}
	}
	
	void RemoveAllSectors(){
		while(pSectorCount > 0){
			pSectorCount--;
			pSectors[pSectorCount]->SetWorld(NULL);
			delete pSectors[pSectorCount];
		}
	}
	
	void SetCollisionFilter(const decCollisionFilter &filter){
		int s;
		
		pCollisionFilter = filter;
		
		for(s=0; s<pSectorCount; s++){
			pSectors[s]->SetCollisionFilter(filter);
		}
	}
};



// Native Structure
/////////////////////

struct sCVegNatDat{
	dedsCachedVegetation *cveg;
};



// Native functions
/////////////////////

// public func new( float sectorDimension, int propfieldCellCount )
deClassCachedVegetation::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsCVeg,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // sectorDimension
	p_AddParameter(init.clsInt); // propfieldCellCount
}
void deClassCachedVegetation::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sCVegNatDat *nd = (sCVegNatDat*)p_GetNativeData(myself);
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	
	deEngine *engine = clsCVeg->GetDS()->GetGameEngine();
	float sectorDim = rt->GetValue(0)->GetFloat();
	int propfieldCellCount = rt->GetValue(1)->GetInt();
	
	// clear ( important )
	nd->cveg = NULL;
	
	// create cached vegetation worker object
	nd->cveg = new dedsCachedVegetation(engine, sectorDim, propfieldCellCount);
	if(!nd->cveg) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassCachedVegetation::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsCVeg,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCachedVegetation::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sCVegNatDat *nd = (sCVegNatDat*)p_GetNativeData(myself);
	
	if(nd->cveg){
		delete nd->cveg;
		nd->cveg = NULL;
	}
}



// Management
///////////////

// public func void setCollisionFilter( CollisionFilter collisionFilter )
deClassCachedVegetation::nfSetCollisionFilter::nfSetCollisionFilter(const sInitData &init) : dsFunction(init.clsCVeg,
"setCollisionFilter", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsCF); // collisionFilter
}
void deClassCachedVegetation::nfSetCollisionFilter::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	deClassCollisionFilter &clsCF = *clsCVeg->GetDS()->GetClassCollisionFilter();
	
	const decCollisionFilter &collisionFilter = clsCF.GetCollisionFilter(rt->GetValue(0)->GetRealObject());
	
	cveg->SetCollisionFilter(collisionFilter);
}



// public func void hasSector( Point coordinates )
deClassCachedVegetation::nfHasSector::nfHasSector(const sInitData &init) : dsFunction(init.clsCVeg,
"hasSector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsPt); // coordinates
}
void deClassCachedVegetation::nfHasSector::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	
	dsRealObject *objCoord = rt->GetValue(0)->GetRealObject();
	if(!objCoord) DSTHROW(dueNullPointer);
	
	const decPoint &scoord = clsCVeg->GetDS()->GetClassPoint()->GetPoint(objCoord);
	
	rt->PushBool(cveg->GetSectorWith(scoord));
}

// public func void addSector( Point coordinates, String cacheFile )
deClassCachedVegetation::nfAddSector::nfAddSector(const sInitData &init) : dsFunction(init.clsCVeg,
"addSector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPt); // coordinates
	p_AddParameter(init.clsStr); // cacheFile
}
void deClassCachedVegetation::nfAddSector::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	
	dsRealObject *objCoord = rt->GetValue(0)->GetRealObject();
	const char *cacheFile = rt->GetValue(1)->GetString();
	
	if(!objCoord) DSTHROW(dueNullPointer);
	
	const decPoint &scoord = clsCVeg->GetDS()->GetClassPoint()->GetPoint(objCoord);
	
	cveg->AddSectorWith(scoord, cacheFile);
}

// public func void removeSector( Point coordinates )
deClassCachedVegetation::nfRemoveSector::nfRemoveSector(const sInitData &init) : dsFunction(init.clsCVeg,
"removeSector", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsPt); // coordinates
}
void deClassCachedVegetation::nfRemoveSector::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	
	dsRealObject *objCoord = rt->GetValue(0)->GetRealObject();
	if(!objCoord) DSTHROW(dueNullPointer);
	
	const decPoint &scoord = clsCVeg->GetDS()->GetClassPoint()->GetPoint(objCoord);
	
	cveg->RemoveSectorWith(scoord);
}

// public func void removeAllSectors()
deClassCachedVegetation::nfRemoveAllSectors::nfRemoveAllSectors(const sInitData &init) : dsFunction(init.clsCVeg,
"removeAllSectors", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassCachedVegetation::nfRemoveAllSectors::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	
	cveg->RemoveAllSectors();
}



// public func void setWorld( World world )
deClassCachedVegetation::nfSetWorld::nfSetWorld(const sInitData &init) : dsFunction(init.clsCVeg,
"setWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsWorld); // world
}
void deClassCachedVegetation::nfSetWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	deClassWorld *clsWorld = clsCVeg->GetDS()->GetClassWorld();
	
	dsRealObject *objWorld = rt->GetValue(0)->GetRealObject();
	
	cveg->SetWorld(clsWorld->GetWorld(objWorld));
}



// public func int hashCode()
deClassCachedVegetation::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsCVeg, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassCachedVegetation::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	
	rt->PushInt((int)(intptr_t)cveg);
}

// public func bool equals( Object obj )
deClassCachedVegetation::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsCVeg, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // obj
}
void deClassCachedVegetation::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	dedsCachedVegetation *cveg = ((sCVegNatDat*)p_GetNativeData(myself))->cveg;
	deClassCachedVegetation *clsCVeg = (deClassCachedVegetation*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsCVeg)){
		rt->PushBool(false);
		
	}else{
		dedsCachedVegetation *other = ((sCVegNatDat*)p_GetNativeData(obj))->cveg;
		
		rt->PushBool(cveg == other);
	}
}




// Class deClassCachedVegetation
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassCachedVegetation::deClassCachedVegetation(deScriptingDragonScript *ds) :
dsClass("CachedVegetation", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED){
	if(!ds) DSTHROW(dueInvalidParam);
	
	pDS = ds;
	
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sCVegNatDat));
}

deClassCachedVegetation::~deClassCachedVegetation(){
}



// Management
///////////////

void deClassCachedVegetation::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	memset(&init, '\0', sizeof(init));
	
	init.clsCVeg = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsStr = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsPt = pDS->GetClassPoint();
	init.clsCF = pDS->GetClassCollisionFilter();
	init.clsWorld = pDS->GetClassWorld();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetCollisionFilter(init));
	
	AddFunction(new nfHasSector(init));
	AddFunction(new nfAddSector(init));
	AddFunction(new nfRemoveSector(init));
	AddFunction(new nfRemoveAllSectors(init));
	
	AddFunction(new nfSetWorld(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}
