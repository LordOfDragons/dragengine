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

#include "deoalAComponent.h"
#include "deoalAComponentTexture.h"
#include "deoalComponent.h"
#include "deoalComponentDebug.h"
#include "deoalComponentTexture.h"
#include "../deAudioOpenAL.h"
#include "../devmode/deoalDevMode.h"
#include "../microphone/deoalMicrophone.h"
#include "../model/deoalModel.h"
#include "../model/deoalAModel.h"
#include "../model/deoalModelFace.h"
#include "../skin/deoalSkin.h"
#include "../skin/deoalSkinTexture.h"
#include "../world/deoalWorld.h"
#include "../world/deoalAWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/resources/component/deComponentTexture.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/model/deModelFace.h>
#include <dragengine/resources/model/deModelTexture.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/world/deWorld.h>



// Class deoalComponent
/////////////////////////

// Constructor, destructor
////////////////////////////

deoalComponentDebug::deoalComponentDebug(deoalComponent &component) :
pComponent(component),
pDirtyFaces(true){
}

deoalComponentDebug::~deoalComponentDebug(){
	pCleanUp();
}



// Management
///////////////

void deoalComponentDebug::DirtyFaces(){
	pDirtyFaces = true;
}

void deoalComponentDebug::Update(){
	if(pDirtyFaces){
		pDirtyFaces = false;
		pUpdateDDFaces();
	}
	
	if(pDDFaces){
		pDDFaces->SetPosition(pComponent.GetComponent().GetPosition());
		pDDFaces->SetOrientation(pComponent.GetComponent().GetOrientation());
		pDDFaces->SetScale(pComponent.GetComponent().GetScaling());
	}
}



// Private Functions
//////////////////////

void deoalComponentDebug::pCleanUp(){
	if(pDDFaces){
		if(pDDFaces->GetParentWorld()){
			pDDFaces->GetParentWorld()->RemoveDebugDrawer(pDDFaces);
		}
	}
}

void deoalComponentDebug::pUpdateDDFaces(){
	const deoalMicrophone * const microphone = pComponent.GetOal().GetActiveMicrophone();
	const deoalWorld * const world = pComponent.GetParentWorld();
	const decLayerMask &layerMask = pComponent.GetComponent().GetLayerMask();
	const deoalAComponent &acomponent = *pComponent.GetAComponent();
	const bool enabled = pComponent.GetOal().GetDevMode()->GetShowAudioModels()
		&& acomponent.GetAffectsSound()
		&& world && microphone && microphone->GetParentWorld() == world
		&& layerMask.Matches(microphone->GetMicrophone().GetLayerMask());
	
	if(!enabled){
		if(pDDFaces){
			if(pDDFaces->GetParentWorld()){
				pDDFaces->GetParentWorld()->RemoveDebugDrawer(pDDFaces);
			}
			pDDFaces = NULL;
		}
		return;
	}
	
	if(!pDDFaces){
		pDDFaces.TakeOver(pComponent.GetOal().GetGameEngine()->GetDebugDrawerManager()->CreateDebugDrawer());
		pDDFaces->SetXRay(true);
		pDDFaces->SetVisible(true);
		pComponent.GetParentWorld()->GetWorld().AddDebugDrawer(pDDFaces);
	}
	
	pDDFaces->RemoveAllShapes();
	
	if(!pComponent.GetAudioModel()){
		return;
	}
	
	const decColor color(1.0f, 0.5f, 0.0f);
	const float alphaFill = 0.1f;
	const float alphaEdge = 0.3f;
	
	const decColor colorFill(color, alphaFill);
	const decColor colorEdge(color, alphaEdge);
	
	const deoalAModel &model = *pComponent.GetAudioModel()->GetAModel();
	const decIntList &mappingTexture = acomponent.GetModelTextureMappings();
	const int textureCount = model.GetTextureNames().GetCount();
	const int faceCount = model.GetFaceCount();
	int i, j;
	
	for(i=0; i<textureCount; i++){
		const int index = mappingTexture.GetAt(i);
		if(index == -1){
			continue;
		}
		
		const deoalComponentTexture &componentTexture = pComponent.GetTextureAt(index);
		if(!componentTexture.GetATexture()->GetAffectsSound()){
			continue;
		}
		
		deDebugDrawerShape *shape = NULL;
		deDebugDrawerShapeFace *shapeFace = NULL;
		try{
			shape = new deDebugDrawerShape();
			shape->SetFillColor(colorFill);
			shape->SetEdgeColor(colorEdge);
			
			if(acomponent.GetFaceCount() == 0){
				for(j=0; j<faceCount; j++){
					const deoalModelFace &face = model.GetFaceAt(j);
					if(face.GetTexture() != i){
						continue;
					}
					
					shapeFace = new deDebugDrawerShapeFace;
					shapeFace->AddVertex(face.GetVertex1());
					shapeFace->AddVertex(face.GetVertex2());
					shapeFace->AddVertex(face.GetVertex3());
					shapeFace->SetNormal(face.GetNormal());
					shape->AddFace(shapeFace);
					shapeFace = NULL;
				}
				
			}else{
				for(j=0; j<faceCount; j++){
					const deoalModelFace &face = acomponent.GetFaceAt(j);
					if(face.GetTexture() != i){
						continue;
					}
					
					shapeFace = new deDebugDrawerShapeFace;
					shapeFace->AddVertex(face.GetVertex1());
					shapeFace->AddVertex(face.GetVertex2());
					shapeFace->AddVertex(face.GetVertex3());
					shapeFace->SetNormal(face.GetNormal());
					shape->AddFace(shapeFace);
					shapeFace = NULL;
				}
			}
			
			if(shape->GetFaceCount() > 0){
				pDDFaces->AddShape(shape);
				
			}else{
				delete shape;
			}
			
		}catch(const deException &){
			if(shapeFace){
				delete shapeFace;
			}
			if(shape){
				delete shape;
			}
			throw;
		}
	}
}
