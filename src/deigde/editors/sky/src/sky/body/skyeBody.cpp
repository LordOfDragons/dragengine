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

#include "skyeBody.h"
#include "../skyeSky.h"
#include "../layer/skyeLayer.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class skyeBody
////////////////////

// Constructor, destructor
////////////////////////////

skyeBody::skyeBody(deEngine *engine) :
pEngine(engine),

pLayer(nullptr),

pSize(5.0f, 5.0f),
pColor(1.0f, 1.0f, 1.0f),

pSelected(false),
pActive(false)
{
	if(!engine){
		DETHROW(deeInvalidParam);
	}
}

skyeBody::~skyeBody(){
}



// Management
///////////////

void skyeBody::SetLayer(skyeLayer *layer){
	if(layer == pLayer){
		return;
	}
	
	pLayer = layer;
	UpdateRelativeResources();
}

void skyeBody::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	NotifyBodyChanged();
}

void skyeBody::SetSize(const decVector2 &size){
	if(size.IsEqualTo(pSize)){
		return;
	}
	
	pSize = size;
	NotifyBodyChanged();
}

void skyeBody::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	NotifyBodyChanged();
}

void skyeBody::SetSkinPath(const char *skinPath){
	if(pPathSkin == skinPath){
		return;
	}
	
	pPathSkin = skinPath;
	pUpdateSkin();
	NotifyBodyChanged();
}



void skyeBody::SetSelected(bool selected){
	pSelected = selected;
}

void skyeBody::SetActive(bool active){
	pActive = active;
}

void skyeBody::UpdateRelativeResources(){
	pUpdateSkin();
}



void skyeBody::NotifyBodyChanged(){
	if(!pLayer){
		return;
	}
	
	skyeSky * const sky = pLayer->GetSky();
	if(!sky){
		return;
	}
	
	sky->NotifyBodyChanged(pLayer, this);
}



// Private Functions
//////////////////////

void skyeBody::pUpdateSkin(){
	if(pPathSkin.IsEmpty()){
		pEngSkin = nullptr;
		return;
	}
	
	deSkinManager &skinMgr = *pEngine->GetSkinManager();
	const char *basePath = "/";
	if(pLayer && pLayer->GetSky()){
		basePath = pLayer->GetSky()->GetDirectoryPath();
	}
	
	try{
		pEngSkin = skinMgr.LoadSkin(pPathSkin, basePath);
		
	}catch(const deException &){
		if(pLayer && pLayer->GetSky()){
			pEngSkin = pLayer->GetSky()->GetEnvironment()->GetStockSkin(igdeEnvironment::essError);
		}
	}
}
