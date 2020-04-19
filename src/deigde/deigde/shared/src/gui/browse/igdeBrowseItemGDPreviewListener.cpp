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

#include "igdeBrowseItemGDPreviewListener.h"
#include "../igdeIconListBox.h"
#include "../model/igdeListItem.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeIconReference.h"

#include <dragengine/resources/image/deImage.h>



// Class igdeBrowseItemGDPreviewListener
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeBrowseItemGDPreviewListener::igdeBrowseItemGDPreviewListener(
igdeIconListBox *iconListBox, igdeListItem *listItem, int iconSize ) :
pIconListBox( iconListBox ),
pListItem( listItem ),
pIconSize( iconSize ){
}

igdeBrowseItemGDPreviewListener::~igdeBrowseItemGDPreviewListener(){
}



// Management
///////////////

void igdeBrowseItemGDPreviewListener::ImageCreated( deImage *image ){
	const int index = pIconListBox->IndexOfItem( pListItem );
	if( index == -1 ){
		return;
	}
	
	igdeIconReference icon;
	if( image->GetWidth() > image->GetHeight() ){
		icon.TakeOver( new igdeIcon( *image, pIconSize, pIconSize * image->GetHeight() / image->GetWidth() ) );
		
	}else{
		icon.TakeOver( new igdeIcon( *image, pIconSize * image->GetWidth() / image->GetHeight(), pIconSize ) );
	}
	pListItem->SetIcon( icon );
	pIconListBox->ItemChangedAt( index );
}
