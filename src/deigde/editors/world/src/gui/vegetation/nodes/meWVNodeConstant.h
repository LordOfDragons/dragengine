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

#ifndef _MEWVNODECONSTANT_H_
#define _MEWVNODECONSTANT_H_

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>

#include "meWVNode.h"

class meHTVRuleConstant;



/**
 * \brief Vegetation Editing Window Node Constant.
 */
class meWVNodeConstant : public meWVNode{
private:
	meHTVRuleConstant *pRuleConstant;
	
	igdeContainerReference pFraParameters;
	igdeEditVectorReference pEditVector;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeConstant( meWindowVegetation &windowVegetation, meHTVRuleConstant *rule );
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNodeConstant();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleConstant *GetRuleConstant() const{ return pRuleConstant; }
	
	/** \brief Update node. */
	virtual void Update();
	/*@}*/
};

#endif
