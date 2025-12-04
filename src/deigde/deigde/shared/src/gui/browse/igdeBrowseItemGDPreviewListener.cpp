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

#include "igdeBrowseItemGDPreviewListener.h"
#include "../igdeIconListBox.h"
#include "../model/igdeListItem.h"
#include "../resources/igdeIcon.h"
#include "../resources/igdeIcon::Ref.h"

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
	
	igdeIcon::Ref icon;
	if( image->GetWidth() > image->GetHeight() ){
		icon.TakeOver( new igdeIcon( *image, pIconSize, pIconSize * image->GetHeight() / image->GetWidth() ) );
		
	}else{
		icon.TakeOver( new igdeIcon( *image, pIconSize * image->GetWidth() / image->GetHeight(), pIconSize ) );
	}
	pListItem->SetIcon( icon );
	pIconListBox->ItemChangedAt( index );
}
