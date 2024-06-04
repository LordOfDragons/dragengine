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

#ifndef _DESKYCONTROLLERTARGET_H_
#define _DESKYCONTROLLERTARGET_H_

#include "../../common/collection/decIntList.h"


/**
 * \brief Sky controller target.
 */
class DE_DLL_EXPORT deSkyControllerTarget{
private:
	decIntList pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create target. */
	deSkyControllerTarget();
	
	/** \brief Create copy of target. */
	deSkyControllerTarget( const deSkyControllerTarget &target );
	
	/** \brief Clean up target. */
	~deSkyControllerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/**
	 * \brief Link at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal
	 * to GetLinkCount().
	 */
	int GetLinkAt( int index ) const;
	
	/** \brief Link is present. */
	bool HasLink( int link ) const;
	
	/** \brief Add link. */
	void AddLink( int link );
	
	/**
	 * \brief Remove link.
	 * \throws deeInvalidParam \em link is absent.
	 */
	void RemoveLink( int link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	/*@}*/
};

#endif
