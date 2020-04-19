/* 
 * Drag[en]gine Android Launcher
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

#ifndef _DEALGAMELIST_H_
#define _DEALGAMELIST_H_

#include "../common/collection/decObjectList.h"

class dealGame;



/**
 * \brief Game List.
 */
class dealGameList{
private:
	decObjectList pGames;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create game list. */
	dealGameList();
	
	/** \brief Clean up game list. */
	~dealGameList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of games. */
	int GetCount() const;
	
	/** \brief Game at the given position. */
	dealGame *GetAt( int index ) const;
	
	/** \brief Game with the given identifier or \em NULL if not found. */
	dealGame *GetWithID( const char *id ) const;
	
	/** \brief Game exists. */
	bool Has( dealGame *game ) const;
	
	/** \brief Game with the given identifier eixsts. */
	bool HasWithID( const char *id ) const;
	
	/** \brief Index of a game or -1 if not found. */
	int IndexOf( dealGame *game ) const;
	
	/** \brief Index of the game with the given identifier or -1 if not found. */
	int IndexOfWithID( const char *id ) const;
	
	/** \brief Add game. */
	void Add( dealGame *game );
	
	/** \brief Remove game. */
	void Remove( dealGame *game );
	
	/** \brief Removes all games. */
	void RemoveAll();
	/*@}*/
};

#endif
