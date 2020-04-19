/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CEPLAYBACKCOMMANDLIST_H_
#define _CEPLAYBACKCOMMANDLIST_H_

#include <dragengine/common/collection/decObjectOrderedSet.h>

class cePlaybackCommand;



/**
 * \brief Playback Command List.
 */
class cePlaybackCommandList{
private:
	decObjectOrderedSet pCommands;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new list. */
	cePlaybackCommandList();
	/** \brief Creates a copy of a list. */
	cePlaybackCommandList( const cePlaybackCommandList &list );
	/** \brief Cleans up the list. */
	~cePlaybackCommandList();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of commands. */
	int GetCount() const;
	/** \brief Retrieves the command at the given position. */
	cePlaybackCommand *GetAt( int index ) const;
	/** \brief Retrieves the command by command or NULL if not found. */
	cePlaybackCommand *GetWith( const char *command ) const;
	/** \brief Retrieves the index of a command or -1 if not found. */
	int IndexOf( cePlaybackCommand *command ) const;
	/** \brief Retrieves the index of a command by command or -1 if not found. */
	int IndexWith( const char *command ) const;
	/** \brief Determines if a command exists. */
	bool Has( cePlaybackCommand *command ) const;
	/** \brief Determines if a command by command exists. */
	bool HasWith( const char *command ) const;
	/** \brief Adds a command. */
	void Add( cePlaybackCommand *command );
	/** \brief Removes a command. */
	void Remove( cePlaybackCommand *command );
	/** \brief Removes all commands. */
	void RemoveAll();
	
	/** Sets the list from another list. */
	cePlaybackCommandList &operator=( const cePlaybackCommandList &list );
	/*@}*/
};

#endif
