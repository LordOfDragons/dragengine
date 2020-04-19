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

#ifndef _DEANIMATORCONTROLLERTARGET_H_
#define _DEANIMATORCONTROLLERTARGET_H_


/**
 * \brief Animator Controller Target.
 *
 * Holds links for a controller target.
 */
class deAnimatorControllerTarget{
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
