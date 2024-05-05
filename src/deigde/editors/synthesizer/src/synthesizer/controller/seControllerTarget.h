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

#ifndef _SECONTROLLERTARGET_H_
#define _SECONTROLLERTARGET_H_

#include "../link/seLinkList.h"

class seLink;
class seSynthesizer;
class deSynthesizerControllerTarget;



/**
 * \brief Links for controller target.
 */
class seControllerTarget{
private:
	seLinkList pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create controller target. */
	seControllerTarget();
	
	/** \brief Create copy of controller target. */
	seControllerTarget( const seControllerTarget &copy );
	
	/** \brief Clean up controller target. */
	~seControllerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at index. */
	seLink *GetLinkAt( int index ) const;
	
	/** \brief Index of link or -1 if absent. */
	int IndexOfLink( seLink *link ) const;
	
	/** \brief Link is present. */
	bool HasLink( seLink *link ) const;
	
	/** \brief Add link. */
	void AddLink( seLink *link );
	
	/** \brief Remove link. */
	void RemoveLink( seLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Update engine target. */
	void UpdateEngineTarget( seSynthesizer *synthesizer, deSynthesizerControllerTarget &target ) const;
	
	/** \brief Add links to list of links. */
	void AddLinksToList( seLinkList &list );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Set from another controller target. */
	seControllerTarget &operator=( const seControllerTarget &copy );
	/*@}*/
};

#endif
