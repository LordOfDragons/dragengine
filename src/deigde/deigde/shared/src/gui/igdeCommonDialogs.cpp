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

#include "igdeCommonDialogs.h"
#include "igdeWidget.h"
#include "igdeContainer.h"
#include "igdeContainerReference.h"
#include "igdeComboBoxFilter.h"
#include "igdeComboBoxFilterReference.h"
#include "igdeComboBoxFilterReference.h"
#include "igdeUIHelper.h"
#include "dialog/igdeDialog.h"
#include "dialog/igdeDialogReference.h"
#include "dialog/igdeDialogMultilineValue.h"
#include "dialog/igdeDialogMultilineValueReference.h"
#include "filedialog/igdeFilePattern.h"
#include "filedialog/igdeFilePatternList.h"
#include "layout/igdeContainerFlow.h"
#include "native/toolkit.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/logger/deLogger.h>



// Class igdeCommonDialogs
////////////////////////////

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
	return igdeNativeCommonDialogs::Message( owner, buttons, icon, title, text );
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
	Exception(owner, "Application Error", exception);
}

void igdeCommonDialogs::Exception(igdeWidget *owner, const char *title,
const deException &exception){
	DEASSERT_NOTNULL(owner)
	DEASSERT_NOTNULL(title)
	
	// TODO create a dialog to display the exception. this allows to display the exception
	//      trace in a separate list field keeping the output compact compared to using
	//      FXMessageBox::error . this also allows to add buttons to copy the exception to
	//      file bugs directly not requiring to go through the log files
	Error(owner, title, FormatException(exception));
}



bool igdeCommonDialogs::GetInteger( igdeWidget *owner, const char *title, const char *text,
int &value ){
	return igdeNativeCommonDialogs::GetInteger( owner, title, text, value );
}

bool igdeCommonDialogs::GetFloat( igdeWidget *owner, const char *title, const char *text,
float &value ){
	return igdeNativeCommonDialogs::GetFloat( owner, title, text, value );
}

bool igdeCommonDialogs::GetString( igdeWidget *owner, const char *title, const char *text,
decString &value ){
	return igdeNativeCommonDialogs::GetString( owner, title, text, value );
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

bool igdeCommonDialogs::GetString( igdeWidget *owner, const char *title,
const char *text, decString &value, const decStringList &proposals ){
	if( ! owner ){
		DETHROW( deeInvalidParam );
	}
	
	igdeEnvironment &environment = owner->GetEnvironment();
	igdeUIHelper &helper = environment.GetUIHelper();
	igdeDialogReference dialog;
	dialog.TakeOver( new igdeDialog( environment, title ) );
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerFlow( environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 10 ) );
	
	igdeComboBoxFilterReference comboBox;
	helper.Label( content, text );
	helper.ComboBoxFilter( content, 50, 10, true, "Enter value or select from list", comboBox, NULL );
	const int count = proposals.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		comboBox->AddItem( proposals.GetAt( i ) );
	}
	comboBox->StoreFilterItems();
	comboBox->SetText( value );
	
	igdeContainerReference buttonBar;
	dialog->CreateButtonBar( buttonBar, "Accept", "Discard" );
	
	dialog->AddContent( content, buttonBar );
	
	if( dialog->Run( owner ) ){
		value = comboBox->GetText();
		return true;
		
	}else{
		return false;
	}
}

bool igdeCommonDialogs::SelectString( igdeWidget *owner, const char *title,
const char *text, const decStringList &list, int &selection ){
	return igdeNativeCommonDialogs::SelectString( owner, title, text, list, selection );
}



bool igdeCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
const igdeFilePatternList &filePatterns, decString &filename ){
	return igdeNativeCommonDialogs::GetFileOpen( owner, title, filePatterns, filename );
}

bool igdeCommonDialogs::GetFileOpen( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePatternList &filePatterns, decString &filename ){
	return igdeNativeCommonDialogs::GetFileOpen( owner, title, vfs, filePatterns, filename );
}

bool igdeCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
const igdeFilePatternList &filePatterns, decString &filename ){
	return igdeNativeCommonDialogs::GetFileSave( owner, title, filePatterns, filename );
}

bool igdeCommonDialogs::GetFileSave( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, const igdeFilePatternList &filePatterns, decString &filename ){
	return igdeNativeCommonDialogs::GetFileSave( owner, title, vfs, filePatterns, filename );
}

bool igdeCommonDialogs::GetDirectory( igdeWidget *owner, const char *title, decString &dirname ){
	return igdeNativeCommonDialogs::GetDirectory( owner, title, dirname );
}

bool igdeCommonDialogs::GetDirectory( igdeWidget *owner, const char *title,
deVirtualFileSystem &vfs, decString &dirname ){
	return igdeNativeCommonDialogs::GetDirectory( owner, title, vfs, dirname );
}

bool igdeCommonDialogs::SelectSystemFont( igdeWidget *owner, const char *title,
igdeFont::sConfiguration &config ){
	return igdeNativeCommonDialogs::SelectSystemFont( owner, title, config );
}
