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

#ifndef _MEWVNODESLOT_H_
#define _MEWVNODESLOT_H_

#include <deigde/gui/nodeview/igdeNVSlot.h>


class meWVNode;


/**
 * \brief Vegetation Edit Node Slot.
 */
class meWVNodeSlot : public igdeNVSlot{
public:
	/** \brief Slot Types. */
	enum eSlotTypes{
		estValue,
		estVector
	};
	
	
	
private:
	meWVNode &pParentNode;
	const eSlotTypes pType;
	const int pRuleSlot;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create slot. */
	meWVNodeSlot( igdeEnvironment &environment, const char *text, const char *description,
		bool isInput, meWVNode &node, eSlotTypes type, int ruleSlot );
	
protected:
	/** \brief Clean up slot. */
	virtual ~meWVNodeSlot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent node. */
	inline meWVNode &GetParentNode() const{ return pParentNode; }
	
	/** \brief Slot type. */
	inline eSlotTypes GetType() const{ return pType; }
	
	/** \brief Slot number in the rule the parent node belongs to. */
	inline int GetRuleSlot() const{ return pRuleSlot; }
	/*@}*/
};

#endif
