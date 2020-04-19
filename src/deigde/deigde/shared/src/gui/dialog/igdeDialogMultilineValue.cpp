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

#include "igdeDialogMultilineValue.h"
#include "../igdeLabel.h"
#include "../igdeTextArea.h"
#include "../igdeUIHelper.h"
#include "../igdeContainerReference.h"
#include "../layout/igdeContainerFlow.h"

#include <dragengine/common/exceptions.h>



// Class igdeDialogMultilineValue
///////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeDialogMultilineValue::igdeDialogMultilineValue( igdeEnvironment &environment,
	const char *title, const char *message, int rows, int columns ) :
igdeDialog( environment, title )
{
	igdeUIHelper &helper = environment.GetUIHelper();
	
	igdeContainerReference content;
	content.TakeOver( new igdeContainerFlow( environment, igdeContainerFlow::eaY, igdeContainerFlow::esLast, 10 ) );
	
	helper.Label( content, message );
	helper.EditString( content, "", pEditValue, columns, rows, NULL );
	
	igdeContainerReference buttonBar;
	CreateButtonBar( buttonBar, "Accept", "Discard" );
	
	AddContent( content, buttonBar );
}

igdeDialogMultilineValue::~igdeDialogMultilineValue(){
}



// Management
///////////////

const decString &igdeDialogMultilineValue::GetValue() const{
	return pEditValue->GetText();
}

void igdeDialogMultilineValue::SetValue( const char *value ){
	pEditValue->SetText( value );
}
