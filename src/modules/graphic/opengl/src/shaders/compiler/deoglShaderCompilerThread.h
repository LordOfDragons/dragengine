/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DEOGLSHADERCOMPILERTHREAD_H_
#define _DEOGLSHADERCOMPILERTHREAD_H_

#include <dragengine/threading/deThread.h>

class deoglShaderLanguage;
class deoglShaderCompiler;


/**
 * Shader compiler thread.
 */
class deoglShaderCompilerThread : public deThread{
public:
	enum class State{
		prepare,
		ready,
		failed
	};
	
private:
	deoglShaderLanguage &pLanguage;
	int pContextIndex;
	deoglShaderCompiler *pCompiler;
	bool pExitThread;
	State pState;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shader compiler thread. */
	deoglShaderCompilerThread(deoglShaderLanguage &language, int contextIndex);
	
	/** Clean up shader compiler thread. */
	~deoglShaderCompilerThread();
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** Run function of thread. */
	void Run() override;
	
	/** Request thread to exit. */
	void RequestExit();
	
	/** Thread state. */
	State GetState();
	/*@}*/
	
	
private:
	void pCleanUp();
	void pActivateContext();
	bool pExitThreadRequested();
};

#endif
