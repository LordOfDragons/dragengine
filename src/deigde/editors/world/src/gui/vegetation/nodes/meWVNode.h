/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEWVNODE_H_
#define _MEWVNODE_H_

#include <deigde/gui/nodeview/igdeNVNode.h>
#include <deigde/undo/igdeUndoReference.h>

class meWVNodeSlot;
class meHTVRule;
class meWindowVegetation;



/**
 * \brief Vegetation Editing Window Node.
 */
class meWVNode : public igdeNVNode{
private:
	meWindowVegetation &pWindowVegetation;
	meHTVRule *pRule;
	
	igdeUndoReference pUndoMove;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	meWVNode( meWindowVegetation &windowVegetation, meHTVRule *rule );
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNode();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Vegetation editor window. */
	inline meWindowVegetation &GetWindowVegetation() const{ return pWindowVegetation; }
	
	/** \brief Rule. */
	inline meHTVRule *GetRule() const{ return pRule; }
	
	/** \brief Update node. */
	virtual void Update();
	
	/** \brief Update class lists. */
	virtual void UpdateClassLists();
	
	/** \brief Can delete node. */
	virtual bool CanDelete() const;
	
	/** \brief Can duplicate node. */
	virtual bool CanDuplicate() const;
	
	/** \brief Set node position. */
	void UpdatePositionFromRule();
	/*@}*/
};

#endif
