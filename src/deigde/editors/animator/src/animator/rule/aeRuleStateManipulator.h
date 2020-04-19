/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AERULESTATEMANIPULATOR_H_
#define _AERULESTATEMANIPULATOR_H_

#include "aeRule.h"



/**
 * \brief Animator rule state manipulator.
 */
class aeRuleStateManipulator : public aeRule{
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
	
	aeControllerTarget pTargetPosition;
	aeControllerTarget pTargetRotation;
	aeControllerTarget pTargetSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create rule. */
	aeRuleStateManipulator();
	
	/** \brief Create copy of rule. */
	aeRuleStateManipulator( const aeRuleStateManipulator &copy );
	
	/** \brief Clean up rule. */
	virtual ~aeRuleStateManipulator();
	/*@}*/
	
	
	
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
	
	
	
	/** \brief Position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Rotation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableRotation; }
	
	/** \brief Set if rotation manipulation is enabled. */
	void SetEnableRotation( bool enabled );
	
	/** \brief Size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	
	
	/** \brief Position target. */
	inline aeControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	
	/** \brief Rotation target. */
	inline aeControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	
	/** \brief Size target. */
	inline aeControllerTarget &GetTargetSize(){ return pTargetSize; }
	
	
	
	/** \brief Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** \brief Update targets. */
	virtual void UpdateTargets();
	
	/** \brief Retrieve the number of targets using a given link. */
	virtual int CountLinkUsage( aeLink *link ) const;
	
	/** \brief Remove a link from all targets using it. */
	virtual void RemoveLinkFromTargets( aeLink *link );
	
	/** \brief Remove all links from all targets. */
	virtual void RemoveLinksFromAllTargets();
	
	
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another state manipulator rule to this state manipulator rule. */
	virtual aeRuleStateManipulator &operator=( const aeRuleStateManipulator &copy );
	/*@}*/
};

#endif
