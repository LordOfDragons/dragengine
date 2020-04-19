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

#ifndef _MEWVNODECLOSESTPROP_H_
#define _MEWVNODECLOSESTPROP_H_

#include "meWVNode.h"

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/event/igdeActionContextMenuReference.h>

class meHTVRuleClosestProp;


/**
 * \brief Vegetation Editing Window Node Closest Prop.
 */
class meWVNodeClosestProp : public meWVNode{
private:
	meHTVRuleClosestProp *pRuleCP;
	
	igdeActionContextMenuReference pActionMenuClass;
	
	igdeContainerReference pFraParameters;
	igdeComboBoxFilterReference pCBPropClass;
	igdeButtonReference pBtnPropClass;
	igdeTextFieldReference pEditSearchRadius;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create node. */
	meWVNodeClosestProp( meWindowVegetation &windowVegetation, meHTVRuleClosestProp *rule );
	
protected:
	/** \brief Clean up node. */
	virtual ~meWVNodeClosestProp();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleClosestProp *GetRuleClosestProp() const{ return pRuleCP; }
	
	
	
	/** \brief Update node. */
	virtual void Update();
	
	/** \brief Update class lists. */
	virtual void UpdateClassLists();
	/*@}*/
};

#endif
