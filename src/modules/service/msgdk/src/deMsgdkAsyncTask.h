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

#ifndef DEMSGDKASYNCTASK_H
#define DEMSGDKASYNCTASK_H

#include "gdk_include.h"
#include <dragengine/deObject.h>


/**
 * Asynchronous task.
 */
class deMsgdkAsyncTask : public deObject
{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deMsgdkAsyncTask> Ref;


public:
	/**
	 * Invalidator used if owner of task is destroyed to avoid segfaults.
	 * Owner has one invalidator shared between all running tasks.
	 */
	class Invalidator : public deObject
	{
	private:
		bool pValid;

	public:
		typedef deTObjectReference<Invalidator> Ref;
	
	
		Invalidator();
		inline bool IsValid() const{ return pValid; }
		void Invalidate();
	};



private:
	const Invalidator::Ref pInvalidator;
	XAsyncBlock pAsyncBlock;



public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create asynchronous task. Object removed reference itself once finished. */
	deMsgdkAsyncTask(const Invalidator::Ref &invalidator);
	
protected:
	/** Delete asynchronous task. */
	~deMsgdkAsyncTask() override;
	/*@}*/



public:
	/** Frame update task if registered. */
	virtual void OnFrameUpdate(float elapsed);



protected:
	inline XAsyncBlock &GetAsyncBlock(){ return pAsyncBlock; }
	inline const XAsyncBlock &GetAsyncBlock() const{ return pAsyncBlock; }
	inline XAsyncBlock *GetAsyncBlockPtr(){ return &pAsyncBlock; }
	void ResetAsyncBlock();
	virtual void OnFinished() = 0;
};

#endif
