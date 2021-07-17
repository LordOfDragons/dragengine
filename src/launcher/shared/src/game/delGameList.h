/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DELGAMELIST_H_
#define _DELGAMELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGame;

class decUuid;


/**
 * \brief Game List.
 */
class delGameList{
private:
	decObjectList pGames;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game list. */
	delGameList();
	
	/** \brief Clean up game list. */
	~delGameList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of games. */
	int GetCount() const;
	
	/** \brief Game at index. */
	delGame *GetAt( int index ) const;
	
	/** \brief Game with identifier or NULL if not found. */
	delGame *GetWithID( const decUuid &id ) const;
	
	/** \brief Games with identifier or empty list. */
	delGameList GetWithAlias( const char *alias ) const;
	
	/** \brief Game is present. */
	bool Has( delGame *game ) const;
	
	/** \brief Game with identifier eixsts. */
	bool HasWithID( const decUuid &id ) const;
	
	/** \brief Index of game or -1 if not found. */
	int IndexOf( delGame *game ) const;
	
	/** \brief Index of game with identifier or -1 if not found. */
	int IndexOfWithID( const decUuid &id ) const;
	
	/** \brief Add game. */
	void Add( delGame *game );
	
	/** \brief Remove game. */
	void Remove( delGame *game );
	
	/** \brief Remove all games. */
	void RemoveAll();
	/*@}*/
};

#endif
