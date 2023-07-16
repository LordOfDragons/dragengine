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

#ifndef _DEOGLSCCONSTRUCTEDDYNAMIC_H_
#define _DEOGLSCCONSTRUCTEDDYNAMIC_H_

#include <dragengine/resources/skin/property/node/deSkinPropertyNodeVisitor.h>


/**
 * Determine if constructed property is dynamic.
 */
class deoglSCConstructedDynamic : public deSkinPropertyNodeVisitor{
private:
	bool pDynamic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglSCConstructedDynamic();
	
	/** Clean up visitor. */
	virtual ~deoglSCConstructedDynamic() override;
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Is dynamic. */
	static bool IsDynamic( deSkinPropertyNode &node );
	
	/** Dynamic. */
	inline bool GetDynamic() const{ return pDynamic; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit node. */
	virtual void VisitNode( deSkinPropertyNode &node ) override;
	
	/** Visit group node. */
	virtual void VisitGroup( deSkinPropertyNodeGroup &node ) override;
	
	/** Visit image node. */
	virtual void VisitImage( deSkinPropertyNodeImage &node ) override;
	
	/** Visit shape node. */
	virtual void VisitShape( deSkinPropertyNodeShape &node ) override;
	
	/** Visit text node. */
	virtual void VisitText( deSkinPropertyNodeText &node ) override;
	/*@}*/
};

#endif

