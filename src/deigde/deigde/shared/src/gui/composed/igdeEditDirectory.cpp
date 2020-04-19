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

#include "igdeEditDirectory.h"
#include "igdeEditDirectoryListener.h"
#include "../igdeButton.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../browse/igdeDialogBrowserSkin.h"
#include "../event/igdeActionReference.h"
#include "../event/igdeTextFieldListenerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>



// Class igdeEditDirectory::cListenerTextField
////////////////////////////////////////////////

igdeEditDirectory::cListenerTextField::cListenerTextField( igdeEditDirectory &editDirectory ) :
pEditDirectory( editDirectory ){
}

igdeEditDirectory::cListenerTextField::~cListenerTextField(){
}

void igdeEditDirectory::cListenerTextField::OnTextChanged( igdeTextField *textField ){
	// SetDirectory checks against textField->GetText() so ValidateDirectory() would be skipped
	//pEditDirectory.SetDirectory( textField->GetText() );
	if( pEditDirectory.GetAutoValidateDirectory() ){
		pEditDirectory.ValidateDirectory();
	}
	pEditDirectory.NotifyEditDirectoryChanged();
}



// Class igdeEditDirectory::cActionButton
//////////////////////////////////////

igdeEditDirectory::cActionButton::cActionButton( igdeEditDirectory &editDirectory,
	igdeTextField &textField, const char *description ) :
igdeActionSelectDirectory( editDirectory.GetEnvironment(), textField, editDirectory.GetUseGameVFS() ),
pEditDirectory( editDirectory )
{
	SetDescription( description );
}

igdeEditDirectory::cActionButton::~cActionButton(){
}

decString igdeEditDirectory::cActionButton::DefaultDirectory(){
	return pEditDirectory.GetDefaultDirectory();
}

void igdeEditDirectory::cActionButton::Update(){
	SetDescription( pEditDirectory.GetDescription() );
	SetEnabled( pEditDirectory.GetEnabled() );
}



// Class igdeEditDirectory
////////////////////////////

// Constructor, destructor
////////////////////////////

igdeEditDirectory::igdeEditDirectory( igdeUIHelper &helper, const char *description, bool useGameVFS ) :
igdeContainerFlow( helper.GetEnvironment(), igdeContainerFlow::eaX, igdeContainerFlow::esFirst, 2 ),
pAutoValidateDirectory( true ),
pUseGameVFS( useGameVFS )
{
	helper.EditString( *this, description, pText, new cListenerTextField( *this ) );
	
	pActionButton.TakeOver( new cActionButton( *this, pText, description ) );
	helper.Button( *this, pButton, pActionButton );
}

igdeEditDirectory::~igdeEditDirectory(){
	DestroyNativeWidget();
}



// Management
///////////////

const decString &igdeEditDirectory::GetDirectory() const{
	return pText->GetText();
}

void igdeEditDirectory::SetDirectory( const char *directory ){
	if( pText->GetText() == directory ){
		return;
	}
	
	pText->SetText( directory );
	if( pAutoValidateDirectory ){
		ValidateDirectory();
	}
}

void igdeEditDirectory::ClearDirectory(){
	pText->ClearText();
	pText->SetInvalidValue( false );
}

bool igdeEditDirectory::GetEnabled() const{
	return pText->GetEnabled();
}

void igdeEditDirectory::SetEnabled( bool enabled ){
	pText->SetEnabled( enabled );
	pActionButton->Update();
}

const decString &igdeEditDirectory::GetDescription() const{
	return pText->GetDescription();
}

void igdeEditDirectory::SetDescription( const char *description ){
	pText->SetDescription( description );
	pActionButton->Update();
}

void igdeEditDirectory::SetDefaultDirectory( const char *path ){
	pDefaultDirectory = path;
}

bool igdeEditDirectory::IsDirectoryValid() const{
	return ! pText->GetInvalidValue();
}

void igdeEditDirectory::ValidateDirectory(){
	// path can be absolute or relative. if relative we are missing additional information
	// what path to use. so for the time being only absolute path are verified
	if( decPath::IsUnixPathAbsolute( pText->GetText() ) ){
		if( pUseGameVFS ){
			if( ! pText->GetText().IsEmpty() ){
				deVirtualFileSystem &vfs = *GetEnvironment().GetFileSystemGame();
				const decPath path( decPath::CreatePathUnix( pText->GetText() ) );
				pText->SetInvalidValue( ! vfs.ExistsFile( path )
					|| vfs.GetFileType( path ) != deVFSContainer::eftDirectory );
			}
			
		}else{
			// TODO implement this
		}
	}
	
	pText->SetInvalidValue( false );
}

void igdeEditDirectory::Focus(){
	pText->Focus();
}

void igdeEditDirectory::SetAutoValidateDirectory( bool autoValidateDirectory ){
	pAutoValidateDirectory = autoValidateDirectory;
}



void igdeEditDirectory::AddListener( igdeEditDirectoryListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeEditDirectory::RemoveListener( igdeEditDirectoryListener *listener ){
	pListeners.Remove( listener );
}

void igdeEditDirectory::NotifyEditDirectoryChanged(){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeEditDirectoryListener* )listeners.GetAt( i ) )->OnEditDirectoryChanged( this );
	}
}
