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

#ifndef _DEANIMATORCONTROLLERTARGET_H_
#define _DEANIMATORCONTROLLERTARGET_H_

#include "../../../dragengine_export.h"

/**
 * \brief Animator Controller Target.
 *
 * Holds links for a controller target.
 */
class DE_DLL_EXPORT deAnimatorControllerTarget{
private:
	int *pLinks;
	int pLinkCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new target. */
	deAnimatorControllerTarget();
	
	/** \brief Clean up target. */
	~deAnimatorControllerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	inline int GetLinkCount() const{ return pLinkCount; }
	
	/** \brief Link at the given index. */
	int GetLinkAt( int index ) const;
	
	/** \brief Index of the link or -1 if not found. */
	int IndexOfLink( int link ) const;
	
	/** \brief Adds a new link. */
	void AddLink( int link );
	
	/** \brief Removes a link. */
	void RemoveLink( int link );
	
	/** \brief Removes all links. */
	void RemoveAllLinks();
	/*@}*/
	
private:
};

#endif
