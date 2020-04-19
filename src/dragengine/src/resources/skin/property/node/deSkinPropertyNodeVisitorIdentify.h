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

#ifndef _DESKINPROPERTYNODEVISITORIDENTIFY_H_
#define _DESKINPROPERTYNODEVISITORIDENTIFY_H_

#include "deSkinPropertyNodeVisitor.h"


/**
 * \brief Skin constructed property node identify visitor.
 */
class deSkinPropertyNodeVisitorIdentify : public deSkinPropertyNodeVisitor{
public:
	/** \brief Node types. */
	enum eNodeTypes{
		/** \brief Unknown node. */
		entUnknown,
		
		/** \brief Group node. */
		entGroup,
		
		/** \brief Image node. */
		entImage,
		
		/** \brief Shape node. */
		entShape,
		
		/** \brief Text node. */
		entText
	};
	
	
	
private:
	deSkinPropertyNode *pNode;
	eNodeTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin constructed property node identify visitor. */
	deSkinPropertyNodeVisitorIdentify();
	
	/** \brief Clean up skin constructed property node identify visitor. */
	virtual ~deSkinPropertyNodeVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Node type. */
	inline eNodeTypes GetType() const{ return pType; }
	
	
	
	/** \brief Node is of type unknown. */
	inline bool IsUnknown() const{ return pType == entUnknown; }
	
	/** \brief Node is of type group. */
	inline bool IsGroup() const{ return pType == entGroup; }
	
	/** \brief Node is of type image. */
	inline bool IsImage() const{ return pType == entImage; }
	
	/** \brief Node is of type shape. */
	inline bool IsShape() const{ return pType == entShape; }
	
	/** \brief Node is of type text. */
	inline bool IsText() const{ return pType == entText; }
	
	
	
	/** \brief Cast to group node. */
	deSkinPropertyNodeGroup &CastToGroup() const;
	
	/** \brief Cast to image node. */
	deSkinPropertyNodeImage &CastToImage() const;
	
	/** \brief Cast to shape node. */
	deSkinPropertyNodeShape &CastToShape() const;
	
	/** \brief Cast to text node. */
	deSkinPropertyNodeText &CastToText() const;
	
	
	
	/** \brief Reset visitor. */
	void Reset();
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
