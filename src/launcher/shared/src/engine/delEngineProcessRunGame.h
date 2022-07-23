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

#ifndef _DELENGINEPROCESSRUNGAME_H_
#define _DELENGINEPROCESSRUNGAME_H_

#include <dragengine/threading/deThread.h>

class delEngineProcess;


/**
 * \brief Game Engine Process Run Game Thread.
 * 
 * A separate thread used while the game is running. To avoid spending time
 * checking the in pipe for commands this task is handled by a separate
 * thread instead. This way most of the time this thread is blocking waiting
 * for a command on the pipe consuming no CPU time while the game runs in
 * the main thread of the process.
 */
class delEngineProcessRunGame : public deThread{
private:
	delEngineProcess &pProcess;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create run game thread. */
	delEngineProcessRunGame( delEngineProcess &process );
	
	/** \brief Cleas up run game thread. */
	virtual ~delEngineProcessRunGame();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Run thread function. */
	virtual void Run();
	
	/**
	 * \brief Read command from pipe.
	 * 
	 * blocks the thread until the pipe has a command. in the unholy case of the game
	 * crashing an exception is thrown. in this case we just go home an leave the
	 * cleaning up to the parent process.
	 * 
	 * This read call is different from the conventional read calls used in the engine
	 * process since it is blocking for the entire runtime of the game. in some
	 * situations though an interrupt can come around breakin the read call and
	 * returning EINTR. in this case the read call has to be restartet. the read calls
	 * used in the engine process are called if data is expected to be ready and are
	 * thus not prone to this problem. here though we have to take care of this
	 * peticular situation.
	 */
	int BlockingReadCommandFromPipe();
	/*@}*/
};

#endif
