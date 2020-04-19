/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDETRIGGEREXPRESSION_H_
#define _IGDETRIGGEREXPRESSION_H_

#include <dragengine/deObject.h>

class igdeTriggerExpressionComponent;
class igdeTriggerTargetList;
class igdeTriggerListener;



/**
 * \brief Trigger Expression.
 */
class igdeTriggerExpression : public deObject{
private:
	igdeTriggerExpressionComponent *pRootComponent;
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
	inline igdeTriggerExpressionComponent *GetRootComponent() const{ return pRootComponent; }
	
	/** \brief Set root expression component or null if not set. */
	void SetRootComponent( igdeTriggerExpressionComponent *component );
	
	/** \brief Determines if the expression is empty hence root component is null. */
	bool IsEmpty() const;
	
	/** \brief Determines if the expression is not empty hence root component is not null. */
	bool IsNotEmpty() const;
	
	/** \brief Result of the expression. */
	inline bool GetResult() const{ return pResult; }
	
	/** \brief Set result of the expression. */
	void SetResult( bool result );
	
	/** \brief Determines if the expression is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set expression is enabled. */
	void SetEnabled( bool enabled );
	
	/**
	 * \brief Link trigger targets using the given trigger table.
	 * 
	 * The given listener is registered for all targets. A previously existing listener
	 * is removed first. After linking the expression is evaluated.
	 */
	void LinkTriggerTargets( igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener );
	
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
