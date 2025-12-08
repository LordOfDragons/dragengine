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

#ifndef _IGDETRIGGEREXPRESSION_H_
#define _IGDETRIGGEREXPRESSION_H_

#include <dragengine/deObject.h>

#include "igdeTriggerExpressionComponent.h"
class igdeTriggerTargetList;
class igdeTriggerListener;



/**
 * \brief Trigger Expression.
 */
class DE_DLL_EXPORT igdeTriggerExpression : public deObject{

public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeTriggerExpression> Ref;
	
	
private:
	igdeTriggerExpressionComponent::Ref pRootComponent;
	bool pResult;
	bool pEnabled;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create trigger expression. */
	igdeTriggerExpression();
	
	/** \brief Clean up trigger expression. */
	virtual ~igdeTriggerExpression();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Root expression component or null if not set. */
	inline const igdeTriggerExpressionComponent::Ref &GetRootComponent() const{ return pRootComponent; }
	
	/** \brief Set root expression component or null if not set. */
	void SetRootComponent(igdeTriggerExpressionComponent *component);
	
	/** \brief Determines if the expression is empty hence root component is null. */
	bool IsEmpty() const;
	
	/** \brief Determines if the expression is not empty hence root component is not null. */
	bool IsNotEmpty() const;
	
	/** \brief Result of the expression. */
	inline bool GetResult() const{ return pResult; }
	
	/** \brief Set result of the expression. */
	void SetResult(bool result);
	
	/** \brief Determines if the expression is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set expression is enabled. */
	void SetEnabled(bool enabled);
	
	/**
	 * \brief Link trigger targets using the given trigger table.
	 * 
	 * The given listener is registered for all targets. A previously existing listener
	 * is removed first. After linking the expression is evaluated.
	 */
	void LinkTriggerTargets(igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener);
	
	/** \brief Unlink trigger targets. A previously existing listener is removed first. */
	void UnlinkTriggerTargets();
	
	/**
	 * \brief Evaluate expression if not disabled.
	 * 
	 * Stores result to be later obtained. If result changed true is returned otherwise false.
	 */
	bool Evaluate();
	/*@}*/
};

#endif
