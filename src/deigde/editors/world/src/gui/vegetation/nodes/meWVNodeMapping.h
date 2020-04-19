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

#ifndef _MEWVNODEMAPPING_H_
#define _MEWVNODEMAPPING_H_

#include "meWVNode.h"

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>

class meHTVRuleMapping;



/**
 * \brief Vegetation Editing Window Node Closest Prop.
 */
class meWVNodeMapping : public meWVNode{
private:
	meHTVRuleMapping *pRuleMapping;
	
	igdeContainerReference pFraParameters;
	igdeTextFieldReference pEditLower;
	igdeTextFieldReference pEditUpper;
	igdeTextFieldReference pEditValue;
	igdeCheckBoxReference pChkInversed;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeMapping( meWindowVegetation &windowVegetation, meHTVRuleMapping *rule );
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNodeMapping();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleMapping *GetRuleMapping() const{ return pRuleMapping; }
	
	/** \brief Update node. */
	virtual void Update();
	/*@}*/
};

#endif
