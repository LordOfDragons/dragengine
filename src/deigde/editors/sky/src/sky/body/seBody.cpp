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

#include "seBody.h"
#include "../seSky.h"
#include "../layer/seLayer.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinManager.h>



// Class seBody
////////////////////

// Constructor, destructor
////////////////////////////

seBody::seBody(deEngine *engine) :
pEngine(engine),

pLayer(NULL),

pSize(5.0f, 5.0f),
pColor(1.0f, 1.0f, 1.0f),

pSelected(false),
pActive(false)
{
	if(! engine){
		DETHROW(deeInvalidParam);
	}
}

seBody::~seBody(){
}



// Management
///////////////

void seBody::SetLayer(seLayer *layer){
	if(layer == pLayer){
		return;
	}
	
	pLayer = layer;
	UpdateRelativeResources();
}

void seBody::SetOrientation(const decVector &orientation){
	if(orientation.IsEqualTo(pOrientation)){
		return;
	}
	
	pOrientation = orientation;
	NotifyBodyChanged();
}

void seBody::SetSize(const decVector2 &size){
	if(size.IsEqualTo(pSize)){
		return;
	}
	
	pSize = size;
	NotifyBodyChanged();
}

void seBody::SetColor(const decColor &color){
	if(color.IsEqualTo(pColor)){
		return;
	}
	
	pColor = color;
	NotifyBodyChanged();
}

void seBody::SetSkinPath(const char *skinPath){
	if(pPathSkin == skinPath){
		return;
	}
	
	pPathSkin = skinPath;
	pUpdateSkin();
	NotifyBodyChanged();
}



void seBody::SetSelected(bool selected){
	pSelected = selected;
}

void seBody::SetActive(bool active){
	pActive = active;
}

void seBody::UpdateRelativeResources(){
	pUpdateSkin();
}



void seBody::NotifyBodyChanged(){
	if(! pLayer){
		return;
	}
	
	seSky * const sky = pLayer->GetSky();
	if(! sky){
		return;
	}
	
	sky->NotifyBodyChanged(pLayer, this);
}



// Private Functions
//////////////////////

void seBody::pUpdateSkin(){
	if(pPathSkin.IsEmpty()){
		pEngSkin = NULL;
		return;
	}
	
	deSkinManager &skinMgr = *pEngine->GetSkinManager();
	const char *basePath = "/";
	if(pLayer && pLayer->GetSky()){
		basePath = pLayer->GetSky()->GetDirectoryPath();
	}
	
	try{
		pEngSkin.TakeOver(skinMgr.LoadSkin(pPathSkin, basePath));
		
	}catch(const deException &){
		if(pLayer && pLayer->GetSky()){
			pEngSkin = pLayer->GetSky()->GetEnvironment()->GetStockSkin(igdeEnvironment::essError);
		}
	}
}
