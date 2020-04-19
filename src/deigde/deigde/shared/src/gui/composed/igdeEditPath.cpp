/* 
 * Drag[en]gine IGDE
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeEditPath.h"
#include "igdeEditPathListener.h"
#include "../igdeButton.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../browse/igdeDialogBrowserSkin.h"
#include "../event/igdeActionReference.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"
#include "../menu/igdeMenuCascade.h"
#include "../menu/igdeMenuCascadeReference.h"
#include "../menu/igdeMenuCommand.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeEditPath::cListenerTextField
///////////////////////////////////////////

igdeEditPath::cListenerTextField::cListenerTextField( igdeEditPath &editPath ) :
pEditPath( editPath ){
}

igdeEditPath::cListenerTextField::~cListenerTextField(){
}

void igdeEditPath::cListenerTextField::OnTextChanged( igdeTextField *textField ){
	// SetPath checks against textField->GetText() so ValidatePath() would be skipped
	//pEditPath.SetPath( textField->GetText() );
	if( pEditPath.GetAutoValidatePath() ){
		pEditPath.ValidatePath();
	}
	pEditPath.NotifyEditPathChanged();
}



// Class igdeEditPath::cActionButton
//////////////////////////////////////

igdeEditPath::cActionButton::cActionButton( igdeEditPath &editPath, const char *description ) :
igdeActionContextMenu( "...", NULL, description ),
pEditPath( editPath ){
}

igdeEditPath::cActionButton::~cActionButton(){
}

void igdeEditPath::cActionButton::OnAction(){
	if( pEditPath.GetSelectPathActionCount() == 1 ){
		pEditPath.GetSelectPathActionAt( 0 )->OnAction();
		
	}else{
		igdeActionContextMenu::OnAction();
	}
}

void igdeEditPath::cActionButton::AddContextMenuEntries( igdeMenuCascade &contextMenu ){
	igdeUIHelper &helper = pEditPath.GetEnvironment().GetUIHelper();
	const int count = pEditPath.GetSelectPathActionCount();
	int i;
	
	for( i=0; i<count; i++ ){
		helper.MenuCommand( contextMenu, pEditPath.GetSelectPathActionAt( i ) );
	}
}

void igdeEditPath::cActionButton::Update(){
	SetDescription( pEditPath.GetDescription() );
	SetEnabled( pEditPath.GetEnabled() && pEditPath.GetSelectPathActionCount() > 0 );
}



// Class igdeEditPath::cActionSelectFileDialog
////////////////////////////////////////////////

igdeEditPath::cActionSelectFileDialog::cActionSelectFileDialog(
	igdeEditPath &editPath, igdeTextField &textField ) :
igdeActionSelectFile( editPath.GetEnvironment(),
	editPath.GetCustomPatternList().GetFilePatternCount() > 0 ? editPath.GetCustomPatternList()
		: *editPath.GetEnvironment().GetOpenFilePatternList( editPath.GetResourceType() ),
	textField, editPath.GetUseGameVFS() ),
pEditPath( editPath )
{
	SetText( "Select From File..." );
	SetDescription( "Select From File" );
}

igdeEditPath::cActionSelectFileDialog::~cActionSelectFileDialog(){
}

decString igdeEditPath::cActionSelectFileDialog::DefaultPath(){
	return pEditPath.GetDefaultPath();
}



// Class igdeEditPath::cActionSkinSelectDialog
////////////////////////////////////////////////

igdeEditPath::cActionSkinSelectDialog::cActionSkinSelectDialog( igdeEditPath &editPath ) :
igdeAction( "Select From Skin List...", "Select From Skin List" ),
pEditPath( editPath ){
}

igdeEditPath::cActionSkinSelectDialog::~cActionSkinSelectDialog(){
}

void igdeEditPath::cActionSkinSelectDialog::OnAction(){
	if( ! pEditPath.GetUseGameVFS() ){
		return; // not possible on native file system
	}
	
	decString skin( pEditPath.GetPath() );
	if( igdeDialogBrowserSkin::SelectSkin( &pEditPath, skin ) && skin != pEditPath.GetPath() ){
		pEditPath.SetPath( skin );
		pEditPath.NotifyEditPathChanged();
	}
}



// Class igdeEditPath
///////////////////////

// Constructor, destructor
////////////////////////////

igdeEditPath::igdeEditPath( igdeUIHelper &helper, igdeEnvironment::eFilePatternListTypes resourceType,
	const char *description, bool useGameVFS ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaX, igdeContainerFlow::esFirst, 2 ),
pResourceType( resourceType ),
pAutoValidatePath( true ),
pUseGameVFS( useGameVFS )
{
	pActionButton.TakeOver( new cActionButton( *this, description ) );
	
	helper.EditString( *this, description, pText, new cListenerTextField( *this ) );
	helper.Button( *this, pButton, pActionButton );
	
	pActionButton->SetWidget( pButton );
	
	SetSelectPathActions();
}

igdeEditPath::igdeEditPath( igdeUIHelper &helper, const igdeFilePatternList &customFilePatterns,
	const char *description, bool useGameVFS ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaX, igdeContainerFlow::esFirst, 2 ),
pResourceType( igdeEnvironment::efpltAll ),
pCustomPatternList( customFilePatterns ),
pAutoValidatePath( true ),
pUseGameVFS( useGameVFS )
{
	pActionButton.TakeOver( new cActionButton( *this, description ) );
	
	helper.EditString( *this, description, pText, new cListenerTextField( *this ) );
	helper.Button( *this, pButton, pActionButton );
	
	pActionButton->SetWidget( pButton );
	
	SetSelectPathActions();
}

igdeEditPath::~igdeEditPath(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeEditPath::SetResourceType( igdeEnvironment::eFilePatternListTypes resourceType ){
	if( resourceType == pResourceType ){
		return;
	}
	
	pResourceType = resourceType;
	
	SetSelectPathActions();
}



const decString &igdeEditPath::GetPath() const{
	return pText->GetText();
}

void igdeEditPath::SetPath( const char *path ){
	if( pText->GetText() == path ){
		return;
	}
	
	pText->SetText( path );
	if( pAutoValidatePath ){
		ValidatePath();
	}
}

void igdeEditPath::ClearPath(){
	pText->ClearText();
	pText->SetInvalidValue( false );
}

bool igdeEditPath::GetEnabled() const{
	return pText->GetEnabled();
}

void igdeEditPath::SetEnabled( bool enabled ){
	pText->SetEnabled( enabled );
	pActionButton->Update();
}

const decString &igdeEditPath::GetDescription() const{
	return pText->GetDescription();
}

void igdeEditPath::SetDescription( const char *description ){
	pText->SetDescription( description );
	pActionButton->Update();
}

void igdeEditPath::SetDefaultPath( const char *path ){
	pDefaultPath = path;
}

bool igdeEditPath::IsPathValid() const{
	return ! pText->GetInvalidValue();
}

void igdeEditPath::ValidatePath(){
	// path can be absolute or relative. if relative we are missing additional information
	// what path to use. so for the time being only absolute path are verified
	if( decPath::IsUnixPathAbsolute( pText->GetText() ) ){
		if( pUseGameVFS ){
			deVirtualFileSystem &vfs = *GetEnvironment().GetFileSystemGame();
			pText->SetInvalidValue( ! pText->GetText().IsEmpty()
				&& ! vfs.ExistsFile( decPath::CreatePathUnix( pText->GetText() ) ) );
			
		}else{
			pText->SetInvalidValue( false ); // TODO implement this
		}
		
	}else{
		pText->SetInvalidValue( false );
	}
}

void igdeEditPath::Focus(){
	pText->Focus();
}

void igdeEditPath::SetAutoValidatePath( bool autoValidatePath ){
	pAutoValidatePath = autoValidatePath;
}



int igdeEditPath::GetSelectPathActionCount() const{
	return pSelectPathActions.GetCount();
}

igdeAction *igdeEditPath::GetSelectPathActionAt( int index ) const{
	return ( igdeAction* )pSelectPathActions.GetAt( index );
}

void igdeEditPath::AddSelectPathAction( igdeAction *action ){
	if( ! action ){
		DETHROW( deeInvalidParam );
	}
	pSelectPathActions.AddIfAbsent( action );
	pActionButton->Update();
}

void igdeEditPath::RemoveSelectPathAction( igdeAction *action ){
	pSelectPathActions.RemoveIfPresent( action );
	pActionButton->Update();
}

void igdeEditPath::RemoveAllSelectPathActions(){
	pSelectPathActions.RemoveAll();
	pActionButton->Update();
}

void igdeEditPath::UpdateSelectPathActions(){
	const int count = pSelectPathActions.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeAction* )pSelectPathActions.GetAt( i ) )->Update();
	}
}

void igdeEditPath::SetSelectPathActions(){
	RemoveAllSelectPathActions();
	
	igdeActionReference action;
	
	switch( pResourceType ){
	case igdeEnvironment::efpltSkin:
		action.TakeOver( new cActionSkinSelectDialog( *this ) );
		AddSelectPathAction( action );
		break;
		
	default:
		break;
	}
	
	action.TakeOver( new cActionSelectFileDialog( *this, pText ) );
	AddSelectPathAction( action );
}



void igdeEditPath::AddListener( igdeEditPathListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditPath::RemoveListener( igdeEditPathListener *listener ){
	pListeners.Remove( listener );
}

void igdeEditPath::NotifyEditPathChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditPathListener* )listeners.GetAt( i ) )->OnEditPathChanged( this );
	}
}
