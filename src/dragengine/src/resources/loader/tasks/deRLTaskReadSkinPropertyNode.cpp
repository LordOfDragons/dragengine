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

#include "deRLTaskReadSkinPropertyNode.h"
#include "deRLTaskReadSkinInternal.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../font/deFont.h"
#include "../../font/deFontManager.h"
#include "../../skin/property/node/deSkinPropertyNodeGroup.h"
#include "../../skin/property/node/deSkinPropertyNodeImage.h"
#include "../../skin/property/node/deSkinPropertyNodeText.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"



// Class deRLTaskReadSkinPropertyNode
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkinPropertyNode::deRLTaskReadSkinPropertyNode( deResourceLoader &resourceLoader,
deRLTaskReadSkinInternal &task, deEngine &engine, deVirtualFileSystem *vfs, const char *basePath ) :
pResourceLoader( resourceLoader ),
pTask( task ),
pEngine( engine ),
pVirtualFileSystem( vfs ),
pBasePath( basePath )
{
	(void)pEngine; // silence compiler moaning
}

deRLTaskReadSkinPropertyNode::~deRLTaskReadSkinPropertyNode(){
}



// Visiting
/////////////

void deRLTaskReadSkinPropertyNode::VisitNode( deSkinPropertyNode &node ){
	if( node.GetMask() ){
		node.GetMask()->Visit( *this );
	}
}

void deRLTaskReadSkinPropertyNode::VisitGroup( deSkinPropertyNodeGroup &node ){
	deSkinPropertyNodeVisitor::VisitGroup( node );
	
	const int count = node.GetNodeCount();
	int i;
	
	for( i=0; i<count; i++ ){
		node.GetNodeAt( i )->Visit( *this );
	}
}

void deRLTaskReadSkinPropertyNode::VisitImage( deSkinPropertyNodeImage &node ){
	deSkinPropertyNodeVisitor::VisitImage( node );
	
	if( node.GetImage() ){
		return;
	}
	
	decString path( node.GetPath() );
	if( path.IsEmpty() ){
		return;
	}
	
	if( ! decPath::IsUnixPathAbsolute( path ) ){
		decPath resourcePath;
		resourcePath.SetFromUnix( pBasePath );
		resourcePath.AddUnixPath( path );
		path = resourcePath.GetPathUnix();
	}
	
	deResourceLoaderTask * const task = pResourceLoader.AddLoadRequest(
		pVirtualFileSystem, path, deResourceLoader::ertImage );
	deRLTaskReadSkinInternal::cInternalTask *internalTask = NULL;
	
	try{
		internalTask = new deRLTaskReadSkinInternal::cInternalTask( &node, task );
		pTask.AddInternalTask( internalTask );
		internalTask->FreeReference();
		
	}catch( const deException & ){
		if( internalTask ){
			internalTask->FreeReference();
		}
		throw;
	}
}

void deRLTaskReadSkinPropertyNode::VisitText( deSkinPropertyNodeText &node ){
	deSkinPropertyNodeVisitor::VisitText( node );
	
	if( node.GetFont() ){
		return;
	}
	
	decString path( node.GetPath() );
	if( path.IsEmpty() ){
		return;
	}
	
	if( ! decPath::IsUnixPathAbsolute( path ) ){
		decPath resourcePath( decPath::CreatePathUnix( pBasePath ) );
		resourcePath.AddUnixPath( path );
		path = resourcePath.GetPathUnix();
	}
	
	deResourceLoaderTask * const task = pResourceLoader.AddLoadRequest(
		pVirtualFileSystem, path, deResourceLoader::ertFont );
	deRLTaskReadSkinInternal::cInternalTask *internalTask = NULL;
	
	try{
		internalTask = new deRLTaskReadSkinInternal::cInternalTask( &node, task );
		pTask.AddInternalTask( internalTask );
		internalTask->FreeReference();
		
	}catch( const deException & ){
		if( internalTask ){
			internalTask->FreeReference();
		}
		throw;
	}
}
