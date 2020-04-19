/* 
 * Drag[en]gine Skin Module
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

#ifndef _DESMWRITEPROPERTYNODE_H_
#define _DESMWRITEPROPERTYNODE_H_

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>
#include <dragengine/common/math/decMath.h>

class decXmlWriter;



/**
 * \brief Write property node visitor.
 */
class desmWritePropertyNode : public deSkinPropertyNodeVisitor{
private:
	decXmlWriter &pWriter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	desmWritePropertyNode( decXmlWriter &writer );
	
	/** \brief Clean up visitor. */
	virtual ~desmWritePropertyNode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Write common node properties. */
	void WriteNodeCommon( const deSkinPropertyNode &node );
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
