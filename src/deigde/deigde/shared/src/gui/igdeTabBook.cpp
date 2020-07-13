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

#include "igdeTabBook.h"
#include "native/toolkit.h"
#include "resources/igdeIcon.h"
#include "resources/igdeFont.h"
#include "resources/igdeFontReference.h"
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
		( ( igdeNativeTabBook* )GetNativeWidget() )->AddHeader( ( const cHeader & )header );
		
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
