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
class DE_DLL_EXPORT delEngineProcessRunGame : public deThread{
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
