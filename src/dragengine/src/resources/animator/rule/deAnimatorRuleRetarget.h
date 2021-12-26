/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEANIMATORRULERETARGET_H_
#define _DEANIMATORRULERETARGET_H_

#include "deAnimatorRule.h"
#include "../../rig/deRigReference.h"


/**
 * \brief Retarget Animation Rule Class.
 * Adjusts the existing animation to match a different rig. For this
 * a source and destination rig as well as a list of bone matchings
 * have to be provided. The source rig defines the structure of the
 * existing animation data. The destination rig defines the structure
 * to match the animation to. The destination rig can be NULL in
 * which case the rig from the attached component is used. The bone
 * matching list defines pairs of bone names assigning bones from the
 * existing animation to a new bone. Bones without such a matching
 * are assumed to stay the same but with different rest poses.
 */
class DE_DLL_EXPORT deAnimatorRuleRetarget : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleRetarget> Ref;
	
	
	
private:
	deRigReference pSrcRig;
	deRigReference pDestRig;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleRetarget();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleRetarget();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Source rig or NULL if not set. */
	inline deRig *GetSourceRig() const{ return pSrcRig; }
	
	/** \brief Set source rig or NULL if not set. */
	void SetSourceRig( deRig *rig );
	
	/** \brief Destination rig or NULL to use the rig from the attached component. */
	inline deRig *GetDestinationRig() const{ return pDestRig; }
	
	/** \brief Set destination rig or NULL to use the rig from the attached component. */
	void SetDestinationRig( deRig *rig );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
