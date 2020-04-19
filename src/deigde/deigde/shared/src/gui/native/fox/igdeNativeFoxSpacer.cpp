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
#include <stdint.h>

#include "igdeNativeFoxSpacer.h"

#include <dragengine/common/exceptions.h>



// Event map
//////////////

FXDEFMAP( igdeNativeFoxSpacer ) igdeNativeFoxSpacerMap[] = { };

FXIMPLEMENT( igdeNativeFoxSpacer, FXFrame, igdeNativeFoxSpacerMap, ARRAYNUMBER( igdeNativeFoxSpacerMap ) )



// Class igdeNativeFoxSpacer
//////////////////////////////

// Constructor, destructor
////////////////////////////

igdeNativeFoxSpacer::igdeNativeFoxSpacer(){
}

igdeNativeFoxSpacer::igdeNativeFoxSpacer( FXComposite *parent, int width, int height, int childFlags ) :
FXFrame( parent, LayoutFlags( childFlags ), 0, 0, width, height ),
pWidth( width ),
pHeight( height ){
}

igdeNativeFoxSpacer::~igdeNativeFoxSpacer(){
}

void igdeNativeFoxSpacer::SetSize( int width, int height ){
	pWidth = width;
	pHeight = height;
	resize( width, height );
}

int igdeNativeFoxSpacer::getDefaultWidth(){
	return pWidth;
}

int igdeNativeFoxSpacer::getDefaultHeight(){
	return pHeight;
}

int igdeNativeFoxSpacer::LayoutFlags( int childFlags ){
	childFlags &= ~( LAYOUT_FILL_COLUMN | LAYOUT_FILL_ROW );
	childFlags &= ~LAYOUT_FILL;
	childFlags |= LAYOUT_FIX_WIDTH | LAYOUT_FIX_HEIGHT;
	return childFlags;
}
