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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meLSPFCache.h"
#include "meLSPFCacheTypeList.h"

#include "../../world/heightterrain/meHTVegetationLayer.h"
#include "../../world/heightterrain/meHTVVariation.h"
#include "../../world/heightterrain/meHTVInstance.h"
#include "../../world/terrain/meHeightTerrain.h"
#include "../../world/terrain/meHeightTerrainSector.h"
#include "../../world/terrain/meHeightTerrainTexture.h"
#include "../../world/terrain/meHeightTerrainPropField.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decBaseFileReader.h>
#include <dragengine/common/file/decBaseFileWriter.h>
#include <dragengine/resources/propfield/dePropField.h>



// Class meLSPFCache
//////////////////////

// Constructor, destructor
////////////////////////////

meLSPFCache::meLSPFCache(){
	pName = "Drag[en]gine Prop Field Cache";
	pPattern = ".depfc";
}

meLSPFCache::~meLSPFCache(){
}



// Management
///////////////

void meLSPFCache::LoadFromFile(meHeightTerrainSector &sector, decBaseFileReader &file){
	double sectorDim = (double)sector.GetHeightTerrain()->GetSectorSize();
	int p, propfieldCount = sector.GetPropFieldCount();
	const decPoint &scoord = sector.GetCoordinates();
	meHeightTerrainPropField *propfield;
	meLSPFCacheTypeList typeList;
	decVector propfieldPosition;
	meHTVegetationLayer *vlayer;
	meHTVVariation *variation;
	decDVector sectorPosition;
	decVector scalePosition;
	decVector scaleRotation;
	int i, instanceCount;
	float scaleScaling;
	int t, typeCount;
	decVector irot;
	decVector ipos;
	float iscale;
	
	//printf( "meLSPFCache: load cache from %s\n", file.GetFilename() );
	
	// determine sector position
	sectorPosition.x = sectorDim * (double)scoord.x;
	sectorPosition.y = 0.0f;
	sectorPosition.z = sectorDim * (double)scoord.y;
	
	// determine scaling factors
	scaleRotation.Set((DEG2RAD * 360.0f) / 255.0f, (DEG2RAD * 360.0f) / 255.0f, (DEG2RAD * 360.0f) / 255.0f);
	
	scaleScaling = 5.0f / 255.0f;
	
	// read the header
	char header[10];
	file.Read((char*)&header, 10); // signature {char[10]}
	if(strncmp(header, "DEPFCache ", 10) != 0){
		DETHROW_INFO(deeInvalidFileFormat, file.GetFilename());
	}
	
	int version = file.ReadByte(); // version {byte}
	if(version != 1){
		DETHROW_INFO(deeInvalidFileFormat, file.GetFilename());
	}
	
	scalePosition.x = file.ReadFloat(); // scalePositionX {float}
	scalePosition.y = file.ReadFloat(); // scalePositionY {float}
	scalePosition.z = file.ReadFloat(); // scalePositionZ {float}
	
	// read the list of types and determine which vlayer and variation they might belong to
	typeCount = file.ReadShort(); // number of types {short}
	
	int lookforVLayer, lookforVariation;
	float rotPerForce, restitution;
	decString pathModel, pathSkin;
	int l, layerCount = sector.GetHeightTerrain()->GetVLayerCount();
	int v, variationCount;
	
	for(t=0; t<typeCount; t++){
		pathModel = file.ReadString8(); // model path {string8}
		pathSkin = file.ReadString8(); // skin path {string8}
		rotPerForce = file.ReadFloat(); // rotation per force {float}
		restitution = file.ReadFloat(); // restitution {float}
		
		lookforVLayer = -1;
		lookforVariation = -1;
		for(l=0; l<layerCount; l++){
			vlayer = sector.GetHeightTerrain()->GetVLayerAt(l);
			variationCount = vlayer->GetVariationCount();
			
			for(v=0; v<variationCount; v++){
				variation = vlayer->GetVariationAt(v);
				
				if(pathModel.Equals(variation->GetPathModel()) && pathSkin.Equals(variation->GetPathSkin())
				&&  fabsf(rotPerForce - variation->GetRotationPerForce()) < 0.001f
				&&  fabsf(restitution - variation->GetRestitution()) < 0.001f){
					lookforVLayer = l;
					lookforVariation = v;
					break;
				}
			}
		}
		
		if(lookforVLayer == -1 || lookforVariation == -1) DETHROW(deeInvalidAction);
		
		typeList.AddType(lookforVLayer, lookforVariation);
	}
	
	// read the prop fields
	propfieldCount = file.ReadShort(); // prop field count {short}
	if(propfieldCount != sector.GetPropFieldCount()) DETHROW(deeInvalidAction);
	
	for(p=0; p<propfieldCount; p++){
		propfield = sector.GetPropFieldAt(p);
		
		propfieldPosition = (propfield->GetEnginePropField()->GetPosition() - sectorPosition).ToVector();
		
		file.ReadFloat(); // prop field position X {float}
		file.ReadFloat(); // prop field position Y {float}
		file.ReadFloat(); // prop field position Z {float}
		instanceCount = file.ReadInt(); // instance count {int}
		
		propfield->RemoveAllVInstances();
		
		for(i=0; i<instanceCount; i++){
			meHTVInstance &instance = propfield->AddVInstance();
			
			t = file.ReadShort(); // instance type {short}
			const meLSPFCacheTypeList::sType &type = typeList.GetTypeAt(t);
			
			ipos.x = (float)file.ReadShort() * scalePosition.x; // instance position X {short, encoded}
			ipos.y = file.ReadFloat(); // instance position Y {float}
			ipos.z = (float)file.ReadShort() * scalePosition.z; // instance position Z {short, encoded}
			irot.x = (float)file.ReadByte() * scaleRotation.x; // instance rotation X {byte, encoded}
			irot.y = (float)file.ReadByte() * scaleRotation.y; // instance rotation Y {byte, encoded}
			irot.z = (float)file.ReadByte() * scaleRotation.z; // instance rotation Z {byte, encoded}
			iscale = (float)file.ReadByte() * scaleScaling; // instance scaling {byte, encoded}
			
			instance.SetParameters(type.vlayer, type.variation, ipos, irot, iscale);
		}
		
		propfield->MarkVInstancesValid();
		propfield->Validate();
	}
}

void meLSPFCache::SaveToFile(meHeightTerrainSector &sector, decBaseFileWriter &file){
	double sectorDim = (double)sector.GetHeightTerrain()->GetSectorSize();
	int p, propfieldCount = sector.GetPropFieldCount();
	const decPoint &scoord = sector.GetCoordinates();
	meHeightTerrainPropField *propfield;
	float twopi = DEG2RAD * 360.0f;
	meLSPFCacheTypeList typeList;
	decVector propfieldPosition;
	meHTVegetationLayer *vlayer;
	meHTVVariation *variation;
	decDVector sectorPosition;
	decVector scalePosition;
	decVector scaleRotation;
	int i, instanceCount;
	float scaleScaling;
	int t, typeCount;
	decVector irot;
	
	// build the list of all types
	for(p=0; p<propfieldCount; p++){
		propfield = sector.GetPropFieldAt(p);
		instanceCount = propfield->GetVInstanceCount();
		
		for(i=0; i<instanceCount; i++){
			const meHTVInstance &instance = propfield->GetVInstanceAt(i);
			
			typeList.AddType(instance.GetVLayer(), instance.GetVariation());
		}
	}
	
	// determine sector position
	sectorPosition.x = sectorDim * (double)scoord.x;
	sectorPosition.y = 0.0f;
	sectorPosition.z = sectorDim * (double)scoord.y;
	
	// determine scaling factors
	//scalePosition.Set( 32768.0f / 100.0f, 1.0f, 32768.0f / 100.0f );
	scalePosition.Set(327.68f, 1.0f, 327.68f);
	
	scaleRotation.Set(255.0f / twopi, 255.0f / twopi, 255.0f / twopi);
	
	scaleScaling = 255.0f / 5.0f;
	
	// write the header
	file.WriteString("DEPFCache "); // signature {char[10]}
	file.WriteByte(1); // version {byte}
	file.WriteFloat(1.0f / scalePosition.x); // scalePositionX {float}
	file.WriteFloat(1.0f / scalePosition.y); // scalePositionY {float}
	file.WriteFloat(1.0f / scalePosition.z); // scalePositionZ {float}
	
	// write the list of types
	typeCount = typeList.GetTypeCount();
	file.WriteShort((short)typeCount); // number of types {short}
	
	for(t=0; t<typeCount; t++){
		const meLSPFCacheTypeList::sType &type = typeList.GetTypeAt(t);
		vlayer = sector.GetHeightTerrain()->GetVLayerAt(type.vlayer);
		variation = vlayer->GetVariationAt(type.variation);
		
		file.WriteString8(variation->GetPathModel().GetString()); // model path {string8}
		file.WriteString8(variation->GetPathSkin().GetString()); // skin path {string8}
		file.WriteFloat(variation->GetRotationPerForce()); // rotation per force {float}
		file.WriteFloat(variation->GetRestitution()); // restitution {float}
	}
	
	// write the prop fields
	file.WriteShort((short)propfieldCount); // prop field count {short}
	
	for(p=0; p<propfieldCount; p++){
		propfield = sector.GetPropFieldAt(p);
		instanceCount = propfield->GetVInstanceCount();
		
		if(! propfield->GetEnginePropField()) DETHROW(deeInvalidParam); // HACK because the position is not yet in the propfield itself
		
		propfieldPosition = (propfield->GetEnginePropField()->GetPosition() - sectorPosition).ToVector();
		
		file.WriteFloat(propfieldPosition.x); // prop field position X {float}
		file.WriteFloat(propfieldPosition.y); // prop field position Y {float}
		file.WriteFloat(propfieldPosition.z); // prop field position Z {float}
		file.WriteInt(instanceCount); // instance count {int}
		
		for(i=0; i<instanceCount; i++){
			const meHTVInstance &instance = propfield->GetVInstanceAt(i);
			const decVector &ipos = instance.GetPosition();
			
			irot = instance.GetRotation();
			if(irot.x < 0.0f){
				irot.x = fmodf(irot.x, twopi) + twopi;
				
			}else{
				irot.x = fmodf(irot.x, twopi);
			}
			if(irot.y < 0.0f){
				irot.y = fmodf(irot.y, twopi) + twopi;
				
			}else{
				irot.y = fmodf(irot.y, twopi);
			}
			if(irot.z < 0.0f){
				irot.z = fmodf(irot.z, twopi) + twopi;
				
			}else{
				irot.z = fmodf(irot.z, twopi);
			}
			
			t = typeList.IndexOfTypeWith(instance.GetVLayer(), instance.GetVariation());
			if(t == -1) DETHROW(deeInvalidAction); // can never happen but better safe then sorry
			
			file.WriteShort((short)t); // instance type {short}
			file.WriteShort((short)(ipos.x * scalePosition.x)); // instance position X {short, encoded}
			file.WriteFloat(ipos.y); // instance position Y {float}
			file.WriteShort((short)(ipos.z * scalePosition.z)); // instance position Z {short, encoded}
			file.WriteByte((unsigned char)(irot.x * scaleRotation.x)); // instance rotation X {byte, encoded}
			file.WriteByte((unsigned char)(irot.y * scaleRotation.y)); // instance rotation Y {byte, encoded}
			file.WriteByte((unsigned char)(irot.z * scaleRotation.z)); // instance rotation Z {byte, encoded}
			file.WriteByte((unsigned char)(instance.GetScaling() * scaleScaling)); // instance scaling {byte, encoded}
		}
	}
}
