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

#include "lpeWPLangPack.h"
#include "lpeWPLangPackListener.h"
#include "lpeWindowProperties.h"
#include "../lpeWindowMain.h"
#include "../../langpack/lpeLangPack.h"
#include "../../undosys/langpack/lpeULangPackSetIdentifier.h"
#include "../../undosys/langpack/lpeULangPackSetName.h"
#include "../../undosys/langpack/lpeULangPackSetDescription.h"
#include "../../undosys/langpack/lpeULangPackSetMissingText.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextArea.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeTextAreaListener.h>
#include <deigde/gui/event/igdeTextAreaListenerReference.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/event/igdeTextFieldListenerReference.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Events
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	lpeWPLangPack &pPanel;
	
public:
	cBaseTextFieldListener( lpeWPLangPack &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		lpeLangPack * const langpack = pPanel.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, langpack ) );
		if( undo ){
			langpack->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, lpeLangPack *langpack ) = 0;
};

class cBaseTextAreaListener : public igdeTextAreaListener{
protected:
	lpeWPLangPack &pPanel;
	
public:
	cBaseTextAreaListener( lpeWPLangPack &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextArea *textArea ){
		lpeLangPack * const langpack = pPanel.GetLangPack();
		if( ! langpack ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textArea, langpack ) );
		if( undo ){
			langpack->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, lpeLangPack *langpack ) = 0;
};



class cTextIdentifier : public cBaseTextFieldListener{
public:
	cTextIdentifier( lpeWPLangPack &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, lpeLangPack *langpack ){
		return textField->GetText() != langpack->GetIdentifier()
			? new lpeULangPackSetIdentifier( langpack, textField->GetText() ) : nullptr;
	}
};

class cTextName : public cBaseTextFieldListener{
public:
	cTextName( lpeWPLangPack &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, lpeLangPack *langpack ){
		decUnicodeString value( decUnicodeString::NewFromUTF8( textField->GetText() ) );
		if( value == langpack->GetName() ){
			return NULL;
		}
		return new lpeULangPackSetName( langpack, value );
	}
};

class cTextDescription : public cBaseTextAreaListener{
public:
	cTextDescription( lpeWPLangPack &panel ) : cBaseTextAreaListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextArea *textArea, lpeLangPack *langpack ){
		decUnicodeString value( decUnicodeString::NewFromUTF8( textArea->GetText() ) );
		if( value == langpack->GetDescription() ){
			return NULL;
		}
		return new lpeULangPackSetDescription( langpack, value );
	}
};

class cTextMissingText : public cBaseTextFieldListener{
public:
	cTextMissingText( lpeWPLangPack &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, lpeLangPack *langpack ){
		decUnicodeString value( decUnicodeString::NewFromUTF8( textField->GetText() ) );
		if( value == langpack->GetMissingText() ){
			return NULL;
		}
		return new lpeULangPackSetMissingText( langpack, value );
	}
};

}



// Class lpeWPLangPack
////////////////////////

// Constructor, destructor
////////////////////////////

lpeWPLangPack::lpeWPLangPack( lpeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( new lpeWPLangPackListener( *this ) ),
pLangPack( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	// language pack
	helper.GroupBox( content, groupBox, "Language Pack:" );
	
	helper.EditString( groupBox, "Identifier:", "Unique identifier of language pack.",
		pEditIdentifier, new cTextIdentifier( *this ) );
	helper.EditString( groupBox, "Name:", "Name of language pack.",
		pEditName, new cTextName( *this ) );
	helper.EditString( groupBox, "Description:", "Description of language pack.",
		pEditDescription, 5, new cTextDescription( *this ) );
	helper.EditString( groupBox, "Missing Text:", "Text to use for missing entries.",
		pEditMissingText, new cTextMissingText( *this ) );
}

lpeWPLangPack::~lpeWPLangPack(){
	SetLangPack( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void lpeWPLangPack::SetLangPack( lpeLangPack *langpack ){
	if( langpack == pLangPack ){
		return;
	}
	
	if( pLangPack ){
		pLangPack->RemoveListener( pListener );
		pLangPack->FreeReference();
	}
	
	pLangPack = langpack;
	
	if( langpack ){
		langpack->AddListener( pListener );
		langpack->AddReference();
	}
	
	UpdateLangPack();
}



void lpeWPLangPack::UpdateLangPack(){
	if( pLangPack ){
		pEditIdentifier->SetText( pLangPack->GetIdentifier() );
		pEditName->SetText( pLangPack->GetName().ToUTF8() );
		pEditDescription->SetText( pLangPack->GetDescription().ToUTF8() );
		pEditMissingText->SetText( pLangPack->GetMissingText().ToUTF8() );
		
	}else{
		pEditIdentifier->SetText( "" );
		pEditName->SetText( "" );
		pEditDescription->SetText( "" );
		pEditMissingText->SetText( "" );
	}
	
	const bool enabled = pLangPack != nullptr;
	pEditIdentifier->SetEnabled( enabled );
	pEditName->SetEnabled( enabled );
	pEditDescription->SetEnabled( enabled );
	pEditMissingText->SetEnabled( enabled );
}
