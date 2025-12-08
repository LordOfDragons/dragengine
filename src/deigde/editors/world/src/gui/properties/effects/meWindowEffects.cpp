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

#if 0

#include <stdint.h>

#include "meWindowEffects.h"
#include "meWEFFilterKernel.h"
#include "meWEFOverlayImage.h"
#include "meWEFColorMatrix.h"
#include "meWEFDistortImage.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../world/meCamera.h"
#include "../../worldedit.h"
#include "dragengine/resources/world/deWorld.h"
#include "dragengine/resources/camera/deCamera.h"
#include "dragengine/resources/effect/deEffect.h"
#include "dragengine/resources/effect/deEffectFilterKernel.h"
#include "dragengine/resources/effect/deEffectOverlayImage.h"
#include "dragengine/resources/effect/deEffectColorMatrix.h"
#include "dragengine/resources/effect/deEffectDistortImage.h"
#include "dragengine/resources/effect/deEffectManager.h"
#include "dragengine/resources/effect/deEffectVisitorIdentify.h"
#include "dragengine/deEngine.h"
#include "dragengine/common/exceptions.h"



// events
///////////
FXDEFMAP(meWindowEffects) meWindowEffectsMap[]={
	FXMAPFUNC(SEL_CLOSE, 0, meWindowEffects::onClose),
	FXMAPFUNC(SEL_COMMAND, meWindowEffects::ID_BTNADDEFFECT, meWindowEffects::onBtnAddEffectCommand),
	FXMAPFUNC(SEL_COMMAND, meWindowEffects::ID_LISTEFFECTS, meWindowEffects::onListEffectsChanged),
};

	

// class meWindowEffects
/////////////////////////

FXIMPLEMENT(meWindowEffects, FXTopWindow, meWindowEffectsMap, ARRAYNUMBER(meWindowEffectsMap))

// Constructor, destructor
////////////////////////////

meWindowEffects::meWindowEffects(){}

meWindowEffects::meWindowEffects(FXApp *app, meWindowMain *wndMain) :
FXTopWindow(app, "Effects Manager", NULL, NULL, DECOR_ALL, 0, 0, 300, 400, 0, 0, 0, 0, 0, 0){
	if(!wndMain) DETHROW(deeInvalidParam);
	int padding = 3;
	int spacing = 3;
	
	pWndMain = wndMain;
	pWorld = NULL;
	pPanel = NULL;
	
	// create window
	FXScrollWindow *scroll = new FXScrollWindow(this, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| SCROLLERS_NORMAL | HSCROLLING_OFF | SCROLLERS_TRACK);
	pFrameContent = new FXVerticalFrame(scroll, LAYOUT_FILL_X | LAYOUT_FILL_Y
		| LAYOUT_TOP | LAYOUT_LEFT, 0, 0, 0, 0, 10, 10, 10, 10);
	
	// effect types list
	FXGroupBox *groupBox = new FXGroupBox(pFrameContent, "Effects:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding);
	FXVerticalFrame *frameBox = new FXVerticalFrame(groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	
	FXHorizontalFrame *frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	pCBEffectTypes = new FXComboBox(frameLine, 10, this, ID_CBEFFECTTYPES, FRAME_SUNKEN | LAYOUT_FILL_X);
	pCBEffectTypes->setEditable(false);
	pCBEffectTypes->setNumVisible(6);
 	new FXButton(frameLine, "Add Effect", NULL, this, ID_BTNADDEFFECT, BUTTON_NORMAL);
	
	pCBEffectTypes->appendItem(GetEffectTypeNameFor(deEffectVisitorIdentify::eetOverlayImage), (void*)deEffectVisitorIdentify::eetOverlayImage);
	pCBEffectTypes->appendItem(GetEffectTypeNameFor(deEffectVisitorIdentify::eetFilterKernel), (void*)deEffectVisitorIdentify::eetFilterKernel);
	pCBEffectTypes->appendItem(GetEffectTypeNameFor(deEffectVisitorIdentify::eetColorMatrix), (void*)deEffectVisitorIdentify::eetColorMatrix);
	pCBEffectTypes->appendItem(GetEffectTypeNameFor(deEffectVisitorIdentify::eetDistortImage), (void*)deEffectVisitorIdentify::eetDistortImage);
	
	// effects list
	pListEffects = new FXList(frameBox, this, ID_LISTEFFECTS, FX::LIST_BROWSESELECT |
		LAYOUT_FILL_X | LAYOUT_FILL_Y | FRAME_SUNKEN);
	pListEffects->setNumVisible(5);
	
	// realize window
	create();
}

meWindowEffects::~meWindowEffects(){
}



// Management
///////////////

void meWindowEffects::UpdateWorld(){
	meWorld *world = pWndMain->GetWorld();
	
	// check if the world changed
	if(world == pWorld) return;
	
	// clear panel and lists
	if(pPanel){
		delete pPanel;
		pPanel = NULL;
	}
	pListEffects->clearItems();
	
	// set world and update lists
	pWorld = world;
	UpdateEffectsList();
	update();
}

const char *meWindowEffects::GetEffectTypeNameFor(int type) const{
	switch(type){
	case deEffectVisitorIdentify::eetFilterKernel:
		return "Filter Kernel";
		
	case deEffectVisitorIdentify::eetOverlayImage:
		return "Overlay Image";
		
	case deEffectVisitorIdentify::eetColorMatrix:
		return "Color Matrix";
		
	case deEffectVisitorIdentify::eetDistortImage:
		return "Distort Image";
		
	default:
		return "Unknown";
	}
}

void meWindowEffects::UpdateEffectsList(){
	deEffectVisitorIdentify identify;
	deCamera *engCamera = NULL;
	deEffect::Ref engEffect;
	int e, effectCount;
	meCamera *camera;
	
	// clear list
	pListEffects->clearItems();
	
	// add effects if there is a camera
	if(pWorld){
		camera = pWorld->GetActiveCamera();
		if(camera) engCamera = camera->GetEngineCamera();
	}
	
	if(engCamera){
		effectCount = engCamera->GetEffectCount();
		for(e=0; e<effectCount; e++){
			engEffect = engCamera->GetEffectAt(e);
			engEffect->Visit(identify);
			pListEffects->appendItem(GetEffectTypeNameFor(identify.GetType()), NULL, engEffect);
		}
	}
}

void meWindowEffects::DisplayPanelFor(deEffect::Ref engEffect){
	deEffectVisitorIdentify identify;
	
	// remove the old panel if existing
	if(pPanel){
		delete pPanel;
		pPanel = NULL;
	}
	
	// create new panel if there is an effect
	if(engEffect){
		engEffect->Visit(&identify);
		if(identify.IsFilterKernel()){
			pPanel = new meWEFFilterKernel(identify.CastToFilterKernel(), this, pFrameContent);
			if(!pPanel) DETHROW(deeOutOfMemory);
			
		}else if(identify.IsOverlayImage()){
			pPanel = new meWEFOverlayImage(identify.CastToOverlayImage(), this, pFrameContent);
			if(!pPanel) DETHROW(deeOutOfMemory);
			
		}else if(identify.IsColorMatrix()){
			pPanel = new meWEFColorMatrix(identify.CastToColorMatrix(), this, pFrameContent);
			if(!pPanel) DETHROW(deeOutOfMemory);
			
		}else if(identify.IsDistortImage()){
			pPanel = new meWEFDistortImage(identify.CastToDistortImage(), this, pFrameContent);
			if(!pPanel) DETHROW(deeOutOfMemory);
		}
	}
}



// Events
///////////

long meWindowEffects::onClose(FXObject *sender, FXSelector selector, void *data){
	hide();
	return 1; // 1 = do not close window
}

long meWindowEffects::onBtnAddEffectCommand(FXObject *sender, FXSelector selector, void *data){
	int selection = pCBEffectTypes->getCurrentItem();
	deEffectManager *engEffectManager;
	deCamera *engCamera = NULL;
	deEffect::Ref engEffect = NULL;
	deEngine *engine;
	meCamera *camera;
	int type;
	
	// nothing select means nothing to do
	if(selection == -1) return 1;
	
	// determine the type of the effect to add
	type = (intptr_t)pCBEffectTypes->getItemData(selection);
	if(type == deEffectVisitorIdentify::eetUnknown) return 1;
	
	// if there is no usable camera we go home
	if(pWorld){
		engine = pWorld->GetEngine();
		
		camera = pWorld->GetActiveCamera();
		if(camera) engCamera = camera->GetEngineCamera();
	}
	
	if(!engCamera) return 1;
	engEffectManager = engine->GetEffectManager();
	
	try{
		// create effect
		switch(type){
		case deEffectVisitorIdentify::eetFilterKernel:
			engEffect = engEffectManager->CreateEffectFilterKernel();
			break;
			
		case deEffectVisitorIdentify::eetOverlayImage:
			engEffect = engEffectManager->CreateEffectOverlayImage();
			break;
			
		case deEffectVisitorIdentify::eetColorMatrix:
			engEffect = engEffectManager->CreateEffectColorMatrix();
			break;
			
		case deEffectVisitorIdentify::eetDistortImage:
			engEffect = engEffectManager->CreateEffectDistortImage();
			break;
			
		default:
			DETHROW(deeInvalidParam);
		}
		
		// add effect to world
		engCamera->AddEffect(engEffect);
	}catch(const deException &){
		if(engEffect) engEffect->FreeReference();
		throw;
	}
	
	// add to list
	pListEffects->appendItem(GetEffectTypeNameFor(type), NULL, engEffect);
	pListEffects->setCurrentItem(pListEffects->getNumItems() - 1);
	
	// add panel
	DisplayPanelFor(engEffect);
	
	return 1;
}

long meWindowEffects::onListEffectsChanged(FXObject *sender, FXSelector selector, void *data){
	int selection = pListEffects->getCurrentItem();
	deEffect::Ref engEffect = NULL;
	
	if(selection > -1){
		engEffect = (deEffect*)pListEffects->getItemData(selection);
	}
	
	DisplayPanelFor(engEffect);
	
	return 1;
}

#endif
