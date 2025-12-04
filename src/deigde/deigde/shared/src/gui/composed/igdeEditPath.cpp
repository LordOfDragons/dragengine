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

#include "igdeEditPath.h"
#include "igdeEditPathListener.h"
#include "../igdeButton.h"
#include "../igdeTextField.h"
#include "../igdeContainerReference.h"
#include "../igdeUIHelper.h"
#include "../browse/igdeDialogBrowserSkin.h"
#include "../event/igdeAction.h"
#include "../event/igdeActionExternOpen.h"
#include "../event/igdeActionExternOpenReference.h"
#include "../event/igdeTextFieldListener.h"
#include "../layout/igdeContainerFlow.h"
#include "../menu/igdeMenuCascade.h"
#include "../menu/igdeMenuCascadeReference.h"
#include "../menu/igdeMenuCommand.h"
#include "../../gameproject/igdeGameProject.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decDiskFileReader.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/filesystem/deVirtualFileSystem.h>
#include <dragengine/filesystem/deVFSDiskDirectory.h>



// Class igdeEditPath::cListenerTextField
///////////////////////////////////////////

igdeEditPath::cListenerTextField::cListenerTextField( igdeEditPath &editPath ) :
pEditPath( editPath ){
}

igdeEditPath::cListenerTextField::~cListenerTextField(){
}

void igdeEditPath::cListenerTextField::OnTextChanged( igdeTextField* ){
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
igdeAction( "...", nullptr, description ),
pEditPath( editPath ){
}

igdeEditPath::cActionButton::~cActionButton(){
}

void igdeEditPath::cActionButton::OnAction(){
	if( pEditPath.GetSelectPathActionCount() > 0 ){
		// guard reference to avoid action getting deleted while in use
		igdeAction::Ref( pEditPath.GetSelectPathActionAt( 0 ) )->OnAction();
	}
}

void igdeEditPath::cActionButton::Update(){
	SetDescription( pEditPath.GetDescription() );
	SetEnabled( pEditPath.GetEnabled() && pEditPath.GetSelectPathActionCount() > 0 );
}



// Class igdeEditPath::cActionButtonMenu
//////////////////////////////////////////

igdeEditPath::cActionButtonMenu::cActionButtonMenu( igdeEditPath &editPath, const char *description ) :
igdeActionContextMenu( "", editPath.GetEnvironment().GetStockIcon( igdeEnvironment::esiSmallDown ), description ),
pEditPath( editPath ){
}

igdeEditPath::cActionButtonMenu::~cActionButtonMenu(){
}

void igdeEditPath::cActionButtonMenu::AddContextMenuEntries( igdeMenuCascade &contextMenu ){
	pEditPath.AddContextMenuEntries( contextMenu );
}

void igdeEditPath::cActionButtonMenu::Update(){
	SetDescription( pEditPath.GetDescription() );
}



// Class igdeEditPath::cActionSelectFileDialog
////////////////////////////////////////////////

igdeEditPath::cActionSelectFileDialog::cActionSelectFileDialog(
	igdeEditPath &editPath, igdeTextField &textField ) :
igdeActionSelectFile( editPath.GetEnvironment(),
	editPath.GetCustomPatternList().GetFilePatternCount() > 0 ? editPath.GetCustomPatternList()
		: *editPath.GetEnvironment().GetOpenFilePatternList( editPath.GetResourceType() ),
	textField, editPath.GetUseGameVFS() ),
pEditPath( editPath ),
pUseRelativePath( false )
{
	SetText( "Select From File..." );
	SetDescription( "Select From File" );
}

igdeEditPath::cActionSelectFileDialog::~cActionSelectFileDialog(){
}

void igdeEditPath::cActionSelectFileDialog::PrepareFile( decString &path ){
	if( pEditPath.GetBasePath().IsEmpty() ){
		return;
	}
	
	if( pEditPath.GetUseGameVFS() ){
		pUseRelativePath = ! path.IsEmpty() && ! decPath::IsUnixPathAbsolute( path );
		path = decPath::AbsolutePathUnix( path, pEditPath.GetBasePath() ).GetPathUnix();
		
	}else{
		pUseRelativePath = ! path.IsEmpty() && ! decPath::IsNativePathAbsolute( path );
		path = decPath::AbsolutePathNative( path, pEditPath.GetBasePath() ).GetPathNative();
	}
}

bool igdeEditPath::cActionSelectFileDialog::AcceptFile( decString &path ){
	if( pUseRelativePath && ! pEditPath.GetBasePath().IsEmpty() ){
		if( pEditPath.GetUseGameVFS() ){
			path = decPath::RelativePathUnix( path, pEditPath.GetBasePath() ).GetPathUnix();
			
		}else{
			path = decPath::RelativePathNative( path, pEditPath.GetBasePath() ).GetPathNative();
		}
	}
	
	return igdeActionSelectFile::AcceptFile( path );
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
	
	decString path( pEditPath.GetPath() );
	bool useRelativePath;
	
	if( pEditPath.GetUseGameVFS() ){
		useRelativePath = ! decPath::IsUnixPathAbsolute( path );
		path = decPath::AbsolutePathUnix( path, pEditPath.GetBasePath() ).GetPathUnix();
		
	}else{
		useRelativePath = ! decPath::IsNativePathAbsolute( path );
		path = decPath::AbsolutePathNative( path, pEditPath.GetBasePath() ).GetPathNative();
	}
	
	if( ! igdeDialogBrowserSkin::SelectSkin( &pEditPath, path ) && path != pEditPath.GetPath() ){
		return;
	}
	
	if( useRelativePath && ! pEditPath.GetBasePath().IsEmpty() ){
		if( pEditPath.GetUseGameVFS() ){
			path = decPath::RelativePathUnix( path, pEditPath.GetBasePath() ).GetPathUnix();
			
		}else{
			path = decPath::RelativePathNative( path, pEditPath.GetBasePath() ).GetPathNative();
		}
	}
	
	pEditPath.SetPath( path );
	pEditPath.NotifyEditPathChanged();
}



// Class igdeEditPath::cActionBrowseFile
//////////////////////////////////////////

igdeEditPath::cActionBrowseFile::cActionBrowseFile( igdeEditPath &editPath ) :
igdeAction( "Browse Directory...", editPath.GetEnvironment().GetStockIcon( igdeEnvironment::esiOpen ),
	"Open file system browser with directory containing file" ),
pEditPath( editPath ){
}

igdeEditPath::cActionBrowseFile::~cActionBrowseFile(){
}

void igdeEditPath::cActionBrowseFile::OnAction(){
	pEditPath.BrowsePath();
}

void igdeEditPath::cActionBrowseFile::Update(){
	SetEnabled( ! pEditPath.GetPath().IsEmpty() );
}



// Class igdeEditPath::cActionConvertAbsolute
///////////////////////////////////////////////

igdeEditPath::cActionConvertAbsolute::cActionConvertAbsolute( igdeEditPath &editPath ) :
igdeAction( "To Absolute Path", editPath.GetEnvironment().GetStockIcon( igdeEnvironment::esiRight ),
	"Convert path to absolute path" ),
pEditPath( editPath ){
}

igdeEditPath::cActionConvertAbsolute::~cActionConvertAbsolute(){
}

void igdeEditPath::cActionConvertAbsolute::OnAction(){
	pEditPath.ToAbsolutePath();
}

void igdeEditPath::cActionConvertAbsolute::Update(){
	const decString &path = pEditPath.GetPath();
	
	if( pEditPath.GetUseGameVFS() ){
		SetEnabled( ! path.IsEmpty() && ! decPath::IsUnixPathAbsolute( path ) );
		
	}else{
		SetEnabled( ! path.IsEmpty() && ! decPath::IsNativePathAbsolute( path ) );
	}
}



// Class igdeEditPath::cActionConvertRelative
///////////////////////////////////////////////

igdeEditPath::cActionConvertRelative::cActionConvertRelative( igdeEditPath &editPath ) :
igdeAction( "To Relative Path", editPath.GetEnvironment().GetStockIcon( igdeEnvironment::esiLeft ),
	"Convert path to relative path" ),
pEditPath( editPath ){
}

igdeEditPath::cActionConvertRelative::~cActionConvertRelative(){
}

void igdeEditPath::cActionConvertRelative::OnAction(){
	pEditPath.ToRelativePath();
}

void igdeEditPath::cActionConvertRelative::Update(){
	const decString &path = pEditPath.GetPath();
	
	if( pEditPath.GetUseGameVFS() ){
		SetEnabled( ! path.IsEmpty() && decPath::IsUnixPathAbsolute( path ) );
		
	}else{
		SetEnabled( ! path.IsEmpty() && decPath::IsNativePathAbsolute( path ) );
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
	pActionButtonMenu.TakeOver( new cActionButtonMenu( *this, description ) );
	
	helper.EditString( *this, description, pText, new cListenerTextField( *this ) );
	helper.Button( *this, pButton, pActionButton );
	helper.Button( *this, pButtonMenu, pActionButtonMenu );
	
	pActionButtonMenu->SetWidget( pButtonMenu );
	
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
	pActionButtonMenu.TakeOver( new cActionButtonMenu( *this, description ) );
	
	helper.EditString( *this, description, pText, new cListenerTextField( *this ) );
	helper.Button( *this, pButton, pActionButton );
	helper.Button( *this, pButtonMenu, pActionButtonMenu );
	
	pActionButtonMenu->SetWidget( pButtonMenu );
	
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

decString igdeEditPath::GetAbsolutePath() const{
	if(pText->GetText().IsEmpty()){
		return "";
	}
	
	if(pUseGameVFS){
		return decPath::IsUnixPathAbsolute(pText->GetText())
			? pText->GetText()
			: decPath::AbsolutePathUnix(pText->GetText(), pBasePath).GetPathUnix();
		
	}else{
		return decPath::IsNativePathAbsolute(pText->GetText())
			? pText->GetText()
			: decPath::AbsolutePathNative( pText->GetText(), pBasePath ).GetPathNative();
	}
}

decString igdeEditPath::AsAbsolutePath(const decString &path) const{
	if(pBasePath.IsEmpty()){
		DETHROW_INFO(deeInvalidParam, "Base path is empty string");
	}
	
	return pUseGameVFS
		? decPath::AbsolutePathUnix(path, pBasePath).GetPathUnix()
		: decPath::AbsolutePathNative(path, pBasePath).GetPathNative();
}

decString igdeEditPath::GetRelativePath() const{
	if(pText->GetText().IsEmpty()){
		return "";
	}
	
	if(pUseGameVFS){
		return decPath::IsUnixPathAbsolute(pText->GetText())
			? decPath::RelativePathUnix(pText->GetText(), pBasePath, false).GetPathUnix()
			: pText->GetText();
		
	}else{
		return decPath::IsNativePathAbsolute(pText->GetText())
			? decPath::RelativePathNative(pText->GetText(), pBasePath, false).GetPathNative()
			: pText->GetText();
	}
}

decString igdeEditPath::AsRelativePath(const decString &path) const{
	if(pBasePath.IsEmpty()){
		DETHROW_INFO(deeInvalidParam, "Base path is empty string");
	}
	
	return pUseGameVFS
		? decPath::RelativePathUnix(path, pBasePath, false).GetPathUnix()
		: decPath::RelativePathNative(path, pBasePath, false).GetPathNative();
}

bool igdeEditPath::IsAbsolutePath() const{
	return IsAbsolutePath(pText->GetText());
}

bool igdeEditPath::IsAbsolutePath(const decString &path) const{
	if(path.IsEmpty()){
		return false;
	}
	
	return pUseGameVFS
		? decPath::IsUnixPathAbsolute(path)
		: decPath::IsNativePathAbsolute(path);
}

bool igdeEditPath::IsRelativePath() const{
	return IsRelativePath(pText->GetText());
}

bool igdeEditPath::IsRelativePath(const decString &path) const{
	if(path.IsEmpty()){
		return false;
	}
	
	return pUseGameVFS
		? !decPath::IsUnixPathAbsolute(path)
		: !decPath::IsNativePathAbsolute(path);
}

bool igdeEditPath::GetEnabled() const{
	return pText->GetEnabled();
}

void igdeEditPath::SetEnabled( bool enabled ){
	pText->SetEnabled( enabled );
	pActionButton->Update();
	pActionButtonMenu->Update();
}

const decString &igdeEditPath::GetDescription() const{
	return pText->GetDescription();
}

void igdeEditPath::SetDescription( const char *description ){
	pText->SetDescription( description );
	pActionButton->Update();
	pActionButtonMenu->Update();
}

void igdeEditPath::SetDefaultPath( const char *path ){
	pDefaultPath = path;
}

bool igdeEditPath::IsPathValid() const{
	return ! pText->GetInvalidValue();
}

void igdeEditPath::ValidatePath(){
	if( pText->GetText().IsEmpty() ){
		pText->SetInvalidValue( false );
		return;
	}
	
	try{
		decPath path;
		if( pUseGameVFS ){
			if( decPath::IsUnixPathAbsolute( pText->GetText() ) ){
				path.SetFromUnix( pText->GetText() );
				
			}else{
				path = decPath::AbsolutePathUnix( pText->GetText(), pBasePath );
			}
			
			pText->SetInvalidValue( ! GetEnvironment().GetFileSystemGame()->CanReadFile( path ) );
			
		}else{
			if( decPath::IsNativePathAbsolute( pText->GetText() ) ){
				path.SetFromNative( pText->GetText() );
				
			}else{
				path = decPath::AbsolutePathNative( pText->GetText(), pBasePath );
			}
			
			decDiskFileReader::Ref::New( new decDiskFileReader( path.GetPathNative() ) );
			pText->SetInvalidValue( false );
		}
		
	}catch( const deException & ){
		pText->SetInvalidValue( true );
	}
}

void igdeEditPath::SetBasePath( const char *path ){
	if( pBasePath == path ){
		return;
	}
	
	pBasePath = path;
	if( pAutoValidatePath ){
		ValidatePath();
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
	
	igdeAction::Ref action;
	
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



void igdeEditPath::AddContextMenuEntries( igdeMenuCascade &contextMenu ){
	igdeUIHelper &helper = GetEnvironment().GetUIHelper();
	
	const int count = pSelectPathActions.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		helper.MenuCommand( contextMenu, ( igdeAction* )pSelectPathActions.GetAt( i ) );
	}
	
	if( ! pBasePath.IsEmpty() ){
		helper.MenuSeparator( contextMenu );
		helper.MenuCommand( contextMenu, new cActionConvertAbsolute( *this ), true );
		helper.MenuCommand( contextMenu, new cActionConvertRelative( *this ), true );
	}
	
	helper.MenuSeparator( contextMenu );
	helper.MenuCommand( contextMenu, new cActionBrowseFile( *this ), true );
}

void igdeEditPath::ToAbsolutePath(){
	if( pBasePath.IsEmpty() ){
		return;
	}
	
	if( pUseGameVFS ){
		SetPath( decPath::AbsolutePathUnix( pText->GetText(), pBasePath ).GetPathUnix() );
		
	}else{
		SetPath( decPath::AbsolutePathNative( pText->GetText(), pBasePath ).GetPathNative() );
	}
}

void igdeEditPath::ToRelativePath(){
	if( pBasePath.IsEmpty() ){
		return;
	}
	
	if( pUseGameVFS ){
		SetPath( decPath::RelativePathUnix( pText->GetText(), pBasePath, false ).GetPathUnix() );
		
	}else{
		SetPath( decPath::RelativePathNative( pText->GetText(), pBasePath, false ).GetPathNative() );
	}
}

void igdeEditPath::BrowsePath(){
	if( pText->GetText().IsEmpty() ){
		return;
	}
	
	decPath path;
	
	if( pUseGameVFS ){
		DEASSERT_NOTNULL( GetEnvironment().GetGameProject() )
		const igdeGameProject &gameProject = *GetEnvironment().GetGameProject();
		const decPath pathData( decPath::CreatePathNative( gameProject.GetDirectoryPath() ) );
		DEASSERT_TRUE( pathData.GetComponentCount() > 0 )
		path = pathData;
		path.AddUnixPath( gameProject.GetPathData() );
		path.AddUnixPath( GetAbsolutePath() );
		
	}else{
		path.SetFromNative( GetAbsolutePath() );
	}
	
	path.RemoveLastComponent();
	
	igdeActionExternOpenReference action;
	action.TakeOver( new igdeActionExternOpen( GetEnvironment(), "", nullptr, "", path.GetPathNative() ) );
	action->OnAction();
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
