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

#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeComboBoxListenerReference.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeTextFieldListenerReference.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

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
	cBaseTextFieldListener( feWPGlyph &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		feFont * const font = pPanel.GetFont();
		feFontGlyph * const glyph = pPanel.GetGlyph();
		if( ! font || ! glyph ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, font, glyph ) );
		if( undo ){
			font->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font, feFontGlyph *glyph ) = 0;
};



class cComboGlyph : public igdeComboBoxListener{
protected:
	feWPGlyph &pPanel;
	
public:
	cComboGlyph( feWPGlyph &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		feFont * const font = pPanel.GetFont();
		if( ! font ){
			return;
		}
		
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		font->GetGlyphSelection().SetActiveGlyph(
			selection ? ( feFontGlyph* )selection->GetData() : NULL );
	}
};

class cActionAdd : public igdeAction{
protected:
	feWPGlyph &pPanel;
	
public:
	cActionAdd( feWPGlyph &panel ) :
	igdeAction( "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ),
		"Add glyph" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		// TODO
	}
};

class cActionRemove : public igdeAction{
protected:
	feWPGlyph &pPanel;
	
public:
	cActionRemove( feWPGlyph &panel ) :
	igdeAction( "", panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallMinus ),
		"Remove glyph" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		// TODO
	}
};



class cTextU : public cBaseTextFieldListener{
public:
	cTextU( feWPGlyph &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font, feFontGlyph *glyph ){
		const int u = textField->GetInteger();
		if( u == glyph->GetU() ){
			return NULL;
		}
		return new feUGlyphSetU( glyph, u );
	}
};

class cTextV : public cBaseTextFieldListener{
public:
	cTextV( feWPGlyph &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font, feFontGlyph *glyph ){
		const int v = textField->GetInteger();
		if( v == glyph->GetV() ){
			return NULL;
		}
		return new feUGlyphSetV( glyph, v );
	}
};

class cTextWidth : public cBaseTextFieldListener{
public:
	cTextWidth( feWPGlyph &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font, feFontGlyph *glyph ){
		const int width = textField->GetInteger();
		if( width == glyph->GetWidth() ){
			return NULL;
		}
		return new feUGlyphSetWidth( glyph, width );
	}
};

class cTextHeight : public cBaseTextFieldListener{
public:
	cTextHeight(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int height = textField->GetInteger();
		return height != glyph->GetHeight() ? new feUGlyphSetHeight(glyph, height) : nullptr;
	}
};

class cTextBearing : public cBaseTextFieldListener{
public:
	cTextBearing( feWPGlyph &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font, feFontGlyph *glyph ){
		const int width = textField->GetInteger();
		if( width == glyph->GetBearing() ){
			return NULL;
		}
		return new feUGlyphSetBearing( glyph, width );
	}
};

class cTextBearingY : public cBaseTextFieldListener{
public:
	cTextBearingY(feWPGlyph &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField *textField, feFont *font, feFontGlyph *glyph) override{
		const int bearing = textField->GetInteger();
		return bearing != glyph->GetBearingY() ? new feUGlyphSetBearingY(glyph, bearing) : nullptr;
	}
};

class cTextAdvance : public cBaseTextFieldListener{
public:
	cTextAdvance( feWPGlyph &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font, feFontGlyph *glyph ){
		const int width = textField->GetInteger();
		if( width == glyph->GetAdvance() ){
			return NULL;
		}
		return new feUGlyphSetAdvance( glyph, width );
	}
};

};


// Class feWPGlyph
///////////////////

// Constructor, destructor
////////////////////////////

feWPGlyph::feWPGlyph( feWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pFont( NULL ),
pGlyph( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new feWPGlyphListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// glyph
	helper.GroupBox( content, groupBox, "Glyph:" );
	
	helper.FormLineStretchFirst( groupBox, "Glyph:", "Glyph to edit.", frameLine );
	helper.ComboBox( frameLine, "Glyph to edit.", pCBGlyph, new cComboGlyph( *this ) );
	helper.Button( frameLine, pBtnGlyphAdd, new cActionAdd( *this ), true );
	helper.Button( frameLine, pBtnGlyphDel, new cActionRemove( *this ), true );
	
	helper.FormLine( groupBox, "Tex-Coords:", "Texture coordinate in pixels.", frameLine );
	helper.EditInteger( frameLine, "U texture coordinate in pixels", pEditU, new cTextU( *this ) );
	helper.EditInteger( frameLine, "V texture coordinate in pixels", pEditV, new cTextV( *this ) );
	
	helper.EditInteger( groupBox, "Width:", "Width of the glyph in pixels.",
		pEditWidth, new cTextWidth( *this ) );
	helper.EditInteger(groupBox, "Height:", "Height of the glyph in pixels.",
		pEditHeight, new cTextHeight(*this));
	helper.EditInteger( groupBox, "Bearing:", "Horizontal bearing in pixels.",
		pEditBearing, new cTextBearing( *this ) );
	helper.EditInteger(groupBox, "Bearing Y:", "Vertical bearing in pixels.",
		pEditBearingY, new cTextBearingY(*this));
	helper.EditInteger( groupBox, "Advance:", "Advance in pixels.",
		pEditAdvance, new cTextAdvance( *this ) );
}

feWPGlyph::~feWPGlyph(){
	SetGlyph( NULL );
	SetFont( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void feWPGlyph::SetFont( feFont *font ){
	if( font == pFont ){
		return;
	}
	
	SetGlyph( NULL );
	
	if( pFont ){
		pFont->RemoveNotifier( pListener );
		pFont->FreeReference();
		pFont = NULL;
	}
	
	pFont = font;
	
	if( font ){
		font->AddNotifier( pListener );
		font->AddReference();
	}
	
	UpdateGlyphList();
}

void feWPGlyph::SetGlyph( feFontGlyph *glyph ){
	if( glyph && ! pFont ){
		DETHROW( deeInvalidParam );
	}
	
	if( glyph == pGlyph ){
		return;
	}
	
	if( pGlyph ){
		pGlyph->FreeReference();
		pGlyph = NULL;
	}
	
	pGlyph = glyph;
	
	if( glyph ){
		glyph->AddReference();
	}
	
	pCBGlyph->SetSelection( pFont->IndexOfGlyph( glyph ) );
	
	UpdateGlyph();
}



void feWPGlyph::UpdateGlyphList(){
	feFontGlyph *glyph = NULL;
	int g, count = 0;
	decString text;
	
	pCBGlyph->RemoveAllItems();
	
	if( pFont ){
		count = pFont->GetGlyphCount();
		
		for( g=0; g<count; g++ ){
			glyph = pFont->GetGlyphAt( g );
			text.Format( "Code %i", glyph->GetCode() );
			pCBGlyph->AddItem( text, NULL, glyph );
		}
	}
	
	g = -1;
	if( pGlyph ){
		g = pFont->IndexOfGlyph( pGlyph );
		glyph = pGlyph;
	}
	if( g == -1 && pFont && count > 0 ){
		g = 0;
		glyph = pFont->GetGlyphAt( 0 );
	}
	pCBGlyph->SetSelection( g );
	
	if( glyph != pGlyph ){
		SetGlyph( glyph );
		
	}else{
		UpdateGlyph();
	}
}

void feWPGlyph::UpdateGlyph(){
	if( pGlyph ){
		pEditU->SetInteger( pGlyph->GetU() );
		pEditV->SetInteger( pGlyph->GetV() );
		pEditWidth->SetInteger( pGlyph->GetWidth() );
		pEditHeight->SetInteger(pGlyph->GetHeight());
		pEditBearing->SetInteger( pGlyph->GetBearing() );
		pEditBearingY->SetInteger(pGlyph->GetBearingY());
		pEditAdvance->SetInteger( pGlyph->GetAdvance() );
		
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
