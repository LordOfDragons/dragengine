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
