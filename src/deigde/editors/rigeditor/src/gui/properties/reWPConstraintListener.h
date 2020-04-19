/* 
 * Drag[en]gine IGDE Rig Editor
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

#ifndef _REWPCONSTRAINTLISTENER_H_
#define _REWPCONSTRAINTLISTENER_H_

#include "../../rig/reRigNotifier.h"

class reWPConstraint;



/**
 * \brief Constraint panel listener.
 */
class reWPConstraintListener : public reRigNotifier{
private:
	reWPConstraint &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	reWPConstraintListener( reWPConstraint &panel );
	
	/** \brief Clean up listener. */
	virtual ~reWPConstraintListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Bone count changed. */
	virtual void BoneCountChanged( reRig *rig );
	
	/** \brief A bone changed. */
	virtual void BoneChanged( reRig *rig, reRigBone *bone );
	
	/** \brief A constraint changed. */
	virtual void ConstraintChanged( reRig *rig, reRigConstraint *constraint );
	
	/** \brief A constraint degree of freedom changed. */
	virtual void ConstraintDofChanged( reRig *rig, reRigConstraint *constraint,
		deColliderConstraint::eDegreesOfFreedom dof );
	
	/** \brief The active constraint changed. */
	virtual void ActiveConstraintChanged( reRig *rig );
	/*@}*/
};

#endif
