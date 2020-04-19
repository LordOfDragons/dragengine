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

#ifndef _IGDETRIGGEREXPRESSIONCOMPONENT_H_
#define _IGDETRIGGEREXPRESSIONCOMPONENT_H_

#include <dragengine/deObject.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>

class igdeTriggerTarget;
class igdeTriggerListener;
class igdeTriggerTargetList;


/**
 * \brief Trigger Expression Component.
 */
class igdeTriggerExpressionComponent : public deObject{
public:
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
	igdeTriggerTarget *pTarget;
	igdeTriggerListener *pTargetListener;
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
	void SetNegate( bool negate );
	
	/** \brief Current state is queried instead of testing if the trigger ever fired. */
	inline bool GetCurState() const{ return pCurState; }
	
	/** \brief Set if current state is queried instead of testing if the trigger ever fired. */
	void SetCurState( bool curState );
	
	/** \brief Type of component. */
	inline eComponentTypes GetType() const{ return pType; }
	
	/** \brief Set type of component. */
	void SetType( eComponentTypes type );
	
	/** \brief Target name. */
	inline const decString &GetTargetName() const{ return pTargetName; }
	
	/** \brief Set target name. */
	void SetTargetName( const char *name );
	
	/** \brief Trigger target or NULL. */
	inline igdeTriggerTarget *GetTarget() const{ return pTarget; }
	
	/** \brief Set trigger target or NULL. */
	void SetTarget( igdeTriggerTarget *target );
	
	/** \brief Target listener or NULL. */
	inline igdeTriggerListener *GetTargetListener() const{ return pTargetListener; }
	
	/** \brief Set target listener or NULL. */
	void SetTargetListener( igdeTriggerListener *listener );
	
	/** \brief Link trigger targets using the given trigger table. */
	void LinkTargets( igdeTriggerTargetList &triggerTable, igdeTriggerListener *listener );
	
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
	igdeTriggerExpressionComponent *GetChildAt( int index ) const;
	
	/** \brief Index of child component or -1 if not found. */
	int IndexOfChild( igdeTriggerExpressionComponent *child ) const;
	
	/** \brief Add child. */
	void AddChild( igdeTriggerExpressionComponent *child );
	
	/** \brief Insert child before index. */
	void InsertChild( igdeTriggerExpressionComponent *child, int index );
	
	/** \brief Remove child. */
	void RemoveChild( igdeTriggerExpressionComponent *child );
	
	/** \brief Remove all children. */
	void RemoveAllChildren();
	/*@}*/
};

#endif
