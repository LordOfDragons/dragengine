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

#ifndef _MEWVNODEVECTORMATH_H_
#define _MEWVNODEVECTORMATH_H_

#include "meWVNode.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>

class meHTVRuleVectorMath;



/**
 * \brief Vegetation Editing Window Node Vector Math.
 */
class meWVNodeVectorMath : public meWVNode{
private:
	meHTVRuleVectorMath *pRuleVectorMath;
	
	igdeContainerReference pFraParameters;
	igdeComboBoxReference pCBOperator;
	igdeEditVectorReference pEditVectorA;
	igdeEditVectorReference pEditVectorB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeVectorMath( meWindowVegetation &windowVegetation, meHTVRuleVectorMath *rule );
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNodeVectorMath();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleVectorMath *GetRuleVectorMath() const{ return pRuleVectorMath; }
	
	/** \brief Update node. */
	virtual void Update();
	/*@}*/
};

#endif
