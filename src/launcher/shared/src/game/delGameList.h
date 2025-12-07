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

#ifndef _DELGAMELIST_H_
#define _DELGAMELIST_H_

#include <dragengine/common/collection/decObjectList.h>

class delGame;

class decUuid;


/**
 * \brief Game List.
 */
class DE_DLL_EXPORT delGameList{
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
	delGame *GetAt(int index) const;
	
	/** \brief Game with identifier or NULL if not found. */
	delGame *GetWithID(const decUuid &id) const;
	
	/** \brief Games with identifier or empty list. */
	delGameList GetWithAlias(const char *alias) const;
	
	/** \brief Game is present. */
	bool Has(delGame *game) const;
	
	/** \brief Game with identifier eixsts. */
	bool HasWithID(const decUuid &id) const;
	
	/** \brief Index of game or -1 if not found. */
	int IndexOf(delGame *game) const;
	
	/** \brief Index of game with identifier or -1 if not found. */
	int IndexOfWithID(const decUuid &id) const;
	
	/** \brief Add game. */
	void Add(delGame *game);
	
	/** \brief Remove game. */
	void Remove(delGame *game);
	
	/** \brief Remove all games. */
	void RemoveAll();
	/*@}*/
};

#endif
