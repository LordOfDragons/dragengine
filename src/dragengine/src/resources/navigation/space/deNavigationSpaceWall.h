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

#ifndef _DENAVIGATIONSPACEWALL_H_
#define _DENAVIGATIONSPACEWALL_H_


/**
 * \brief Navigation Space Wall.
 */
class deNavigationSpaceWall{
private:
	unsigned short pFace;
	unsigned short pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new navigation space wall. */
	deNavigationSpaceWall();
	
	/** \brief Clean up navigation space wall. */
	~deNavigationSpaceWall();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Index of the face. */
	inline unsigned short GetFace() const{ return pFace; }
	
	/** \brief Set index of the face. */
	void SetFace( unsigned short face );
	
	/** \brief Type to use for crossing the face. */
	inline unsigned short GetType() const{ return pType; }
	
	/** \brief Set type to use for crossing the face. */
	void SetType( unsigned short type );
	/*@}*/
};

#endif
