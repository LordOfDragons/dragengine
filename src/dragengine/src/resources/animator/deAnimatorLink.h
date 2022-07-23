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

#ifndef _DEANIMATORLINK_H_
#define _DEANIMATORLINK_H_

#include "../../common/curve/decCurveBezier.h"
#include "../../common/string/decString.h"


/**
 * \brief Animator controller link.
 *
 * Describes a controller linked to a rule. This class stores besides
 * the index of the controller in the animator also two transformation
 * parameters. These can be used to alter the value obtained from the
 * controller to reuse controllers. The controller value is first clamped
 * by the lower and upper clamp boundaries. Controller always yield a
 * value in the range from 0 to 1. The default clamp boundaries of 0 for
 * the lower and 1 for the upper therefore leave the controller value
 * untouched. The clamped value is then multiplied by the multiplier
 * parameter and then offset by the offset parameter. Summed up the final
 * value returned by this link equals to
 * "value = offset + clamp( controllerValue, lower, upper ) * multiplier.
 * The default value is 0 for offset and 1 for the multiplier. The
 * transformation parameters should map the values from the range 0-1 back
 * into the range 0-1. Certain rules can provide exceptions to allow
 * special effects for which especially clamping can be useful.
 * 
 * \par Bone Input (version 1.6)
 * 
 * If controller is -1 a bone name can be set. This will use a bone parameter
 * as input instead of a controller value. The bone parameter is mapped from
 * the specified range to the 0 to 1 range.
 */
class DE_DLL_EXPORT deAnimatorLink{
public:
	/**
	 * \brief Bone parameter to use.
	 * \version 1.6
	 */
	enum eBoneParameter{
		/** \brief Use local bone position along in X axis. */
		ebpPositionX,
		
		/** \brief Use local bone position along Y axis. */
		ebpPositionY,
		
		/** \brief Use local bone position along Z axis. */
		ebpPositionZ,
		
		/** \brief Use local bone rotation around X axis in radians. */
		ebpRotationX,
		
		/** \brief Use local bone rotation around Y axis in radians. */
		ebpRotationY,
		
		/** \brief Use local bone rotation around Z axis in radians. */
		ebpRotationZ,
		
		/** \brief Use local bone scale along X axis. */
		ebpScaleX,
		
		/** \brief Use local bone scale along Y axis. */
		ebpScaleY,
		
		/** \brief Use local bone scale along Z axis. */
		ebpScaleZ
	};
	
	
	
private:
	int pController;
	decCurveBezier pCurve;
	int pRepeat;
	decString pBone;
	eBoneParameter pBoneParameter;
	float pBoneMinValue;
	float pBoneMaxValue;
	bool pWrapY;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create link. */
	deAnimatorLink();
	
	/** \brief Create copy of link. */
	deAnimatorLink( const deAnimatorLink &copy );
	
	/** \brief Clean up link. */
	~deAnimatorLink();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Linked controller or -1 if not set. */
	inline int GetController() const{ return pController; }
	
	/** \brief Controller is set. */
	inline bool HasController() const{ return pController != -1; }
	
	/**
	 * \brief Set linked controller or -1 to not set.
	 * \throws deeInvalidParam \em controller is less than -1.
	 */
	void SetController( int controller );
	
	/** \brief Curve. */
	inline const decCurveBezier &GetCurve() const{ return pCurve; }
	
	/** \brief Set curve. */
	void SetCurve( const decCurveBezier &curve );
	
	/** \brief Repeat count of input value. */
	inline int GetRepeat() const{ return pRepeat; }
	
	/**
	 * \brief Set repeat count of input value.
	 * \throws deeInvalidParam \em repeat is less than 1.
	 */
	void SetRepeat( int repeat );
	
	/**
	 * \brief Bone to use parameter of as input or empty string to not use.
	 * \version 1.6
	 */
	inline const decString &GetBone() const{ return pBone; }
	
	/**
	 * \brief Set bone to use parameter of as input or empty string to not use.
	 * \version 1.6
	 */
	void SetBone( const char *bone );
	
	/**
	 * \brief Bone parameter to use as input.
	 * \version 1.6
	 */
	inline eBoneParameter GetBoneParameter() const{ return pBoneParameter; }
	
	/**
	 * \brief Set bone parameter to use as input.
	 * \version 1.6
	 */
	void SetBoneParameter( eBoneParameter parameter );
	
	/**
	 * \brief Minimum bone parameter value.
	 * \version 1.6
	 */
	inline float GetBoneMinimumValue() const{ return pBoneMinValue; }
	
	/**
	 * \brief Maximum bone parameter value.
	 * \version 1.6
	 */
	inline float GetBoneMaximumValue() const{ return pBoneMaxValue; }
	
	/**
	 * \brief Set bone value range.
	 * \version 1.6
	 */
	void SetBoneValueRange( float minimum, float maximum );
	
	/**
	 * \brief Wrap Y value instead of clamping.
	 * \version 1.9
	 */
	inline bool GetWrapY() const{ return pWrapY; }
	
	/**
	 * \brief Set to wrap Y value instead of clamping.
	 * \version 1.9
	 */
	void SetWrapY( bool wrap );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Copy link to this link. */
	deAnimatorLink &operator=( const deAnimatorLink &copy );
	/*@}*/
};

#endif
