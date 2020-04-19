/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglVSRetainImageData.h"
#include "../../texture/deoglImage.h"
#include "../../texture/deoglRImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/skin/property/deSkinProperty.h>
#include <dragengine/resources/skin/property/deSkinPropertyConstructed.h>
#include <dragengine/resources/skin/property/deSkinPropertyImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNode.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>



// Class deoglVSRetainImageData
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVSRetainImageData::deoglVSRetainImageData() :
pForceLock( false ){
}



// Management
///////////////

void deoglVSRetainImageData::RetainPropertyImages( deSkinProperty &property, bool forceLock ){
	pForceLock = forceLock;
	property.Visit( *this );
}

void deoglVSRetainImageData::ReleaseImages(){
	const int count = pImages.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglImage* )pImages.GetAt( i ) )->ReleasePixelBuffer();
	}
	pImages.RemoveAll();
}

void deoglVSRetainImageData::ProcessImage( deImage &image, bool forceLock ){
	deoglImage &oglImage = *( ( deoglImage* )image.GetPeerGraphic() );
	
	// NOTE skin use is changed during delayed operations on the render thread by the first
	//      skin assigning the processed texture
	
	if( oglImage.GetRImage()->GetSkinUse() && ! forceLock ){
		// a skin assigned a texture to this image. it is not required anymore to build it.
		// disregarded if lock is forced. this is the case for constructed textures which
		// can not take advantage of reusing a shared image
		return;
	}
	
	// no skin is using this image. we need to acquire a lock to build it. it is possible
	// multiple skins attempt to build the same texture. since we do not know in which order
	// the skins finish building we can not determine what skin to wait for to build the
	// texture only once. building too is suboptimal but the only safe solution
	oglImage.CreatePixelBuffer();
	pImages.Add( &oglImage );
}



// Visiting Property
//////////////////////

void deoglVSRetainImageData::VisitProperty( deSkinProperty& ){
}

void deoglVSRetainImageData::VisitValue( deSkinPropertyValue& ){
}

void deoglVSRetainImageData::VisitColor( deSkinPropertyColor& ){
}

void deoglVSRetainImageData::VisitImage( deSkinPropertyImage &property ){
	if( property.GetImage() ){
		ProcessImage( *property.GetImage(), pForceLock );
	}
}

void deoglVSRetainImageData::VisitVideo( deSkinPropertyVideo& ){
}

void deoglVSRetainImageData::VisitConstructed( deSkinPropertyConstructed &property ){
	VisitGroup( property.GetContent() );
}



// Visiting Node
//////////////////

void deoglVSRetainImageData::VisitNode( deSkinPropertyNode &node ){
	if( node.GetMask() ){
		node.GetMask()->Visit( *this );
	}
}

void deoglVSRetainImageData::VisitGroup( deSkinPropertyNodeGroup &node ){
	VisitNode( node );
	
	const int count = node.GetNodeCount();
	int i;
	for( i=0; i<count; i++ ){
		node.GetNodeAt( i )->Visit( *this );
	}
}

void deoglVSRetainImageData::VisitImage( deSkinPropertyNodeImage &node ){
	VisitNode( node );
	
	if( node.GetImage() ){
		ProcessImage( *node.GetImage(), true );
	}
}

void deoglVSRetainImageData::VisitShape( deSkinPropertyNodeShape &node ){
	VisitNode( node );
}

void deoglVSRetainImageData::VisitText( deSkinPropertyNodeText &node ){
	VisitNode( node );
	
	// we have to retain the image of the font because the font consumes the pixel buffer turning
	// it into a texture. we need here though direct access to the image data. this is no problem
	// since the texture is constructed once and then cached to file causing the image file data
	// to be retained only once
	if( node.GetFont() && node.GetFont()->GetImage() ){
		ProcessImage( *node.GetFont()->GetImage(), true );
	}
}
/*@}*/
