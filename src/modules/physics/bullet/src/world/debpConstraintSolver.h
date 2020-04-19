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

#ifndef _DEBPCONSTRAINTSOLVER_H_
#define _DEBPCONSTRAINTSOLVER_H_

#include "BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h"



/**
 * \brief Enhances btMultiBodyConstraintSolver to support constraint frictions.
 */
class debpConstraintSolver : public btMultiBodyConstraintSolver{
private:
	btAlignedObjectArray<int> pPoolSolverConstraintFrictionSizes;
	btConstraintArray pPoolSolverConstraintFriction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constraint solver. */
	debpConstraintSolver();
	
	/** \brief Clean up constraint solver. */
	virtual ~debpConstraintSolver();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Set up solving.
	 * \details Modified to add friction constraints for constraint friction.
	 */
	virtual btScalar solveGroupCacheFriendlySetup( btCollisionObject **bodies, int numBodies,
		btPersistentManifold **manifoldPtr, int numManifolds, btTypedConstraint **constraints,
		int numConstraints, const btContactSolverInfo &infoGlobal, btIDebugDraw *debugDrawer );
	
	/**
	 * \brief Solve single interation step.
	 * \details Modified to update constraint friction after contact friction.
	 */
	virtual btScalar solveSingleIteration( int iteration, btCollisionObject **bodies, int numBodies,
		btPersistentManifold **manifoldPtr, int numManifolds, btTypedConstraint **constraints,
		int numConstraints, const btContactSolverInfo &infoGlobal, btIDebugDraw *debugDrawer );
	
	/**
	 * \brief Finish solving.
	 * \details Does some cleaning up. Base class functionality is not altered.
	 * \note If warmstarting is used the base class functionatliy has to be modified.
	 *       Warmstarting is though not used.
	 */
	virtual btScalar solveGroupCacheFriendlyFinish( btCollisionObject **bodies, int numBodies,
		const btContactSolverInfo &infoGlobal );
	/*@}*/
};

#endif
