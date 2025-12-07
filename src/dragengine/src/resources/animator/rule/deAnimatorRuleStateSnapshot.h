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
class DE_DLL_EXPORT deAnimatorRuleStateSnapshot : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleStateSnapshot> Ref;
	
	
private:
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
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
	inline bool GetEnablePosition() const{return pEnablePosition;}
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition(bool enabled);
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{return pEnableOrientation;}
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation(bool enabled);
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{return pEnableSize;}
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize(bool enabled);
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{return pEnableVertexPositionSet;}
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet(bool enabled);
	
	/** \brief Last state of the component is used or only the stored state. */
	inline bool GetUseLastState() const{return pUseLastState;}
	
	/** \brief Set if last state of the component is used or only the stored state. */
	void SetUseLastState(bool useLastState);
	
	/** \brief Identifier to snapshot state using animator instance. */
	inline int GetID() const{return pID;}
	
	/** \brief Set identifier to snapshot state using animator instance. */
	void SetID(int id);
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit(deAnimatorRuleVisitor &visitor);
	/*@}*/
};

#endif
