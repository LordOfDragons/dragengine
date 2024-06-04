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

#ifndef _AECONTROLLERTARGET_H_
#define _AECONTROLLERTARGET_H_

#include "../link/aeLinkList.h"

class aeLink;
class aeAnimator;
class deAnimatorControllerTarget;



/**
 * Links for a controller target.
 */
class aeControllerTarget{
private:
	aeLinkList pLinks;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new controller target. */
	aeControllerTarget();
	/** Create a copy of a controller target. */
	aeControllerTarget( const aeControllerTarget &copy );
	/** Cleans up the controller target. */
	~aeControllerTarget();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of links. */
	int GetLinkCount() const;
	/** Retrieves the link at the given index. */
	aeLink *GetLinkAt( int index ) const;
	/** Retrieves the index of the link or -1 if not found. */
	int IndexOfLink( aeLink *link ) const;
	/** Determines if the link exists. */
	bool HasLink( aeLink *link ) const;
	/** Adds a new link. */
	void AddLink( aeLink *link );
	/** Removes a link. */
	void RemoveLink( aeLink *link );
	/** Removes all links. */
	void RemoveAllLinks();
	
	/** Updates the given engine target. */
	void UpdateEngineTarget( aeAnimator *animator, deAnimatorControllerTarget &target ) const;
	
	/** Add links to a list of links. */
	void AddLinksToList( aeLinkList &list );
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** Copy another controller target to this controller target. */
	aeControllerTarget &operator=( const aeControllerTarget &copy );
	/*@}*/
};

#endif
