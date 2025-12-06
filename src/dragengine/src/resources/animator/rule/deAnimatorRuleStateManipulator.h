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
class DE_DLL_EXPORT deAnimatorRuleStateManipulator : public deAnimatorRule{
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
	float pMinVertexPositionSet;
	float pMaxVertexPositionSet;
	bool pEnablePosition;
	bool pEnableRotation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	deAnimatorControllerTarget pTargetPosition;
	deAnimatorControllerTarget pTargetRotation;
	deAnimatorControllerTarget pTargetSize;
	deAnimatorControllerTarget pTargetVertexPositionSet;
	
	
	
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
	
	/** \brief Minimum vertex position set. */
	inline float GetMinimumVertexPositionSet() const{ return pMinVertexPositionSet; }
	
	/** \brief Set minimum vertex position set. */
	void SetMinimumVertexPositionSet( float weight );
	
	/** \brief Maximum vertex position set. */
	inline float GetMaximumVertexPositionSet() const{ return pMaxVertexPositionSet; }
	
	/** \brief Set maximum vertex position set. */
	void SetMaximumVertexPositionSet( float weight );
	
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
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** \brief Position target. */
	inline deAnimatorControllerTarget &GetTargetPosition(){ return pTargetPosition; }
	inline const deAnimatorControllerTarget &GetTargetPosition() const{ return pTargetPosition; }
	
	/** \brief Rotation target. */
	inline deAnimatorControllerTarget &GetTargetRotation(){ return pTargetRotation; }
	inline const deAnimatorControllerTarget &GetTargetRotation() const{ return pTargetRotation; }
	
	/** \brief Size target. */
	inline deAnimatorControllerTarget &GetTargetSize(){ return pTargetSize; }
	inline const deAnimatorControllerTarget &GetTargetSize() const{ return pTargetSize; }
	
	/** \brief Vertex position set target. */
	inline deAnimatorControllerTarget &GetTargetVertexPositionSet(){ return pTargetVertexPositionSet; }
	inline const deAnimatorControllerTarget &GetTargetVertexPositionSet() const{ return pTargetVertexPositionSet; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
