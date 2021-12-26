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

#ifndef _DEANIMATORRULEANIMATION_H_
#define _DEANIMATORRULEANIMATION_H_

#include "deAnimatorRule.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Animation Rule Class.
 * Retrieves the animation state of the stored bones from the bound
 * animation and blends them over the existing state. The state to
 * fetch is indicated with the name of the move and the the time
 * from the beginning of the move.
 */
class DE_DLL_EXPORT deAnimatorRuleAnimation : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleAnimation> Ref;
	
	
	
private:
	decString pMoveName;
	float pMoveTime;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	
	deAnimatorControllerTarget pTargetMoveTime;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleAnimation();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleAnimation();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Name of the animation move. */
	inline const decString &GetMoveName() const{ return pMoveName; }
	
	/** \brief Set animation move name. */
	void SetMoveName( const char *moveName );
	
	/** \brief Animation move time. */
	inline float GetMoveTime() const{ return pMoveTime; }
	
	/** \brief Set animation move time. */
	void SetMoveTime( float moveTime );
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Move time target. */
	inline deAnimatorControllerTarget &GetTargetMoveTime(){ return pTargetMoveTime; }
	inline const deAnimatorControllerTarget &GetTargetMoveTime() const{ return pTargetMoveTime; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
