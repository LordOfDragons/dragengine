/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEANIMATORRULEBONETRANSFORMATOR_H_
#define _DEANIMATORRULEBONETRANSFORMATOR_H_

#include "deAnimatorRule.h"
#include "../../../common/math/decMath.h"
#include "../../../common/string/decString.h"


/**
 * \brief Animator Bone Transformator Rule Class.
 * 
 * Applies a rotation to the current state of the stored bones.
 * The rotation is considered relative to the component. Often
 * it is useful to use the position of the respective bone as
 * the reference point for applying the rotation. The bone-local
 * parameter indicates this behaviour. As this is the often looked
 * for behavior it is set true by default. The rotation axes are
 * always parallel to the component axes no matter if bone-local
 * rotation is used. If a controller is specified it is used to
 * modulate the rotation from a minimum to a maximum rotation.
 */
class DE_DLL_EXPORT deAnimatorRuleBoneTransformator : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleBoneTransformator> Ref;
	
	/** \brief Coordinate frame. */
	enum eCoordinateFrames{
		/** \brief Bone local coordinate frame. */
		ecfBoneLocal,
		
		/** \brief Component coordinate frame. */
		ecfComponent,
		
		/** \brief Target bone coordinate frame. */
		ecfTargetBone
	};
	
	/** \brief Input source. */
	enum eInputSources{
		/** \brief Use controller targets to blend between minimum and maximum. */
		eisTargetBlend,
		
		/** \brief Use controller target vectors directly. */
		eisTargetDirect,
		
		/** \brief Use input bone local state directly. */
		eisBoneState,
		
		/** \brief Use inverse input bone local state directly. */
		eisBoneStateInverse
	};
	
	
private:
	decVector pMinTranslation, pMaxTranslation, pMinRotation, pMaxRotation;
	decVector pMinScaling, pMaxScaling, pAxis;
	float pMinAngle, pMaxAngle;
	
	eCoordinateFrames pCoordinateFrame;
	bool pEnablePosition, pEnableOrientation, pEnableSize, pUseAxis;
	
	decString pTargetBone, pInputBone;
	eInputSources pInputSource;
	
	deAnimatorControllerTarget pTargetTranslation, pTargetRotation, pTargetScaling;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleBoneTransformator();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleBoneTransformator();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Minimum translation. */
	inline const decVector &GetMinimumTranslation() const{ return pMinTranslation; }
	
	/** \brief Set minimum translation. */
	void SetMinimumTranslation( const decVector &translation );
	
	/** \brief Maximum translation. */
	inline const decVector &GetMaximumTranslation() const{ return pMaxTranslation; }
	
	/** \brief Set maximum translation. */
	void SetMaximumTranslation( const decVector &translation );
	
	/** \brief Minimum rotation. */
	inline const decVector &GetMinimumRotation() const{ return pMinRotation; }
	
	/** \brief Set minimum rotation. */
	void SetMinimumRotation( const decVector &rotation );
	
	/** \brief Maximum rotation. */
	inline const decVector &GetMaximumRotation() const{ return pMaxRotation; }
	
	/** \brief Set maximum rotation. */
	void SetMaximumRotation( const decVector &rotation );
	
	/** \brief Minimum scaling. */
	inline const decVector &GetMinimumScaling() const{ return pMinScaling; }
	
	/** \brief Set minimum scaling. */
	void SetMinimumScaling( const decVector &scaling );
	
	/** \brief Maximum scaling. */
	inline const decVector &GetMaximumScaling() const{ return pMaxScaling; }
	
	/** \brief Set maximum scaling. */
	void SetMaximumScaling( const decVector &scaling );
	
	/**
	 * \brief Rotation axis.
	 * \version 1.6
	 */
	inline const decVector &GetAxis() const{ return pAxis; }
	
	/**
	 * \brief Set rotation axis.
	 * \version 1.6
	 */
	void SetAxis( const decVector &axis );
	
	/**
	 * \brief Minimum axis rotation angle.
	 * \version 1.6
	 */
	inline float GetMinimumAngle() const{ return pMinAngle; }
	
	/**
	 * \brief Set minimum axis rotation angle.
	 * \version 1.6
	 */
	void SetMinimumAngle( float angle );
	
	/**
	 * \brief Maximum axis rotation angle.
	 * \version 1.6
	 */
	inline float GetMaximumAngle() const{ return pMaxAngle; }
	
	/**
	 * \brief Set maximum axis rotation angle.
	 * \version 1.6
	 */
	void SetMaximumAngle( float angle );
	
	/** \brief Transformation coordinate frame. */
	inline eCoordinateFrames GetCoordinateFrame() const{ return pCoordinateFrame; }
	
	/** \brief Set transformation coordinate frame. */
	void SetCoordinateFrame( eCoordinateFrames coordinateFrame );
	
	/** \brief Determines if position is altered. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position is altered. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation is altered. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation is altered. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/**
	 * \brief Use rotation axis instead of rotation directly.
	 * \version 1.6
	 */
	inline bool GetUseAxis() const{ return pUseAxis; }
	
	/**
	 * \brief Set to use rotation axis instead of rotation directly.
	 * \version 1.6
	 */
	void SetUseAxis( bool useAxis );
	
	/** \brief Name of the target bone. */
	inline const decString &GetTargetBone() const{ return pTargetBone; }
	
	/** \brief Set name of the target bone. */
	void SetTargetBone( const char *boneName );
	
	/** \brief Name of the input bone. */
	inline const decString &GetInputBone() const{ return pInputBone; }
	
	/** \brief Set name of the input bone. */
	void SetInputBone(const char *boneName);
	
	/** \brief Input source. */
	inline eInputSources GetInputSource() const{ return pInputSource; }
	
	/** \brief Set input source. */
	void SetInputSource(eInputSources source);
	
	/** \brief Translation target. */
	inline deAnimatorControllerTarget &GetTargetTranslation(){ return pTargetTranslation; }
	inline const deAnimatorControllerTarget &GetTargetTranslation() const{ return pTargetTranslation; }
	
	/** \brief Rotation target. */
	inline deAnimatorControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	inline const deAnimatorControllerTarget &GetTargetRotation() const{ return pTargetRotation; }
	
	/** \brief Scaling target. */
	inline deAnimatorControllerTarget &GetTargetScaling(){ return pTargetScaling; }
	inline const deAnimatorControllerTarget &GetTargetScaling() const{ return pTargetScaling; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
