/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AEELEMENTVISITOR_H_
#define _AEELEMENTVISITOR_H_

class igdeGizmo;


/**
 * Element visitor.
 */
class aeElementVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create visitor. */
	aeElementVisitor();
	
	/** Clean up visitor. */
	virtual ~aeElementVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** Visit gizmo. */
	virtual void visitGizmo( igdeGizmo *gizmo );
	/*@}*/
};

#endif
