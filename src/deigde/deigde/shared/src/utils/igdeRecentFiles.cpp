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

#include "igdeRecentFiles.h"
#include "../environment/igdeEnvironment.h"
#include "../gui/igdeEditorWindow.h"
#include "../gui/igdeUIHelper.h"
#include "../gui/event/igdeAction.h"
#include "../gui/event/igdeActionReference.h"
#include "../gui/menu/igdeMenuCascade.h"

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/xmlparser/decXmlAttValue.h>
#include <dragengine/common/xmlparser/decXmlElementTag.h>
#include <dragengine/common/xmlparser/decXmlCharacterData.h>
#include <dragengine/common/xmlparser/decXmlWriter.h>



// Events
///////////

namespace {

class igdeRecentFiles_ActionOpenFile : public igdeAction{
	igdeRecentFiles &pRecentFiles;
	decString pFilename;
	
public:
	igdeRecentFiles_ActionOpenFile( igdeRecentFiles &recentFiles, const char *filename ) :
	pRecentFiles( recentFiles ), pFilename( filename ){
		/*
		decPath path;
		
		if( recentFiles.GetUnixPath() ){
			path.SetFromUnix( filename );
			
		}else{
			path.SetFromNative( filename );
		}
		
		SetText( path.GetLastComponent() );
		*/
		
		SetText( filename );
		SetDescription( decString( "Load " ) + filename );
	}
	
	virtual void OnAction(){
		igdeActionReference guard( this );
		pRecentFiles.OpenFile( pFilename );
	}
};

class igdeRecentFiles_ActionClear : public igdeAction{
	igdeRecentFiles &pRecentFiles;
	
public:
	igdeRecentFiles_ActionClear( igdeRecentFiles &recentFiles ) : igdeAction( "Clear List",
	recentFiles.GetEnvironment().GetStockIcon( igdeEnvironment::esiDelete ),
	"Clear List" ), pRecentFiles( recentFiles ){
	}
	
	virtual void OnAction(){
		pRecentFiles.RemoveAllFiles();
	}
	
	virtual void Update(){
		SetEnabled( pRecentFiles.GetFiles().GetCount() > 0 );
	}
};

}


// Class igdeRecentFiles
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

igdeRecentFiles::igdeRecentFiles( igdeEnvironment &environment, bool unixPath, int size ) :
pEnvironment( environment ),
pEditorWindow( NULL ),
pSize( 0 ),
pUnixPath( unixPath )
{
	SetSize( size );
}

igdeRecentFiles::igdeRecentFiles( igdeEditorWindow &editorWindow, bool unixPath, int size ) :
pEnvironment( editorWindow.GetEnvironment() ),
pEditorWindow( &editorWindow ),
pSize( 0 ),
pUnixPath( unixPath )
{
	SetSize( size );
}

igdeRecentFiles::~igdeRecentFiles(){
}



// Management
///////////////

void igdeRecentFiles::SetSize( int size ){
	if( size < 0 ){
		DETHROW( deeInvalidParam );
	}
	if( size == pSize ){
		return;
	}
	
	pSize = size;
	EnsureSize();
	UpdateMenu();
}

void igdeRecentFiles::SetFiles( const decStringList &files ){
	if( files == pFiles ){
		return;
	}
	
	pFiles = files;
	EnsureSize();
	UpdateMenu();
	FilesChanged();
}

void igdeRecentFiles::SetMenu( igdeMenuCascade *menu ){
	if( menu == pMenu ){
		return;
	}
	
	if( pMenu ){
		pMenu->RemoveAllChildren();
	}
	
	pMenu = menu;
	
	UpdateMenu();
}

void igdeRecentFiles::AddFile( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	if( pUnixPath ){
		if( decPath::CreatePathUnix( filename ).GetComponentCount() == 0 ){
			DETHROW( deeInvalidParam );
		}
		
	}else{
		if( decPath::CreatePathNative( filename ).GetComponentCount() == 0 ){
			DETHROW( deeInvalidParam );
		}
	}
	
	const int index = pFiles.IndexOf( filename );
	if( index == 0 ){
		return;
	}
	
	if( index != -1 ){
		pFiles.RemoveFrom( index );
	}
	pFiles.InsertAt( filename, 0 );
	EnsureSize();
	UpdateMenu();
	FilesChanged();
	
	// add to environment recent editor files if attached to an editor window
	if( pEditorWindow ){
		pEnvironment.AddRecentEditorFile( filename );
	}
}

void igdeRecentFiles::RemoveAllFiles(){
	if( pFiles.GetCount() == 0 ){
		return;
	}
	
	pFiles.RemoveAll();
	UpdateMenu();
	FilesChanged();
}

void igdeRecentFiles::ReadFromXml( const decXmlElementTag &root ){
	const int count = root.GetElementCount();
	int i;
	
	pFiles.RemoveAll();
	
	const decXmlAttValue * const attributeSize = root.FindAttribute( "size" );
	if( attributeSize ){
		SetSize( attributeSize->GetValue().ToInt() );
	}
	
	for( i=0; i<count; i++ ){
		const decXmlElementTag * const tag = root.GetElementIfTag( i );
		if( ! tag ){
			continue;
		}
		
		if( tag->GetName() == "filename" ){
			const decXmlCharacterData * const cdata = tag->GetFirstData();
			if( ! cdata ){
				DETHROW( deeInvalidParam );
			}
			pFiles.Add( cdata->GetData() );
			
		}else{
			DETHROW( deeInvalidParam );
		}
	}
	
	EnsureSize();
	UpdateMenu();
}

void igdeRecentFiles::WriteToXml( decXmlWriter &writer, const char *tagName ) const{
	const int count = pFiles.GetCount();
	int i;
	
	writer.WriteOpeningTagStart( tagName );
	writer.WriteAttributeInt( "size", pSize );
	writer.WriteOpeningTagEnd();
	
	for( i=0; i<count; i++ ){
		writer.WriteDataTagString( "filename", pFiles.GetAt( i ) );
	}
	
	writer.WriteClosingTag( tagName );
}



void igdeRecentFiles::OpenFile( const char *filename ){
	if( ! filename ){
		DETHROW( deeInvalidParam );
	}
	
	if( pEditorWindow ){
		pEditorWindow->LoadDocument( filename );
	}
}

void igdeRecentFiles::UpdateMenu(){
	if( ! pMenu ){
		return;
	}
	
	pMenu->RemoveAllChildren();
	
	igdeUIHelper &helper = pMenu->GetEnvironment().GetUIHelper();
	const int count = pFiles.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		helper.MenuCommand( pMenu, new igdeRecentFiles_ActionOpenFile( *this, pFiles.GetAt( i ) ), true );
	}
	
	helper.MenuSeparator( pMenu );
	helper.MenuCommand( pMenu, new igdeRecentFiles_ActionClear( *this ), true );
}

void igdeRecentFiles::FilesChanged(){
	if( pEditorWindow ){
		pEditorWindow->RecentFilesChanged();
	}
}



// Protected Functions
////////////////////////

void igdeRecentFiles::EnsureSize(){
	while( pSize < pFiles.GetCount() ){
		pFiles.RemoveFrom( pFiles.GetCount() - 1 );
	}
}
