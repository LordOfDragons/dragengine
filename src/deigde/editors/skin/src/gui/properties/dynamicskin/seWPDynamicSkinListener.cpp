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

#include <stdlib.h>
#include <string.h>

#include "seWPDynamicSkin.h"
#include "seWPDynamicSkinListener.h"
#include "../../../skin/seSkin.h"
#include "../../../skin/dynamicskin/seDynamicSkin.h"
#include "../../../skin/dynamicskin/seDynamicSkinRenderable.h"

#include <dragengine/common/exceptions.h>



// Class seWPDynamicSkinListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

seWPDynamicSkinListener::seWPDynamicSkinListener(seWPDynamicSkin &panel) :
pPanel(panel){
}

seWPDynamicSkinListener::~seWPDynamicSkinListener(){
}



// Management
///////////////

void seWPDynamicSkinListener::DynamicSkinRenderableStructureChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.UpdateRenderableList();
}

void seWPDynamicSkinListener::DynamicSkinRenderableChanged(seSkin *skin,
seDynamicSkinRenderable *renderable){
	if(skin != pPanel.GetSkin() || ! renderable->GetActive()){
		return;
	}
	
	pPanel.ShowValuePanel();
	pPanel.UpdateRenderable();
}

void seWPDynamicSkinListener::DynamicSkinRenderableNameChanged(seSkin *skin,
seDynamicSkinRenderable *renderable){
	if(skin != pPanel.GetSkin() || ! renderable->GetActive()){
		return;
	}
	
	pPanel.UpdateRenderableList();
}

void seWPDynamicSkinListener::DynamicSkinActiveRenderableChanged(seSkin *skin){
	if(skin != pPanel.GetSkin()){
		return;
	}
	
	pPanel.SelectActiveRenderable();
	pPanel.ShowValuePanel();
}
