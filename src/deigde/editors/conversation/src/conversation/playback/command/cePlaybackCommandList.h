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
