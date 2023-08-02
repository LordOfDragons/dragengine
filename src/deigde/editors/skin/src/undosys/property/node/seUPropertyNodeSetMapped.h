/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEUPROPERTYNODESETMAPPED_H_
#define _SEUPROPERTYNODESETMAPPED_H_

#include "../../../skin/property/node/sePropertyNode.h"
#include "../../../skin/mapped/seMapped.h"

#include <deigde/undo/igdeUndo.h>


/** Undo action property node set mapped. */
class seUPropertyNodeSetMapped : public igdeUndo{
private:
	const sePropertyNode::Ref pNode;
	const int pType;
	
	const seMapped::Ref pOldValue;
	const seMapped::Ref pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	seUPropertyNodeSetMapped( sePropertyNode *node, int type, seMapped *newValue );
	
protected:
	/** Clean up undo. */
	virtual ~seUPropertyNodeSetMapped() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Undo action. */
	virtual void Undo() override;
	
	/** Redo action. */
	virtual void Redo() override;
	/*@}*/
};

#endif
