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

#include "deSkinPropertyNodeGroup.h"
#include "deSkinPropertyNodeImage.h"
#include "deSkinPropertyNodeText.h"
#include "deSkinPropertyNodeVisitorLoad.h"
#include "../../../image/deImage.h"
#include "../../../image/deImageManager.h"
#include "../../../font/deFont.h"
#include "../../../font/deFontManager.h"
#include "../../../../deEngine.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeVisitorLoad
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeVisitorLoad::deSkinPropertyNodeVisitorLoad(
deEngine &engine, deVirtualFileSystem *vfs, const char *basePath ) :
pEngine( engine ),
pVirtualFileSystem( vfs ),
pBasePath( basePath ){
}

deSkinPropertyNodeVisitorLoad::~deSkinPropertyNodeVisitorLoad(){
}



// Visiting
/////////////

void deSkinPropertyNodeVisitorLoad::VisitNode( deSkinPropertyNode &node ){
	if( node.GetMask() ){
		node.GetMask()->Visit( *this );
	}
}

void deSkinPropertyNodeVisitorLoad::VisitGroup( deSkinPropertyNodeGroup &node ){
	deSkinPropertyNodeVisitor::VisitGroup( node );
	
	const int count = node.GetNodeCount();
	int i;
	
	for( i=0; i<count; i++ ){
		node.GetNodeAt( i )->Visit( *this );
	}
}

void deSkinPropertyNodeVisitorLoad::VisitImage( deSkinPropertyNodeImage &node ){
	deSkinPropertyNodeVisitor::VisitImage( node );
	
	if( node.GetImage() ){
		return;
	}
	
	const decString &path = node.GetPath();
	deImage *image = NULL;
	
	if( ! path.IsEmpty() ){
		try{
			image = pEngine.GetImageManager()->LoadImage( pVirtualFileSystem, path, pBasePath );
			node.SetImage( image );
			image->FreeReference();
			image = NULL;
			
		}catch( const deException & ){
			if( image ){
				image->FreeReference();
				image = NULL;
			}
		}
	}
}

void deSkinPropertyNodeVisitorLoad::VisitText( deSkinPropertyNodeText &node ){
	deSkinPropertyNodeVisitor::VisitText( node );
	
	if( node.GetFont() ){
		return;
	}
	
	const decString &path = node.GetPath();
	deFont *font = NULL;
	
	if( ! path.IsEmpty() ){
		try{
			font = pEngine.GetFontManager()->LoadFont( pVirtualFileSystem, path, pBasePath );
			node.SetFont( font );
			font->FreeReference();
			font = NULL;
			
		}catch( const deException & ){
			if( font ){
				font->FreeReference();
				font = NULL;
			}
		}
	}
}
