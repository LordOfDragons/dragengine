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

// includes
/////////////
#include "meWEFOverlayImage.h"
#include "meWindowEffects.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../worldedit.h"

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/effect/deEffectOverlayImage.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>



// events
///////////
FXDEFMAP(meWEFOverlayImage) meWEFOverlayImageMap[] = {
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITIMAGE, meWEFOverlayImage::onEditImageCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITTRANSPARENCY, meWEFOverlayImage::onEditTransparencyCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER1X, meWEFOverlayImage::onEditCorner1XCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER1Y, meWEFOverlayImage::onEditCorner1YCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER2X, meWEFOverlayImage::onEditCorner2XCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER2Y, meWEFOverlayImage::onEditCorner2YCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER3X, meWEFOverlayImage::onEditCorner3XCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER3Y, meWEFOverlayImage::onEditCorner3YCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER4X, meWEFOverlayImage::onEditCorner4XCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_EDITCORNER4Y, meWEFOverlayImage::onEditCorner4YCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFOverlayImage::ID_CHKENABLE, meWEFOverlayImage::onCheckEnableCommand),
};




// class meWEFOverlayImage
//////////////////////

FXIMPLEMENT(meWEFOverlayImage, FXVerticalFrame, meWEFOverlayImageMap, ARRAYNUMBER(meWEFOverlayImageMap))

// Constructor, destructor
////////////////////////////

meWEFOverlayImage::meWEFOverlayImage(){}

meWEFOverlayImage::meWEFOverlayImage(deEffectOverlayImage *effectOverlay, meWindowEffects *windowEffects, FXComposite *container) :
FXVerticalFrame(container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT,
0, 0, 0, 0, 0, 0, 0, 0){
	if(!effectOverlay || !windowEffects) DETHROW(deeInvalidParam);
	int padding = 3;
	int spacing = 3;
	FXString text;
	
	// prepare
	pWndEffects = windowEffects;
	pEffectOverlay = effectOverlay;
	pEngine = windowEffects->GetWindowMain()->GetWorld()->GetEngine();
	
	// group box
	FXGroupBox *groupBox = new FXGroupBox(this, "Overlay Effect:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding);
	FXVerticalFrame *frameBox = new FXVerticalFrame(groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	
	// image selection
	FXHorizontalFrame *frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Image:");
	pEditImage = new FXTextField(frameLine, 10, this, ID_EDITIMAGE, FRAME_SUNKEN | LAYOUT_FILL_X);
// 	new FXButton( frameLine, "...", nullptr, this, ID_BTNSELECTCLASS, BUTTON_NORMAL );
 	new FXButton(frameLine, "...", nullptr, nullptr, 0, BUTTON_NORMAL);
	
	// transparency
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Transparency:");
	pEditTransparency = new FXTextField(frameLine, 5, this, ID_EDITTRANSPARENCY, FRAME_SUNKEN);
	
	// image location
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Corner 1 X:");
	pEditCorner1X = new FXTextField(frameLine, 5, this, ID_EDITCORNER1X, FRAME_SUNKEN);
	new FXLabel(frameLine, "Y:");
	pEditCorner1Y = new FXTextField(frameLine, 5, this, ID_EDITCORNER1Y, FRAME_SUNKEN);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Corner 2 X:");
	pEditCorner2X = new FXTextField(frameLine, 5, this, ID_EDITCORNER2X, FRAME_SUNKEN);
	new FXLabel(frameLine, "Y:");
	pEditCorner2Y = new FXTextField(frameLine, 5, this, ID_EDITCORNER2Y, FRAME_SUNKEN);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Corner 3 X:");
	pEditCorner3X = new FXTextField(frameLine, 5, this, ID_EDITCORNER3X, FRAME_SUNKEN);
	new FXLabel(frameLine, "Y:");
	pEditCorner3Y = new FXTextField(frameLine, 5, this, ID_EDITCORNER3Y, FRAME_SUNKEN);
	
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Corner 4 X:");
	pEditCorner4X = new FXTextField(frameLine, 5, this, ID_EDITCORNER4X, FRAME_SUNKEN);
	new FXLabel(frameLine, "Y:");
	pEditCorner4Y = new FXTextField(frameLine, 5, this, ID_EDITCORNER4Y, FRAME_SUNKEN);
	
	// disabled
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
 	pChkEnable = new FXCheckButton(frameLine, "Enable Effect", this, ID_CHKENABLE, CHECKBUTTON_NORMAL);
	pChkEnable->setCheck(pEffectOverlay->GetEnabled());
	
	// set values
	deImage::Ref image = pEffectOverlay->GetImage();
	if(image){
		pEditImage->setText(image->GetFilename().GetString());
	}else{
		pEditImage->setText("");
	}
	text.format("%g", pEffectOverlay->GetTransparency());
	pEditTransparency->setText(text);
	
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(0).x);
	pEditCorner1X->setText(text);
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(0).y);
	pEditCorner1Y->setText(text);
	
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(1).x);
	pEditCorner2X->setText(text);
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(1).y);
	pEditCorner2Y->setText(text);
	
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(2).x);
	pEditCorner3X->setText(text);
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(2).y);
	pEditCorner3Y->setText(text);
	
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(3).x);
	pEditCorner4X->setText(text);
	text.format("%g", pEffectOverlay->GetTextureCoordinatesFor(3).y);
	pEditCorner4Y->setText(text);
	
	// realize
	create();
}

meWEFOverlayImage::~meWEFOverlayImage(){
}



// Management
///////////////



// Events
///////////

long meWEFOverlayImage::onEditImageCommand(FXObject *sender, FXSelector selector, void *data){
	deImageManager *imageManager = pEngine->GetImageManager();
	deVirtualFileSystem *vfs = pEngine->GetVirtualFileSystem();
	const FXString &filename = pEditImage->getText();
	deImage::Ref image;
	decPath path;
	
	path.SetFromUnix(filename.text());
	if(vfs->ExistsFile(path)){
		try{
			image = imageManager->LoadImage(filename.text(), "/");
			pEffectOverlay->SetImage(image);
		}catch(const deException &e){
			pWndEffects->GetWindowMain()->GetLogger()->LogException("World Editor", e);
		}
	}
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditTransparencyCommand(FXObject *sender, FXSelector selector, void *data){
	pEffectOverlay->SetTransparency(strtof(pEditTransparency->getText().text(), nullptr));
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner1XCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(0);
	
	position.x = strtof(pEditCorner1X->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(0, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner1YCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(0);
	
	position.y = strtof(pEditCorner1Y->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(0, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner2XCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(1);
	
	position.x = strtof(pEditCorner2X->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(1, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner2YCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(1);
	
	position.y = strtof(pEditCorner2Y->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(1, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner3XCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(2);
	
	position.x = strtof(pEditCorner3X->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(2, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner3YCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(2);
	
	position.y = strtof(pEditCorner3Y->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(2, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner4XCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(3);
	
	position.x = strtof(pEditCorner4X->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(3, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onEditCorner4YCommand(FXObject *sender, FXSelector selector, void *data){
	decVector2 position = pEffectOverlay->GetTextureCoordinatesFor(3);
	
	position.y = strtof(pEditCorner4Y->getText().text(), nullptr);
	pEffectOverlay->SetTextureCoordinatesFor(3, position);
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFOverlayImage::onCheckEnableCommand(FXObject *sender, FXSelector selector, void *data){
	pEffectOverlay->SetEnabled(pChkEnable->getCheck());
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

#endif
