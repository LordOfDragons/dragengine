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

#include <stdio.h>
#include <stdlib.h>

#include "debpConstraintSolver.h"
#include "../collider/bpconstraint/debpBPConstraintBase.h"

#include <dragengine/common/exceptions.h>



// Class debpConstraintSolver
///////////////////////////////

// Constructor, Destructor
////////////////////////////

debpConstraintSolver::debpConstraintSolver(){
}

debpConstraintSolver::~debpConstraintSolver(){
}



// Overloads
//////////////

btScalar debpConstraintSolver::solveGroupCacheFriendlySetup(
btCollisionObject **bodies, int numBodies, btPersistentManifold **manifoldPtr,
int numManifolds, btTypedConstraint **constraints, int numConstraints,
const btContactSolverInfo &infoGlobal, btIDebugDraw *debugDrawer ){
	return btMultiBodyConstraintSolver::solveGroupCacheFriendlySetup( bodies, numBodies,
		manifoldPtr, numManifolds, constraints, numConstraints, infoGlobal, debugDrawer );
	
#if 0
	btScalar returnValueWithoutUse =
		btMultiBodyConstraintSolver::solveGroupCacheFriendlySetup( bodies, numBodies,
		manifoldPtr, numManifolds, constraints, numConstraints, infoGlobal, debugDrawer );
	
	// determine number of friction rows required
	int frictionRowCount = 0;
	int i;
	
	pPoolSolverConstraintFrictionSizes.resizeNoInitialize( numConstraints );
	for( i=0; i<numConstraints; i++ ){
		debpBPConstraintBase * const constraint = debpBPConstraintBase::GetBase( constraints[ i ] );
		if( ! constraint ){
			pPoolSolverConstraintFrictionSizes[ i ] = 0;
			continue;
		}
		
		const int rowCount = constraint->GetConstraintFrictionCount();
		pPoolSolverConstraintFrictionSizes[ i ] = rowCount;
		frictionRowCount += rowCount;
	}
	
	// set up friction rows
	pPoolSolverConstraintFriction.resizeNoInitialize( frictionRowCount );
	
	if( frictionRowCount > 0 ){
		btTypedConstraint::btConstraintInfo2 frictionInfo;
		int frictionRow = 0;
		int constraintRow = 0;
		int j;
		
		frictionInfo.fps = BT_ONE / infoGlobal.m_timeStep;
		frictionInfo.rowskip = sizeof( btSolverConstraint ) / sizeof( btScalar );
		
		for( i=0; i<numConstraints; i++ ){
			debpBPConstraintBase * const constraint = debpBPConstraintBase::GetBase( constraints[ i ] );
			const int constraintRowCount = m_tmpConstraintSizesPool[ i ].m_numConstraintRows;
			
			if( ! constraint ){
				constraintRow += constraintRowCount;
				continue;
			}
			
			// init rows. there is no constructor so we have to init all fields. some fields can be copied
			// from the linked constraint row to avoid extra work
			const int frictionRowCount = pPoolSolverConstraintFrictionSizes[ i ];
			for( j=0; j<frictionRowCount; j++ ){
				const btSolverConstraint &constraintSolver = m_tmpSolverNonContactConstraintPool[ constraintRow + j ];
				btSolverConstraint &solver = pPoolSolverConstraintFriction[ frictionRow + j ];
				
				solver.m_frictionIndex = constraintRow + j;
				
				solver.m_solverBodyIdA = constraintSolver.m_solverBodyIdA;
				solver.m_solverBodyIdB = constraintSolver.m_solverBodyIdB;
				solver.m_originalContactPoint = constraintSolver.m_originalContactPoint;
				solver.m_contactNormal1.setZero();
				solver.m_relpos1CrossNormal.setZero();
				solver.m_angularComponentA.setZero();
				solver.m_contactNormal2.setZero();
				solver.m_relpos2CrossNormal.setZero();
				solver.m_angularComponentB.setZero();
				solver.m_jacDiagABInv = BT_ZERO;
				solver.m_rhs = BT_ZERO;
				solver.m_rhsPenetration = BT_ZERO;
				solver.m_cfm = BT_ZERO; // slip
				solver.m_lowerLimit = BT_ZERO;
				solver.m_upperLimit = BT_ZERO;
				solver.m_appliedImpulse = BT_ZERO;
				solver.m_appliedPushImpulse = BT_ZERO;
			}
			
			btSolverConstraint &solverFriction = pPoolSolverConstraintFriction[ frictionRow ];
			frictionInfo.m_J1linearAxis = solverFriction.m_contactNormal1;
			frictionInfo.m_J1angularAxis = solverFriction.m_relpos1CrossNormal;
			frictionInfo.m_J2linearAxis = solverFriction.m_contactNormal2;
			frictionInfo.m_J2angularAxis = solverFriction.m_relpos2CrossNormal;
			frictionInfo.m_constraintError = &solverFriction.m_rhs;
			frictionInfo.m_lowerLimit = &solverFriction.m_lowerLimit;
			frictionInfo.m_upperLimit = &solverFriction.m_upperLimit;
			constraint->GetConstraintFriction( frictionInfo );
			
			// finish setup
			btRigidBody &rbA = constraints[ i ]->getRigidBodyA();
			btRigidBody &rbB = constraints[ i ]->getRigidBodyB();
			
			for( j=0; j<frictionRowCount; j++ ){
				btSolverConstraint &solver = pPoolSolverConstraintFriction[ frictionRow + j ];
				
				// store friction from upper limit and set limits to infinity
				solver.m_friction = solver.m_upperLimit;
				solver.m_upperLimit = BT_INFINITY;
				solver.m_lowerLimit = -BT_INFINITY;
				
				// use the body velocities and external forces as error to compensate.
				// only friction part of error is compensated
				solver.m_angularComponentA = rbA.getInvInertiaTensorWorld() * solver.m_relpos1CrossNormal * rbA.getAngularFactor();
				solver.m_angularComponentB = rbB.getInvInertiaTensorWorld() * solver.m_relpos2CrossNormal * rbB.getAngularFactor();
				
				const btVector3 iMJlA( solver.m_contactNormal1 * rbA.getInvMass() );
				const btVector3 iMJaA( rbA.getInvInertiaTensorWorld() * solver.m_relpos1CrossNormal );
				const btVector3 iMJlB( solver.m_contactNormal2 * rbB.getInvMass() );
				const btVector3 iMJaB( rbB.getInvInertiaTensorWorld() * solver.m_relpos2CrossNormal );
				
				const btScalar sum = iMJlA.dot( solver.m_contactNormal1 )
					+ iMJaA.dot( solver.m_relpos1CrossNormal )
					+ iMJlB.dot( solver.m_contactNormal2 )
					+ iMJaB.dot( solver.m_relpos2CrossNormal );
				const btScalar fsum = btFabs( sum );
				btAssert( fsum > SIMD_EPSILON );
				solver.m_jacDiagABInv = fsum > SIMD_EPSILON ? BT_ONE / sum : BT_ZERO;
				
				const btSolverConstraint &constraintSolver = m_tmpSolverNonContactConstraintPool[ constraintRow + j ];
				const btSolverBody &bodyA = m_tmpSolverBodyPool[ constraintSolver.m_solverBodyIdA ];
				const btSolverBody &bodyB = m_tmpSolverBodyPool[ constraintSolver.m_solverBodyIdB ];
				
				const btVector3 externalForceImpulseA( bodyA.m_originalBody ? bodyA.m_externalForceImpulse : btVector3(0,0,0) );
				const btVector3 externalTorqueImpulseA( bodyA.m_originalBody ? bodyA.m_externalTorqueImpulse : btVector3(0,0,0) );
				const btVector3 externalForceImpulseB( bodyB.m_originalBody ? bodyB.m_externalForceImpulse : btVector3(0,0,0) );
				const btVector3 externalTorqueImpulseB( bodyB.m_originalBody ? bodyB.m_externalTorqueImpulse : btVector3(0,0,0) );
				
				const btScalar vel1Dotn = solver.m_contactNormal1.dot( rbA.getLinearVelocity() + externalForceImpulseA )
					+ solver.m_relpos1CrossNormal.dot( rbA.getAngularVelocity() + externalTorqueImpulseA );
				const btScalar vel2Dotn = solver.m_contactNormal2.dot( rbB.getLinearVelocity() + externalForceImpulseB )
					+ solver.m_relpos2CrossNormal.dot( rbB.getAngularVelocity() + externalTorqueImpulseB );
				const btScalar rel_vel = vel1Dotn + vel2Dotn;
				solver.m_rhs = -rel_vel * solver.m_jacDiagABInv * solver.m_friction;
				
				printf( "setup(%d): j1l=(%f,%f,%f) rhs=%f(%f) fric=%f\n", j, solver.m_contactNormal1.getX(),
					solver.m_contactNormal1.getY(), solver.m_contactNormal1.getZ(), solver.m_rhs,
						-rel_vel * solver.m_jacDiagABInv, solver.m_friction );
			}
			
			constraintRow += constraintRowCount;
			frictionRow += frictionRowCount;
		}
	}
	
	return returnValueWithoutUse;
#endif
}

btScalar debpConstraintSolver::solveSingleIteration( int iteration,
btCollisionObject **bodies, int numBodies, btPersistentManifold **manifoldPtr,
int numManifolds, btTypedConstraint **constraints, int numConstraints,
const btContactSolverInfo &infoGlobal, btIDebugDraw *debugDrawer ){
	return btMultiBodyConstraintSolver::solveSingleIteration( iteration, bodies, numBodies,
		manifoldPtr, numManifolds, constraints, numConstraints, infoGlobal, debugDrawer );
	
#if 0
	btScalar leastSquaresResidual =
		btMultiBodyConstraintSolver::solveSingleIteration( iteration, bodies, numBodies,
		manifoldPtr, numManifolds, constraints, numConstraints, infoGlobal, debugDrawer );
	
	if( iteration >= infoGlobal.m_numIterations ){
		return leastSquaresResidual;
	}
	
	// constraint friction. the idea is the following. m_rhs contains the body velocity and
	// external forces to compensate. by now the m_appliedImpulse contains also the impulse
	// build up during constraint fixing. this impulse modified by m_friction has to be
	// added to m_rhs (more impulse to componensate). after resolving m_rhs has to be
	// restored for the next iteration to operate on the correct values again. this is
	// required since the impulse does not build up during the iterations but is refined
	// to better solve the equations.
	const int solverCount = pPoolSolverConstraintFriction.size();
	int i;
	
	for( i=0; i<solverCount; i++ ){
		btSolverConstraint &solver = pPoolSolverConstraintFriction[ i ];
		const btScalar totalImpulse = solver.m_friction * m_tmpSolverNonContactConstraintPool[ solver.m_frictionIndex ].m_appliedImpulse;
		const btScalar rhs = solver.m_rhs;
		
		if( iteration == 0 ){
			printf( "solve(%d): fric=%f rhs=%f totimp=%f(%f)\n", i, solver.m_friction, solver.m_rhs,
				totalImpulse, m_tmpSolverNonContactConstraintPool[ solver.m_frictionIndex ].m_appliedImpulse );
		}
		
		solver.m_rhs -= totalImpulse;
		const btScalar residual = resolveSingleConstraintRowGeneric(
			m_tmpSolverBodyPool[ solver.m_solverBodyIdA ], m_tmpSolverBodyPool[ solver.m_solverBodyIdB ], solver );
		leastSquaresResidual += residual * residual;
		solver.m_rhs = rhs;
	}
	
	return leastSquaresResidual;
#endif
}

btScalar debpConstraintSolver::solveGroupCacheFriendlyFinish(
btCollisionObject **bodies, int numBodies, const btContactSolverInfo &infoGlobal ){
	//pPoolSolverConstraintFriction.resizeNoInitialize( 0 );
	
	return btMultiBodyConstraintSolver::solveGroupCacheFriendlyFinish( bodies, numBodies, infoGlobal );
}
