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

#ifndef _DEANIMATORRULE_H_
#define _DEANIMATORRULE_H_

#include "../controller/deAnimatorControllerTarget.h"
#include "../../../common/string/decStringSet.h"
#include "../../../deObject.h"

class deAnimator;
class deAnimatorRuleVisitor;


/**
 * \brief Animator Rule Class.
 * \details
 * Animator rules are the construction unit of an animator. There are
 * many different ways how an animation can be produced so the rules
 * are split into different classes providing different functionality.
 * The animator rule class is the base class for all the various rule
 * classes. To make life easier this class provides two ways to work
 * with the rule classes. One way is visiting and the other is safe
 * casting. Both ways have their advantage in the right situation.
 * Furthermore the base class provides some common functionality that
 * all rules share. Each rule can have one or more controllers and
 * bones assigned. The subclass DE_DLL_EXPORT decides how many of those controllers
 * are used. Hence specifying more than one controller for a rule
 * using only one controller does not hurt. The rule in question simply
 * uses the first controller then. Rules can be disabled to take them
 * out of the production process easy without requiring to remove and
 * add them from the animator. The list of bones specifies on what
 * bones the rule operates. For ease of use a back reference to the
 * animator is stored. This reference is a weak reference.
 * The blending is controlled with the source and destination blend
 * factor similar to the ones in the animator.
 */
class DE_DLL_EXPORT deAnimatorRule : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRule> Ref;
	
	
public:
	/** \brief Blending Modes. */
	enum eBlendModes{
		/** \brief Blend from the existing state over to the state produced by the rule. */
		ebmBlend,
		
		/** \brief Overlay the existing state with to the state produced by the rule. */
		ebmOverlay
	};
	
	
	
private:
	deAnimator *pAnimator;
	decStringSet pListBones;
	decStringSet pListVertexPositionSets;
	eBlendModes pBlendMode;
	float pBlendFactor;
	bool pInvertBlendFactor;
	bool pEnabled;
	
	deAnimatorControllerTarget pTargetBlendFactor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRule();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRule();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Parent animator. */
	inline deAnimator *GetAnimator() const{ return pAnimator; }
	
	/**
	 * \brief Set parent animator.
	 * 
	 * This function has to be used only by the deAnimator class to
	 * assign itself as a parent. Do not call this function directly.
	 */
	void SetAnimator(deAnimator *animator);
	
	/** \brief Rule is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if rule is enabled. */
	void SetEnabled(bool enabled);
	
	/** \brief Blend mode. */
	inline eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** \brief Set blend mode. */
	void SetBlendMode(eBlendModes mode);
	
	/** \brief Blend factor. */
	inline float GetBlendFactor() const{ return pBlendFactor; }
	
	/** \brief Set source blend factor. */
	void SetBlendFactor(float factor);
	
	/**
	 * \brief Invert blend factor (1 - factor).
	 * \version 1.13
	 */
	inline bool GetInvertBlendFactor() const{ return pInvertBlendFactor; }
	
	/**
	 * \brief Set to invert blend factor (1 - factor).
	 * \version 1.13
	 */
	void SetInvertBlendFactor(bool invert);
	
	/** \brief List of bones. */
	inline decStringSet &GetListBones(){ return pListBones; }
	inline const decStringSet &GetListBones() const{ return pListBones; }
	
	/**
	 * \brief List of vertex position sets.
	 * \version 1.17
	 */
	inline decStringSet &GetListVertexPositionSets(){ return pListVertexPositionSets; }
	inline const decStringSet &GetListVertexPositionSets() const{ return pListVertexPositionSets; }
	
	/** \brief Blend factor target. */
	inline deAnimatorControllerTarget &GetTargetBlendFactor(){ return pTargetBlendFactor; }
	inline const deAnimatorControllerTarget &GetTargetBlendFactor() const{ return pTargetBlendFactor; }
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit(deAnimatorRuleVisitor &visitor);
	/*@}*/
};

#endif
