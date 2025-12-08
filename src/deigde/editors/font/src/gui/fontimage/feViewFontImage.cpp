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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feViewFontImage.h"
#include "feViewFIImage.h"
#include "feViewFontImageListener.h"
#include "../feWindowMain.h"
#include "../../configuration/feConfiguration.h"
#include "../../font/feFont.h"
#include "../../font/image/feFontImage.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/event/igdeScrollBarListener.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>

#include <dragengine/deEngine.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

class cScrollView : public igdeScrollBarListener{
public:
	typedef deTObjectReference<cScrollView> Ref;
	
protected:
	feViewFontImage &pView;
	
public:
	cScrollView(feViewFontImage &view) : pView(view){}
	
	virtual void OnValueChanged(igdeScrollBar *scrollBar){
		pView.ScrollView();
	}
};


class cChangeZoom : public igdeComboBoxListener{
public:
	typedef deTObjectReference<cChangeZoom> Ref;
	
protected:
	feViewFontImage &pView;
	
public:
	cChangeZoom(feViewFontImage &view) : pView(view){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		pView.GetViewImage().SetZoom(comboBox->GetText().ToInt());
		pView.UpdateScrollbarRanges();
		
		decString text;
		text.Format("%d", pView.GetViewImage().GetZoom());
		comboBox->SetText(text);
	}
};



// Class feViewFontImage
//////////////////////////

// Constructor, destructor
////////////////////////////

feViewFontImage::feViewFontImage(feWindowMain &windowMain) :
igdeContainerBorder(windowMain.GetEnvironment()),
pWindowMain(windowMain),
pListener(NULL),
pFont(NULL),
pViewImage(NULL)
{
	pListener.TakeOverWith(*this);
	
	igdeEnvironment &env = windowMain.GetEnvironment();
	
	igdeContainerFlow::Ref bottomLine(igdeContainerFlow::Ref::NewWith(
		env, igdeContainerFlow::eaX, igdeContainerFlow::esFirst));
	AddChild(bottomLine, igdeContainerBorder::eaBottom);
	
	cScrollView::Ref scrollView(cScrollView::Ref::NewWith(*this));
	
	pSBHorizontal.TakeOverWith(env, igdeScrollBar::eoHorizontal);
	pSBHorizontal->AddListener(scrollView);
	bottomLine->AddChild(pSBHorizontal);
	
	pCBZoom.TakeOverWith(env, 6, 7, true);
	pCBZoom->SetDescription("Select zoom factor");
	pCBZoom->AddItem("100");
	pCBZoom->AddItem("150");
	pCBZoom->AddItem("200");
	pCBZoom->AddItem("300");
	pCBZoom->AddItem("400");
	pCBZoom->AddItem("600");
	pCBZoom->AddItem("800");
	pCBZoom->SetSelection(0); // 100
	bottomLine->AddChild(pCBZoom);
	
	cChangeZoom::Ref changeZoom(cChangeZoom::Ref::NewWith(*this));
	pCBZoom->AddListener(changeZoom);
	
	pSBVertical.TakeOverWith(env, igdeScrollBar::eoVertical);
	pSBVertical->AddListener(scrollView);
	AddChild(pSBVertical, igdeContainerBorder::eaRight);
	
	pViewImage.TakeOverWith(windowMain);
	AddChild(pViewImage, igdeContainerBorder::eaCenter);
}

feViewFontImage::~feViewFontImage(){
	SetFont(NULL);
	
}



// Management
///////////////

void feViewFontImage::SetFont(feFont *font){
	if(pFont == font){
		return;
	}
	
	if(pFont){
		pFont->RemoveNotifier(pListener);
	}
	
	pFont = font;
	
	if(font){
		font->AddReference();
		font->AddNotifier(pListener);
	}
	
	pViewImage->SetFont(font);
	
	UpdateScrollbarRanges();
}

void feViewFontImage::SetEnableRendering(bool enable){
	pViewImage->SetEnableRendering(enable);
}



void feViewFontImage::OnAfterEngineStart(){
	pViewImage->OnAfterEngineStart();
}

void feViewFontImage::OnBeforeEngineStop(){
	pViewImage->OnBeforeEngineStop();
}

void feViewFontImage::ResetView(){
	pViewImage->ResetView();
}

void feViewFontImage::OnFrameUpdate(float elapsed){
	pViewImage->OnFrameUpdate(elapsed);
}

void feViewFontImage::OnResize(){
	UpdateScrollbarRanges();
}



void feViewFontImage::UpdateScrollbarRanges(){
	const decPoint size(pViewImage->GetRenderAreaSize());
	const decPoint &offset = pViewImage->GetOffset();
	
	pContentSize = pViewImage->GetContentSize();
	
	const decPoint range(decPoint().Largest(pContentSize - size));
	const decPoint pageSize(decPoint(1, 1).Largest(size / 4));
	
	pScrollOffset = range / 2;
	
	pSBHorizontal->SetUpper(decMath::max(range.x + pageSize.x, 0));
	pSBVertical->SetUpper(decMath::max(range.y + pageSize.y, 0));
	
	pSBHorizontal->SetPageSize(pageSize.x);
	pSBVertical->SetPageSize(pageSize.y);
	
	pSBHorizontal->SetValue(pScrollOffset.x - offset.x);
	pSBVertical->SetValue(pScrollOffset.y - offset.y);
	
	ScrollView();
}

void feViewFontImage::UpdateImageViewCanvas(){
	pViewImage->ResizeCanvas();
}

void feViewFontImage::ScrollView(){
	pViewImage->SetOffset(pScrollOffset - decPoint(
		pSBHorizontal->GetValue(), pSBVertical->GetValue()));
}
