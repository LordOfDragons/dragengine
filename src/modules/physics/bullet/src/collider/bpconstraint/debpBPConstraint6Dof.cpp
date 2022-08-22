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

#include "debpBPConstraint6Dof.h"
#include "../debpColliderConstraint.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/collider/deColliderConstraint.h>



// Class debpBPConstraint6Dof
///////////////////////////////

// Constructor, destructor
////////////////////////////

debpBPConstraint6Dof::debpBPConstraint6Dof( debpColliderConstraint &constraint,
btRigidBody &rbA, btRigidBody &rbB, const btTransform &frameInA, const btTransform &frameInB ) :
btGeneric6DofConstraint( rbA, rbB, frameInA, frameInB, true ),
pConstraint( constraint ),
pIndexMotorX( 0 ),
pIndexMotorY( 1 ),
pIndexMotorZ( 2 ),
pDamping( BT_ONE )
{
	int i;
	for( i=0; i<3; i++ ){
		pHasJointFrictionLinear[ i ] = false;
		pHasJointFrictionAngular[ i ] = false;
	}
	
	setUserConstraintPtr( ( debpBPConstraintBase* )this );
}

debpBPConstraint6Dof::debpBPConstraint6Dof( debpColliderConstraint &constraint,
btRigidBody &rbA, const btTransform &frameInA, const btTransform &frameInB ) :
btGeneric6DofConstraint( rbA, getFixedBody(), frameInA, frameInB, true ),
pConstraint( constraint ),
pIndexMotorX( 0 ),
pIndexMotorY( 1 ),
pIndexMotorZ( 2 ),
pDamping( BT_ONE )
{
	int i;
	for( i=0; i<3; i++ ){
		pHasJointFrictionLinear[ i ] = false;
		pHasJointFrictionAngular[ i ] = false;
	}
	
	setUserConstraintPtr( ( debpBPConstraintBase* )this );
}

debpBPConstraint6Dof::~debpBPConstraint6Dof(){
	if( m_jointFeedback ){
		delete m_jointFeedback;
	}
}



// Management
///////////////

void debpBPConstraint6Dof::SetMotorIndices( int x, int y, int z ){
	pIndexMotorX = x;
	pIndexMotorY = y;
	pIndexMotorZ = z;
}

void debpBPConstraint6Dof::SaveLimits(){
	pOrgLimitLinearLower[ pIndexMotorX ] = m_linearLimits.m_lowerLimit.getX();
	pOrgLimitLinearLower[ pIndexMotorY ] = m_linearLimits.m_lowerLimit.getY();
	pOrgLimitLinearLower[ pIndexMotorZ ] = m_linearLimits.m_lowerLimit.getZ();
	
	pOrgLimitLinearUpper[ pIndexMotorX ] = m_linearLimits.m_upperLimit.getX();
	pOrgLimitLinearUpper[ pIndexMotorY ] = m_linearLimits.m_upperLimit.getY();
	pOrgLimitLinearUpper[ pIndexMotorZ ] = m_linearLimits.m_upperLimit.getZ();
	
	pOrgLimitAngularLower[ 0 ] = m_angularLimits[ 0 ].m_loLimit;
	pOrgLimitAngularLower[ 1 ] = m_angularLimits[ 1 ].m_loLimit;
	pOrgLimitAngularLower[ 2 ] = m_angularLimits[ 2 ].m_loLimit;
	
	pOrgLimitAngularUpper[ 0 ] = m_angularLimits[ 0 ].m_hiLimit;
	pOrgLimitAngularUpper[ 1 ] = m_angularLimits[ 1 ].m_hiLimit;
	pOrgLimitAngularUpper[ 2 ] = m_angularLimits[ 2 ].m_hiLimit;
}

void debpBPConstraint6Dof::SetHasJointFrictionLinear( int index, bool enabled ){
	if( index < 0 || index >= 3 ){
		DETHROW( deeInvalidParam );
	}
	pHasJointFrictionLinear[ index ] = enabled;
}

void debpBPConstraint6Dof::SetHasJointFrictionAngular( int index, bool enabled ){
	if( index < 0 || index >= 3 ){
		DETHROW( deeInvalidParam );
	}
	pHasJointFrictionAngular[ index ] = enabled;
}

void debpBPConstraint6Dof::SetDamping( btScalar damping ){
	pDamping = damping;
}

void debpBPConstraint6Dof::PrepareForStep(){
	if( ! m_jointFeedback ){
		return;
	}
	
#if 0
	printf( "6DOF.PrepareForStep: jfb.force=%f pos=%f velo=%f force=%f\n", m_jointFeedback->m_appliedForceBodyA.y(),
		m_rbA.getWorldTransform().getOrigin().getY(), m_rbA.getLinearVelocity().getY(), m_rbA.getTotalForce().getY() );
	
	// this is not working as expected. bullet has huge problems with this static friction
	// situation. disabled for the time being.
	
	printf( "6DOF.JointFeedback: appliedForceBodyA.y = %g\n", m_jointFeedback->m_appliedForceBodyA.y() );
	
	if( pFirstStep ){
		pFirstStep = false;
		calculateTransforms();
	}
	
	printf( "OOPS: %g %g\n", m_rbA.getWorldTransform().getOrigin().getY(), m_rbB.getWorldTransform().getOrigin().getY() );
	
	// update motors. note the forces are reversed. they contain the force applied to fix the
	// constraint error not the force acting on the constraint. hence gravitation and objects
	// pushing down cause an upward applied force. same goes for applied torque
	// 
	// NOTE joint feedback contains applied force and torque for both bodies. due to physics this has
	//      to be opposite and equal in length thus it is only required to look at one of the two forces.
	//      body b can be the world and thus static but body a is by definition always a true body. using
	//      body a applied force and torque is thus the best solution
	// 
	// NOTE the constraint has to track if the joint friction state is static or kinematic. if not
	//      the current state is used causing the constraint to drift over time. by tracking this
	//      state the current position can be stored at the time of switching to static type to
	//      keep the constraint at the same position until it starts moving again.
	// 
	// NOTE switching between static and kinematic state has to be done over multiple simulation steps.
	//      if switching check is done every simulation step the constraint switches constantly between
	//      static and kinematic due to the impulse solver requiring a simulation step or two to
	//      stabilize movement. switching from static to kinematic has to be done the first simulation
	//      step the condition is detected. switching to static should be done only after for example
	//      5 simulation steps passed without detecting kinematic state again
	// 
	// NOTE static joint friction is the force required to deblock the joint and to get it moving.
	//      kinematic joint friction is the force-per-angle applied against the pushing force. if
	//      this force is larger than the pushing force the joint blocks again reverting to static
	//      friction.
	// 
	// NOTE use an own constraint class to handle joint friction since the handling is complex and
	//      needs to track states. also allows to add this constraint to any other constraint
	//      without needing force the constraint to be of generic 6dof type
	const deColliderConstraint &engConstraint = pConstraint.GetConstraint();
	
	if( pHasJointFrictionLinear[ 0 ] || pHasJointFrictionLinear[ 1 ] || pHasJointFrictionLinear[ 2 ] ){
		const btVector3 appliedForce( m_jointFeedback->m_appliedForceBodyA.absolute() );
		const btVector3 linearVelocity( ( m_rbA.getLinearVelocity() - m_rbB.getLinearVelocity() ).absolute() );
		
		if( pHasJointFrictionLinear[ pIndexMotorX ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			
			if( linearVelocity.dot( axis ) < ( btScalar )0.001
			&& appliedForce[ pIndexMotorX ] < ( btScalar )engConstraint.GetDofLinearX().GetKinematicFriction() ){
				if( m_linearLimits.m_enableMotor[ pIndexMotorX ] ){
					m_linearLimits.m_lowerLimit[ pIndexMotorX ] = getRelativePivotPosition( pIndexMotorX );
					m_linearLimits.m_upperLimit[ pIndexMotorX ] = m_linearLimits.m_lowerLimit[ pIndexMotorX ];
					m_linearLimits.m_enableMotor[ pIndexMotorX ] = false;
				}
				
			}else{
				m_linearLimits.m_lowerLimit[ pIndexMotorX ] = pOrgLimitLinearLower[ pIndexMotorX ];
				m_linearLimits.m_upperLimit[ pIndexMotorX ] = pOrgLimitLinearUpper[ pIndexMotorX ];
				m_linearLimits.m_enableMotor[ pIndexMotorX ] = true;
			}
		}
		
		if( pHasJointFrictionLinear[ pIndexMotorY ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			
			if( linearVelocity.dot( axis ) < ( btScalar )0.001
			&& appliedForce[ pIndexMotorY ] < ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction() ){
				if( m_linearLimits.m_enableMotor[ pIndexMotorY ] ){
					m_linearLimits.m_lowerLimit[ pIndexMotorY ] = getRelativePivotPosition( pIndexMotorY );
					m_linearLimits.m_upperLimit[ pIndexMotorY ] = m_linearLimits.m_lowerLimit[ pIndexMotorY ];
					m_linearLimits.m_enableMotor[ pIndexMotorY ] = false;
				}
				printf( "6DOF.JointFeedback: LESS: lv=%g(%g,%g) af=%g kf=%g -> %g %g\n", linearVelocity.dot( axis ),
					m_rbA.getLinearVelocity().y(), m_rbB.getLinearVelocity().y(),
					appliedForce[ pIndexMotorY ], ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction(),
					m_linearLimits.m_upperLimit[ pIndexMotorY ], m_linearLimits.m_lowerLimit[ pIndexMotorY ] );
				
			}else{
				m_linearLimits.m_lowerLimit[ pIndexMotorY ] = pOrgLimitLinearLower[ pIndexMotorY ];
				m_linearLimits.m_upperLimit[ pIndexMotorY ] = pOrgLimitLinearUpper[ pIndexMotorY ];
				m_linearLimits.m_enableMotor[ pIndexMotorY ] = true;
				printf( "6DOF.JointFeedback: LARGER: lv=%g(%g,%g) af=%g kf=%g -> %g %g\n", linearVelocity.dot( axis ),
					m_rbA.getLinearVelocity().y(), m_rbB.getLinearVelocity().y(),
					appliedForce[ pIndexMotorY ], ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction(),
					m_linearLimits.m_upperLimit[ pIndexMotorY ], m_linearLimits.m_lowerLimit[ pIndexMotorY ] );
			}
		}
		
		if( pHasJointFrictionLinear[ pIndexMotorZ ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			
			if( linearVelocity.dot( axis ) < ( btScalar )0.001
			&& appliedForce[ pIndexMotorZ ] < ( btScalar )engConstraint.GetDofLinearZ().GetKinematicFriction() ){
				if( m_linearLimits.m_enableMotor[ pIndexMotorZ ] ){
					m_linearLimits.m_lowerLimit[ pIndexMotorZ ] = getRelativePivotPosition( pIndexMotorZ );
					m_linearLimits.m_upperLimit[ pIndexMotorZ ] = m_linearLimits.m_lowerLimit[ pIndexMotorZ ];
					m_linearLimits.m_enableMotor[ pIndexMotorZ ] = false;
				}
				
			}else{
				m_linearLimits.m_lowerLimit[ pIndexMotorZ ] = pOrgLimitLinearLower[ pIndexMotorZ ];
				m_linearLimits.m_upperLimit[ pIndexMotorZ ] = pOrgLimitLinearUpper[ pIndexMotorZ ];
				m_linearLimits.m_enableMotor[ pIndexMotorZ ] = true;
			}
		}
	}
	
	if( pHasJointFrictionAngular[ 0 ] || pHasJointFrictionAngular[ 1 ] || pHasJointFrictionAngular[ 2 ] ){
		const btVector3 appliedTorque( m_jointFeedback->m_appliedTorqueBodyA.absolute() );
		const btVector3 angularVelocity( ( m_rbA.getAngularVelocity() - m_rbB.getAngularVelocity() ).absolute() );
		btRotationalLimitMotor &motorX = m_angularLimits[ pIndexMotorX ];
		btRotationalLimitMotor &motorY = m_angularLimits[ pIndexMotorY ];
		btRotationalLimitMotor &motorZ = m_angularLimits[ pIndexMotorZ ];
		
		if( pHasJointFrictionAngular[ pIndexMotorX ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			
			if( angularVelocity.dot( axis ) < ( btScalar )0.01
			&& appliedTorque[ pIndexMotorX ] < ( btScalar )engConstraint.GetDofAngularX().GetKinematicFriction() ){
				if( motorX.m_enableMotor ){
					motorX.m_loLimit = getAngle( pIndexMotorX );
					motorX.m_hiLimit = motorX.m_loLimit;
					motorX.m_enableMotor = false;
				}
				
			}else{
				motorX.m_loLimit = pOrgLimitAngularLower[ pIndexMotorX ];
				motorX.m_hiLimit = pOrgLimitAngularUpper[ pIndexMotorX ];
				motorX.m_enableMotor = true;
			}
		}
		
		if( pHasJointFrictionAngular[ pIndexMotorY ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			
			if( angularVelocity.dot( axis ) < ( btScalar )0.01
			&& appliedTorque[ pIndexMotorY ] < ( btScalar )engConstraint.GetDofAngularY().GetKinematicFriction() ){
				if( motorY.m_enableMotor ){
					motorY.m_loLimit = getAngle( pIndexMotorY );
					motorY.m_hiLimit = motorY.m_loLimit;
					motorY.m_enableMotor = false;
				}
				
			}else{
				motorY.m_loLimit = pOrgLimitAngularLower[ pIndexMotorY ];
				motorY.m_hiLimit = pOrgLimitAngularUpper[ pIndexMotorY ];
				motorY.m_enableMotor = true;
			}
		}
		
		if( pHasJointFrictionAngular[ pIndexMotorZ ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			
			if( angularVelocity.dot( axis ) < ( btScalar )0.01
			&& appliedTorque[ pIndexMotorZ ] < ( btScalar )engConstraint.GetDofAngularZ().GetKinematicFriction() ){
				if( motorZ.m_enableMotor ){
					motorZ.m_loLimit = getAngle( pIndexMotorZ );
					motorZ.m_hiLimit = motorZ.m_loLimit;
					motorZ.m_enableMotor = false;
				}
				
			}else{
				motorZ.m_loLimit = pOrgLimitAngularLower[ pIndexMotorZ ];
				motorZ.m_hiLimit = pOrgLimitAngularUpper[ pIndexMotorZ ];
				motorZ.m_enableMotor = true;
			}
		}
	}
#endif
}



int debpBPConstraint6Dof::GetConstraintFrictionCount(){
	return 0;
	/*
	int rowCount = 0;
	
	if( pHasJointFrictionLinear[ pIndexMotorX ] ){
		rowCount++;
	}
	if( pHasJointFrictionLinear[ pIndexMotorY ] ){
		rowCount++;
	}
	if( pHasJointFrictionLinear[ pIndexMotorZ ] ){
		rowCount++;
	}
	if( pHasJointFrictionAngular[ pIndexMotorX ] ){
		rowCount++;
	}
	if( pHasJointFrictionAngular[ pIndexMotorY ] ){
		rowCount++;
	}
	if( pHasJointFrictionAngular[ pIndexMotorZ ] ){
		rowCount++;
	}
	
	return rowCount;
	*/
}

void debpBPConstraint6Dof::GetConstraintFriction( btTypedConstraint::btConstraintInfo2 &info ){
	const deColliderConstraint &engConstraint = pConstraint.GetConstraint();
	int row = 0;
	
	// equation for linear equations: p*v - q*v = rhs. p and q are filled by the constraint.
	// v and rhs are filled in by the constraint solver. friction is stored in upper and
	// lower limits due to lack of a better place to put it
	if( pHasJointFrictionLinear[ pIndexMotorX ] ){
		const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
		const btScalar friction = ( btScalar )engConstraint.GetDofLinearX().GetKinematicFriction();
		
		info.m_J1linearAxis[ row + 0 ] = axis.getX();
		info.m_J1linearAxis[ row + 1 ] = axis.getY();
		info.m_J1linearAxis[ row + 2 ] = axis.getZ();
		info.m_J2linearAxis[ row + 0 ] = -axis.getX();
		info.m_J2linearAxis[ row + 1 ] = -axis.getY();
		info.m_J2linearAxis[ row + 2 ] = -axis.getZ();
		info.m_upperLimit[ row ] = friction;
		info.m_lowerLimit[ row ] = -friction;
		
		row += info.rowskip;
	}
	
	if( pHasJointFrictionLinear[ pIndexMotorY ] ){
		const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
		const btScalar friction = ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction();
		
		info.m_J1linearAxis[ row + 0 ] = axis.getX();
		info.m_J1linearAxis[ row + 1 ] = axis.getY();
		info.m_J1linearAxis[ row + 2 ] = axis.getZ();
		info.m_J2linearAxis[ row + 0 ] = -axis.getX();
		info.m_J2linearAxis[ row + 1 ] = -axis.getY();
		info.m_J2linearAxis[ row + 2 ] = -axis.getZ();
		info.m_upperLimit[ row ] = friction;
		info.m_lowerLimit[ row ] = -friction;
		
		row += info.rowskip;
	}
	
	if( pHasJointFrictionLinear[ pIndexMotorZ ] ){
		const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
		const btScalar friction = ( btScalar )engConstraint.GetDofLinearZ().GetKinematicFriction();
		
		info.m_J1linearAxis[ row + 0 ] = axis.getX();
		info.m_J1linearAxis[ row + 1 ] = axis.getY();
		info.m_J1linearAxis[ row + 2 ] = axis.getZ();
		info.m_J2linearAxis[ row + 0 ] = -axis.getX();
		info.m_J2linearAxis[ row + 1 ] = -axis.getY();
		info.m_J2linearAxis[ row + 2 ] = -axis.getZ();
		info.m_upperLimit[ row ] = friction;
		info.m_lowerLimit[ row ] = -friction;
		
		row += info.rowskip;
	}
	
	if( pHasJointFrictionAngular[ pIndexMotorX ] ){
		//info.friction[ row ] = invMass * ( btScalar )engConstraint.GetDofAngularX().GetKinematicFriction();
		//row += info.rowSkip;
	}
	
	if( pHasJointFrictionAngular[ pIndexMotorY ] ){
		//info.friction[ row ] = invMass * ( btScalar )engConstraint.GetDofAngularY().GetKinematicFriction();
		//row += info.rowSkip;
	}
	
	if( pHasJointFrictionAngular[ pIndexMotorZ ] ){
		//info.friction[ row ] = invMass * ( btScalar )engConstraint.GetDofAngularZ().GetKinematicFriction();
		//row += info.rowSkip;
	}
}



void debpBPConstraint6Dof::getInfo1( btTypedConstraint::btConstraintInfo1 *info ){
	// we have to enable motors for joint frictions to get the right number of rows
	m_linearLimits.m_enableMotor[ 0 ] = pHasJointFrictionLinear[ 0 ];
	m_linearLimits.m_enableMotor[ 1 ] = pHasJointFrictionLinear[ 1 ];
	m_linearLimits.m_enableMotor[ 2 ] = pHasJointFrictionLinear[ 2 ];
	m_angularLimits[ 0 ].m_enableMotor = pHasJointFrictionAngular[ 0 ];
	m_angularLimits[ 1 ].m_enableMotor = pHasJointFrictionAngular[ 1 ];
	m_angularLimits[ 2 ].m_enableMotor = pHasJointFrictionAngular[ 2 ];
	
	btGeneric6DofConstraint::getInfo1( info );
}

#if 0
void debpBPConstraint6Dof::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	btGeneric6DofConstraint::getInfo2( info );
}
#endif

//#if 0
void debpBPConstraint6Dof::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	// update motor to apply friction force.
	// 
	// the kinematic joint friction has the dimenion [F/s] whereas s is distance for linear
	// degrees of freedom and rotation angle for angular degrees of freedom. both velocity and
	// external force have to be converted to [s] before applying the kinematic joint friction.
	// velocity is simple: s=v*dt
	// external force: J=F*dt=m*v, v=s/dt -> F*dt=m*s/dt -> s=F*dt*dt/m
	// this results in the friction impulse (instead of force since s is per time step and
	// therefore friction is per time step too). the motor max force is impulse in contrary
	// to what the name makes you believe. no further calculation is required.
	
	// NOTE
	// gravity 9.81
	// kinematic friction coefficient 60 just about compensates for gravity
	// kinematic friction coefficient 75 and 1kg box gets it moving
	// kinematic friction coefficient 100 and 1kg box not moving but 2kg and up is enough
	// kinematic friction coefficient 200 and 2kg box not moving but 3kg and 4kg enough
	// 
	// for angular rotation coefficients have to be higher.
	// 
	
	const deColliderConstraint &engConstraint = pConstraint.GetConstraint();
	const btScalar timeStep = BT_ONE / info->fps;
	
// 	info->m_damping = pDamping; // not working too U_U
	
	if( pHasJointFrictionLinear[ 0 ] || pHasJointFrictionLinear[ 1 ] || pHasJointFrictionLinear[ 2 ] ){
		const btVector3 velocity( m_rbA.getLinearVelocity() - m_rbB.getLinearVelocity() );
		//const btVector3 velocityMass( m_rbA.getLinearVelocity() * pGetMassA() - m_rbB.getLinearVelocity() * pGetMassB() );
		//const btVector3 externalForce( m_rbA.getTotalForce() - m_rbB.getTotalForce() );
		const btVector3 externalForceInvMass( m_rbA.getTotalForce() * m_rbA.getInvMass() - m_rbB.getTotalForce() * m_rbB.getInvMass() );
		
		m_linearLimits.m_targetVelocity.setZero();
		
		if( pHasJointFrictionLinear[ pIndexMotorX ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			const btScalar friction = btFabs( velocity.dot( axis ) + externalForceInvMass.dot( axis ) * timeStep )
				* timeStep * ( btScalar )engConstraint.GetDofLinearX().GetKinematicFriction();
			//const btScalar force = velocityMass.dot( axis ) * info->fps + externalForce.dot( axis ) * timeStep;
			
			m_linearLimits.m_targetVelocity[ pIndexMotorX ] = BT_ZERO;
			m_linearLimits.m_maxMotorForce[ pIndexMotorX ] = friction;
			m_linearLimits.m_enableMotor[ pIndexMotorX ] = true;
			/*
			if( force < BT_ZERO ){
				pLeftoverFrictionLinear[ pIndexMotorX ] = btMax( friction + force, BT_ZERO );
				
			}else{
				pLeftoverFrictionLinear[ pIndexMotorX ] = btMax( -force - friction, BT_ZERO );
			}
			*/
		}
		
		if( pHasJointFrictionLinear[ pIndexMotorY ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			const btScalar friction = btFabs( velocity.dot( axis ) + externalForceInvMass.dot( axis ) * timeStep )
				* timeStep * ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction();
			
			m_linearLimits.m_targetVelocity[ pIndexMotorY ] = BT_ZERO;
			m_linearLimits.m_maxMotorForce[ pIndexMotorY ] = friction;
			m_linearLimits.m_enableMotor[ pIndexMotorY ] = true;
			/*
			printf( "6DOF.getInfo2: pos=%f velo=%f force=%f(%f) fric=%f move=%f accumimp=%f\n",
				m_rbA.getWorldTransform().getOrigin().getY(), velocity.getY(),
				( m_rbA.getTotalForce().getY() - m_rbB.getTotalForce().getY() ) * timeStep,
				m_rbA.getTotalForce().getY() - m_rbB.getTotalForce().getY(), friction,
				btFabs( velocity.dot( axis ) + externalForceInvMass.dot( axis ) * timeStep ) * timeStep,
				m_linearLimits.m_accumulatedImpulse[ pIndexMotorY ] );
			*/
		}
		
		if( pHasJointFrictionLinear[ pIndexMotorZ ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			const btScalar friction = btFabs( velocity.dot( axis ) + externalForceInvMass.dot( axis ) * timeStep )
				* timeStep * ( btScalar )engConstraint.GetDofLinearZ().GetKinematicFriction();
			
			m_linearLimits.m_targetVelocity[ pIndexMotorZ ] = BT_ZERO;
			m_linearLimits.m_maxMotorForce[ pIndexMotorZ ] = friction;
			m_linearLimits.m_enableMotor[ pIndexMotorZ ] = true;
		}
	}
	
	if( pHasJointFrictionAngular[ 0 ] || pHasJointFrictionAngular[ 1 ] || pHasJointFrictionAngular[ 2 ] ){
		const btVector3 velocity( m_rbA.getAngularVelocity() - m_rbB.getAngularVelocity() );
		const btVector3 externalTorqueInvInertia( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld()
			- m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() );
		const btVector3 externalForceInvMass( (-m_frameInA.getOrigin()).cross( m_rbA.getTotalForce() ) * m_rbA.getInvMass()
			- (-m_frameInB.getOrigin()).cross( m_rbB.getTotalForce() ) * m_rbB.getInvMass() );
		
		if( pHasJointFrictionAngular[ pIndexMotorX ] ){
			const btVector3 axis( getAxis( pIndexMotorX ) );
			const btScalar friction = btFabs( velocity.dot( axis )
				+ ( externalTorqueInvInertia + externalForceInvMass ).dot( axis ) * timeStep )
				* timeStep * ( btScalar )engConstraint.GetDofAngularX().GetKinematicFriction();
			
			m_angularLimits[ pIndexMotorX ].m_targetVelocity = BT_ZERO;
			m_angularLimits[ pIndexMotorX ].m_maxMotorForce = friction;
			m_angularLimits[ pIndexMotorX ].m_enableMotor = true;
			/*
			printf( "6DOF.getInfo2: pos=%f velo=%f torque=%f(%f) force=%f(%f) fric=%f move=%f accumimp=%f\n",
				m_rbA.getWorldTransform().getOrigin().getY(), velocity.getY(),
				( m_rbA.getTotalTorque().getX() - m_rbB.getTotalTorque().getY() ) * timeStep,
				m_rbA.getTotalTorque().getX() - m_rbB.getTotalTorque().getY(),
				((-m_frameInA.getOrigin()).cross( m_rbA.getTotalForce() ).getY()
					- (-m_frameInB.getOrigin()).cross( m_rbB.getTotalForce() ).getY()) * timeStep,
				(-m_frameInA.getOrigin()).cross( m_rbA.getTotalForce() ).getY()
					- (-m_frameInB.getOrigin()).cross( m_rbB.getTotalForce() ).getY(), friction,
				btFabs( velocity.dot( axis ) + ( externalTorqueInvInertia + externalForceInvMass ).dot( axis ) * timeStep ) * timeStep,
				m_angularLimits[ pIndexMotorX ].m_accumulatedImpulse );
			*/
		}
		
		if( pHasJointFrictionAngular[ pIndexMotorY ] ){
			const btVector3 axis( getAxis( pIndexMotorY ) );
			const btScalar friction = btFabs( velocity.dot( axis )
				+ ( externalTorqueInvInertia + externalForceInvMass ).dot( axis ) * timeStep )
				* timeStep * ( btScalar )engConstraint.GetDofAngularY().GetKinematicFriction();
			
			m_angularLimits[ pIndexMotorY ].m_targetVelocity = BT_ZERO;
			m_angularLimits[ pIndexMotorY ].m_maxMotorForce = friction;
			m_angularLimits[ pIndexMotorY ].m_enableMotor = true;
		}
		
		if( pHasJointFrictionAngular[ pIndexMotorZ ] ){
			const btVector3 axis( getAxis( pIndexMotorZ ) );
			const btScalar friction = btFabs( velocity.dot( axis )
				+ ( externalTorqueInvInertia + externalForceInvMass ).dot( axis ) * timeStep )
				* timeStep * ( btScalar )engConstraint.GetDofAngularZ().GetKinematicFriction();
			
			m_angularLimits[ pIndexMotorZ ].m_targetVelocity = BT_ZERO;
			m_angularLimits[ pIndexMotorZ ].m_maxMotorForce = friction;
			m_angularLimits[ pIndexMotorZ ].m_enableMotor = true;
		}
	}
	
	btGeneric6DofConstraint::getInfo2( info );
}
//#endif

#if 0
// this is not working since bullet fucks up damping big time. using damping
// makes the constraint explode and bounce like a pogo-stick. totally useless
void debpBPConstraint6Dof::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	btGeneric6DofConstraint::getInfo2( info );
	
	// kinematic joint friction can be simulated using damping since the kinematic friction
	// is a dimensionless coefficient. requires the damping patch to allow per-row damping
	// 
	// NOTE btGeneric6DofConstraint solved angular limits before linear limits for stability.
	//      thus linear limits are offset by 3 instead of angular limits
	
	if( pHasJointFrictionLinear[ pIndexMotorX ] ){
		info->m_damping[ 3 + pIndexMotorX ] = BT_ONE - ( btScalar )pConstraint.GetConstraint().GetDofLinearX().GetKinematicFriction();
	}
	if( pHasJointFrictionLinear[ pIndexMotorY ] ){
		info->m_damping[ 3 + pIndexMotorY ] = BT_ONE - ( btScalar )pConstraint.GetConstraint().GetDofLinearY().GetKinematicFriction();
	}
	if( pHasJointFrictionLinear[ pIndexMotorZ ] ){
		info->m_damping[ 3 + pIndexMotorZ ] = BT_ONE - ( btScalar )pConstraint.GetConstraint().GetDofLinearZ().GetKinematicFriction();
	}
	
	if( pHasJointFrictionAngular[ pIndexMotorX ] ){
		info->m_damping[ pIndexMotorX ] = BT_ONE - ( btScalar )pConstraint.GetConstraint().GetDofAngularX().GetKinematicFriction();
	}
	if( pHasJointFrictionAngular[ pIndexMotorY ] ){
		info->m_damping[ pIndexMotorY ] = BT_ONE - ( btScalar )pConstraint.GetConstraint().GetDofAngularY().GetKinematicFriction();
	}
	if( pHasJointFrictionAngular[ pIndexMotorZ ] ){
		info->m_damping[ pIndexMotorZ ] = BT_ONE - ( btScalar )pConstraint.GetConstraint().GetDofAngularZ().GetKinematicFriction();
	}
}
#endif

#if 0
void debpBPConstraint6Dof::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	// inner joint frictions are modeled using a motor which tries to achieve a target velocity
	// of 0 but only with a maximum force. the force is set to the joint friction either static
	// if not moving or kinematic if moving. this results in a constraint canceling velocities
	// unless the force goes beyond the force the friction can muster up.
	// 
	// NOTE using btScalar* since btVector3 does not support operator[] only operator btScalar*
	const deColliderConstraint &engConstraint = pConstraint.GetConstraint();
	const btScalar timeStep = BT_ONE / info->fps;
	
	if( pHasJointFrictionLinear[ 0 ] || pHasJointFrictionLinear[ 1 ] || pHasJointFrictionLinear[ 2 ] ){
		m_linearLimits.m_targetVelocity.setZero();
		
		if( pHasJointFrictionLinear[ pIndexMotorX ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			const btScalar velocity = m_rbA.getLinearVelocity().dot( axis ) - m_rbB.getLinearVelocity().dot( axis );
			const btScalar externalVelocity = m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() * timeStep
				- m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() * timeStep;
			const btScalar kinematicFriction = ( btScalar )engConstraint.GetDofLinearX().GetKinematicFriction();
			
			m_linearLimits.m_maxMotorForce[ pIndexMotorX ] = kinematicFriction * btFabs( velocity + externalVelocity ) * timeStep;
		}
		
		if( pHasJointFrictionLinear[ pIndexMotorY ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			const btScalar velocity = m_rbA.getLinearVelocity().dot( axis ) - m_rbB.getLinearVelocity().dot( axis );
			const btScalar externalVelocity = m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() * timeStep
				- m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() * timeStep;
			const btScalar kinematicFriction = ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction();
			
			m_linearLimits.m_maxMotorForce[ pIndexMotorY ] = kinematicFriction * btFabs( velocity + externalVelocity ) * timeStep;
		}
		
		if( pHasJointFrictionLinear[ pIndexMotorZ ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			const btScalar velocity = m_rbA.getLinearVelocity().dot( axis ) - m_rbB.getLinearVelocity().dot( axis );
			const btScalar externalVelocity = m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() * timeStep
				- m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() * timeStep;
			const btScalar kinematicFriction = ( btScalar )engConstraint.GetDofLinearZ().GetKinematicFriction();
			
			m_linearLimits.m_maxMotorForce[ pIndexMotorZ ] = kinematicFriction * btFabs( velocity + externalVelocity ) * timeStep;
		}
	}
	
	if( pHasJointFrictionAngular[ 0 ] || pHasJointFrictionAngular[ 1 ] || pHasJointFrictionAngular[ 2 ] ){
		if( pHasJointFrictionAngular[ pIndexMotorX ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			const btScalar velocity = m_rbA.getAngularVelocity().dot( axis ) - m_rbB.getAngularVelocity().dot( axis );
			const btScalar externalVelocity = ( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld() ).dot( axis ) * timeStep
				- ( m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() ).dot( axis ) * timeStep;
			const btScalar kinematicFriction = ( btScalar )engConstraint.GetDofAngularX().GetKinematicFriction();
			
			m_angularLimits[ pIndexMotorX ].m_maxMotorForce = kinematicFriction * btFabs( velocity + externalVelocity ) * timeStep;
		}
		
		if( pHasJointFrictionAngular[ pIndexMotorY ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			const btScalar velocity = m_rbA.getAngularVelocity().dot( axis ) - m_rbB.getAngularVelocity().dot( axis );
			const btScalar externalVelocity = ( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld() ).dot( axis ) * timeStep
				- ( m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() ).dot( axis ) * timeStep;
			const btScalar kinematicFriction = ( btScalar )engConstraint.GetDofAngularY().GetKinematicFriction();
			
			m_angularLimits[ pIndexMotorY ].m_maxMotorForce = kinematicFriction * btFabs( velocity + externalVelocity ) * timeStep;
		}
		
		if( pHasJointFrictionAngular[ pIndexMotorZ ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			const btScalar velocity = m_rbA.getAngularVelocity().dot( axis ) - m_rbB.getAngularVelocity().dot( axis );
			const btScalar externalVelocity = ( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld() ).dot( axis ) * timeStep
				- ( m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() ).dot( axis ) * timeStep;
			const btScalar kinematicFriction = ( btScalar )engConstraint.GetDofAngularZ().GetKinematicFriction();
			
			m_angularLimits[ pIndexMotorZ ].m_maxMotorForce = kinematicFriction * btFabs( velocity + externalVelocity ) * timeStep;
		}
	}
	
	btGeneric6DofConstraint::getInfo2( info );
}
#endif

#if 0
void debpBPConstraint6Dof::getInfo2( btTypedConstraint::btConstraintInfo2 *info ){
	// friction is force [F]. for constraint error we need velocity [m/s].
	// F=am thus a=F/m. With v=at this is v=Ft/m
	// factor=t/m with t=1/fps this is factor=1/(fps*m)
	// With getInvMass(im)=1/m this is factor=im/fps
	// Using maximum mass this is max(1/m)=min(im) this is factor=min(im)/fps
	// 
	// NOTE see if using world as m_rbB could lead to problems due to max(mass) calculation
	// 
	// NOTE using btScalar* since btVector3 does not support operator[] only operator btScalar*
	const deColliderConstraint &engConstraint = pConstraint.GetConstraint();
	const btScalar timeStep = BT_ONE / info->fps;
	
	if( m_linearLimits.m_enableMotor[ 0 ] || m_linearLimits.m_enableMotor[ 1 ] || m_linearLimits.m_enableMotor[ 2 ] ){
		btScalar * const targetVelocity = ( btScalar* )m_linearLimits.m_targetVelocity;
		const btScalar factor = pForceFactor( info->fps );
		
		if( m_linearLimits.m_enableMotor[ pIndexMotorX ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			const btScalar linearVelocity = m_rbA.getLinearVelocity().dot( axis ) - m_rbB.getLinearVelocity().dot( axis )
				+ m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() * timeStep
				- m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() * timeStep;
			
			targetVelocity[ pIndexMotorX ] = BT_ZERO;
			
			// joint friction linear x axis
			const btScalar kinematicFriction = factor * ( btScalar )engConstraint.GetDofLinearX().GetKinematicFriction();
			
			if( linearVelocity > BT_ZERO ){
				targetVelocity[ pIndexMotorX ] -= decMath::min( kinematicFriction, linearVelocity );
				
			}else{
				targetVelocity[ pIndexMotorX ] += decMath::min( kinematicFriction, -linearVelocity );
			}
			
			printf( "BP6DOF.getInfo2: linearX: cv=%g kf=%g\n", linearVelocity, kinematicFriction );
		}
		
		if( m_linearLimits.m_enableMotor[ pIndexMotorY ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			const btScalar linearVelocity = m_rbA.getLinearVelocity().dot( axis ) - m_rbB.getLinearVelocity().dot( axis )
				+ m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() * timeStep
				- m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() * timeStep;
			
			targetVelocity[ pIndexMotorY ] = BT_ZERO;
			
			// joint friction linear y axis
			const btScalar kinematicFriction = factor * ( btScalar )engConstraint.GetDofLinearY().GetKinematicFriction();
			
			if( linearVelocity > BT_ZERO ){
				targetVelocity[ pIndexMotorY ] -= decMath::min( kinematicFriction, linearVelocity );
				
			}else{
				targetVelocity[ pIndexMotorY ] += decMath::min( kinematicFriction, -linearVelocity );
			}
			
			//targetVelocity[ pIndexMotorY ] = -linearVelocity;
			
			// PROBLEM:
			// getTotalForce and getTotalTorque are forces like gravity and manual pushes applied.
			// it does not contain forces applied by contact constraints. this does not work since
			// the two forces need to combine to work. need to look for manual damping to fix the
			// problem since this applies to the final velocity
			printf( "BP6DOF.getInfo2: linearY: cv=%g kf=%g efi=%g;%g\n", linearVelocity, kinematicFriction,
				m_rbA.getTotalForce().dot( axis ), m_rbB.getTotalForce().dot( axis ) );
			//( m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() - m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() ) / info->fps );
		}
		
		if( m_linearLimits.m_enableMotor[ pIndexMotorZ ] ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			const btScalar linearVelocity = m_rbA.getLinearVelocity().dot( axis ) - m_rbB.getLinearVelocity().dot( axis )
				+ m_rbA.getTotalForce().dot( axis ) * m_rbA.getInvMass() * timeStep
				- m_rbB.getTotalForce().dot( axis ) * m_rbB.getInvMass() * timeStep;
			
			targetVelocity[ pIndexMotorZ ] = BT_ZERO;
			
			// joint friction linear z axis
			const btScalar kinematicFriction = factor * ( btScalar )engConstraint.GetDofLinearZ().GetKinematicFriction();
			
			if( linearVelocity > BT_ZERO ){
				targetVelocity[ pIndexMotorZ ] -= decMath::min( kinematicFriction, linearVelocity );
				
			}else{
				targetVelocity[ pIndexMotorZ ] += decMath::min( kinematicFriction, -linearVelocity );
			}
			
			printf( "BP6DOF.getInfo2: linearZ: cv=%g kf=%g\n", linearVelocity, kinematicFriction );
		}
	}
	
	if( m_angularLimits[ 0 ].m_enableMotor || m_angularLimits[ 1 ].m_enableMotor || m_angularLimits[ 2 ].m_enableMotor ){
		btRotationalLimitMotor &motorX = m_angularLimits[ pIndexMotorX ];
		btRotationalLimitMotor &motorY = m_angularLimits[ pIndexMotorY ];
		btRotationalLimitMotor &motorZ = m_angularLimits[ pIndexMotorZ ];
		const btScalar factor = pForceFactor( info->fps );
		
		if( motorX.m_enableMotor ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorX ) );
			const btScalar angularVelocity = m_rbA.getAngularVelocity().dot( axis ) - m_rbB.getAngularVelocity().dot( axis )
				+ ( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld() ).dot( axis ) * timeStep
				- ( m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() ).dot( axis ) * timeStep;
			
			motorX.m_targetVelocity = BT_ZERO;
			
			// joint friction angular x axis
			const btScalar kinematicFriction = factor * ( btScalar )engConstraint.GetDofAngularX().GetKinematicFriction();
			
			if( angularVelocity > BT_ZERO ){
				motorX.m_targetVelocity -= decMath::min( kinematicFriction, angularVelocity );
				
			}else{
				motorX.m_targetVelocity += decMath::min( kinematicFriction, -angularVelocity );
			}
		}
		
		if( motorY.m_enableMotor ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorY ) );
			const btScalar angularVelocity = m_rbA.getAngularVelocity().dot( axis ) - m_rbB.getAngularVelocity().dot( axis )
				+ ( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld() ).dot( axis ) * timeStep
				- ( m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() ).dot( axis ) * timeStep;
			
			motorY.m_targetVelocity = BT_ZERO;
			
			// joint friction angular y axis
			const btScalar kinematicFriction = factor * ( btScalar )engConstraint.GetDofAngularY().GetKinematicFriction();
			
			if( angularVelocity > BT_ZERO ){
				motorY.m_targetVelocity -= decMath::min( kinematicFriction, angularVelocity );
				
			}else{
				motorY.m_targetVelocity += decMath::min( kinematicFriction, -angularVelocity );
			}
		}
		
		if( motorZ.m_enableMotor ){
			const btVector3 axis( m_calculatedTransformA.getBasis().getColumn( pIndexMotorZ ) );
			const btScalar angularVelocity = m_rbA.getAngularVelocity().dot( axis ) - m_rbB.getAngularVelocity().dot( axis )
				+ ( m_rbA.getTotalTorque() * m_rbA.getInvInertiaTensorWorld() ).dot( axis ) * timeStep
				- ( m_rbB.getTotalTorque() * m_rbB.getInvInertiaTensorWorld() ).dot( axis ) * timeStep;
			
			motorZ.m_targetVelocity = BT_ZERO;
			
			// joint friction angular z axis
			const btScalar kinematicFriction = factor * ( btScalar )engConstraint.GetDofAngularZ().GetKinematicFriction();
			
			if( angularVelocity > BT_ZERO ){
				motorZ.m_targetVelocity -= decMath::min( kinematicFriction, angularVelocity );
				
			}else{
				motorZ.m_targetVelocity += decMath::min( kinematicFriction, -angularVelocity );
			}
		}
	}
	
	btGeneric6DofConstraint::getInfo2( info );
}
#endif



// Private Functions
//////////////////////

btScalar debpBPConstraint6Dof::pConstraintInvMass() const{
	const btScalar inverseMass1 = m_rbA.getInvMass();
	const btScalar inverseMass2 = m_rbB.getInvMass();
	
	if( inverseMass1 < SIMD_EPSILON && inverseMass2 < SIMD_EPSILON ){
		// both bodies are static. no force can be applied in this case
		return BT_ZERO;
		
	}else if( inverseMass2 < SIMD_EPSILON ){
		// second body is static. use first one
		return inverseMass1;
		
	}else if( inverseMass1 < SIMD_EPSILON ){
		// first body is static. use second one
		return inverseMass2;
		
	}else{
		// both bodies are not static. use the larger mass of both. what would be better?
		return decMath::min( inverseMass1, inverseMass2 );
	}
}

btScalar debpBPConstraint6Dof::pFrictionFactor( btScalar timeStep ) const{
	// factor=t/m with t=timeStep this is factor=t/m
	// With getInvMass(im)=1/m this is factor=im*t
	// Using maximum mass this is max(1/m)=min(im) this is factor=min(im)*t
	const btScalar inverseMass1 = m_rbA.getInvMass();
	const btScalar inverseMass2 = m_rbB.getInvMass();
	
	if( inverseMass1 < SIMD_EPSILON && inverseMass2 < SIMD_EPSILON ){
		// both bodies are static. no force can be applied in this case
		return BT_ZERO;
		
	}else if( inverseMass2 < SIMD_EPSILON ){
		// second body is static. use first one
		return inverseMass1 * timeStep;
		
	}else if( inverseMass1 < SIMD_EPSILON ){
		// first body is static. use second one
		return inverseMass2 * timeStep;
		
	}else{
		// both bodies are not static. use the larger mass of both. what would be better?
		return decMath::min( inverseMass1, inverseMass2 ) * timeStep;
	}
}

btScalar debpBPConstraint6Dof::pGetMassA() const{
	if( m_rbA.getInvMass() > ( btScalar )FLOAT_SAFE_EPSILON ){
		return BT_ONE / m_rbA.getInvMass();
		
	}else{
		return BT_ZERO;
	}
}

btScalar debpBPConstraint6Dof::pGetMassB() const{
	if( m_rbB.getInvMass() > ( btScalar )FLOAT_SAFE_EPSILON ){
		return BT_ONE / m_rbB.getInvMass();
		
	}else{
		return BT_ZERO;
	}
}
