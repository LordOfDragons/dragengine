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

#ifndef _DEANIMATORRULESTATEMANIPULATOR_H_
#define _DEANIMATORRULESTATEMANIPULATOR_H_

#include "deAnimatorRule.h"
#include "../../../common/math/decMath.h"


/**
 * \brief Animator State Manipulator Rule Class.
 *
 * Sometimes manipulating the state of a bone directly is desired. This could be
 * used to move around eyes or any other bone where the location of the bone has
 * no influence on the rotation of the bone itself. For this you specify the
 * minimum and maximum values of position, rotation and size. For each a
 * controller can be assigned the same way as in the bone rotator. You can then
 * enable position, rotation and size manipulation individually ( if you wish
 * only one or two of those properties to be altered ). 
 */
class deAnimatorRuleStateManipulator : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleStateManipulator> Ref;
	
	
	
private:
	decVector pMinPosition;
	decVector pMaxPosition;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinSize;
	decVector pMaxSize;
	bool pEnablePosition;
	bool pEnableRotation;
	bool pEnableSize;
	
	deAnimatorControllerTarget pTargetPosition;
	deAnimatorControllerTarget pTargetRotation;
	deAnimatorControllerTarget pTargetSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleStateManipulator();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleStateManipulator();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Minimum position. */
	inline const decVector &GetMinimumPosition() const{ return pMinPosition; }
	
	/** \brief Set minimum position. */
	void SetMinimumPosition( const decVector &position );
	
	/** \brief Maximum position. */
	inline const decVector &GetMaximumPosition() const{ return pMaxPosition; }
	
	/** \brief Set maximum position. */
	void SetMaximumPosition( const decVector &position );
	
	/** \brief Minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	
	/** \brief Set minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	
	/** \brief Maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	
	/** \brief Set maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	
	/** \brief Minimum size. */
	inline const decVector &GetMinimumSize() const{ return pMinSize; }
	
	/** \brief Set minimum size. */
	void SetMinimumSize( const decVector &size );
	
	/** \brief Maximum size. */
	inline const decVector &GetMaximumSize() const{ return pMaxSize; }
	
	/** \brief Set maximum size. */
	void SetMaximumSize( const decVector &size );
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if rotation manipulation is enabled. */
	inline bool GetEnableRotation() const{ return pEnableRotation; }
	
	/** \brief Sets if rotation manipulation is enabled. */
	void SetEnableRotation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Position target. */
	inline deAnimatorControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const deAnimatorControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Rotation target. */
	inline deAnimatorControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	inline const deAnimatorControllerTarget &GetTargetRotation() const{ return pTargetRotation; }
	
	/** \brief Size target. */
	inline deAnimatorControllerTarget &GetTargetSize(){ return pTargetSize; }
	inline const deAnimatorControllerTarget &GetTargetSize() const{ return pTargetSize; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
