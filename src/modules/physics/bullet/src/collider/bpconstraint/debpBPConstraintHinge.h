/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPBPCONSTRAINTHINGE_H_
#define _DEBPBPCONSTRAINTHINGE_H_

#include "debpBPConstraintBase.h"

#include <BulletDynamics/ConstraintSolver/btHingeConstraint.h>



/**
 * @brief Bullet physics hinge constraint adapter.
 */
class debpBPConstraintHinge : public btHingeConstraint, debpBPConstraintBase{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new constraint. */
	debpBPConstraintHinge( btRigidBody &rbA, btRigidBody &rbB, const btTransform &rbAFrame, const btTransform &rbBFrame );
	/** Creates a new constraint. */
	debpBPConstraintHinge( btRigidBody &rbA, const btTransform &rbAFrame, const btTransform &rbBFrame );
	/** Cleans up the constraint. */
	virtual ~debpBPConstraintHinge();
	
	/** @name Management */
	/*@{*/
#if 0
	/** \brief Calculate constraint solver information. */
	virtual void getInfo2( btConstraintInfo2 *info );
	/** \brief Set breaking parameters. */
	void SetBreakingParameters( btConstraintInfo2 &info );
#endif
	/*@}*/
};

#endif
