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

#ifndef _AERULESUBANIMATOR_H_
#define _AERULESUBANIMATOR_H_

#include "aeRule.h"

#include <dragengine/common/collection/decObjectList.h>

class aeController;
class aeLoadSaveSystem;

class deAnimator;
class deAnimatorRuleSubAnimator;



/**
 * Animator rule sub animator.
 */
class aeRuleSubAnimator : public aeRule{
public:
	typedef deTObjectReference<aeRuleSubAnimator> Ref;
	
	
private:
	decString pPathSubAnimator;
	deAnimator *pSubAnimator;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	decObjectList pConnections;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new sub animator rule. */
	aeRuleSubAnimator();
	/** Create a copy of a sub animator rule. */
	aeRuleSubAnimator( const aeRuleSubAnimator &copy );
	/** Clean up the sub animator rule. */
	virtual ~aeRuleSubAnimator();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieve the path to the sub animator. */
	inline const decString &GetPathSubAnimator() const{ return pPathSubAnimator; }
	/** Set the path to the sub animator. */
	void SetPathSubAnimator( const char *path );
	/** Retrieve the sub animator or NULL if not existing. */
	inline deAnimator *GetSubAnimator() const{ return pSubAnimator; }
	/** Load the sub animator using the stored path. */
	void LoadSubAnimator();
	
	/** Number of connections. */
	int GetConnectionCount() const;
	
	/** Controller for target controller or \em NULL. */
	aeController *GetControllerAt( int position ) const;
	
	/** Set controller for target controller or \em NULL. */
	void SetControllerAt( int position, aeController *controller );
	
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
	
	/** Update Component and Animation. */
	virtual void UpdateCompAnim();
	
	/** Create a copy of this rule. */
	virtual aeRule *CreateCopy() const;
	
	/** List all links of all rule targets. */
	virtual void ListLinks( aeLinkList& list );
	
	/** Parent animator changed. */
	virtual void OnParentAnimatorChanged();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another sub animator rule to this sub animator rule. */
	virtual aeRuleSubAnimator &operator=( const aeRuleSubAnimator &copy );
	/*@}*/
	
private:
	void pUpdateConnections( deAnimatorRuleSubAnimator &rule ) const;
};

#endif
