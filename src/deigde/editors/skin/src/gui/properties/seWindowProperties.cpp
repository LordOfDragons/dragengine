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

#include "seWindowProperties.h"
#include "seWPMapped.h"
#include "seWPTexture.h"
#include "seWPNode.h"
#include "seWPView.h"
#include "seWPUndoHistory.h"
#include "dynamicskin/seWPDynamicSkin.h"
#include "../seWindowMain.h"
#include "../../skin/seSkin.h"

#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/common/exceptions.h>



// Class seWindowProperties
/////////////////////////////

// Constructor, destructor
////////////////////////////

seWindowProperties::seWindowProperties(seWindowMain &windowMain) :
igdeTabBook(windowMain.GetEnvironment()),
pWindowMain(windowMain)
{
	SetWidgetGuiThemeName(igdeGuiThemeNames::properties);
	
	pPanelMapped.TakeOver(new seWPMapped(*this));
	AddChild(pPanelMapped, "Mapped");
	
	pPanelTexture.TakeOver(new seWPTexture(*this));
	AddChild(pPanelTexture, "Texture");
	
	pPanelNode.TakeOver(new seWPNode(*this));
	AddChild(pPanelNode, "Node");
	
	pPanelDynamicSkin.TakeOver(new seWPDynamicSkin(*this));
	AddChild(pPanelDynamicSkin, "Dynamic Skin");
	
	pPanelView.TakeOver(new seWPView(*this));
	AddChild(pPanelView, "View");
	
	pPanelUndoHistory.TakeOver(new seWPUndoHistory(GetEnvironment()));
	AddChild(pPanelUndoHistory, "Undo");
	
	SetActivePanel(1); // texture
}

seWindowProperties::~seWindowProperties(){
}



// Management
///////////////

void seWindowProperties::SetSkin(seSkin *skin){
	((seWPMapped&)(igdeWidget&)pPanelMapped).SetSkin(skin);
	((seWPTexture&)(igdeWidget&)pPanelTexture).SetSkin(skin);
	((seWPNode&)(igdeWidget&)pPanelNode).SetSkin(skin);
	((seWPDynamicSkin&)(igdeWidget&)pPanelDynamicSkin).SetSkin(skin);
	((seWPView&)(igdeWidget&)pPanelView).SetSkin(skin);
	((seWPUndoHistory&)(igdeWidget&)pPanelUndoHistory).SetSkin(skin);
}

void seWindowProperties::OnSkinPathChanged(){
	((seWPTexture&)(igdeWidget&)pPanelTexture).OnSkinPathChanged();
	((seWPNode&)(igdeWidget&)pPanelNode).OnSkinPathChanged();
}
