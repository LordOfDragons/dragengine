#if 0

/* 
 * Drag[en]gine Bullet Physics Module
 *
 * Copyright (C) 2014, Plüss Roland ( roland@rptd.ch )
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

#ifndef _DEBPBPCONSTRAINTSHARED_H_
#define _DEBPBPCONSTRAINTSHARED_H_

#include "BulletDynamics/ConstraintSolver/btTypedConstraint.h"

class debpColliderConstraint;


/**
 * \brief Shared class for bullet physics constraint adapters.
 * \details Provides additional properties the basic bullet constraints do not offer.
 *          Furthermore provides a common base class for all bullet constraint adapters.
 *          This class is used as the second base class allowing for bullet constraint
 *          adapters to the stored as debpBPConstraintShared pointers. The object can
 *          be safely destructed with a pointer to debpBPConstraintShared.
 * \par Constraint breaking
 * Constraint breaking is tracked for all linear and angular axes even if the constraint
 * does not use them. During construction time all breaking axes are set to being not
 * broke. Calling TestBreaking(eBreakingAxes,btConstraintInfo2&,int) tests if the
 * solving parameters would break the constraint for a breaking axis. The limit impulses
 * of the solver row is compared against the breaking impulse for the breaking axis. If
 * the limit is exceeded the axis is marked as broken and the limits adjusted. The
 * breaking result can be retrieved later on to convert a constraint to a new one.
 */
class debpBPConstraintShared{
public:
	/** \brief Breaking axes. */
	enum eBreakingAxes{
		ebaLinearX,
		ebaLinearY,
		ebaLinearZ,
		ebaAngularX,
		ebaAngularY,
		ebaAngularZ
	};
	
private:
	debpColliderConstraint *pConstraint;
	bool pAxisBroke[ 6 ];
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new shared bullet constraint. */
	debpBPConstraintShared( debpColliderConstraint *constraint );
	/** \brief Cleans up the shared bullet constraint. */
	virtual ~debpBPConstraintShared();
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the collider constraint. */
	inline debpColliderConstraint *GetConstraint() const{ return pConstraint; }
	
	/** \brief Determines if an axis is broken. */
	bool IsAxisBroke( eBreakingAxes axis ) const;
	/** \brief Determines if the linear x axis is broke. */
	inline IsLinearXBroke() const{ return pAxisBroke[ ebaLinearX ]; }
	/** \brief Determines if the linear y axis is broke. */
	inline IsLinearYBroke() const{ return pAxisBroke[ ebaLinearY ]; }
	/** \brief Determines if the linear z axis is broke. */
	inline IsLinearZBroke() const{ return pAxisBroke[ ebaLinearZ ]; }
	/** \brief Determines if the angular x axis is broke. */
	inline IsAngularXBroke() const{ return pAxisBroke[ ebaAngularX ]; }
	/** \brief Determines if the angular y axis is broke. */
	inline IsAngularYBroke() const{ return pAxisBroke[ ebaAngularY ]; }
	/** \brief Determines if the angular z axis is broke. */
	inline IsAngularZBroke() const{ return pAxisBroke[ ebaAngularZ ]; }
	/** \brief Sets if an axis is broken. */
	void SetAxisBroke( eBreakingAxes axis, bool broke );
	/** \brief Sets if the linear x axis is broke. */
	void SetLinearXBroke( bool broke );
	/** \brief Sets if the linear y axis is broke. */
	void SetLinearYBroke( bool broke );
	/** \brief Sets if the linear z axis is broke. */
	void SetLinearZBroke( bool broke );
	/** \brief Sets if the angular x axis is broke. */
	void SetAngularXBroke( bool broke );
	/** \brief Sets if the angular y axis is broke. */
	void SetAngularYBroke( bool broke );
	/** \brief Sets if the angular z axis is broke. */
	void SetAngularZBroke( bool broke );
	/** \brief Reset all axis to not broken. */
	void ResetAxisBreaking();
	
	/**
	 * \brief Prepare solver for breaking calculation for a breaking axis.
	 * \details Clamps the solver limits to the breaking axis impulse threshold.
	 */
	void SolverPrepareBreaking( eBreakingAxes axis, btTypedConstraint::btConstraintInfo2 &info, int row ) const;
	/**
	 * \brief Test if a solver state breaks an axis.
	 * \details If the value excees the constraint breaking value the axis is marked
	 *          as broken and the limits clamped. Returns true if the constraint broke.
	 */
	bool TestBreaking( eBreakingAxes axis, btTypedConstraint::btConstraintInfo2 &info, int row );
	
	/**
	 * \brief Create a bew constraint using the breaking state.
	 * \details Returns NULL if the constraint has to be completely removed or the
	 *          new constraint otherwise. The default implementation returns NULL.
	 */
	virtual debpBPConstraintShared *BreakConstraint() const;
	/*@}*/
};

#endif

#endif
