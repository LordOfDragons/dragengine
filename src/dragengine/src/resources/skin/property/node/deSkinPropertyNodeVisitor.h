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

#ifndef _DESKINPROPERTYNODEVISITOR_H_
#define _DESKINPROPERTYNODEVISITOR_H_

#include "../../../../dragengine_export.h"

class deSkinPropertyNode;
class deSkinPropertyNodeGroup;
class deSkinPropertyNodeImage;
class deSkinPropertyNodeShape;
class deSkinPropertyNodeText;


/**
 * \brief Visitor for skin constructed proprty nodes.
 */
class DE_DLL_EXPORT deSkinPropertyNodeVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deSkinPropertyNodeVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~deSkinPropertyNodeVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit node. */
	virtual void VisitNode( deSkinPropertyNode &node );
	
	/** \brief Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node );
	
	/** \brief Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node );
	
	/** \brief Visit shape node. */
	virtual void VisitShape( deSkinPropertyNodeShape &node );
	
	/** \brief Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node );
	/*@}*/
};

#endif
