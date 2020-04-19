/* 
 * Drag[en]gine Game Engine
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

#include "deCanvasImage.h"
#include "deCanvasVisitor.h"
#include "../image/deImage.h"
#include "../../common/exceptions.h"



// Class deCanvasImage
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasImage::deCanvasImage( deCanvasManager *manager ) :
deCanvas( manager ),
pRepeatX( 1 ),
pRepeatY( 1 ){
}

deCanvasImage::~deCanvasImage(){
}



// Management
///////////////

void deCanvasImage::SetImage( deImage *image ){
	if( image == pImage ){
		return;
	}
	
	pImage = image;
	
	NotifyContentChanged();
}

void deCanvasImage::SetRepeatX( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pRepeatX ){
		return;
	}
	
	pRepeatX = count;
	NotifyContentChanged();
}

void deCanvasImage::SetRepeatY( int count ){
	if( count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pRepeatY ){
		return;
	}
	
	pRepeatY = count;
	NotifyContentChanged();
}



// Visiting
/////////////

void deCanvasImage::Visit( deCanvasVisitor &visitor ){
	visitor.VisitImage( *this );
}
