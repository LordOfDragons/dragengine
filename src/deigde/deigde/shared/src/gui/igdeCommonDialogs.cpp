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

#include "native/toolkit.h"
#include "igdeCommonDialogs.h"
#include "igdeWidget.h"
#include "dialog/igdeDialogMultilineValue.h"
#include "dialog/igdeDialogMultilineValueReference.h"
#include "filedialog/igdeFilePattern.h"
#include "filedialog/igdeFilePatternList.h"
#include "native/fox/dialog/igdeNativeFoxFileDialog.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>



// Class igdeWidget
/////////////////////

// Constructor, destructor
////////////////////////////

igdeCommonDialogs::igdeCommonDialogs( igdeEditorModule &editorModule ) :
pEditorModule( editorModule ){
}

igdeCommonDialogs::~igdeCommonDialogs(){
}



// Management
///////////////

void igdeCommonDialogs::Information( igdeWidget *owner, const char *title, const char *text ){
	Message( owner, ebsOk, eiInfo, title, text );
}

void igdeCommonDialogs::InformationFormat( igdeWidget *owner, const char *title,
const char *textFormat, ... ){
	decString text;
	
	va_list list;
	va_start( list, textFormat );
	text.FormatUsing( textFormat, list );
	va_end( list );
	
	Message( owner, ebsOk, eiInfo, title, text );
}

igdeCommonDialogs::eButton igdeCommonDialogs::Question( igdeWidget *owner,
eButtonSet buttons, const char *title, const char *text ){
	return Message( owner, buttons, eiQuestion, title, text );
}

igdeCommonDialogs::eButton igdeCommonDialogs::QuestionFormat( igdeWidget *owner,
eButtonSet buttons, const char *title, const char *textFormat, ... ){
	decString text;
	
	va_list list;
	va_start( list, textFormat );
	text.FormatUsing( textFormat, list );
	va_end( list );
	
	return Message( owner, buttons, eiQuestion, title, text );
}

void igdeCommonDialogs::Warning( igdeWidget *owner, const char *title, const char *text ){
	Message( owner, ebsOk, eiWarning, title, text );
}

void igdeCommonDialogs::WarningFormat( igdeWidget *owner, const char *title,
const char *textFormat, ... ){
	decString text;
	
	va_list list;
	va_start( list, textFormat );
	text.FormatUsing( textFormat, list );
	va_end( list );
	
	Message( owner, ebsOk, eiWarning, title, text );
}

void igdeCommonDialogs::Error( igdeWidget *owner, const char *title, const char *text ){
	Message( owner, ebsOk, eiError, title, text );
}

void igdeCommonDialogs::ErrorFormat( igdeWidget *owner, const char *title,
const char *textFormat, ... ){
	decString text;
	
	va_list list;
	va_start( list, textFormat );
	text.FormatUsing( textFormat, list );
	va_end( list );
	
	Message( owner, ebsOk, eiError, title, text );
}



igdeCommonDialogs::eButton igdeCommonDialogs::Message( igdeWidget *owner,
eButtonSet buttons, eIcon icon, const char *title, const char *text ){
	if( ! owner || ! owner->GetNativeWidget() || ! title || ! text ){
		DETHROW( deeInvalidParam );
	}
	
	int foxButtons;
	
	switch( buttons ){
	case ebsOk:
		foxButtons = FX::MBOX_OK;
		break;
		
	case ebsOkCancel:
		foxButtons = FX::MBOX_OK_CANCEL;
		break;
		
	case ebsYesNo:
		foxButtons = FX::MBOX_YES_NO;
		break;
		
	case ebsYesNoCancel:
		foxButtons = FX::MBOX_YES_NO_CANCEL;
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	int foxResult;
	
	switch( icon ){
	case eiQuestion:
		foxResult = FXMessageBox::question( foxOwner, foxButtons, title, text, "" );
		break;
		
	case eiWarning:
		foxResult = FXMessageBox::warning( foxOwner, foxButtons, title, text, "" );
		break;
		
	case eiError:
		foxResult = FXMessageBox::error( foxOwner, foxButtons, title, text, "" );
		break;
		
	case eiInfo:
	case eiNone:
		foxResult = FXMessageBox::information( foxOwner, foxButtons, title, text, "" );
		break;
		
	default:
		DETHROW( deeInvalidParam );
	}
	
	switch( foxResult ){
	case FX::MBOX_CLICKED_OK:
		return ebOk;
		
	case FX::MBOX_CLICKED_CANCEL:
		return ebCancel;
		
	case FX::MBOX_CLICKED_YES:
		return ebYes;
		
	case FX::MBOX_CLICKED_NO:
		return ebNo;
		
	default:
		DETHROW( deeInvalidParam );
	}
}

igdeCommonDialogs::eButton igdeCommonDialogs::MessageFormat( igdeWidget *owner,
eButtonSet buttons, eIcon icon, const char *title, const char *textFormat, ... ){
	decString text;
	
	va_list list;
	va_start( list, textFormat );
	text.FormatUsing( textFormat, list );
	va_end( list );
	
	return Message( owner, buttons, icon, title, text );
}



decString igdeCommonDialogs::FormatException( const deException &exception ){
	decString message;
	
	message.AppendFormat( "Exception: %s\n", exception.GetName().GetString() );
	message.AppendFormat( "Description: %s\n", exception.GetDescription().GetString() );
	message.AppendFormat( "Source File: %s\n", exception.GetFile().GetString() );
	message.AppendFormat( "Source Line: %d\n", exception.GetLine() );
	
	message.Append( "Backtrace:\n" );
	const decStringList &backtrace = exception.GetBacktrace();
	const int btcount = backtrace.GetCount();
	int i;
	for( i=0; i<btcount; i++ ){
		message.AppendFormat( "%d) %s\n", i + 1, backtrace.GetAt( i ).GetString() );
	}
	
	return message;
}

void igdeCommonDialogs::Exception( igdeWidget *owner, const deException &exception ){
	if( ! owner ){
		DETHROW( deeInvalidParam );
	}
	
	// TODO create a dialog to display the exception. this allows to display the exception
	//      trace in a separate list field keeping the output compact compared to using
	//      FXMessageBox::error . this also allows to add buttons to copy the exception to
	//      file bugs directly not requiring to go through the log files
	Error( owner, "Application Error", FormatException( exception ) );
}



bool igdeCommonDialogs::GetInteger( igdeWidget *owner, const char *title, const char *text,
int &value ){
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

bool igdeCommonDialogs::GetFloat( igdeWidget *owner, const char *title, const char *text,
float &value ){
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

bool igdeCommonDialogs::GetString( igdeWidget *owner, const char *title, const char *text,
decString &value ){
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

bool igdeCommonDialogs::GetMultilineString( igdeWidget *owner, const char *title,
const char *text, decString &value ){
	if( ! owner ){
		DETHROW( deeInvalidParam );
	}
	
	igdeDialogMultilineValueReference dialog;
	
	dialog.TakeOver( new igdeDialogMultilineValue( owner->GetEnvironment(), title, text ) );
	dialog->SetValue( value );
	if( dialog->Run( owner ) ){
		value = dialog->GetValue();
		return true;
		
	}else{
		return false;
	}
}

bool igdeCommonDialogs::SelectString( igdeWidget *owner, const char *title,
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



bool igdeCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
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

bool igdeCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
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

bool igdeCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
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

bool igdeCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
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

bool igdeCommonDialogs::GetDirectory( igdeWidget *owner, const char *title, decString &dirname ){
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

bool igdeCommonDialogs::GetDirectory( igdeWidget *owner, const char *title,
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

bool igdeCommonDialogs::SelectSystemFont( igdeWidget *owner, const char *title, igdeFont::sConfiguration &config ){
	FXWindow * const foxOwner = ( FXWindow* )owner->GetNativeWidget();
	FXFontDialog dialog( foxOwner, title );
	
	FXFontDesc fdesc;
	memset( &fdesc, '\0', sizeof( FXFontDesc ) );
	strcpy( fdesc.face, config.name.GetString() );
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
