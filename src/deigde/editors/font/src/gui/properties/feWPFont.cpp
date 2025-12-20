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

#include "feWPFont.h"
#include "feWPFontListener.h"
#include "feWindowProperties.h"
#include "../feWindowMain.h"
#include "../../font/feFont.h"
#include "../../font/image/feFontImage.h"
#include "../../font/glyph/feFontGlyph.h"
#include "../../undosys/font/feUFontSetImagePath.h"
#include "../../undosys/font/feUFontSetLineHeight.h"
#include "../../undosys/font/feUFontSetBaseLine.h"
#include "../../undosys/font/feUFontToggleColorFont.h"
#include "../../undosys/font/feUFontImportImage.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeCheckBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionSelectFile.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/composed/igdeEditPoint.h>
#include <deigde/gui/composed/igdeEditPointListener.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	feWPFont &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(feWPFont &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		feFont * const font = pPanel.GetFont();
		if(!font){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, font));
		if(undo){
			font->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font) = 0;
};



class cTextImagePath : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextImagePath> Ref;
	cTextImagePath(feWPFont &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font) override{
		feFontImage * const image = font->GetFontImage();
		if(image->GetFilename() == textField->GetText()){
			return {};
		}
		return feUFontSetImagePath::Ref::New(image, textField->GetText());
	}
};

class cActionSelectImagePath : public igdeActionSelectFile{
	feWindowMain &pWindow;
public:
	typedef deTObjectReference<cActionSelectImagePath> Ref;
	cActionSelectImagePath(feWindowMain &window, igdeTextField &textField) :
	igdeActionSelectFile(window.GetEnvironment(), igdeEnvironment::efpltImage, textField),
	pWindow(window){}
	
	virtual bool AcceptFile(decString &path){
		if(!pWindow.GetEngine()->GetVirtualFileSystem()->ExistsFile(decPath::CreatePathUnix(path))){
			return true;
		}
		
		const igdeCommonDialogs::eButton answer = igdeCommonDialogs::Question(
			&pWindow, igdeCommonDialogs::ebsYesNoCancel, "Set Font Image",
			"The image to save the font image to exists already. Do you want\n"
			"to import the font image before setting the path?\n"
			"[YES] imports the image and changes the path and size.\n"
			"[NO] just changes the path and the next time the font is saved\n"
			"the image is overwritten.\n"
			"[CANCEL] does neither import nor change the path.");
		
		if(answer == igdeCommonDialogs::ebCancel){
			return false;
		}
		if(answer == igdeCommonDialogs::ebNo){
			return true;
		}
		
		deImage::Ref image(pWindow.GetEngine()->GetImageManager()->LoadImage(path, "/"));
		
		if(image->GetComponentCount() != 4){
			igdeCommonDialogs::Error(&pWindow, "Import Font Image",
				"The font image does not have exactly 4 color channel. Only\n"
				"images with 4 color channel can be used as font images.");
			return false;
		}
		return true;
	}
};

class cTextLineHeight : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextLineHeight> Ref;
	cTextLineHeight(feWPFont &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font) override{
		const int lineHeight = textField->GetInteger();
		if(lineHeight == font->GetLineHeight()){
			return {};
		}
		return feUFontSetLineHeight::Ref::New(font, lineHeight);
	}
};

class cActionColorFont : public igdeAction{
public:
	typedef deTObjectReference<cActionColorFont> Ref;
	
private:
	feWPFont &pPanel;
public:
	cActionColorFont(feWPFont &panel) : 
	igdeAction("Color Font", "Font is a color font"),
	pPanel(panel){}
	
	void OnAction() override{
		feFont * const font = pPanel.GetFont();
		if(!font){
			return;
		}
		
		font->GetUndoSystem()->Add(feUFontToggleColorFont::Ref::New(font));
	}
};

class cTextBaseLine : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBaseLine> Ref;
	cTextBaseLine(feWPFont &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font) override{
		const int baseLine = textField->GetInteger();
		return baseLine != font->GetBaseLine() ? feUFontSetBaseLine::Ref::New(font, baseLine) : feUFontSetBaseLine::Ref();
	}
};

};



// Class feWPFont
///////////////////

// Constructor, destructor
////////////////////////////

feWPFont::feWPFont(feWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = feWPFontListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// font
	helper.GroupBox(content, groupBox, "Font:");
	
	helper.FormLineStretchFirst(groupBox, "Image:", "Font image.", frameLine);
	helper.EditString(frameLine, "Font image.", pEditImagePath, cTextImagePath::Ref::New(*this));
	helper.Button(frameLine, pBtnImagePath, cActionSelectImagePath::Ref::New(
		windowProperties.GetWindowMain(), pEditImagePath));
	
	helper.EditInteger(groupBox, "Line Height:", "Line height in pixels.",
		pEditLineHeight, cTextLineHeight::Ref::New(*this));
	helper.CheckBox(groupBox, pChkColorFont, cActionColorFont::Ref::New(*this));
	
	helper.EditInteger(groupBox, "Base Line:", "Base line from top in pixels.",
		pEditBaseLine, cTextBaseLine::Ref::New(*this));
}

feWPFont::~feWPFont(){
	SetFont(nullptr);
}



// Management
///////////////

void feWPFont::SetFont(feFont *font){
	if(font == pFont){
		return;
	}
	
	if(pFont){
		pFont->RemoveNotifier(pListener);
		pFont = nullptr;
	}
	
	pFont = font;
	
	if(font){
		font->AddNotifier(pListener);
	}
	
	UpdateFont();
}



void feWPFont::UpdateFont(){
	if(pFont){
		pEditImagePath->SetText(pFont->GetFontImage()->GetFilename());
		pEditLineHeight->SetInteger(pFont->GetLineHeight());
		pChkColorFont->SetChecked(pFont->GetColorFont());
		pEditBaseLine->SetInteger(pFont->GetBaseLine());
		
	}else{
		pEditImagePath->ClearText();
		pEditLineHeight->ClearText();
		pChkColorFont->SetChecked(false);
		pEditBaseLine->ClearText();
	}
}
