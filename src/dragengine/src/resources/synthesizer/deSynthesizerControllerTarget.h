/* 
 * Drag[en]gine Game Engine
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

#ifndef _DESYNTHESIZERCONTROLLERTARGET_H_
#define _DESYNTHESIZERCONTROLLERTARGET_H_

#include "../../common/collection/decIntList.h"


/**
 * \brief Synthesizer controller target.
 */
class DE_DLL_EXPORT deSynthesizerControllerTarget{
private:
	decIntList pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create target. */
	deSynthesizerControllerTarget();
	
	/** \brief Create copy of target. */
	deSynthesizerControllerTarget( const deSynthesizerControllerTarget &target );
	
	/** \brief Clean up target. */
	~deSynthesizerControllerTarget();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/**
	 * \brief Link at index.
	 * \throws deeOutOfBoundary \em index is less than 0 or greater than or equal to GetLinkCount().
	 */
	int GetLinkAt( int index ) const;
	
	/** \brief Index of link or -1 if absent. */
	int IndexOfLink( int link ) const;
	
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
