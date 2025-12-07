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
#include "meWEFDistortImage.h"
#include "meWindowEffects.h"
#include "../meWindowMain.h"
#include "../../world/meWorld.h"
#include "../../worldedit.h"

#include "dragengine/deEngine.h"
#include <dragengine/logger/deLogger.h>
#include "dragengine/resources/effect/deEffectDistortImage.h"
#include "dragengine/resources/image/deImage.h"
#include "dragengine/resources/image/deImageManager.h"
#include "dragengine/filesystem/deVirtualFileSystem.h"
#include "dragengine/common/file/decPath.h"
#include "dragengine/common/exceptions.h"



// events
///////////
FXDEFMAP(meWEFDistortImage) meWEFDistortImageMap[] = {
	FXMAPFUNC(SEL_COMMAND, meWEFDistortImage::ID_EDITIMAGE, meWEFDistortImage::onEditImageCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFDistortImage::ID_EDITSTRENGTHU, meWEFDistortImage::onEditStrengthUCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFDistortImage::ID_EDITSTRENGTHV, meWEFDistortImage::onEditStrengthVCommand),
	FXMAPFUNC(SEL_COMMAND, meWEFDistortImage::ID_CHKENABLE, meWEFDistortImage::onCheckEnableCommand),
};




// class meWEFDistortImage
//////////////////////

FXIMPLEMENT(meWEFDistortImage, FXVerticalFrame, meWEFDistortImageMap, ARRAYNUMBER(meWEFDistortImageMap))

// Constructor, destructor
////////////////////////////

meWEFDistortImage::meWEFDistortImage(){}

meWEFDistortImage::meWEFDistortImage(deEffectDistortImage *effectDistort, meWindowEffects *windowEffects, FXComposite *container) :
FXVerticalFrame(container, LAYOUT_FILL_X | LAYOUT_FILL_Y | LAYOUT_TOP | LAYOUT_LEFT,
0, 0, 0, 0, 0, 0, 0, 0){
	if(! effectDistort || ! windowEffects) DETHROW(deeInvalidParam);
	int padding = 3;
	int spacing = 3;
	FXString text;
	
	// prepare
	pWndEffects = windowEffects;
	pEffectDistort = effectDistort;
	pEngine = windowEffects->GetWindowMain()->GetWorld()->GetEngine();
	
	// group box
	FXGroupBox *groupBox = new FXGroupBox(this, "Distort Effect:",
		GROUPBOX_TITLE_LEFT | FRAME_RIDGE | LAYOUT_FILL_X, 0, 0, 0, 0,
		padding, padding, padding, padding);
	FXVerticalFrame *frameBox = new FXVerticalFrame(groupBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	
	// image selection
	FXHorizontalFrame *frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Image:");
	pEditImage = new FXTextField(frameLine, 10, this, ID_EDITIMAGE, FRAME_SUNKEN | LAYOUT_FILL_X);
// 	new FXButton( frameLine, "...", NULL, this, ID_BTNSELECTCLASS, BUTTON_NORMAL );
 	new FXButton(frameLine, "...", NULL, NULL, 0, BUTTON_NORMAL);
	
	// transparency
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X | LAYOUT_FILL_Y,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
	new FXLabel(frameLine, "Strength U:");
	pEditStrengthU = new FXTextField(frameLine, 5, this, ID_EDITSTRENGTHU, FRAME_SUNKEN);
	new FXLabel(frameLine, "Strength V:");
	pEditStrengthV = new FXTextField(frameLine, 5, this, ID_EDITSTRENGTHV, FRAME_SUNKEN);
	
	// disabled
	frameLine = new FXHorizontalFrame(frameBox, LAYOUT_SIDE_TOP | LAYOUT_FILL_X,
		0, 0, 0, 0, 0, 0, 0, 0, spacing, spacing);
 	pChkEnable = new FXCheckButton(frameLine, "Enable Effect", this, ID_CHKENABLE, CHECKBUTTON_NORMAL);
	pChkEnable->setCheck(pEffectDistort->GetEnabled());
	
	// set values
	deImage *image = pEffectDistort->GetImage();
	if(image){
		pEditImage->setText(image->GetFilename().GetString());
	}else{
		pEditImage->setText("");
	}
	text.format("%g", pEffectDistort->GetStrengthU());
	pEditStrengthU->setText(text);
	text.format("%g", pEffectDistort->GetStrengthV());
	pEditStrengthV->setText(text);
	
	// realize
	create();
}

meWEFDistortImage::~meWEFDistortImage(){
}



// Management
///////////////



// Events
///////////

long meWEFDistortImage::onEditImageCommand(FXObject *sender, FXSelector selector, void *data){
	deImageManager *imageManager = pEngine->GetImageManager();
	deVirtualFileSystem *vfs = pEngine->GetVirtualFileSystem();
	const FXString &filename = pEditImage->getText();
	deImage *image = NULL;
	decPath path;
	
	path.SetFromUnix(filename.text());
	if(vfs->ExistsFile(path)){
		try{
			image = imageManager->LoadImage(filename.text(), "/");
			pEffectDistort->SetImage(image);
			image->FreeReference();
			
		}catch(const deException &e){
			if(image) image->FreeReference();
			pWndEffects->GetWindowMain()->GetLogger()->LogException("World Editor", e);
		}
	}
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFDistortImage::onEditStrengthUCommand(FXObject *sender, FXSelector selector, void *data){
	pEffectDistort->SetStrengthU(strtof(pEditStrengthU->getText().text(), NULL));
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFDistortImage::onEditStrengthVCommand(FXObject *sender, FXSelector selector, void *data){
	pEffectDistort->SetStrengthV(strtof(pEditStrengthV->getText().text(), NULL));
	
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

long meWEFDistortImage::onCheckEnableCommand(FXObject *sender, FXSelector selector, void *data){
	pEffectDistort->SetEnabled(pChkEnable->getCheck());
	pWndEffects->GetWindowMain()->UpdateWindows(meWindowMain::eumAllViews);
	
	return 1;
}

#endif
