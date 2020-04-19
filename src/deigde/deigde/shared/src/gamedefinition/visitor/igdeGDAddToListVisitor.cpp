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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "igdeGDAddToListVisitor.h"
#include "../class/igdeGDClass.h"
#include "../preview/igdeGDPreviewManager.h"
#include "../skin/igdeGDSkin.h"
#include "../sky/igdeGDSky.h"
#include "../../environment/igdeEnvironment.h"
#include "../../gui/igdeIconListBox.h"
#include "../../gui/browse/igdeBrowseItemGDPreviewListener.h"
#include "../../gui/model/igdeListItem.h"
#include "../../gui/model/igdeListItemReference.h"
#include "../../gui/resources/igdeIcon.h"
#include "../../gui/resources/igdeIconReference.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class igdeGDAddToListVisitor
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDAddToListVisitor::igdeGDAddToListVisitor( igdeEnvironment &environment,
	igdeIconListBox *iconListBox, int size ) :
pEnvironment( environment ),
pIconListBox( iconListBox ),
pSize( size ){
}

igdeGDAddToListVisitor::~igdeGDAddToListVisitor(){
}



// Management
///////////////

void igdeGDAddToListVisitor::AddItemToList( const char *caption, deImage &image, void *userPointer ){
	igdeListItemReference item;
	AddItemToList( item, caption, image, userPointer );
}

void igdeGDAddToListVisitor::AddItemToList( igdeListItemReference &item,
const char *caption, deImage &image, void *userPointer ){
	if( ! caption ){
		DETHROW( deeInvalidParam );
	}
	
	/*
	const int maxCharPerLine = pSize / 4;
	const char *lastBlank = NULL;
	const char *line = caption;
	decString text;
	
	while( *caption ){
		if( isblank( *caption ) ){
			lastBlank = caption;
		}
		
		if( ( int )( caption - line ) >= maxCharPerLine ){
			if( ! text.IsEmpty() ){
				text.AppendCharacter( '\n' );
			}
			
			decString appendText( line );
			
			if( lastBlank ){
				appendText[ ( int )( lastBlank - line ) ] = '\0';
				line = lastBlank + 1;
				
			}else{
				appendText[ ( int )( caption - line ) ] = '\0';
				line = caption;
			}
			
			text.Append( appendText );
			
			// right now more than 1 line can not be drawn. cut it off
			text.Append( "..." );
			while( *caption ){
				caption++;
			}
			line = caption;
			break;
		}
		
		caption++;
	}
	
	if( *line ){
		if( ! text.IsEmpty() ){
			text.AppendCharacter( '\n' );
		}
		text.Append( line );
	}
	*/
	const char * const text = caption;
	
	igdeIconReference icon;
	if( image.GetWidth() > image.GetHeight() ){
		icon.TakeOver( new igdeIcon( image, pSize, pSize * image.GetHeight() / image.GetWidth() ) );
		
	}else{
		icon.TakeOver( new igdeIcon( image, pSize * image.GetWidth() / image.GetHeight(), pSize ) );
	}
	
	item.TakeOver( new igdeListItem( text, icon, userPointer ) );
	pIconListBox->AddItem( item );
}



// Visiting
/////////////

void igdeGDAddToListVisitor::VisitObjectClass( igdeGDClass *gdclass ){
	if( ! gdclass->GetCanInstanciate() ){
		return;
	}
	
	if( pIconListBox->GetItemCount() >= 100 ){
		// too many items. FOX has problems with this. will be changed once we have a better system
		printf( "Too Many List Items (%s:%d)\n", __FILE__, __LINE__ );
		return;
	}
	
	deImage * const image = pEnvironment.GetGDPreviewManager()->GetPreviewObjectClass( gdclass );
	igdeListItemReference item;
	if( image ){
		AddItemToList( item, gdclass->GetName(), *image, gdclass );
		return;
	}
	
	AddItemToList( item,  gdclass->GetName(), *pEnvironment.GetGDPreviewManager()->GetImageCreating(), gdclass );
	
	deObjectReference listener;
	listener.TakeOver( new igdeBrowseItemGDPreviewListener( pIconListBox, item, pSize ) );
	pEnvironment.GetGDPreviewManager()->CreatePreviewObjectClass(
		gdclass, ( igdeGDPreviewListener* )( deObject* )listener );
}

void igdeGDAddToListVisitor::VisitSkin( igdeGDSkin *gdskin ){
	if( pIconListBox->GetItemCount() >= 100 ){
		// too many items. FOX has problems with this. will be changed once we have a better system
		printf( "Too Many List Items (%s:%d)\n", __FILE__, __LINE__ );
		return;
	}
	
	deImage * const image = pEnvironment.GetGDPreviewManager()->GetPreviewSkin( gdskin );
	igdeListItemReference item;
	if( image ){
		AddItemToList( item, gdskin->GetName(), *image, gdskin );
		return;
	}
	
	AddItemToList( item,  gdskin->GetName(), *pEnvironment.GetGDPreviewManager()->GetImageCreating(), gdskin );
	
	deObjectReference listener;
	listener.TakeOver( new igdeBrowseItemGDPreviewListener( pIconListBox, item, pSize ) );
	pEnvironment.GetGDPreviewManager()->CreatePreviewSkin(
		gdskin, ( igdeGDPreviewListener* )( deObject* )listener );
}

void igdeGDAddToListVisitor::VisitSky( igdeGDSky *gdsky ){
	if( pIconListBox->GetItemCount() >= 100 ){
		// too many items. FOX has problems with this. will be changed once we have a better system
		printf( "Too Many List Items (%s:%d)\n", __FILE__, __LINE__ );
		return;
	}
	
	deImage * const image = pEnvironment.GetGDPreviewManager()->GetPreviewSky( gdsky );
	igdeListItemReference item;
	if( image ){
		AddItemToList( item, gdsky->GetName(), *image, gdsky );
		return;
	}
	
	AddItemToList( item,  gdsky->GetName(), *pEnvironment.GetGDPreviewManager()->GetImageCreating(), gdsky );
	
	deObjectReference listener;
	listener.TakeOver( new igdeBrowseItemGDPreviewListener( pIconListBox, item, pSize ) );
	pEnvironment.GetGDPreviewManager()->CreatePreviewSky(
		gdsky, ( igdeGDPreviewListener* )( deObject* )listener );
}
