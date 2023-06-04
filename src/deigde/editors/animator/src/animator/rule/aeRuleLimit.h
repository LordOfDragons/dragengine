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
 * Animator limit rule.
 */
class aeRuleLimit : public aeRule{
public:
	typedef deTObjectReference<aeRuleLimit> Ref;
	
private:
	decVector pMinPosition;
	decVector pMaxPosition;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinScaling;
	decVector pMaxScaling;
	float pMinVertexPositionSet;
	float pMaxVertexPositionSet;
	
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
	
	bool pEnableVertexPositionSetMin;
	bool pEnableVertexPositionSetMax;
	
	decString pTargetBone;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new limit rule. */
	aeRuleLimit();
	/** Create a copy of a limit rule. */
	aeRuleLimit( const aeRuleLimit &copy );
	/** Clean up the limit rule. */
	virtual ~aeRuleLimit();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the minimum position. */
	inline const decVector &GetMinimumPosition() const{ return pMinPosition; }
	/** Set the minimum position. */
	void SetMinimumPosition( const decVector &position );
	/** Retrieve the maximum position. */
	inline const decVector &GetMaximumPosition() const{ return pMaxPosition; }
	/** Set the maximum position. */
	void SetMaximumPosition( const decVector &position );
	/** Retrieve the minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	/** Set the minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	/** Retrieve the maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	/** Set the maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	
	/** Retrieve the minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	
	/** Set the minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	
	/** Retrieve the maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	
	/** Set the maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/** Retrieve the minimum vertex position set. */
	inline float GetMinimumVertexPositionSet() const{ return pMinVertexPositionSet; }
	
	/** Set the minimum vertex position set. */
	void SetMinimumVertexPositionSet( float weight );
	
	/** Retrieve the maximum vertex position set. */
	inline float GetMaximumVertexPositionSet() const{ return pMaxVertexPositionSet; }
	
	/** Set the maximum vertex poosition set. */
	void SetMaximumVertexPositionSet( float weight );
	
	/** Retrieve the coordinate frame to rotate around. */
	inline deAnimatorRuleLimit::eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	/** Set the coordinate frame to rotate around. */
	void SetCoordinateFrame( deAnimatorRuleLimit::eCoordinateFrames coordinateFrame );
	
	/** Determine if x position is limited to a minimum. */
	inline bool GetEnablePositionXMin() const{ return pEnablePositionXMin; }
	/** Set if x position is limited to a minimum. */
	void SetEnablePositionXMin( bool enabled );
	/** Determine if x position is limited to a maximum. */
	inline bool GetEnablePositionXMax() const{ return pEnablePositionXMax; }
	/** Set if x position is limited to a maximum. */
	void SetEnablePositionXMax( bool enabled );
	/** Determine if y position is limited to a minimum. */
	inline bool GetEnablePositionYMin() const{ return pEnablePositionYMin; }
	/** Set if y position is limited to a minimum. */
	void SetEnablePositionYMin( bool enabled );
	/** Determine if y position is limited to a maximum. */
	inline bool GetEnablePositionYMax() const{ return pEnablePositionYMax; }
	/** Set if y position is limited to a maximum. */
	void SetEnablePositionYMax( bool enabled );
	/** Determine if z position is limited to a minimum. */
	inline bool GetEnablePositionZMin() const{ return pEnablePositionZMin; }
	/** Set if z position is limited to a minimum. */
	void SetEnablePositionZMin( bool enabled );
	/** Determine if z position is limited to a maximum. */
	inline bool GetEnablePositionZMax() const{ return pEnablePositionZMax; }
	/** Set if z position is limited to a maximum. */
	void SetEnablePositionZMax( bool enabled );
	
	/** Determine if x rotation is limited to a minimum. */
	inline bool GetEnableRotationXMin() const{ return pEnableRotationXMin; }
	/** Set if x rotation is limited to a minimum. */
	void SetEnableRotationXMin( bool enabled );
	/** Determine if x rotation is limited to a maximum. */
	inline bool GetEnableRotationXMax() const{ return pEnableRotationXMax; }
	/** Set if x rotation is limited to a maximum. */
	void SetEnableRotationXMax( bool enabled );
	/** Determine if y rotation is limited to a minimum. */
	inline bool GetEnableRotationYMin() const{ return pEnableRotationYMin; }
	/** Set if y rotation is limited to a minimum. */
	void SetEnableRotationYMin( bool enabled );
	/** Determine if y rotation is limited to a maximum. */
	inline bool GetEnableRotationYMax() const{ return pEnableRotationYMax; }
	/** Set if y rotation is limited to a maximum. */
	void SetEnableRotationYMax( bool enabled );
	/** Determine if z rotation is limited to a minimum. */
	inline bool GetEnableRotationZMin() const{ return pEnableRotationZMin; }
	/** Set if z rotation is limited to a minimum. */
	void SetEnableRotationZMin( bool enabled );
	/** Determine if z rotation is limited to a maximum. */
	inline bool GetEnableRotationZMax() const{ return pEnableRotationZMax; }
	/** Set if z rotation is limited to a maximum. */
	void SetEnableRotationZMax( bool enabled );
	
	/** Determine if x scaling is limited to a minimum. */
	inline bool GetEnableScalingXMin() const{ return pEnableScalingXMin; }
	/** Set if x scaling is limited to a minimum. */
	void SetEnableScalingXMin( bool enabled );
	/** Determine if x scaling is limited to a maximum. */
	inline bool GetEnableScalingXMax() const{ return pEnableScalingXMax; }
	/** Set if x scaling is limited to a maximum. */
	void SetEnableScalingXMax( bool enabled );
	/** Determine if y scaling is limited to a minimum. */
	inline bool GetEnableScalingYMin() const{ return pEnableScalingYMin; }
	/** Set if y scaling is limited to a minimum. */
	void SetEnableScalingYMin( bool enabled );
	/** Determine if y scaling is limited to a maximum. */
	inline bool GetEnableScalingYMax() const{ return pEnableScalingYMax; }
	/** Set if y scaling is limited to a maximum. */
	void SetEnableScalingYMax( bool enabled );
	/** Determine if z scaling is limited to a minimum. */
	inline bool GetEnableScalingZMin() const{ return pEnableScalingZMin; }
	/** Set if z scaling is limited to a minimum. */
	void SetEnableScalingZMin( bool enabled );
	/** Determine if z scaling is limited to a maximum. */
	inline bool GetEnableScalingZMax() const{ return pEnableScalingZMax; }
	/** Set if z scaling is limited to a maximum. */
	void SetEnableScalingZMax( bool enabled );
	
	/** Vertex position set is limited to a minimum. */
	inline bool GetEnableVertexPositionSetMin() const{ return pEnableVertexPositionSetMin; }
	
	/** Set if vertex position set is limited to a minimum. */
	void SetEnableVertexPositionSetMin( bool enabled );
	
	/** Vertex position set is limited to a maximum. */
	inline bool GetEnableVertexPositionSetMax() const{ return pEnableVertexPositionSetMax; }
	
	/** Set if vertex position set is limited to a maximum. */
	void SetEnableVertexPositionSetMax( bool enabled );
	
	/** Retrieve the name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	
	/** Set the name of the target bone. */
	void SetTargetBone( const char *boneName );
	
	/** Creates an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another limit rule to this limit rule. */
	virtual aeRuleLimit &operator=( const aeRuleLimit &copy );
	/*@}*/
};

#endif
