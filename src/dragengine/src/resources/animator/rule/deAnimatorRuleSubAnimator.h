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

#ifndef _DEANIMATORRULESUBANIMATOR_H_
#define _DEANIMATORRULESUBANIMATOR_H_

#include "deAnimatorRule.h"
#include "../deAnimator::Ref.h"


/**
 * \brief Sub Animator Rule.
 * The sub animator rule uses the state of another animator as the
 * state of this rule. This allows to extend animators without
 * recreating the structure in every animator based on a common
 * base. The additional list of connections determines which
 * controller value in the animator this rule belongs to is
 * written to which controller in the sub animator. When the
 * animator is assigned the connections are created with the
 * number of controllers in the sub animator initialized to map
 * to the 0 local controller no matter if it exists or not. If the
 * sub animator changes especially in terms of controller count
 * you have to detach and reattach it again and update the
 * connections again.
 */
class DE_DLL_EXPORT deAnimatorRuleSubAnimator : public deAnimatorRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deAnimatorRuleSubAnimator> Ref;
	
	
	
private:
	deAnimator::Ref pSubAnimator;
	
	bool pEnablePosition;
	bool pEnableOrientation;
	bool pEnableSize;
	bool pEnableVertexPositionSet;
	
	int *pConnections;
	int pConnectionCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new animator rule. */
	deAnimatorRuleSubAnimator();
	
protected:
	/**
	 * \brief Clean up animator rule.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deAnimatorRuleSubAnimator();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sub animator or NULL if not assigned. */
	deAnimator *GetSubAnimator() const{ return pSubAnimator; }
	
	/** \brief Set sub animator or NULL if not assigned. */
	void SetSubAnimator( deAnimator *animator );
	
	/** \brief Determines if position manipulation is enabled. */
	inline bool GetEnablePosition() const{ return pEnablePosition; }
	
	/** \brief Sets if position manipulation is enabled. */
	void SetEnablePosition( bool enabled );
	
	/** \brief Determines if orientation manipulation is enabled. */
	inline bool GetEnableOrientation() const{ return pEnableOrientation; }
	
	/** \brief Sets if orientation manipulation is enabled. */
	void SetEnableOrientation( bool enabled );
	
	/** \brief Determines if size manipulation is enabled. */
	inline bool GetEnableSize() const{ return pEnableSize; }
	
	/** \brief Sets if size manipulation is enabled. */
	void SetEnableSize( bool enabled );
	
	/** \brief Vertex position sets are enabled. */
	inline bool GetEnableVertexPositionSet() const{ return pEnableVertexPositionSet; }
	
	/** \brief Set if vertex position sets are enabled. */
	void SetEnableVertexPositionSet( bool enabled );
	
	/**
	 * \brief Update connection count.
	 * 
	 * You have to call this method after you changed the component of a sub animator
	 * without removing the sub animator first.
	 */
	void UpdateConnectionCount();
	
	/**
	 * \brief Count of connections which is the number of controllers in the sub animator
	 *        if existing or 0 if there is no sub animator assigned.
	 */
	inline int GetConnectionCount() const{ return pConnectionCount; }
	
	/** \brief Index of the local controller for the given controller in the sub animator. */
	int GetConnectionAt( int targetController ) const;
	
	/** \brief Set index of the local controller for the given controller in the sub animator. */
	void SetConnectionAt( int targetController, int localController );
	
	/** \brief Set connections by matching controller names. */
	void SetMatchingConnections( const deAnimator &animator );
	
	/** \brief Clear all connections setting them to -1. */
	void ClearConnections();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visits the rule. */
	virtual void Visit( deAnimatorRuleVisitor &visitor );
	/*@}*/
};

#endif
