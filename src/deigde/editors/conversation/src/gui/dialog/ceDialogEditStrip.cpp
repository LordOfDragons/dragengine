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

#include "ceDialogEditStrip.h"
#include "../../conversation/strip/ceStrip.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeApplication.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainer.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>

#include <dragengine/common/exceptions.h>



namespace {

class cComboIdentifier : public igdeComboBoxListener{
	ceDialogEditStrip &pDialog;
	
public:
	cComboIdentifier( ceDialogEditStrip &dialog ) : pDialog( dialog ){}
	
	virtual void OnTextChanged( igdeComboBox* ){
		if( pDialog.GetAutoResetDuration() ){
			pDialog.ResetDuration();
		}
	}
};

class cActionResetDuration : public igdeAction{
	ceDialogEditStrip &pDialog;
	
public:
	cActionResetDuration( ceDialogEditStrip &dialog ) : igdeAction( "",
		dialog.GetEnvironment().GetStockIcon( igdeEnvironment::esiUndo ),
		"Reset duration" ), pDialog( dialog ){}
	
	virtual void OnAction(){
		pDialog.ResetDuration();
	}
};

}



// Class ceDialogEditStrip::Listener
//////////////////////////////////////

ceDialogEditStrip::Listener::Listener(){
}

ceDialogEditStrip::Listener::~Listener(){
}

float ceDialogEditStrip::Listener::DefaultDuration( const decString & ){
	return 0.5f;
}



// Class ceDialogEditStrip
////////////////////////////

// Constructor, destructor
////////////////////////////

ceDialogEditStrip::ceDialogEditStrip( igdeEnvironment &environment,
const char *windowTitle, const char *textLabel ) :
igdeDialog( environment, windowTitle ),
pAutoResetDuration( true )
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainer::Ref content;
	content.TakeOver(new igdeContainerForm(environment));
	
	helper.ComboBoxFilter( content, textLabel, 25, true, "", pCBID, new cComboIdentifier( *this ) );
	pCBID->SetDefaultSorter();
	
	helper.EditFloat( content, "Pause:", "Time in seconds to wait before strip is activated", pEditPause, NULL );
	
	igdeContainer::Ref line;
	const char *tooltip = "Duration in seconds of strip";
	helper.FormLineStretchFirst( content, "Duration:", tooltip, line );
	helper.EditFloat( line, tooltip, pEditDuration, nullptr );
	helper.Button( line, pBtnResetDuration, new cActionResetDuration( *this ), true );
	
	igdeContainer::Ref buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Cancel" );
	
	AddContent( content, buttonBar );
	
	SetSize(igdeApplication::app().DisplayScaled(decPoint(400, 150)));
}

ceDialogEditStrip::~ceDialogEditStrip(){
}



// Management
///////////////

void ceDialogEditStrip::SetIDList( const decStringList &list ){
	const decString selection( pCBID->GetText() );
	
	const int count = list.GetCount();
	int i;
	
	pCBID->RemoveAllItems();
	for( i=0; i<count; i++ ){
		pCBID->AddItem( list.GetAt( i ) );
	}
	pCBID->SortItems();
	pCBID->StoreFilterItems();
	
	pCBID->SetText( selection );
}

void ceDialogEditStrip::SetID( const char *id ){
	pCBID->SetText( id );
}

void ceDialogEditStrip::SetPause( float pause ){
	pEditPause->SetFloat( pause );
}

void ceDialogEditStrip::SetDuration( float duration ){
	pEditDuration->SetFloat( duration );
}

void ceDialogEditStrip::SetListener( const Listener::Ref &listener ){
	pListener = listener;
}

void ceDialogEditStrip::ResetDuration(){
	if( pListener ){
		pEditDuration->SetFloat( pListener->DefaultDuration( pCBID->GetText() ) );
	}
}

void ceDialogEditStrip::SetAutoResetDuration( bool autoResetDuration ){
	pAutoResetDuration = autoResetDuration;
}



void ceDialogEditStrip::SetFromStrip( const ceStrip &strip ){
	SetID( strip.GetID() );
	SetPause( strip.GetPause() );
	SetDuration( strip.GetDuration() );
}

void ceDialogEditStrip::UpdateStrip( ceStrip &strip ) const{
	strip.SetID( pCBID->GetText() );
	strip.SetPause( pEditPause->GetFloat() );
	strip.SetDuration( pEditDuration->GetFloat() );
}

ceStrip *ceDialogEditStrip::CreateStrip() const{
	ceStrip * const strip = new ceStrip;
	UpdateStrip( *strip );
	return strip;
}



void ceDialogEditStrip::OnDialogShown(){
	pCBID->Focus();
}
