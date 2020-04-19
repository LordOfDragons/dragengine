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

#ifndef _DERLTASKREADSKINPROPERTYNODE_H_
#define _DERLTASKREADSKINPROPERTYNODE_H_

#include "../../skin/property/node/deSkinPropertyNodeVisitor.h"
#include "../../../common/string/decString.h"

class deResourceLoader;
class deRLTaskReadSkinInternal;
class deEngine;
class deVirtualFileSystem;


/**
 * \brief Skin constructed property node visitor loading resources in nodes.
 * 
 * For use by deResourceLoader for loading resource inside skins loading using
 * asynchronous skin loading.
 * 
 * If a resource is already present loading is skiped. This can be used by
 * deSkinBuilder users to use already loaded resources.
 */
class deRLTaskReadSkinPropertyNode : public deSkinPropertyNodeVisitor{
private:
	deResourceLoader &pResourceLoader;
	deRLTaskReadSkinInternal &pTask;
	deEngine &pEngine;
	deVirtualFileSystem * const pVirtualFileSystem;
	const decString pBasePath;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deRLTaskReadSkinPropertyNode( deResourceLoader &resourceLoader, deRLTaskReadSkinInternal &task,
		deEngine &engine, deVirtualFileSystem *vfs, const char *basePath );
	
	/** \brief Clean up visitor. */
	virtual ~deRLTaskReadSkinPropertyNode();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit node. */
	virtual void VisitNode( deSkinPropertyNode &node );
	
	/** \brief Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node );
	
	/** \brief Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node );
	
	/** \brief Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node );
	/*@}*/
};

#endif
