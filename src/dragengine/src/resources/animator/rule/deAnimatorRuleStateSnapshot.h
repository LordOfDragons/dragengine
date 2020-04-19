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

#ifndef _DEANIMATORRULESTATESNAPSHOT_H_
#define _DEANIMATORRULESTATESNAPSHOT_H_

#include "deAnimatorRule.h"


/**
 * \brief Animator State Snapshot Rule Class.
 *
 * A simple rule that applies the current state in the bound component as the new
 * animation state. Useful if you want to blend from an unknown animation state
 * ( like for example after having the physics module altered the state or being
 * in the middle of some other animation ) to a new one without jumping. Also no
 * additional controllers in this rule. You can use the same blending trick
 * mentioned in the examples with this rule.<br>
 * <br>
 * Similar to the last state rule but with an small difference: state is saved.
 * Hence you have to manually create a snapshot of a state ( either from the
 * component or some animation frame ). This snapshot is then used as the new
 * animation state. Also here no additional controllers. The same blending
 * example can be used here too. State snapshots can be useful if you want a
 * certain situation to influence an animation for a longer time after it
 * happened.<br>
 * <br>
 * More rules will be added later on. The system might look a bit daunting at
 * the beginning but with the right abstraction layer in the scripting module
 * this is rather easy to use. Once setup you only have to alter the controllers
 * and from time to time the move names in the rules all of which is a single
 * function call.
 */
class deAnimatorRuleStateSnapshot : public deAnimatorRule{
private:
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pUseLastState;
	int pID;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleStateSnapshot();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleStateSnapshot();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
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
	
	/** \brief Last state of the component is used or only the stored state. */
	inline bool GetUseLastState() const{ return pUseLastState; }
	
	/** \brief Set if last state of the component is used or only the stored state. */
	void SetUseLastState( bool useLastState );
	
	/** \brief Identifier to snapshot state using animator instance. */
	inline int GetID() const{ return pID; }
	
	/** \brief Set identifier to snapshot state using animator instance. */
	void SetID( int id );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
