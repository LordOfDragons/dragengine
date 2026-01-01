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
#include "meHeightTerrainPFLayer.h"
#include "meHeightTerrainPFType.h"
#include "../meWorld.h"
#include "../../utils/meByteArray.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelManager.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>
#include <dragengine/resources/propfield/dePropField.h>
#include <dragengine/resources/propfield/dePropFieldType.h>
#include <dragengine/resources/propfield/dePropFieldInstance.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "World Editor"



// Class meHeightTerrainPFLayer
/////////////////////////////////

// Constructor, destructor
////////////////////////////

meHeightTerrainPFLayer::meHeightTerrainPFLayer(deEngine *engine) :
pHTSector(nullptr),
pEngine(engine),
pMask(nullptr),
pMaskChanged(false),
pMaskSaved(false)
{
	if(!engine){
		DETHROW(deeInvalidParam);
	}
	
	try{
		pResizeMask();
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

meHeightTerrainPFLayer::~meHeightTerrainPFLayer(){
	pCleanUp();
}



// Management
///////////////

void meHeightTerrainPFLayer::SetHTSector(meHeightTerrainSector *htsector){
	if(htsector == pHTSector){
		return;
	}
	
	pHTSector = htsector;
	
	if(htsector){
		pResizeMask();
		RebuildInstances();
	}
}



void meHeightTerrainPFLayer::SetPathMask(const char *path){
	if(!path){
		DETHROW(deeInvalidParam);
	}
	
	if(pPathMask.Equals(path)){
		return;
	}
	
	pPathMask = path;
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetChanged(true);
		pHTSector->GetHeightTerrain()->SetDepChanged(true);
	}
}

void meHeightTerrainPFLayer::SetMaskChanged(bool changed){
	if(changed == pMaskChanged){
		return;
	}
	
	pMaskChanged = changed;
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetDepChanged(true);
	}
}

void meHeightTerrainPFLayer::SetMaskSaved(bool saved){
	pMaskSaved = saved;
}

void meHeightTerrainPFLayer::NotifyMaskChanged(){
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	
	pHTSector->GetHeightTerrain()->SetDepChanged(true);
	//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPropFieldMaskChanged( pHTSector, this );
}

void meHeightTerrainPFLayer::LoadMaskFromImage(){
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	
	const int resolution = pHTSector->GetHeightTerrain()->GetSectorResolution();
	deImage::Ref image;
	decPath path;
	int x, y, i;
	
	if(!pPathMask.IsEmpty()){
		path.SetFromUnix(pPathMask.GetString());
		
		if(pEngine->GetVirtualFileSystem()->ExistsFile(path)){
			try{
				image = pEngine->GetImageManager()->LoadImage(pPathMask.GetString(), "/");
				
				if(image){
					if(image->GetComponentCount() != 1 || image->GetWidth() != resolution || image->GetHeight() != resolution){
						DETHROW(deeInvalidParam);
						// TODO tell the user this is not going to work
					}
					
					if(image->GetBitCount() == 8){
						sGrayscale8 *data = image->GetDataGrayscale8();
						i = 0;
						for(y=0; y<resolution; y++){
							for(x=0; x<resolution; x++){
								pMask->SetValueAt(x, y, data[i++].value);
							}
						}
						
					}else if(image->GetBitCount() == 16){
						sGrayscale16 *data = image->GetDataGrayscale16();
						
						i = 0;
						for(y=0; y<resolution; y++){
							for(x=0; x<resolution; x++){
								pMask->SetValueAt(x, y, data[i++].value >> 8);
							}
						}
						
					}else if(image->GetBitCount() == 32){
						sGrayscale32 *data = image->GetDataGrayscale32();
						float value;
						
						i = 0;
						for(y=0; y<resolution; y++){
							for(x=0; x<resolution; x++){
								value = data[i++].value;
								
								if(value < 0.0f){
									pMask->SetValueAt(x, y, 0);
									
								}else if(value > 1.0f){
									pMask->SetValueAt(x, y, 255);
									
								}else{
									pMask->SetValueAt(x, y, (int)(value * 255.0f));
								}
							}
						}
					}
				}
				
			}catch(const deException &e){
				image = nullptr;
				if(pHTSector && pHTSector->GetHeightTerrain()){
					pHTSector->GetHeightTerrain()->GetWorld().GetLogger()->LogException(LOGSOURCE, e);
				}
			}
		}
	}
	
	SetMaskSaved(image.IsNotNull());
	SetMaskChanged(true);
	
	if(pHTSector){
		pHTSector->InvalidatePropFields(decPoint(), decPoint(resolution, resolution));
	}
	
	NotifyMaskChanged();
}

void meHeightTerrainPFLayer::SectorSizeOrResChanged(){
	pResizeMask();
	RebuildInstances();
}



void meHeightTerrainPFLayer::RebuildInstances(){
}



// Types
//////////

void meHeightTerrainPFLayer::AddType(meHeightTerrainPFType *type){
	DEASSERT_NOTNULL(type)
	pTypes.Add(type);
	type->SetPFLayer(this);
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetChanged(true);
		pHTSector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPFTypeCountChanged( pHTSector, this );
	}
}

void meHeightTerrainPFLayer::RemoveType(meHeightTerrainPFType *type){
	const meHeightTerrainPFType::Ref guard(type);
	pTypes.Remove(type);
	type->SetPFLayer(nullptr);
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetChanged(true);
		pHTSector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPFTypeCountChanged( pHTSector, this );
	}
}

void meHeightTerrainPFLayer::RemoveAllTypes(){
	pTypes.Visit([](meHeightTerrainPFType &type){
		type.SetPFLayer(nullptr);
	});
	
	if(pHTSector && pHTSector->GetHeightTerrain()){
		pHTSector->GetHeightTerrain()->SetChanged(true);
		pHTSector->GetHeightTerrain()->RebuildVegetationPropFieldTypes();
		//pHTSector->GetHeightTerrain()->GetWorld().NotifyHTSPFTypeCountChanged( pHTSector, this );
	}
}




// Private Functions
//////////////////////

void meHeightTerrainPFLayer::pCleanUp(){
	RemoveAllTypes();
	
	if(pMask){
		delete pMask;
	}
}

void meHeightTerrainPFLayer::pResizeMask(){
	if(!pHTSector || !pHTSector->GetHeightTerrain()){
		return;
	}
	
	const int resolution = pHTSector->GetHeightTerrain()->GetSectorResolution();
	meByteArray *newMask = nullptr;
	try{
		newMask = new meByteArray(resolution, resolution);
		newMask->SetAll(255);
		
		if(pMask){
			delete pMask;
		}
		
		pMask = newMask;
		
	}catch(const deException &){
		if(newMask){
			delete newMask;
		}
		throw;
	}
	
	NotifyMaskChanged();
}
