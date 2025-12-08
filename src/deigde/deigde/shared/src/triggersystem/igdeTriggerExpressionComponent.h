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

#ifndef _IGDETRIGGEREXPRESSIONCOMPONENT_H_
#define _IGDETRIGGEREXPRESSIONCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

#include "igdeTriggerTarget.h"
#include "igdeTriggerListener.h"
class igdeTriggerTargetList;


/**
 * \brief Trigger Expression Component.
 */
class DE_DLL_EXPORT igdeTriggerExpressionComponent : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTriggerExpressionComponent> Ref;
	
	
	/** \brief Component type. */
	enum eComponentTypes{
		/** \brief Target component. */
		ectTarget,
		
		/** \brief Logic AND between child components. */
		ectAnd,
		
		/** \brief Logic OR between child components. */
		ectOr
	};
	
	
	
private:
	bool pNegate;
	bool pCurState;
	eComponentTypes pType;
	decString pTargetName;
	igdeTriggerTarget::Ref pTarget;
	igdeTriggerListener::Ref pTargetListener;
	decObjectOrderedSet pChildred;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger expression component. */
	igdeTriggerExpressionComponent();
	
	
	
protected:
	/** \brief Clean up trigger expression component. */
	virtual ~igdeTriggerExpressionComponent();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Test result is negated. */
	inline bool GetNegate() const{ return pNegate; }
	
	/** \brief Set if test result is negated. */
	void SetNegate(bool negate);
	
	/** \brief Current state is queried instead of testing if the trigger ever fired. */
	inline bool GetCurState() const{ return pCurState; }
	
	/** \brief Set if current state is queried instead of testing if the trigger ever fired. */
	void SetCurState(bool curState);
	
	/** \brief Type of component. */
	inline eComponentTypes GetType() const{ return pType; }
	
	/** \brief Set type of component. */
	void SetType(eComponentTypes type);
	
	/** \brief Target name. */
	inline const decString &GetTargetName() const{ return pTargetName; }
	
	/** \brief Set target name. */
	void SetTargetName(const char *name);
	
	/** \brief Trigger target or NULL. */
	inline const igdeTriggerTarget::Ref &GetTarget() const{ return pTarget; }
	
	/** \brief Set trigger target or NULL. */
	void SetTarget(igdeTriggerTarget *target);
	
	/** \brief Target listener or NULL. */
	inline const igdeTriggerListener::Ref &GetTargetListener() const{ return pTargetListener; }
	
	/** \brief Set target listener or NULL. */
	void SetTargetListener(igdeTriggerListener *listener);
	
	/** \brief Link trigger targets using the given trigger table. */
	void LinkTargets(igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener);
	
	/** \brief Unlink trigger targets. */
	void UnlinkTargets();
	
	/** \brief Evaluate expression component. */
	bool Evaluate();
	/*@}*/
	
	
	
	/** \name Children */
	/*@{*/
	/** \brief Count of child components. */
	int GetChildCount() const;
	
	/** \brief Child component at index. */
	igdeTriggerExpressionComponent *GetChildAt(int index) const;
	
	/** \brief Index of child component or -1 if not found. */
	int IndexOfChild(igdeTriggerExpressionComponent *child) const;
	
	/** \brief Add child. */
	void AddChild(igdeTriggerExpressionComponent *child);
	
	/** \brief Insert child before index. */
	void InsertChild(igdeTriggerExpressionComponent *child, int index);
	
	/** \brief Remove child. */
	void RemoveChild(igdeTriggerExpressionComponent *child);
	
	/** \brief Remove all children. */
	void RemoveAllChildren();
	/*@}*/
};

#endif
