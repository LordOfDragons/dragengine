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

#ifdef IGDE_TOOLKIT_FOX

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "foxtoolkit.h"
#include "igdeNativeFoxCommonDialogs.h"
#include "dialog/igdeNativeFoxFileDialog.h"
#include "../../igdeWidget.h"
#include "../../filedialog/igdeFilePattern.h"
#include "../../filedialog/igdeFilePatternList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>



// Class igdeNativeFoxCommonDialogs
/////////////////////////////////////

// Management
///////////////

igdeCommonDialogs::eButton igdeNativeFoxCommonDialogs::Message( igdeWidget *owner,
igdeCommonDialogs::eButtonSet buttons, igdeCommonDialogs::eIcon icon,
const char *title, const char *text ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	int foxButtons;
	
	switch( buttons ){
	case igdeCommonDialogs::ebsOk:
		foxButtons = FX::MBOX_OK;
		break;
		
	case igdeCommonDialogs::ebsOkCancel:
		foxButtons = FX::MBOX_OK_CANCEL;
		break;
		
	case igdeCommonDialogs::ebsYesNo:
		foxButtons = FX::MBOX_YES_NO;
		break;
		
	case igdeCommonDialogs::ebsYesNoCancel:
		foxButtons = FX::MBOX_YES_NO_CANCEL;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	int foxResult;
	
	switch( icon ){
	case igdeCommonDialogs::eiQuestion:
		foxResult = FXMessageBox::question( foxOwner, foxButtons, title, text, "" );
		break;
		
	case igdeCommonDialogs::eiWarning:
		foxResult = FXMessageBox::warning( foxOwner, foxButtons, title, text, "" );
		break;
		
	case igdeCommonDialogs::eiError:
		foxResult = FXMessageBox::error( foxOwner, foxButtons, title, text, "" );
		break;
		
	case igdeCommonDialogs::eiInfo:
	case igdeCommonDialogs::eiNone:
		foxResult = FXMessageBox::information( foxOwner, foxButtons, title, text, "" );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	switch( foxResult ){
	case FX::MBOX_CLICKED_OK:
		return igdeCommonDialogs::ebOk;
		
	case FX::MBOX_CLICKED_CANCEL:
		return igdeCommonDialogs::ebCancel;
		
	case FX::MBOX_CLICKED_YES:
		return igdeCommonDialogs::ebYes;
		
	case FX::MBOX_CLICKED_NO:
		return igdeCommonDialogs::ebNo;
		
	default:
		DETHROW( deeInvalidParam );
	}
}



bool igdeNativeFoxCommonDialogs::GetInteger( igdeWidget *owner, const char *title,
const char *text, int &value ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXInputDialog dialog( foxOwner, title, text, NULL, INPUTDIALOG_INTEGER );
	
	decString initialValue;
	initialValue.Format( "%d", value );
	dialog.setText( initialValue.GetString() );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		value = decString( dialog.getText().text() ).ToInt();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetFloat( igdeWidget *owner, const char *title,
const char *text, float &value ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXInputDialog dialog( foxOwner, title, text, NULL, INPUTDIALOG_REAL );
	
	decString initialValue;
	initialValue.Format( "%g", value );
	dialog.setText( initialValue.GetString() );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		value = decString( dialog.getText().text() ).ToFloat();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetString( igdeWidget *owner, const char *title,
const char *text, decString &value ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXInputDialog dialog( foxOwner, title, text, NULL, INPUTDIALOG_STRING );
	
	dialog.setText( value.GetString() );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		value = dialog.getText().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::SelectString( igdeWidget *owner, const char *title,
const char *text, const decStringList &list, int &selection ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text || list.GetCount() == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXChoiceBox dialog( foxOwner, title, text, NULL, list.Join( "\n" ).GetString() );
	
	//dialog.setCurrentItem( selection );
	// TODO fox does not expose "list" member to set selection. create an own dialog anyways
	
	selection = dialog.execute( PLACEMENT_OWNER );
	return selection != -1;
}



bool igdeNativeFoxCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
const igdeFilePatternList &filePatterns, decString &filename ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXFileDialog dialog( foxOwner, title );
	dialog.setPatternList( igdeUIFoxHelper::FilePatternListToFOX( filePatterns ) );
	dialog.setCurrentPattern( 0 );
	dialog.setFilename( filename.GetString() );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		filename = dialog.getFilename().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePatternList &filePatterns, decString &filename ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxOwner = ( FXComposite* )owner->GetNativeWidget();
	
	igdeNativeFoxFileDialog dialog( *owner, &vfs, foxOwner, title );
	dialog.SetFilePatternList( &filePatterns );
	dialog.SetFilename( filename );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		filename = dialog.GetFilename().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
const igdeFilePatternList &filePatterns, decString &filename ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXFileDialog dialog( foxOwner, title );
	dialog.setPatternList( igdeUIFoxHelper::FilePatternListToFOX( filePatterns ) );
	dialog.setCurrentPattern( 0 );
	dialog.setFilename( filename.GetString() );
	
	if( ! dialog.execute( PLACEMENT_OWNER ) ){
		return false;
	}
	
	filename = dialog.getFilename().text();
	
	// TEMP
	if( true ){
		const igdeFilePattern &pattern = *filePatterns.GetFilePatternAt( dialog.getCurrentPattern() );
		if( ! filename.MatchesPattern( pattern.GetPattern() ) ){
			filename.Append( pattern.GetDefaultExtension() );
		}
	}
	// TEMP
	
	return true;
}

bool igdeNativeFoxCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePatternList &filePatterns, decString &filename ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const foxOwner = ( FXComposite* )owner->GetNativeWidget();
	
	igdeNativeFoxFileDialog dialog( *owner, &vfs, foxOwner, title );
	dialog.SetFilePatternList( &filePatterns );
	dialog.SetFilename( filename );
	
	if( ! dialog.execute( PLACEMENT_OWNER ) ){
		return false;
	}
	
	filename = dialog.GetFilename().text();
	return true;
}

bool igdeNativeFoxCommonDialogs::GetDirectory( igdeWidget *owner, const char *title, decString &dirname ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	
	FXDirDialog dialog( foxOwner, title );
	dialog.setDirectory( dirname.GetString() );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		dirname = dialog.getDirectory().text();
		return true;
		
	}else{
		return false;
	}
}

bool igdeNativeFoxCommonDialogs::GetDirectory( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, decString &dirname ){
	if( ! owner || ! owner->GetNativeWidget() || ! title ){
		DETHROW( deeInvalidParam );
	}
	
	// not supported yet
	DETHROW( deeInvalidAction );
	
	/*
	FXComposite * const foxOwner = ( FXComposite* )owner->GetNativeWidget();
	
	igdeDirDialog dialog( owner->GetEnvironment(), &vfs, foxOwner, title );
	dialog.SetDirectory( dirname );
	
	if( dialog.execute( PLACEMENT_OWNER ) ){
		dirname = dialog.GetDirectory().text();
		return true;
		
	}else{
		return false;
	}
	*/
}

bool igdeNativeFoxCommonDialogs::SelectSystemFont( igdeWidget *owner,
const char *title, igdeFont::sConfiguration &config ){
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	FXFontDialog dialog( foxOwner, title );
	
	FXFontDesc fdesc;
	memset( &fdesc, '\0', sizeof( FXFontDesc ) );
	
	#ifdef OS_W32_VS
	strcpy_s( fdesc.face, sizeof( fdesc.face ), config.name.GetString() );
	#else
	strcpy( fdesc.face, config.name.GetString() );
	#endif

	fdesc.size = ( int )( config.size * 10 + 0.5f );
	fdesc.weight = config.bold ? FXFont::Bold : FXFont::Normal,
	fdesc.slant = config.italic ? FXFont::Italic : FXFont::Straight;
	dialog.setFontDesc( fdesc );
	
	if( ! dialog.execute( PLACEMENT_OWNER ) ){
		return false;
	}
	
	const FXFontDesc &newFDesc = dialog.getFontDesc();
	config.name = newFDesc.face;
	config.size = ( float )newFDesc.size * 0.1f;
	config.bold = newFDesc.weight == FXFont::Bold;
	config.italic = newFDesc.slant == FXFont::Italic;
	return true;
}

#endif
