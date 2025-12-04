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

#include "igdeEditDirectory.h"
#include "igdeEditDirectoryListener.h"
#include "../igdeButton.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../browse/igdeDialogBrowserSkin.h"
#include "../event/igdeAction::Ref.h"
#include "../event/igdeTextFieldListener::Ref.h"
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
