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
	cBaseTextFieldListener( feWPFont &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		feFont * const font = pPanel.GetFont();
		if( ! font ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( OnChanged( textField, font ) );
		if( undo ){
			font->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font ) = 0;
};



class cTextImagePath : public cBaseTextFieldListener{
public:
	cTextImagePath( feWPFont &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font ){
		feFontImage * const image = font->GetFontImage();
		if( image->GetFilename() == textField->GetText() ){
			return NULL;
		}
		return new feUFontSetImagePath( image, textField->GetText() );
	}
};

class cActionSelectImagePath : public igdeActionSelectFile{
	feWindowMain &pWindow;
public:
	cActionSelectImagePath( feWindowMain &window, igdeTextField &textField ) :
	igdeActionSelectFile( window.GetEnvironment(), igdeEnvironment::efpltImage, textField ),
	pWindow( window ){ }
	
	virtual bool AcceptFile( decString &path ){
		if( ! pWindow.GetEngine()->GetVirtualFileSystem()->ExistsFile( decPath::CreatePathUnix( path ) ) ){
			return true;
		}
		
		const igdeCommonDialogs::eButton answer = igdeCommonDialogs::Question(
			&pWindow, igdeCommonDialogs::ebsYesNoCancel, "Set Font Image",
			"The image to save the font image to exists already. Do you want\n"
			"to import the font image before setting the path?\n"
			"[YES] imports the image and changes the path and size.\n"
			"[NO] just changes the path and the next time the font is saved\n"
			"the image is overwritten.\n"
			"[CANCEL] does neither import nor change the path." );
		
		if( answer == igdeCommonDialogs::ebCancel ){
			return false;
		}
		if( answer == igdeCommonDialogs::ebNo ){
			return true;
		}
		
		deImage::Ref image;
		image.TakeOver( pWindow.GetEngine()->GetImageManager()->LoadImage( path, "/" ) );
		
		if( image->GetComponentCount() != 4 ){
			igdeCommonDialogs::Error( &pWindow, "Import Font Image",
				"The font image does not have exactly 4 color channel. Only\n"
				"images with 4 color channel can be used as font images." );
			return false;
		}
		return true;
	}
};

class cTextLineHeight : public cBaseTextFieldListener{
public:
	cTextLineHeight( feWPFont &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, feFont *font ){
		const int lineHeight = textField->GetInteger();
		if( lineHeight == font->GetLineHeight() ){
			return NULL;
		}
		return new feUFontSetLineHeight( font, lineHeight );
	}
};

class cActionColorFont : public igdeAction{
	feWPFont &pPanel;
public:
	cActionColorFont( feWPFont &panel ) : 
	igdeAction( "Color Font", "Font is a color font" ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		feFont * const font = pPanel.GetFont();
		if( ! font ){
			return;
		}
		
		igdeUndo::Ref undo;
		undo.TakeOver( new feUFontToggleColorFont( font ) );
		font->GetUndoSystem()->Add( undo );
	}
};

class cTextBaseLine : public cBaseTextFieldListener{
public:
	cTextBaseLine(feWPFont &panel) : cBaseTextFieldListener(panel){}
	
	igdeUndo *OnChanged(igdeTextField *textField, feFont *font) override{
		const int baseLine = textField->GetInteger();
		return baseLine != font->GetBaseLine() ? new feUFontSetBaseLine(font, baseLine) : nullptr;
	}
};

};



// Class feWPFont
///////////////////

// Constructor, destructor
////////////////////////////

feWPFont::feWPFont( feWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pFont( NULL ),
pListener( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainer::Ref content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new feWPFontListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// font
	helper.GroupBox( content, groupBox, "Font:" );
	
	helper.FormLineStretchFirst( groupBox, "Image:", "Font image.", frameLine );
	helper.EditString( frameLine, "Font image.", pEditImagePath, new cTextImagePath( *this ) );
	helper.Button( frameLine, pBtnImagePath,
		new cActionSelectImagePath( windowProperties.GetWindowMain(), pEditImagePath ), true );
	
	helper.EditInteger( groupBox, "Line Height:", "Line height in pixels.",
		pEditLineHeight, new cTextLineHeight( *this ) );
	helper.CheckBox( groupBox, pChkColorFont, new cActionColorFont( *this ), true );
	
	helper.EditInteger(groupBox, "Base Line:", "Base line from top in pixels.",
		pEditBaseLine, new cTextBaseLine(*this));
}

feWPFont::~feWPFont(){
	SetFont( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void feWPFont::SetFont( feFont *font ){
	if( font == pFont ){
		return;
	}
	
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
	
	UpdateFont();
}



void feWPFont::UpdateFont(){
	if( pFont ){
		pEditImagePath->SetText( pFont->GetFontImage()->GetFilename() );
		pEditLineHeight->SetInteger( pFont->GetLineHeight() );
		pChkColorFont->SetChecked( pFont->GetColorFont() );
		pEditBaseLine->SetInteger(pFont->GetBaseLine());
		
	}else{
		pEditImagePath->ClearText();
		pEditLineHeight->ClearText();
		pChkColorFont->SetChecked( false );
		pEditBaseLine->ClearText();
	}
}
