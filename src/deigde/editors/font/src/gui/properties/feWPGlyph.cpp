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

#include "feWPGlyph.h"
#include "feWPGlyphListener.h"
#include "feWindowProperties.h"
#include "../feWindowMain.h"
#include "../../font/feFont.h"
#include "../../font/image/feFontImage.h"
#include "../../font/glyph/feFontGlyph.h"
#include "../../font/glyph/feFontGlyphSelection.h"
#include "../../undosys/glyph/feUGlyphSetU.h"
#include "../../undosys/glyph/feUGlyphSetV.h"
#include "../../undosys/glyph/feUGlyphSetWidth.h"
#include "../../undosys/glyph/feUGlyphSetHeight.h"
#include "../../undosys/glyph/feUGlyphSetBearing.h"
#include "../../undosys/glyph/feUGlyphSetBearingY.h"
#include "../../undosys/glyph/feUGlyphSetAdvance.h"
#include "../../undosys/glyph/feUGlyphAdd.h"
#include "../../undosys/glyph/feUGlyphRemove.h"

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidget.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndo.h>

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	feWPGlyph &pPanel;
	
public:
	typedef deTObjectReference<cBaseTextFieldListener> Ref;
	cBaseTextFieldListener(feWPGlyph &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeTextField *textField){
		feFont * const font = pPanel.GetFont();
		feFontGlyph * const glyph = pPanel.GetGlyph();
		if(!font || !glyph){
			return;
		}
		
		igdeUndo::Ref undo(OnChanged(textField, font, glyph));
		if(undo){
			font->GetUndoSystem()->Add(undo);
		}
	}
	
	virtual igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) = 0;
};



class cComboGlyph : public igdeComboBoxListener{
protected:
	feWPGlyph &pPanel;
	
public:
	typedef deTObjectReference<cComboGlyph> Ref;
	cComboGlyph(feWPGlyph &panel) : pPanel(panel){}
	
	virtual void OnTextChanged(igdeComboBox *comboBox){
		feFont * const font = pPanel.GetFont();
		if(!font){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		font->GetGlyphSelection().SetActive(
			selection ? (feFontGlyph*)selection->GetData() : nullptr);
	}
};

class cActionAdd : public igdeAction{
	feWPGlyph &pPanel;

public:
	typedef deTObjectReference<cActionAdd> Ref;
	
	cActionAdd(feWPGlyph &panel) :
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallPlus), "Add glyph"),
	pPanel(panel){}
	
	void OnAction() override{
		const feFont::Ref &font = pPanel.GetFont();
		if(!font){
			return;
		}
		
		// find next available glyph code
		int code = pPanel.GetGlyph() ? pPanel.GetGlyph()->GetCode() : 32;
		while(font->GetGlyphs().FindOrDefault([&](const feFontGlyph &g){
			return g.GetCode() == code;
		})){
			code++;
		}
		
		feFontGlyph::Ref glyph;
		
		if(pPanel.GetGlyph()){
			glyph = pPanel.GetGlyph()->Copy();
			
		}else{
			glyph = feFontGlyph::Ref::New();
			glyph->SetHeight(font->GetLineHeight());
			glyph->SetWidth(font->GetLineHeight() / 2);
			glyph->SetAdvance(font->GetLineHeight() / 2);
		}
		
		glyph->SetCode(code);
		
		font->GetUndoSystem()->Add(feUGlyphAdd::Ref::New(font, glyph));
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFont());
	}
};

class cActionRemove : public igdeAction{
	feWPGlyph &pPanel;
	
public:
	typedef deTObjectReference<cActionRemove> Ref;
	
	cActionRemove(feWPGlyph &panel) :
	igdeAction("", panel.GetEnvironment().GetStockIcon(igdeEnvironment::esiSmallMinus), "Remove glyph"),
	pPanel(panel){}
	
	void OnAction() override{
		const feFont::Ref font = pPanel.GetFont();
		const feFontGlyph::Ref glyph = pPanel.GetGlyph();
		if(font && glyph){
			font->GetUndoSystem()->Add(feUGlyphRemove::Ref::New(font, glyph));
		}
	}
	
	void Update() override{
		SetEnabled(pPanel.GetFont() && pPanel.GetGlyph());
	}
};



class cTextU : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextU> Ref;
	cTextU(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int u = textField->GetInteger();
		if(u == glyph->GetU()){
			return {};
		}
		return feUGlyphSetU::Ref::New(glyph, u);
	}
};

class cTextV : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextV> Ref;
	cTextV(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int v = textField->GetInteger();
		if(v == glyph->GetV()){
			return {};
		}
		return feUGlyphSetV::Ref::New(glyph, v);
	}
};

class cTextWidth : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextWidth> Ref;
	cTextWidth(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int width = textField->GetInteger();
		if(width == glyph->GetWidth()){
			return {};
		}
		return feUGlyphSetWidth::Ref::New(glyph, width);
	}
};

class cTextHeight : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextHeight> Ref;
	cTextHeight(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int height = textField->GetInteger();
		return height != glyph->GetHeight() ? feUGlyphSetHeight::Ref::New(glyph, height) : feUGlyphSetHeight::Ref();
	}
};

class cTextBearing : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBearing> Ref;
	cTextBearing(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int width = textField->GetInteger();
		if(width == glyph->GetBearing()){
			return {};
		}
		return feUGlyphSetBearing::Ref::New(glyph, width);
	}
};

class cTextBearingY : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextBearingY> Ref;
	cTextBearingY(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int bearing = textField->GetInteger();
		return bearing != glyph->GetBearingY() ? feUGlyphSetBearingY::Ref::New(glyph, bearing) : feUGlyphSetBearingY::Ref();
	}
};

class cTextAdvance : public cBaseTextFieldListener{
public:
	typedef deTObjectReference<cTextAdvance> Ref;
	cTextAdvance(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo::Ref OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int width = textField->GetInteger();
		if(width == glyph->GetAdvance()){
			return {};
		}
		return feUGlyphSetAdvance::Ref::New(glyph, width);
	}
};

};


// Class feWPGlyph
///////////////////

// Constructor, destructor
////////////////////////////

feWPGlyph::feWPGlyph(feWindowProperties &windowProperties) :
igdeContainerScroll(windowProperties.GetEnvironment(), false, true),
pWindowProperties(windowProperties)
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = feWPGlyphListener::Ref::New(*this);
	
	content = igdeContainerFlow::Ref::New(env, igdeContainerFlow::eaY);
	AddChild(content);
	
	// glyph
	helper.GroupBox(content, groupBox, "Glyph:");
	
	helper.FormLineStretchFirst(groupBox, "Glyph:", "Glyph to edit.", frameLine);
	helper.ComboBox(frameLine, "Glyph to edit.", pCBGlyph, cComboGlyph::Ref::New(*this));
	helper.Button(frameLine, pBtnGlyphAdd, cActionAdd::Ref::New(*this));
	helper.Button(frameLine, pBtnGlyphDel, cActionRemove::Ref::New(*this));
	
	helper.FormLine(groupBox, "Tex-Coords:", "Texture coordinate in pixels.", frameLine);
	helper.EditInteger(frameLine, "U texture coordinate in pixels", pEditU, cTextU::Ref::New(*this));
	helper.EditInteger(frameLine, "V texture coordinate in pixels", pEditV, cTextV::Ref::New(*this));
	
	helper.EditInteger(groupBox, "Width:", "Width of the glyph in pixels.",
		pEditWidth, cTextWidth::Ref::New(*this));
	helper.EditInteger(groupBox, "Height:", "Height of the glyph in pixels.",
		pEditHeight, cTextHeight::Ref::New(*this));
	helper.EditInteger(groupBox, "Bearing:", "Horizontal bearing in pixels.",
		pEditBearing, cTextBearing::Ref::New(*this));
	helper.EditInteger(groupBox, "Bearing Y:", "Vertical bearing in pixels.",
		pEditBearingY, cTextBearingY::Ref::New(*this));
	helper.EditInteger(groupBox, "Advance:", "Advance in pixels.",
		pEditAdvance, cTextAdvance::Ref::New(*this));
}

feWPGlyph::~feWPGlyph(){
	SetGlyph(nullptr);
	SetFont(nullptr);
}



// Management
///////////////

void feWPGlyph::SetFont(feFont *font){
	if(font == pFont){
		return;
	}
	
	SetGlyph(nullptr);
	
	if(pFont){
		pFont->RemoveNotifier(pListener);
		pFont = nullptr;
	}
	
	pFont = font;
	
	if(font){
		font->AddNotifier(pListener);
	}
	
	UpdateGlyphList();
}

void feWPGlyph::SetGlyph(feFontGlyph *glyph){
	if(glyph && !pFont){
		DETHROW(deeInvalidParam);
	}
	
	if(glyph == pGlyph){
		return;
	}
	
	if(pGlyph){
		pGlyph = nullptr;
	}
	
	pGlyph = glyph;
	pCBGlyph->SetSelectionWithData(glyph);
	
	UpdateGlyph();
}



void feWPGlyph::UpdateGlyphList(){
	int count = 0;
	
	pCBGlyph->RemoveAllItems();
	
	if(pFont){
		count = pFont->GetGlyphs().GetCount();
		
		pFont->GetGlyphs().Visit([&](feFontGlyph *g){
			decString text;
			text.Format("Code %d", g->GetCode());
			pCBGlyph->AddItem(text, nullptr, g);
		});
	}
	
	feFontGlyph *glyph = nullptr;
	int g = -1;
	if(pGlyph){
		g = pFont->GetGlyphs().IndexOf(pGlyph);
		glyph = pGlyph;
	}
	if(g == -1 && pFont && count > 0){
		g = 0;
		glyph = pFont->GetGlyphs().First();
	}
	pCBGlyph->SetSelection(g);
	
	if(glyph != pGlyph){
		SetGlyph(glyph);
		
	}else{
		UpdateGlyph();
	}
}

void feWPGlyph::UpdateGlyph(){
	if(pGlyph){
		pEditU->SetInteger(pGlyph->GetU());
		pEditV->SetInteger(pGlyph->GetV());
		pEditWidth->SetInteger(pGlyph->GetWidth());
		pEditHeight->SetInteger(pGlyph->GetHeight());
		pEditBearing->SetInteger(pGlyph->GetBearing());
		pEditBearingY->SetInteger(pGlyph->GetBearingY());
		pEditAdvance->SetInteger(pGlyph->GetAdvance());
		
	}else{
		pEditU->ClearText();
		pEditV->ClearText();
		pEditWidth->ClearText();
		pEditHeight->ClearText();
		pEditBearing->ClearText();
		pEditBearingY->ClearText();
		pEditAdvance->ClearText();
	}
}
