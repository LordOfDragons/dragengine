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

#ifndef _DEANIMATORRULELIMITR_H_
#define _DEANIMATORRULELIMITR_H_

#include "deAnimatorRule.h"
#include "../../../common/math/decMath.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator limit rule.
 * 
 * Limits bone parameters to a range.
 */
class DE_DLL_EXPORT deAnimatorRuleLimit : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleLimit> Ref;
	
	
	
public:
	/** \brief Coordinate frame. */
	enum eCoordinateFrames{
		/** \brief Bone local coordinate frame. */
		ecfBoneLocal,
		
		/** \brief Component coordinate frame. */
		ecfComponent,
		
		/** \brief Target bone coordinate frame. */
		ecfTargetBone
	};
	
	
	
private:
	decVector pMinPosition;
	decVector pMaxPosition;
	decVector pMinRotation;
	decVector pMaxRotation;
	decVector pMinScaling;
	decVector pMaxScaling;
	float pMinVertexPositionSet;
	float pMaxVertexPositionSet;
	
	eCoordinateFrames pCoordinateFrame;
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
	/** \brief Create new animator rule. */
	deAnimatorRuleLimit();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleLimit();
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
	
	/** \brief Minimum rotation in euler angles. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	
	/** \brief Set minimum rotation in euler angles. */
	void SetMinimumRotation( const decVector &rotation );
	
	/** \brief Maximum rotation in euler angles. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	
	/** \brief Set maximum rotation in euler angles. */
	void SetMaximumRotation( const decVector &rotation );
	
	/** \brief Minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	
	/** \brief Set minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	
	/** \brief Maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	
	/** \brief Set maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/** \brief Minimum vertex position set. */
	inline float GetMinimumVertexPositionSet() const{ return pMinVertexPositionSet; }
	
	/** \brief Set minimum vertex position set. */
	void SetMinimumVertexPositionSet( float weight );
	
	/** \brief Maximum vertex position set. */
	inline float GetMaximumVertexPositionSet() const{ return pMaxVertexPositionSet; }
	
	/** \brief Set maximum vertex position set. */
	void SetMaximumVertexPositionSet( float weight );
	
	/** \brief Transformation coordinate frame. */
	inline eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	
	/** \brief Set transformation coordinate frame. */
	void SetCoordinateFrame( eCoordinateFrames coordinateFrame );
	
	/** \brief Determines if x position is limited against the minimum. */
	inline bool GetEnablePositionXMin() const{ return pEnablePositionXMin; }
	
	/** \brief Sets if x position is limited against the minimum. */
	void SetEnablePositionXMin( bool enabled );
	
	/** \brief Determines if x position is limited against the maximum. */
	inline bool GetEnablePositionXMax() const{ return pEnablePositionXMax; }
	
	/** \brief Sets if x position is limited against the maximum. */
	void SetEnablePositionXMax( bool enabled );
	
	/** \brief Determines if y position is limited against the minimum. */
	inline bool GetEnablePositionYMin() const{ return pEnablePositionYMin; }
	
	/** \brief Sets if y position is limited against the minimum. */
	void SetEnablePositionYMin( bool enabled );
	
	/** \brief Determines if y position is limited against the maximum. */
	inline bool GetEnablePositionYMax() const{ return pEnablePositionYMax; }
	
	/** \brief Sets if y position is limited against the maximum. */
	void SetEnablePositionYMax( bool enabled );
	
	/** \brief Determines if z position is limited against the minimum. */
	inline bool GetEnablePositionZMin() const{ return pEnablePositionZMin; }
	
	/** \brief Sets if z position is limited against the minimum. */
	void SetEnablePositionZMin( bool enabled );
	
	/** \brief Determines if z position is limited against the maximum. */
	inline bool GetEnablePositionZMax() const{ return pEnablePositionZMax; }
	
	/** \brief Sets if z position is limited against the maximum. */
	void SetEnablePositionZMax( bool enabled );
	
	/** \brief Determines if x rotation is limited against the minimum. */
	inline bool GetEnableRotationXMin() const{ return pEnableRotationXMin; }
	
	/** \brief Sets if x rotation is limited against the minimum. */
	void SetEnableRotationXMin( bool enabled );
	
	/** \brief Determines if x rotation is limited against the maximum. */
	inline bool GetEnableRotationXMax() const{ return pEnableRotationXMax; }
	
	/** \brief Sets if x rotation is limited against the maximum. */
	void SetEnableRotationXMax( bool enabled );
	
	/** \brief Determines if y rotation is limited against the minimum. */
	inline bool GetEnableRotationYMin() const{ return pEnableRotationYMin; }
	
	/** \brief Sets if y rotation is limited against the minimum. */
	void SetEnableRotationYMin( bool enabled );
	
	/** \brief Determines if y rotation is limited against the maximum. */
	inline bool GetEnableRotationYMax() const{ return pEnableRotationYMax; }
	
	/** \brief Sets if y rotation is limited against the maximum. */
	void SetEnableRotationYMax( bool enabled );
	
	/** \brief Determines if z rotation is limited against the minimum. */
	inline bool GetEnableRotationZMin() const{ return pEnableRotationZMin; }
	
	/** \brief Sets if z rotation is limited against the minimum. */
	void SetEnableRotationZMin( bool enabled );
	
	/** \brief Determines if z rotation is limited against the maximum. */
	inline bool GetEnableRotationZMax() const{ return pEnableRotationZMax; }
	
	/** \brief Sets if z rotation is limited against the maximum. */
	void SetEnableRotationZMax( bool enabled );
	
	/** \brief Determines if x scaling is limited against the minimum. */
	inline bool GetEnableScalingXMin() const{ return pEnableScalingXMin; }
	
	/** \brief Sets if x scaling is limited against the minimum. */
	void SetEnableScalingXMin( bool enabled );
	
	/** \brief Determines if x scaling is limited against the maximum. */
	inline bool GetEnableScalingXMax() const{ return pEnableScalingXMax; }
	
	/** \brief Sets if x scaling is limited against the maximum. */
	void SetEnableScalingXMax( bool enabled );
	
	/** \brief Determines if y scaling is limited against the minimum. */
	inline bool GetEnableScalingYMin() const{ return pEnableScalingYMin; }
	
	/** \brief Sets if y scaling is limited against the minimum. */
	void SetEnableScalingYMin( bool enabled );
	
	/** \brief Determines if y scaling is limited against the maximum. */
	inline bool GetEnableScalingYMax() const{ return pEnableScalingYMax; }
	
	/** \brief Sets if y scaling is limited against the maximum. */
	void SetEnableScalingYMax( bool enabled );
	
	/** \brief Determines if z scaling is limited against the minimum. */
	inline bool GetEnableScalingZMin() const{ return pEnableScalingZMin; }
	
	/** \brief Sets if z scaling is limited against the minimum. */
	void SetEnableScalingZMin( bool enabled );
	
	/** \brief Determines if z scaling is limited against the maximum. */
	inline bool GetEnableScalingZMax() const{ return pEnableScalingZMax; }
	
	/** \brief Sets if z scaling is limited against the maximum. */
	void SetEnableScalingZMax( bool enabled );
	
	/** \brief Vertex position set is limited against the minimum. */
	inline bool GetEnableVertexPositionSetMin() const{ return pEnableVertexPositionSetMin; }
	
	/** \brief Set if vertex position set is limited against the minimum. */
	void SetEnableVertexPositionSetMin( bool enabled );
	
	/** \brief Vertex position set is limited against the maximum. */
	inline bool GetEnableVertexPositionSetMax() const{ return pEnableVertexPositionSetMax; }
	
	/** \brief Set if vertex position set is limited against the maximum. */
	void SetEnableVertexPositionSetMax( bool enabled );
	
	/** \brief Name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	
	/** \brief Set name of the target bone. */
	void SetTargetBone( const char *boneName );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
