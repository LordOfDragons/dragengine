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

#include "igdeTabBook.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFont.h"
#include "theme/igdeGuiTheme.h"
#include "theme/propertyNames.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeTabBook::cHeader
///////////////////////////////

igdeTabBook::cHeader::cHeader( const char *ptext, igdeIcon *picon, const char *pdescription ) :
text( ptext ),
description( pdescription ),
icon( picon ){
}

igdeTabBook::cHeader::~cHeader(){
}



// Class igdeTabBook
//////////////////////

// Constructor, destructor
////////////////////////////

igdeTabBook::igdeTabBook( igdeEnvironment &environment ) :
igdeContainer( environment ),
pActivePanel( -1 ){
}

igdeTabBook::~igdeTabBook(){
	DestroyNativeWidget();
}



// Management
///////////////

void igdeTabBook::SetActivePanel( int index ){
	if( index == pActivePanel ){
		return;
	}
	
	if( index == -1 ){
		if( GetChildCount() > 0 ){
			DETHROW( deeInvalidParam );
		}
		
	}else if( index < 0 || index >= GetChildCount() ){
		DETHROW( deeInvalidParam );
	}
	
	pActivePanel = index;
	
	if( GetNativeWidget() ){
		( ( igdeNativeTabBook* )GetNativeWidget() )->ChangePanel( index );
	}
}



void igdeTabBook::AddChild( igdeWidget *child, const char *text ){
	AddChild( child, text, NULL, "" );
}

void igdeTabBook::AddChild( igdeWidget *child, const char *text, igdeIcon *icon ){
	AddChild( child, text, icon, "" );
}

void igdeTabBook::AddChild( igdeWidget *child, const char *text, igdeIcon *icon,
const char *description ){
	if( ! child || child->GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	igdeContainer::AddChild( child );
	
	deObjectReference header;
	try{
		header.TakeOver( new cHeader( text, icon, description ) );
		pHeaders.Add( header );
		
	}catch( const deException & ){
		igdeContainer::RemoveChild( child );
		throw;
	}
	
	if( ! GetNativeWidget() ){
		return;
	}
	
	try{
		((igdeNativeTabBook*)GetNativeWidget())->AddHeader((const cHeader &)*header);
		
	}catch( const deException & ){
		pHeaders.RemoveFrom( pHeaders.GetCount() - 1 );
		igdeContainer::RemoveChild( child );
		throw;
	}
}

void igdeTabBook::AddChild( igdeWidget *child ){
	DETHROW( deeInvalidParam );
}

void igdeTabBook::RemoveChild( igdeWidget *child ){
	const int index = IndexOfChild( child );
	if( index == -1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( GetNativeWidget() ){
		( ( igdeNativeTabBook* )GetNativeWidget() )->RemoveHeader( index );
	}
	pHeaders.RemoveFrom( index );
	
	igdeContainer::RemoveChild( child );
}

void igdeTabBook::RemoveAllChildren(){
	if( GetNativeWidget() ){
		( ( igdeNativeTabBook* )GetNativeWidget() )->RemoveAllHeaders();
	}
	pHeaders.RemoveAll();
	
	igdeContainer::RemoveAllChildren();
}



void igdeTabBook::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	igdeNativeTabBook * const native = igdeNativeTabBook::CreateNativeWidget( *this );
	SetNativeWidget( native );
	native->PostCreateNativeWidget();
	
	CreateChildWidgetNativeWidgets();
	
	native->ChangePanel( pActivePanel );
}

void igdeTabBook::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	( ( igdeNativeTabBook* )GetNativeWidget() )->DestroyNativeWidget();
	DropNativeWidget();
}

void *igdeTabBook::GetNativeContainer() const{
	if( ! GetNativeWidget() ){
		return NULL;
	}
	return ( ( igdeNativeTabBook* )GetNativeWidget() )->GetNativeContainer();
}



void igdeTabBook::CreateChildWidgetNativeWidgets(){
	igdeNativeTabBook * const native = ( igdeNativeTabBook* )GetNativeWidget();
	if( ! native ){
		DETHROW( deeInvalidParam );
	}
	
	igdeContainer::CreateChildWidgetNativeWidgets();
	
	const int count = pHeaders.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		native->AddHeader( *( ( cHeader* )pHeaders.GetAt( i ) ) );
	}
}
