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

#include "igdeActionContextMenu.h"
#include "../igdeContainer.h"
#include "../igdeWidget.h"
#include "../igdeUIHelper.h"
#include "../menu/igdeMenuCascade.h"

#include <dragengine/common/exceptions.h>



// Class igdeActionContextMenu
////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeActionContextMenu::igdeActionContextMenu( const char *text, igdeIcon *icon,
	const char *description ) :
igdeAction( text, icon, description ){
}

igdeActionContextMenu::igdeActionContextMenu( const char *text, igdeIcon *icon,
	const char *description, deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey ) :
igdeAction( text, icon, description, mnemonic, hotKey ){
}

igdeActionContextMenu::igdeActionContextMenu( igdeWidget *widget,
	const char *text, igdeIcon *icon, const char *description ) :
igdeAction( text, icon, description ),
pWidget( widget ){
}

igdeActionContextMenu::igdeActionContextMenu( igdeWidget *widget,
	const char *text, igdeIcon *icon, const char *description,
	deInputEvent::eKeyCodes mnemonic, const igdeHotKey &hotKey ) :
igdeAction( text, icon, description, mnemonic, hotKey ),
pWidget( widget ){
}

igdeActionContextMenu::~igdeActionContextMenu(){
}



// Management
///////////////

void igdeActionContextMenu::SetWidget( igdeWidget *widget ){
	pWidget = widget;
}



void igdeActionContextMenu::OnAction(){
	if( ! pWidget || ! pWidget->GetVisible() ){
		return;
	}
	
	igdeMenuCascade::Ref menu(igdeMenuCascade::Ref::New( new igdeMenuCascade( pWidget->GetEnvironment() ) ));
	
	AddContextMenuEntries( menu );
	
	if( menu->GetChildCount() != 0 ){
		menu->PopupBottom( pWidget );
	}
}

void igdeActionContextMenu::AddContextMenuEntries( igdeMenuCascade& ){
}
