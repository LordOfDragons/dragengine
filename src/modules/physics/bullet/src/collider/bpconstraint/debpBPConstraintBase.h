/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
