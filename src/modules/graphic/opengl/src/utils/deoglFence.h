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

#ifndef _DEOGLFENCE_H_
#define _DEOGLFENCE_H_

#include "../deoglBasics.h"

#include <dragengine/deObject.h>

class deoglRenderThread;


/**
 * Fence.
 */
class deoglFence : public deObject{
public:
	typedef deTObjectReference<deoglFence> Ref;
	
	
private:
	deoglRenderThread &pRenderThread;
	GLsync pFence;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create fence. */
	deoglFence(deoglRenderThread &renderThread);
	
protected:
	/** Clean up fence. */
	virtual ~deoglFence();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Fence or 0 if not armed. */
	inline GLsync GetFence() const{return pFence;}
	
	/** Resets fence then arms it. */
	void Arm();
	
	/** Test if fence has fired. If fence is not armed true is returned. */
	bool HasFired();
	
	/** Wait for fence to be fired. If fence is not armed returns immediately. */
	void Wait();
	
	/** Deletes fence if present. */
	void Reset();
	/*@}*/
};

#endif
