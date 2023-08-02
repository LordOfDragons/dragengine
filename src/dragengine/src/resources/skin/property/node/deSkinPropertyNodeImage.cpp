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

#include "deSkinPropertyNodeImage.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../image/deImage.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeImage
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeImage::deSkinPropertyNodeImage() :
pRepeat( 1, 1 ){
}

deSkinPropertyNodeImage::~deSkinPropertyNodeImage(){
}



// Management
///////////////

void deSkinPropertyNodeImage::SetPath( const char *path ){
	pPath = path;
}

void deSkinPropertyNodeImage::SetImage( deImage *image ){
	pImage = image;
}

void deSkinPropertyNodeImage::SetRepeat( const decPoint &count ){
	DEASSERT_TRUE( count >= decPoint( 1, 1 ) )
	pRepeat = count;
}



// Visiting
/////////////

void deSkinPropertyNodeImage::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitImage( *this );
}
