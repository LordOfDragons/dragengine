/* 
 * Drag[en]gine IGDE Speech Animation Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "saeWPSAnim.h"
#include "saeWPSAnimListener.h"
#include "saeWindowProperties.h"
#include "../saeWindowMain.h"
#include "../../sanimation/saeSAnimation.h"
#include "../../sanimation/phoneme/saePhoneme.h"
#include "../../sanimation/dictionary/saeWord.h"
#include "../../configuration/saeConfiguration.h"
#include "../../undosys/sanimation/saeUSAnimSetRigPath.h"
#include "../../undosys/sanimation/saeUSAnimSetAnimPath.h"
#include "../../undosys/sanimation/saeUSAnimSetNeutralMoveName.h"
#include "../../undosys/phoneme/saeUPhonemeSetIPA.h"
#include "../../undosys/phoneme/saeUPhonemeSetSampleText.h"
#include "../../undosys/phoneme/saeUPhonemeSetLength.h"
#include "../../undosys/phoneme/saeUPhonemeSetMoveName.h"
#include "../../undosys/dictionary/word/saeUWordSetName.h"
#include "../../undosys/dictionary/word/saeUWordSetPhonetics.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gamedefinition/igdeGameDefinition.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeCommonDialogs.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/igdeButton.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeGroupBox.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/composed/igdeEditPath.h>
#include <deigde/gui/composed/igdeEditPathListener.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeTextFieldListener.h>
#include <deigde/gui/model/igdeListItem.h>
#include <deigde/undo/igdeUndoSystem.h>
#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/animator/deAnimator.h>
#include <dragengine/resources/animation/deAnimation.h>
#include <dragengine/resources/animation/deAnimationMove.h>



// Actions
///////////

namespace{

class cBaseTextFieldListener : public igdeTextFieldListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseTextFieldListener( saeWPSAnim &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeTextField *textField ){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if( ! sanimation ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( textField, sanimation ) );
		if( undo ){
			sanimation->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, saeSAnimation *sanimation ) = 0;
};

class cBaseAction : public igdeAction{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseAction( saeWPSAnim &panel, const char *text, const char *description ) :
	igdeAction( text, description ),
	pPanel( panel ){ }
	
	cBaseAction( saeWPSAnim &panel, igdeIcon *icon, const char *description ) :
	igdeAction( "", icon, description ),
	pPanel( panel ){ }
	
	virtual void OnAction(){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if( ! sanimation ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnAction( sanimation ) );
		if( undo ){
			sanimation->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnAction( saeSAnimation *sanimation ) = 0;
};

class cBaseComboBoxListener : public igdeComboBoxListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseComboBoxListener( saeWPSAnim &panel ) : pPanel( panel ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if( ! sanimation ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnTextChanged( comboBox, sanimation ) );
		if( undo ){
			sanimation->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, saeSAnimation *sanimation ) = 0;
};

class cBaseEditPathListener : public igdeEditPathListener{
protected:
	saeWPSAnim &pPanel;
	
public:
	cBaseEditPathListener( saeWPSAnim &panel ) : pPanel( panel ){ }
	
	virtual void OnEditPathChanged( igdeEditPath *editPath ){
		saeSAnimation * const sanimation = pPanel.GetSAnimation();
		if( ! sanimation ){
			return;
		}
		
		igdeUndoReference undo;
		undo.TakeOver( OnChanged( editPath->GetPath(), sanimation ) );
		if( undo ){
			sanimation->GetUndoSystem()->Add( undo );
		}
	}
	
	virtual igdeUndo *OnChanged( const decString &path, saeSAnimation *sanimation ) = 0;
};



class cEditRigPath : public cBaseEditPathListener{
public:
	cEditRigPath( saeWPSAnim &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decString &path, saeSAnimation *sanimation ){
		if( path == sanimation->GetRigPath() ){
			return NULL;
		}
		return new saeUSAnimSetRigPath( sanimation, path );
	}
};

class cEditAnimationPath : public cBaseEditPathListener{
public:
	cEditAnimationPath( saeWPSAnim &panel ) : cBaseEditPathListener( panel ){ }
	
	virtual igdeUndo *OnChanged( const decString &path, saeSAnimation *sanimation ){
		if( path == sanimation->GetAnimationPath() ){
			return NULL;
		}
		return new saeUSAnimSetAnimPath( sanimation, path );
	}
};

class cComboNeutralMove : public cBaseComboBoxListener{
public:
	cComboNeutralMove( saeWPSAnim &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, saeSAnimation *sanimation ){
		const decString &name = comboBox->GetText();
		if( name == sanimation->GetNeutralMoveName() ){
			return NULL;
		}
		return new saeUSAnimSetNeutralMoveName( sanimation, name );
	}
};



class cComboPhoneme : public cBaseComboBoxListener{
public:
	cComboPhoneme( saeWPSAnim &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, saeSAnimation *sanimation ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if( selection ){
			sanimation->SetActivePhoneme( ( saePhoneme* )selection->GetData() );
			
		}else{
			sanimation->SetActivePhoneme( NULL );
		}
		return NULL;
	}
};

class cBasePhonemeTextFieldListener : public cBaseTextFieldListener{
public:
	cBasePhonemeTextFieldListener( saeWPSAnim &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, saeSAnimation *sanimation ){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if( ! phoneme ){
			return NULL;
		}
		return OnChanged( textField, sanimation, phoneme );
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField,
		saeSAnimation *sanimation, saePhoneme * phoneme ) = 0;
};

class cTextPhonemeIpa : public cBasePhonemeTextFieldListener{
public:
	cTextPhonemeIpa( saeWPSAnim &panel ) : cBasePhonemeTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme ){
		decUnicodeString ipaText;
		try{
			ipaText = decUnicodeString::NewFromUTF8( textField->GetText() );
			
		}catch( const deException & ){
			ResetTextField( *textField, *phoneme, "IPA symbol has to be 1 unicode character" );
			return NULL;
		}
		
		if( ipaText.GetLength() != 1 ){
			ResetTextField( *textField, *phoneme, "IPA symbol has to be 1 unicode character" );
			return NULL;
		}
		const int ipa = ipaText.GetAt( 0 );
		if( ipa == phoneme->GetIPA() ){
			return NULL;
		}
		
		const saePhonemeList &phonemeList = sanimation->GetPhonemeList();
		if( phonemeList.HasIPA( ipa ) ){
			ResetTextField( *textField, *phoneme, "There exists already a Phoneme with this IPA symbol" );
			return NULL;
		}
		
		return new saeUPhonemeSetIPA( phoneme, ipa );
	}
	
	void ResetTextField( igdeTextField &textField, const saePhoneme &phoneme, const char *error ){
		igdeCommonDialogs::Error( &pPanel, "Change Phoneme IPA", error );
		textField.SetText( decUnicodeString( phoneme.GetIPA() ).ToUTF8() );
	}
};

class cTextPhonemeSampleText : public cBasePhonemeTextFieldListener{
public:
	cTextPhonemeSampleText( saeWPSAnim &panel ) : cBasePhonemeTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme ){
		if( textField->GetText() == phoneme->GetSampleText() ){
			return NULL;
		}
		return new saeUPhonemeSetSampleText( phoneme, textField->GetText() );
	}
};

class cTextPhonemeLength : public cBasePhonemeTextFieldListener{
public:
	cTextPhonemeLength( saeWPSAnim &panel ) : cBasePhonemeTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField,
	saeSAnimation *sanimation, saePhoneme * phoneme ){
		const float value = textField->GetFloat();
		if( fabsf( value - phoneme->GetLength() ) < FLOAT_SAFE_EPSILON ){
			return NULL;
		}
		return new saeUPhonemeSetLength( phoneme, value );
	}
};

class cComboPhonemeMove : public cBaseComboBoxListener{
public:
	cComboPhonemeMove( saeWPSAnim &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, saeSAnimation *sanimation ){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		if( ! phoneme ){
			return NULL;
		}
		
		const decString &text = comboBox->GetText();
		if( text == phoneme->GetMoveName() ){
			return NULL;
		}
		return new saeUPhonemeSetMoveName( phoneme, text );
	}
};



class cComboWord : public cBaseComboBoxListener{
public:
	cComboWord( saeWPSAnim &panel ) : cBaseComboBoxListener( panel ){ }
	
	virtual igdeUndo *OnTextChanged( igdeComboBox *comboBox, saeSAnimation *sanimation ){
		const igdeListItem * const selection = comboBox->GetSelectedItem();
		if( selection ){
			sanimation->SetActiveWord( ( saeWord* )selection->GetData() );
			
		}else{
			sanimation->SetActiveWord( NULL );
		}
		return NULL;
	}
};

class cBaseWordTextFieldListener : public cBaseTextFieldListener{
public:
	cBaseWordTextFieldListener( saeWPSAnim &panel ) : cBaseTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, saeSAnimation *sanimation ){
		saeWord * const word = pPanel.GetActiveWord();
		if( ! word ){
			return NULL;
		}
		return OnChanged( textField, sanimation, word );
	}
	
	virtual igdeUndo *OnChanged( igdeTextField *textField,
		saeSAnimation *sanimation, saeWord * word ) = 0;
};

class cTextWordName : public cBaseWordTextFieldListener{
public:
	cTextWordName( saeWPSAnim &panel ) : cBaseWordTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField,
	saeSAnimation *sanimation, saeWord * word ){
		const decString &name = textField->GetText();
		if( name == word->GetName() ){
			return NULL;
		}
		
		if( name.IsEmpty() ){
			ResetTextField( *textField, *word, "Name can not be empty" );
			return NULL;
		}
		
		const saeWordList &wordList = sanimation->GetWordList();
		if( wordList.HasNamed( name ) ){
			ResetTextField( *textField, *word, "There exists already a Word with this name" );
			return NULL;
		}
		
		return new saeUWordSetName( word, name );
	}
	
	void ResetTextField( igdeTextField &textField, const saeWord &word, const char *error ){
		igdeCommonDialogs::Error( &pPanel, "Change Word Name", error );
		textField.SetText( word.GetName() );
	}
};

class cTextWordPhonetics : public cBaseWordTextFieldListener{
public:
	cTextWordPhonetics( saeWPSAnim &panel ) : cBaseWordTextFieldListener( panel ){ }
	
	virtual igdeUndo *OnChanged( igdeTextField *textField, saeSAnimation*, saeWord * word ){
		decUnicodeString phonetics;
		try{
			phonetics = decUnicodeString::NewFromUTF8( textField->GetText() );
			
		}catch( const deException & ){
			ResetTextField( *textField, *word, "Invalid phonetics (wrong UTF8 encoding)" );
			return NULL;
		}
		
		return phonetics != word->GetPhonetics() ? new saeUWordSetPhonetics( word, phonetics ) : NULL;
	}
	
	void ResetTextField( igdeTextField &textField, const saeWord &word, const char *error ){
		igdeCommonDialogs::Error( &pPanel, "Change Word Phonetics", error );
		textField.SetText( word.GetPhonetics().ToUTF8() );
	}
};

class cActionWordAddIpa : public cBaseAction {
private:
	igdeTextField &pTextFieldPhonetics;
	
public:
	cActionWordAddIpa( saeWPSAnim &panel, igdeTextField &textFieldPhonetics ) :
	cBaseAction( panel, panel.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallPlus ),
		"Insert selected phoneme IPA to selected word phonetics at cursor position" ),
	pTextFieldPhonetics( textFieldPhonetics ){ }
	
	virtual igdeUndo *OnAction( saeSAnimation *sanimation ){
		saePhoneme * const phoneme = pPanel.GetActivePhoneme();
		saeWord * const word = pPanel.GetActiveWord();
		if( ! phoneme || ! word ){
			return NULL;
		}
		
		const decString &oldPhoneticsUtf8 = pTextFieldPhonetics.GetText();
		const int position = pTextFieldPhonetics.GetCursorPosition();
		
		decString phoneticsUtf8( oldPhoneticsUtf8.GetLeft( position ) );
		phoneticsUtf8.AppendCharacter( phoneme->GetIPA() );
		phoneticsUtf8 += oldPhoneticsUtf8.GetRight( oldPhoneticsUtf8.GetLength() - position );
		
		const decUnicodeString phonetics( decUnicodeString::NewFromUTF8( phoneticsUtf8 ) );
		if( phonetics == word->GetPhonetics() ){
			return NULL;
		}
		return new saeUWordSetPhonetics( word, phonetics );
	}
};

}



// Class saeWPSAnim
/////////////////////

// Constructor, destructor
////////////////////////////

saeWPSAnim::saeWPSAnim( saeWindowProperties &windowProperties ) :
igdeContainerScroll( windowProperties.GetEnvironment(), false, true ),
pWindowProperties( windowProperties ),
pListener( NULL ),
pSAnimation( NULL )
{
	igdeEnvironment &env = windowProperties.GetEnvironment();
	igdeContainerReference content, groupBox, frameLine;
	igdeUIHelper &helper = env.GetUIHelperProperties();
	
	pListener = new saeWPSAnimListener( *this );
	
	content.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
	AddChild( content );
	
	
	// speech animation
	helper.GroupBox( content, groupBox, "Speech Animation:" );
	
	helper.EditPath( groupBox, "Rig:", "Path to rig resource.",
		igdeEnvironment::efpltRig, pEditRigPath, new cEditRigPath( *this ) );
	helper.EditPath( groupBox, "Animation:", "Path to animation resource.",
		igdeEnvironment::efpltAnimation, pEditAnimPath, new cEditAnimationPath( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Neutral Move:", true,
		"Animation move to use for the neutral mouth position.",
		pCBNeutralMove, new cComboNeutralMove( *this ) );
	pCBNeutralMove->SetDefaultSorter();
	
	
	// phoneme
	helper.GroupBox( content, groupBox, "Phoneme:" );
	
	helper.ComboBox( groupBox, "Phoneme:", "Phoneme to edit.", pCBPhoneme, new cComboPhoneme( *this ) );
	pCBPhoneme->SetDefaultSorter();
	
	helper.EditString( groupBox, "IPA Symbol:", "IPA Symbol (Unicode) representing the phoneme.",
		pEditPhonemeIPA, new cTextPhonemeIpa( *this ) );
	helper.EditString( groupBox, "Sample Text:", "Sample text containing the phoneme.",
		pEditPhonemeSampleText, new cTextPhonemeSampleText( *this ) );
	helper.EditFloat( groupBox, "Length:", "Length of the phoneme in seconds.",
		pEditPhonemeLength, new cTextPhonemeLength( *this ) );
	
	helper.ComboBoxFilter( groupBox, "Move:", true, "Animation move to use for this phoneme.",
		pCBPhonemeMove, new cComboPhonemeMove( *this ) );
	pCBPhonemeMove->SetDefaultSorter();
	
	
	// word
	helper.GroupBox( content, groupBox, "Word:" );
	
	helper.ComboBoxFilter( groupBox, "Word:", "Word to edit.", pCBWord, new cComboWord( *this ) );
	pCBWord->SetDefaultSorter();
	
	helper.EditString( groupBox, "Name:", "Name of the word.",
		pEditWordName, new cTextWordName( *this ) );
	
	helper.FormLineStretchFirst( groupBox, "Phonetics", "Phonetics of the word.", frameLine );
	helper.EditString( frameLine, "Phonetics of the word.",
		pEditWordPhonetics, new cTextWordPhonetics( *this ) );
	pActionWordAddIpa.TakeOver( new cActionWordAddIpa( *this, pEditWordPhonetics ) );
	helper.Button( frameLine, pBtnWordAddIPA, pActionWordAddIpa );
}

saeWPSAnim::~saeWPSAnim(){
	SetSAnimation( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void saeWPSAnim::SetSAnimation( saeSAnimation *sanimation ){
	if( sanimation == pSAnimation ){
		return;
	}
	
	if( pSAnimation ){
		pSAnimation->RemoveListener( pListener );
		pSAnimation->FreeReference();
	}
	
	pSAnimation = sanimation;
	
	if( sanimation ){
		sanimation->AddListener( pListener );
		sanimation->AddReference();
	}
	
	UpdateSAnimation();
}



void saeWPSAnim::UpdateSAnimation(){
	UpdateNeutralMoveList();
	
	if( pSAnimation ){
		pEditRigPath->SetPath( pSAnimation->GetRigPath() );
		pEditAnimPath->SetPath( pSAnimation->GetAnimationPath() );
		pCBNeutralMove->SetText( pSAnimation->GetNeutralMoveName() );
		
	}else{
		pEditRigPath->ClearPath();
		pEditAnimPath->ClearPath();
		pCBNeutralMove->ClearText();
	}
	
	const bool enabled = pSAnimation != NULL;
	pEditRigPath->SetEnabled( enabled );
	pEditAnimPath->SetEnabled( enabled );
	pCBNeutralMove->SetEnabled( enabled );
	
	UpdatePhonemeMoveList();
	UpdatePhonemeList();
	
	UpdateWordList();
}

void saeWPSAnim::UpdateNeutralMoveList(){
	pCBNeutralMove->RemoveAllItems();
	
	if( pSAnimation ){
		const deAnimator * const engAnimator = pSAnimation->GetEngineAnimator();
		
		if( engAnimator ){
			const deAnimation * const engAnimation = engAnimator->GetAnimation();
			
			if( engAnimation ){
				const int count = engAnimation->GetMoveCount();
				int i;
				
				for( i=0; i<count; i++ ){
					pCBNeutralMove->AddItem( engAnimation->GetMove( i )->GetName() );
				}
			}
		}
		
		pCBNeutralMove->SortItems();
	}
	
	pCBNeutralMove->StoreFilterItems();
	pCBNeutralMove->FilterItems();
}



saePhoneme *saeWPSAnim::GetActivePhoneme() const{
	if( pSAnimation ){
		return pSAnimation->GetActivePhoneme();
	}
	
	return NULL;
}

void saeWPSAnim::UpdatePhonemeList(){
	pCBPhoneme->RemoveAllItems();
	
	if( pSAnimation ){
		const saePhonemeList &list = pSAnimation->GetPhonemeList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			saePhoneme * const phoneme = list.GetAt( i );
			decUnicodeString text( phoneme->GetIPA() );
			text.AppendFormat( " (0x%.4x): %s", phoneme->GetIPA(),
				phoneme->GetSampleText().GetString() );
			pCBPhoneme->AddItem( text.ToUTF8(), NULL, phoneme );
		}
		
		pCBPhoneme->SortItems();
	}
	
	SelectActivePhoneme();
}

void saeWPSAnim::SelectActivePhoneme(){
	pCBPhoneme->SetSelection( pCBPhoneme->IndexOfItemWithData( GetActivePhoneme() ) );
	UpdatePhoneme();
}

void saeWPSAnim::UpdatePhoneme(){
	saePhoneme * const phoneme = GetActivePhoneme();
	
	if( phoneme ){
		pEditPhonemeIPA->SetText( decUnicodeString( phoneme->GetIPA() ).ToUTF8() );
		pEditPhonemeSampleText->SetText( phoneme->GetSampleText() );
		pEditPhonemeLength->SetFloat( phoneme->GetLength() );
		pCBPhonemeMove->SetText( phoneme->GetMoveName() );
		
	}else{
		pEditPhonemeIPA->ClearText();
		pEditPhonemeSampleText->ClearText();
		pEditPhonemeLength->ClearText();
		pCBPhonemeMove->ClearText();
	}
	
	const bool enabled = phoneme != NULL;
	pEditPhonemeIPA->SetEnabled( enabled );
	pEditPhonemeSampleText->SetEnabled( enabled );
	pEditPhonemeLength->SetEnabled( enabled );
	pCBPhonemeMove->SetEnabled( enabled );
}

void saeWPSAnim::UpdatePhonemeMoveList(){
	saePhoneme * const phoneme = GetActivePhoneme();
	
	pCBPhonemeMove->RemoveAllItems();
	
	if( pSAnimation ){
		const deAnimator * const engAnimator = pSAnimation->GetEngineAnimator();
		
		if( engAnimator ){
			const deAnimation * const engAnimation = engAnimator->GetAnimation();
			
			if( engAnimation ){
				const int count = engAnimation->GetMoveCount();
				int i;
				
				for( i=0; i<count; i++ ){
					pCBPhonemeMove->AddItem( engAnimation->GetMove( i )->GetName() );
				}
			}
		}
		
		pCBPhonemeMove->SortItems();
	}
	
	pCBPhonemeMove->StoreFilterItems();
	pCBPhonemeMove->FilterItems();
	
	if( pSAnimation ){
		pSAnimation->SetActivePhoneme( phoneme );
	}
}



saeWord *saeWPSAnim::GetActiveWord() const{
	if( pSAnimation ){
		return pSAnimation->GetActiveWord();
	}
	
	return NULL;
}

void saeWPSAnim::UpdateWordList(){
	saeWord * const selection = GetActiveWord();
	
	pCBWord->RemoveAllItems();
	
	if( pSAnimation ){
		const saeWordList &list = pSAnimation->GetWordList();
		const int count = list.GetCount();
		int i;
		
		for( i=0; i<count; i++ ){
			saeWord * const word = list.GetAt( i );
			pCBWord->AddItem( word->GetName(), NULL, word );
		}
		
		pCBWord->SortItems();
	}
	
	pCBWord->StoreFilterItems();
	pCBWord->FilterItems();
	
	if( pSAnimation ){
		pSAnimation->SetActiveWord( selection );
	}
}

void saeWPSAnim::SelectActiveWord(){
	pCBWord->SetSelection( pCBWord->IndexOfItemWithData( GetActiveWord() ) );
	UpdateWord();
}

void saeWPSAnim::UpdateWord(){
	saeWord * const word = GetActiveWord();
	
	if( word ){
		pEditWordName->SetText( word->GetName() );
		pEditWordPhonetics->SetText( word->GetPhonetics().ToUTF8() );
		
	}else{
		pEditWordName->ClearText();
		pEditWordPhonetics->ClearText();
	}
	
	const bool enabled = word != NULL;
	pEditWordName->SetEnabled( enabled );
	pEditWordPhonetics->SetEnabled( enabled );
	pActionWordAddIpa->SetEnabled( enabled );
}
