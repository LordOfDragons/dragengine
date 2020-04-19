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

#ifndef _DEBPBPCONSTRAINTBASE_H_
#define _DEBPBPCONSTRAINTBASE_H_

#include "BulletDynamics/ConstraintSolver/btTypedConstraint.h"

class btTypedConstraint;


/**
 * \brief Base interface for bullet physics constraint adapters.
 * 
 * Provides extended functionality and support to obtain the original constraint and collider.
 */
class debpBPConstraintBase{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create base bullet constraint. */
	debpBPConstraintBase();
	
	/** \brief Clean up shared bullet constraint. */
	virtual ~debpBPConstraintBase();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Base pointer for constraint or \em NULL if not set.
	 * 
	 * Use this call and not the getUserConstraintPtr() of btTypedConstraint because btTypedConstraint
	 * has a bad implementation choise where the user pointer is memory shared with an integer set
	 * to -1. Checking against NULL is not going to work.
	 */
	static debpBPConstraintBase *GetBase( btTypedConstraint *constraint );
	
	/** \brief Prepare constraint for next simulation step. */
	virtual void PrepareForStep();
	
	
	
	/**
	 * \brief Determine number of rows required for constraint friction.
	 * 
	 * This call is used by subclasses of constraint solvers to add constraint friction. This composes
	 * of static and kinematic joint friction.
	 */
	virtual int GetConstraintFrictionCount();
	
	/**
	 * \brief Get constraint friction forces.
	 * 
	 * This call is used by subclasses of constraint solvers to add constraint friction. This composes
	 * of static and kinematic joint friction.
	 */
	virtual void GetConstraintFriction( btTypedConstraint::btConstraintInfo2 &info );
	/*@}*/
};

#endif
