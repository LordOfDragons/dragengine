/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceDialogEditStrip.h"
#include "../../conversation/strip/ceStrip.h"

#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBoxFilter.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextField.h>
#include <deigde/gui/layout/igdeContainerForm.h>

#include <dragengine/common/exceptions.h>



// Class ceDialogEditStrip
////////////////////////////

// Constructor, destructor
////////////////////////////

ceDialogEditStrip::ceDialogEditStrip( igdeEnvironment &environment,
const char *windowTitle, const char *textLabel ) :
igdeDialog( environment, windowTitle )
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerForm( environment, igdeContainerForm::esLast ) );
	
	helper.ComboBoxFilter( content, textLabel, true, "", pCBID, NULL );
	pCBID->SetDefaultSorter();
	
	helper.EditFloat( content, "Pause:", "Time in seconds to wait before strip is activated", pEditPause, NULL );
	helper.EditFloat( content, "Duration:", "Duration in seconds of strip", pEditDuration, NULL );
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Cancel" );
	
	AddContent( content, buttonBar );
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
