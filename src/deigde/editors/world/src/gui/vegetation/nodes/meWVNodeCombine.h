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

#ifndef _MEWVNODECOMBINE_H_
#define _MEWVNODECOMBINE_H_

#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeTextFieldReference.h>

#include "meWVNode.h"

class meHTVRuleCombine;



/**
 * \brief Vegetation Editing Window Node Combine.
 */
class meWVNodeCombine : public meWVNode{
private:
	meHTVRuleCombine *pRuleCombine;
	
	igdeContainerReference pFraParameters;
	igdeTextFieldReference pEditX;
	igdeTextFieldReference pEditY;
	igdeTextFieldReference pEditZ;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meWVNodeCombine( meWindowVegetation &windowVegetation, meHTVRuleCombine *rule );
	
protected:
	/** \brief Clean up object. */
	virtual ~meWVNodeCombine();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Rule. */
	inline meHTVRuleCombine *GetRuleCombine() const{ return pRuleCombine; }
	
	/** \brief Update node. */
	virtual void Update();
	/*@}*/
};

#endif
