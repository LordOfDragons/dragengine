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

#ifndef _AERULELIMIT_H_
#define _AERULELIMIT_H_

#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>

#include "aeRule.h"



/**
 * \brief Animator limit rule.
 */
class aeRuleLimit : public aeRule{
private:
	decVector pMinPosition;
	decVector pMaxPosition;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinScaling;
	decVector pMaxScaling;
	
	deAnimatorRuleLimit::eCoordinateFrames pCoordinateFrame;
	
	bool pEnablePositionXMin;
	bool pEnablePositionXMax;
	bool pEnablePositionYMin;
	bool pEnablePositionYMax;
	bool pEnablePositionZMin;
	bool pEnablePositionZMax;
	
	bool pEnableRotationXMin;
	bool pEnableRotationXMax;
	bool pEnableRotationYMin;
	bool pEnableRotationYMax;
	bool pEnableRotationZMin;
	bool pEnableRotationZMax;
	
	bool pEnableScalingXMin;
	bool pEnableScalingXMax;
	bool pEnableScalingYMin;
	bool pEnableScalingYMax;
	bool pEnableScalingZMin;
	bool pEnableScalingZMax;
	
	decString pTargetBone;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new limit rule. */
	aeRuleLimit();
	/** \brief Create a copy of a limit rule. */
	aeRuleLimit( const aeRuleLimit &copy );
	/** \brief Clean up the limit rule. */
	virtual ~aeRuleLimit();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieve the minimum position. */
	inline const decVector &GetMinimumPosition() const{ return pMinPosition; }
	/** \brief Set the minimum position. */
	void SetMinimumPosition( const decVector &position );
	/** \brief Retrieve the maximum position. */
	inline const decVector &GetMaximumPosition() const{ return pMaxPosition; }
	/** \brief Set the maximum position. */
	void SetMaximumPosition( const decVector &position );
	/** \brief Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	/** \brief Set the minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	/** \brief Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	/** \brief Set the maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	/** \brief Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	/** \brief Set the minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	/** \brief Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	/** \brief Set the maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/** \brief Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleLimit::eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	/** \brief Set the coordinate frame to rotate around. */
	void SetCoordinateFrame( deAnimatorRuleLimit::eCoordinateFrames coordinateFrame );
	
	/** \brief Determine if x position is limited to a minimum. */
	inline bool GetEnablePositionXMin() const{ return pEnablePositionXMin; }
	/** \brief Set if x position is limited to a minimum. */
	void SetEnablePositionXMin( bool enabled );
	/** \brief Determine if x position is limited to a maximum. */
	inline bool GetEnablePositionXMax() const{ return pEnablePositionXMax; }
	/** \brief Set if x position is limited to a maximum. */
	void SetEnablePositionXMax( bool enabled );
	/** \brief Determine if y position is limited to a minimum. */
	inline bool GetEnablePositionYMin() const{ return pEnablePositionYMin; }
	/** \brief Set if y position is limited to a minimum. */
	void SetEnablePositionYMin( bool enabled );
	/** \brief Determine if y position is limited to a maximum. */
	inline bool GetEnablePositionYMax() const{ return pEnablePositionYMax; }
	/** \brief Set if y position is limited to a maximum. */
	void SetEnablePositionYMax( bool enabled );
	/** \brief Determine if z position is limited to a minimum. */
	inline bool GetEnablePositionZMin() const{ return pEnablePositionZMin; }
	/** \brief Set if z position is limited to a minimum. */
	void SetEnablePositionZMin( bool enabled );
	/** \brief Determine if z position is limited to a maximum. */
	inline bool GetEnablePositionZMax() const{ return pEnablePositionZMax; }
	/** \brief Set if z position is limited to a maximum. */
	void SetEnablePositionZMax( bool enabled );
	
	/** \brief Determine if x rotation is limited to a minimum. */
	inline bool GetEnableRotationXMin() const{ return pEnableRotationXMin; }
	/** \brief Set if x rotation is limited to a minimum. */
	void SetEnableRotationXMin( bool enabled );
	/** \brief Determine if x rotation is limited to a maximum. */
	inline bool GetEnableRotationXMax() const{ return pEnableRotationXMax; }
	/** \brief Set if x rotation is limited to a maximum. */
	void SetEnableRotationXMax( bool enabled );
	/** \brief Determine if y rotation is limited to a minimum. */
	inline bool GetEnableRotationYMin() const{ return pEnableRotationYMin; }
	/** \brief Set if y rotation is limited to a minimum. */
	void SetEnableRotationYMin( bool enabled );
	/** \brief Determine if y rotation is limited to a maximum. */
	inline bool GetEnableRotationYMax() const{ return pEnableRotationYMax; }
	/** \brief Set if y rotation is limited to a maximum. */
	void SetEnableRotationYMax( bool enabled );
	/** \brief Determine if z rotation is limited to a minimum. */
	inline bool GetEnableRotationZMin() const{ return pEnableRotationZMin; }
	/** \brief Set if z rotation is limited to a minimum. */
	void SetEnableRotationZMin( bool enabled );
	/** \brief Determine if z rotation is limited to a maximum. */
	inline bool GetEnableRotationZMax() const{ return pEnableRotationZMax; }
	/** \brief Set if z rotation is limited to a maximum. */
	void SetEnableRotationZMax( bool enabled );
	
	/** \brief Determine if x scaling is limited to a minimum. */
	inline bool GetEnableScalingXMin() const{ return pEnableScalingXMin; }
	/** \brief Set if x scaling is limited to a minimum. */
	void SetEnableScalingXMin( bool enabled );
	/** \brief Determine if x scaling is limited to a maximum. */
	inline bool GetEnableScalingXMax() const{ return pEnableScalingXMax; }
	/** \brief Set if x scaling is limited to a maximum. */
	void SetEnableScalingXMax( bool enabled );
	/** \brief Determine if y scaling is limited to a minimum. */
	inline bool GetEnableScalingYMin() const{ return pEnableScalingYMin; }
	/** \brief Set if y scaling is limited to a minimum. */
	void SetEnableScalingYMin( bool enabled );
	/** \brief Determine if y scaling is limited to a maximum. */
	inline bool GetEnableScalingYMax() const{ return pEnableScalingYMax; }
	/** \brief Set if y scaling is limited to a maximum. */
	void SetEnableScalingYMax( bool enabled );
	/** \brief Determine if z scaling is limited to a minimum. */
	inline bool GetEnableScalingZMin() const{ return pEnableScalingZMin; }
	/** \brief Set if z scaling is limited to a minimum. */
	void SetEnableScalingZMin( bool enabled );
	/** \brief Determine if z scaling is limited to a maximum. */
	inline bool GetEnableScalingZMax() const{ return pEnableScalingZMax; }
	/** \brief Set if z scaling is limited to a maximum. */
	void SetEnableScalingZMax( bool enabled );
	
	/** \brief Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	/** \brief Set the name of the target bone. */
	void SetTargetBone( const char *boneName );
	
	/** \brief Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** \brief Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** \brief List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy another limit rule to this limit rule. */
	virtual aeRuleLimit &operator=( const aeRuleLimit &copy );
	/*@}*/
};

#endif
