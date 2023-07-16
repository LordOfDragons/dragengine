/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglSkinStateCNImage.h"
#include "../../../texture/deoglImage.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>


// Class deoglSkinStateCNImage
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCNImage::deoglSkinStateCNImage( deSkinPropertyNodeImage &node ) :
deoglSkinStateConstructedNode( node, etImage ),
pImage( node.GetImage() && node.GetImage()->GetPeerGraphic()
	? ( ( deoglImage* )node.GetImage()->GetPeerGraphic() )->GetRImage() : nullptr ),
pRepeat( node.GetRepeat() ){
}

deoglSkinStateCNImage::deoglSkinStateCNImage( const deoglSkinStateCNImage &node ) :
deoglSkinStateConstructedNode( node ),
pImage( node.pImage ),
pRepeat( node.pRepeat ){
}

deoglSkinStateCNImage::~deoglSkinStateCNImage(){
}



// Management
///////////////

deoglSkinStateConstructedNode::Ref deoglSkinStateCNImage::Copy() const{
	return deoglSkinStateConstructedNode::Ref::New( new deoglSkinStateCNImage( *this ) );
}
