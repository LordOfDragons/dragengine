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

#ifndef _DEBPCONSTRAINTSOLVER_H_
#define _DEBPCONSTRAINTSOLVER_H_

// #include "BulletDynamics/Featherstone/btMultiBodyConstraintSolver.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"



/**
 * \brief Enhances btMultiBodyConstraintSolver to support constraint frictions.
 */
class debpConstraintSolver : public btSequentialImpulseConstraintSolver{
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
