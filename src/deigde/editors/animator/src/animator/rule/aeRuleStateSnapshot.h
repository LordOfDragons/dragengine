/* 
 * Drag[en]gine IGDE Animator Editor
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

#ifndef _AERULESTATESNAPSHOT_H_
#define _AERULESTATESNAPSHOT_H_

#include "aeRule.h"



/**
 * Animator rule state snapshot.
 */
class aeRuleStateSnapshot : public aeRule{
public:
	typedef deTObjectReference<aeRuleStateSnapshot> Ref;
	
private:
	bool pUseLastState;
	int pID;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new state snapshot rule. */
	aeRuleStateSnapshot();
	/** Create a copy of a state snapshot rule. */
	aeRuleStateSnapshot( const aeRuleStateSnapshot &copy );
	/** Clean up the animator rule. */
	virtual ~aeRuleStateSnapshot();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Determine if the last state of the component is used or only the stored state. */
	inline bool GetUseLastState() const{ return pUseLastState; }
	/** Set if the last state of the component is used or only the stored state. */
	void SetUseLastState( bool useLastState );
	
	/** Identifier to snapshot state using animator instance. */
	inline int GetID() const{ return pID; }
	
	/** Set identifier to snapshot state using animator instance. */
	void SetID( int id );
	
	/** Determine if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	/** Set if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** Determine if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** Set if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** Determine if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** Set if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** Vertex position set manipulation is enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** Set if vertex position set manipulation is enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/** Create an engine animator rule. */
	virtual deAnimatorRule *CreateEngineRule();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another state snapshot rule to this state snapshot rule. */
	virtual aeRuleStateSnapshot &operator=( const aeRuleStateSnapshot &copy );
	/*@}*/
};

#endif
