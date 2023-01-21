/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLCOMPONENTTESTFORTOUCH_H_
#define _DEOGLCOMPONENTTESTFORTOUCH_H_

#include "../world/deoglWorldOctreeVisitor.h"

class deoglRComponent;



/**
 * Render component test for touch visitor.
 */
class deoglComponentTestForTouch : public deoglWorldOctreeVisitor{
private:
	deoglRComponent *pComponent;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	deoglComponentTestForTouch( deoglRComponent *component );
	
	/** Clean up visitor. */
	virtual ~deoglComponentTestForTouch();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit ocree node. */
	virtual void VisitNode( deoglDOctree *node, int intersection );
	/*@}*/
};

#endif
