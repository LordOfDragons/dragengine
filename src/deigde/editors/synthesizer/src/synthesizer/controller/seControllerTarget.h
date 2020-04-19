/* 
 * Drag[en]gine IGDE Synthesizer Editor
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
