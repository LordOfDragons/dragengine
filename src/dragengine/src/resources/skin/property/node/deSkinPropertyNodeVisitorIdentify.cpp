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

#include "deSkinPropertyNode.h"
#include "deSkinPropertyNodeGroup.h"
#include "deSkinPropertyNodeImage.h"
#include "deSkinPropertyNodeShape.h"
#include "deSkinPropertyNodeText.h"
#include "deSkinPropertyNodeVisitorIdentify.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeVisitorIdentify
////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeVisitorIdentify::deSkinPropertyNodeVisitorIdentify() :
pNode( NULL ),
pType( entUnknown ){
}

deSkinPropertyNodeVisitorIdentify::~deSkinPropertyNodeVisitorIdentify(){
}



// Management
///////////////

deSkinPropertyNodeGroup &deSkinPropertyNodeVisitorIdentify::CastToGroup() const{
	if( pType != entGroup ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSkinPropertyNodeGroup* )pNode );
}

deSkinPropertyNodeImage &deSkinPropertyNodeVisitorIdentify::CastToImage() const{
	if( pType != entImage ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSkinPropertyNodeImage* )pNode );
}

deSkinPropertyNodeShape &deSkinPropertyNodeVisitorIdentify::CastToShape() const{
	if( pType != entShape ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSkinPropertyNodeShape* )pNode );
}

deSkinPropertyNodeText &deSkinPropertyNodeVisitorIdentify::CastToText() const{
	if( pType != entText ){
		DETHROW( deeInvalidParam );
	}
	return *( ( deSkinPropertyNodeText* )pNode );
}



void deSkinPropertyNodeVisitorIdentify::Reset(){
	pNode = NULL;
	pType = entUnknown;
}



// Visiting
/////////////

void deSkinPropertyNodeVisitorIdentify::VisitNode( deSkinPropertyNode &node ){
	pNode = &node;
	pType = entUnknown;
}

void deSkinPropertyNodeVisitorIdentify::VisitGroup( deSkinPropertyNodeGroup &node ){
	pNode = &node;
	pType = entGroup;
}

void deSkinPropertyNodeVisitorIdentify::VisitImage( deSkinPropertyNodeImage &node ){
	pNode = &node;
	pType = entImage;
}

void deSkinPropertyNodeVisitorIdentify::VisitShape( deSkinPropertyNodeShape &node ){
	pNode = &node;
	pType = entShape;
}

void deSkinPropertyNodeVisitorIdentify::VisitText( deSkinPropertyNodeText &node ){
	pNode = &node;
	pType = entText;
}
