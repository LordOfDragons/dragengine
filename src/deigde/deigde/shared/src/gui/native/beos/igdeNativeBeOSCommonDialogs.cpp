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

#ifdef IGDE_TOOLKIT_BEOS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "beostoolkit.h"
#include "igdeNativeBeOSCommonDialogs.h"
// #include "dialog/igdeNativeBeOSFileDialog.h"
#include "../../igdeWidget.h"
#include "../../filedialog/igdeFilePattern.h"
#include "../../filedialog/igdeFilePatternList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeBeOSCommonDialogs::FileFilter
//////////////////////////////////////////////////

igdeNativeBeOSCommonDialogs::FileFilter::FileFilter( const igdeFilePatternList &list ) :
pList( list ){
}

bool igdeNativeBeOSCommonDialogs::FileFilter::Filter( const entry_ref *ref,
BNode *node, struct stat_beos *stat, const char *mimeType ){
	if( node->IsDirectory() ){
		return true;
	}
	
	const int count = pList.GetFilePatternCount();
	if( count == 0 ){
		return true;
	}
	
	int i;
	for( i=0; i<count; i++ ){
		if( decString::StringMatchesPattern( ref->name, pList.GetFilePatternAt( i )->GetPattern() ) ){
			return true;
		}
	}
	
	return false;
}



// Class igdeNativeBeOSCommonDialogs::OpenSaveDialogHandler
/////////////////////////////////////////////////////////////

igdeNativeBeOSCommonDialogs::OpenSaveDialogHandler::OpenSaveDialogHandler() :
pButton( igdeCommonDialogs::ebCancel ){
}

void igdeNativeBeOSCommonDialogs::OpenSaveDialogHandler::MessageReceived( BMessage *message ){
	switch( message->what ){
	case B_REFS_RECEIVED:{
		entry_ref ref;
		if( message->FindRef( "refs" /*"be:refs"*/, &ref ) != B_OK ){
			break;
		}
		
		BPath path( &ref );
		if( path.InitCheck() != B_OK ){
			break;
		}
		
		pFilename = path.Path();
		pButton = igdeCommonDialogs::ebOk;
		}break;
		
	case B_SAVE_REQUESTED:{
		entry_ref ref;
		if( message->FindRef( "directory", &ref ) != B_OK ){
			break;
		}
		
		BPath path( &ref );
		if( path.InitCheck() != B_OK ){
			break;
		}
		
		BString name;
		if( message->FindString( "name", &name ) != B_OK ){
			break;
		}
		
		if( path.Append( name.String() ) != B_OK ){
			break;
		}
		
		pFilename = path.Path();
		pButton = igdeCommonDialogs::ebOk;
		}break;
		
	case B_CANCEL:
		pButton = igdeCommonDialogs::ebCancel;
		break;
		
	default:
		break;
	}
}



// Class igdeNativeBeOSCommonDialogs
//////////////////////////////////////

// Management
///////////////

igdeCommonDialogs::eButton igdeNativeBeOSCommonDialogs::Message( igdeWidget *owner,
igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon icon,
const char *title, const char *text ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	const char *button1 = NULL;
	const char *button2 = NULL;
	const char *button3 = NULL;
	alert_type alertType = B_EMPTY_ALERT;
	
	switch( buttons ){
	case igdeCommonDialogs::ebsOk:
		button1 = "OK";
		break;
		
	case igdeCommonDialogs::ebsOkCancel:
		button1 = "Cancel";
		button2 = "OK";
		break;
		
	case igdeCommonDialogs::ebsYesNo:
		button1 = "No";
		button2 = "Yes";
		break;
		
	case igdeCommonDialogs::ebsYesNoCancel:
		button1 = "Cancel";
		button2 = "No";
		button3 = "Yes";
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	switch( icon ){
	case igdeCommonDialogs::eiQuestion:
		alertType = B_IDEA_ALERT;
		break;
		
	case igdeCommonDialogs::eiWarning:
		alertType = B_WARNING_ALERT;
		break;
		
	case igdeCommonDialogs::eiError:
		alertType = B_STOP_ALERT;
		break;
		
	case igdeCommonDialogs::eiInfo:
	case igdeCommonDialogs::eiNone:
		alertType = B_INFO_ALERT;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	BAlert * const alert = new BAlert( title, text, button1, button2, button3,
		B_WIDTH_AS_USUAL, B_OFFSET_SPACING, alertType );
	alert->SetShortcut( 0, B_ESCAPE );
	const int32 resultButton = alert->Go();
	
	switch( buttons ){
	case igdeCommonDialogs::ebsOk:
		return igdeCommonDialogs::ebOk;
		
	case igdeCommonDialogs::ebsOkCancel:
		switch( resultButton ){
		case 0:
			return igdeCommonDialogs::ebCancel;
			
		default:
			return igdeCommonDialogs::ebOk;
		}
		
	case igdeCommonDialogs::ebsYesNo:
		switch( resultButton ){
		case 0:
			return igdeCommonDialogs::ebNo;
			
		default:
			return igdeCommonDialogs::ebYes;
		}
		
	case igdeCommonDialogs::ebsYesNoCancel:
		switch( resultButton ){
		case 0:
			return igdeCommonDialogs::ebCancel;
			
		case 1:
			return igdeCommonDialogs::ebNo;
			
		default:
			return igdeCommonDialogs::ebYes;
		}
		
	default:
		DETHROW( deeInvalidParam );
	}
}



bool igdeNativeBeOSCommonDialogs::GetInteger( igdeWidget *owner, const char *title,
const char *text, int &value ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	decString initialValue;
	initialValue.Format( "%d", value );
	
	BAlert * const alert = new BAlert( title, initialValue, "Cancel", "OK",
		NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_EMPTY_ALERT );
	//alert->TextView()->SetLabel( text );
	alert->TextView()->MakeEditable( true );
	alert->SetShortcut( 0, B_ESCAPE );
	
	if( alert->Go() == 0 ){
		return false;
	}
	
	value = decString( alert->TextView()->Text() ).ToInt();
	return true;
}

bool igdeNativeBeOSCommonDialogs::GetFloat( igdeWidget *owner, const char *title,
const char *text, float &value ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	decString initialValue;
	initialValue.Format( "%g", value );
	
	BAlert * const alert = new BAlert( title, initialValue, "Cancel", "OK",
		NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_EMPTY_ALERT );
	//alert->TextView()->SetLabel( text );
	alert->TextView()->MakeEditable( true );
	alert->SetShortcut( 0, B_ESCAPE );
	
	if( alert->Go() == 0 ){
		return false;
	}
	
	value = decString( alert->TextView()->Text() ).ToFloat();
	return true;
}

bool igdeNativeBeOSCommonDialogs::GetString( igdeWidget *owner, const char *title,
const char *text, decString &value ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	BAlert * const alert = new BAlert( title, value, "Cancel", "OK",
		NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_EMPTY_ALERT );
	//alert->TextView()->SetLabel( text );
	alert->TextView()->MakeEditable( true );
	alert->SetShortcut( 0, B_ESCAPE );
	
	if( alert->Go() == 0 ){
		return false;
	}
	
	value = alert->TextView()->Text();
	return true;
}

bool igdeNativeBeOSCommonDialogs::SelectString( igdeWidget *owner, const char *title,
const char *text, const decStringList &list, int &selection ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text || list.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	BAlert * const alert = new BAlert( title, text, "Cancel", "OK",
		NULL, B_WIDTH_AS_USUAL, B_OFFSET_SPACING, B_EMPTY_ALERT );
	
	BListView * const listView = new BListView( "" );
	const int count = list.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		listView->AddItem( new BStringItem( list.GetAt( i ) ), i );
	}
	listView->Select( selection );
	
	alert->TextView()->Parent()->AddChild( listView, alert->TextView()->NextSibling() );
	alert->SetShortcut( 0, B_ESCAPE );
	
	if( alert->Go() == 0 ){
		return false;
	}
	
	selection = listView->CurrentSelection();
	return true;
}



bool igdeNativeBeOSCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
const igdeFilePatternList &filePatterns, decString &filename ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FileFilter filter( filePatterns );
	OpenSaveDialogHandler handler;
	BMessenger messenger( &handler );
	
	BFilePanel panel( B_OPEN_PANEL, &messenger, NULL, B_FILE_NODE, false, NULL, &filter, true, true );
	
	decPath path( decPath::CreatePathNative( filename ) );
	if( path.GetComponentCount() > 1 ){
		const decString fileTitle( path.GetLastComponent() );
		path.RemoveLastComponent();
		panel.SetPanelDirectory( path.GetPathNative() );
		panel.SetSaveText( fileTitle );
		
	}else if( path.GetComponentCount() > 0 ){
		panel.SetSaveText( path.GetComponentAt( 0 ) );
	}
	
	panel.Show();
	
	if( handler.GetButton() == igdeCommonDialogs::ebOk ){
		filename = handler.GetFilename();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeBeOSCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePatternList &filePatterns, decString &filename ){
	// TODO
	return false;
}

bool igdeNativeBeOSCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
const igdeFilePatternList &filePatterns, decString &filename ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FileFilter filter( filePatterns );
	OpenSaveDialogHandler handler;
	BMessenger messenger( &handler );
	
	BFilePanel panel( B_SAVE_PANEL, &messenger, NULL, B_FILE_NODE, false, NULL, &filter, true, true );
	
	decPath path( decPath::CreatePathNative( filename ) );
	if( path.GetComponentCount() > 1 ){
		const decString fileTitle( path.GetLastComponent() );
		path.RemoveLastComponent();
		panel.SetPanelDirectory( path.GetPathNative() );
		panel.SetSaveText( fileTitle );
		
	}else if( path.GetComponentCount() > 0 ){
		panel.SetSaveText( path.GetComponentAt( 0 ) );
	}
	
	panel.Show();
	
	if( handler.GetButton() != igdeCommonDialogs::ebOk ){
		return false;
	}
	
	filename = handler.GetFilename();
	
// 	const igdeFilePattern &pattern = *filePatterns.GetFilePatternAt( dialog.getCurrentPattern() );
// 	if( ! filename.MatchesPattern( pattern.GetPattern() ) ){
// 		filename.Append( pattern.GetDefaultExtension() );
// 	}
	
	return true;
}

bool igdeNativeBeOSCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePatternList &filePatterns, decString &filename ){
	// TODO
	return false;
}

bool igdeNativeBeOSCommonDialogs::GetDirectory( igdeWidget *owner, const char *title, decString &dirname ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	OpenSaveDialogHandler handler;
	BMessenger messenger( &handler );
	
	BFilePanel panel( B_OPEN_PANEL, &messenger, NULL, B_DIRECTORY_NODE, false, NULL, NULL, true, true );
	panel.SetPanelDirectory( dirname );
	
	panel.Show();
	
	if( handler.GetButton() == igdeCommonDialogs::ebOk ){
		dirname = handler.GetFilename();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeBeOSCommonDialogs::GetDirectory( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, decString &dirname ){
	// TODO
	return false;
}

bool igdeNativeBeOSCommonDialogs::SelectSystemFont( igdeWidget *owner,
const char *title, igdeFont::sConfiguration &config ){
// 	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
// 	FXFontDialog dialog( foxOwner, title );
// 	
// 	FXFontDesc fdesc;
// 	memset( &fdesc, '\0', sizeof( FXFontDesc ) );
// 	strcpy( fdesc.face, config.name.GetString() );
// 	fdesc.size = ( int )( config.size * 10 + 0.5f );
// 	fdesc.weight = config.bold ? FXFont::Bold : FXFont::Normal,
// 	fdesc.slant = config.italic ? FXFont::Italic : FXFont::Straight;
// 	dialog.setFontDesc( fdesc );
// 	
// 	if( ! dialog.execute( PLACEMENT_OWNER ) ){
// 		return false;
// 	}
// 	
// 	const FXFontDesc &newFDesc = dialog.getFontDesc();
// 	config.name = newFDesc.face;
// 	config.size = ( float )newFDesc.size * 0.1f;
// 	config.bold = newFDesc.weight == FXFont::Bold;
// 	config.italic = newFDesc.slant == FXFont::Italic;
// 	return true;
	
	// TODO
	return false;
}

#endif
