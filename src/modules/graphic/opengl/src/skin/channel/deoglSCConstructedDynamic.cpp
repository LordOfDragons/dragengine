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

#include "deoglSCConstructedDynamic.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeGroup.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeImage.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeShape.h>
#include <dragengine/resources/skin/property/node/deSkinPropertyNodeText.h>


// Class deoglSCConstructedDynamic
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSCConstructedDynamic::deoglSCConstructedDynamic() :
pDynamic( false ){
}

deoglSCConstructedDynamic::~deoglSCConstructedDynamic(){
}



// Visiting
/////////////

bool deoglSCConstructedDynamic::IsDynamic( deSkinPropertyNode &node ){
	deoglSCConstructedDynamic visitor;
	node.Visit( visitor );
	return visitor.pDynamic;
}

void deoglSCConstructedDynamic::VisitNode( deSkinPropertyNode &node ){
	if( pDynamic ){
		return;
	}
	
	int i;
	for( i=0; i<deSkinPropertyNode::MappedCount; i++ ){
		if( node.GetMappedFor( ( deSkinPropertyNode::eMapped )i ) != -1 ){
			pDynamic = true;
			return;
		}
	}
}

void deoglSCConstructedDynamic::VisitGroup( deSkinPropertyNodeGroup &node ){
	deSkinPropertyNodeVisitor::VisitGroup( node );
	
	const int count = node.GetNodeCount();
	int i;
	for( i=0; !pDynamic && i<count; i++ ){
		node.GetNodeAt( i )->Visit( *this );
	}
}

void deoglSCConstructedDynamic::VisitImage( deSkinPropertyNodeImage &node ){
	deSkinPropertyNodeVisitor::VisitImage( node );
}

void deoglSCConstructedDynamic::VisitShape( deSkinPropertyNodeShape &node ){
	deSkinPropertyNodeVisitor::VisitShape( node );
	
	if( pDynamic ){
		return;
	}
	
	int i;
	for( i=0; i<deSkinPropertyNodeShape::ShapeMappedCount; i++ ){
		if( node.GetShapeMappedFor( ( deSkinPropertyNodeShape::eShapeMapped )i ) != -1 ){
			pDynamic = true;
			return;
		}
	}
}

void deoglSCConstructedDynamic::VisitText( deSkinPropertyNodeText &node ){
	deSkinPropertyNodeVisitor::VisitText( node );
	
	if( pDynamic ){
		return;
	}
	
	int i;
	for( i=0; i<deSkinPropertyNodeText::TextMappedCount; i++ ){
		if( node.GetTextMappedFor( ( deSkinPropertyNodeText::eTextMapped )i ) != -1 ){
			pDynamic = true;
			return;
		}
	}
}
