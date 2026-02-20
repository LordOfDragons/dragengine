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

#ifndef _DEOGLDEBUGTRACEGROUP_H_
#define _DEOGLDEBUGTRACEGROUP_H_

class deoglRenderThread;


/**
 * Scoped debug tracing group.
 */
class deoglDebugTraceGroup{
private:
	const deoglRenderThread &pRenderThread;
	bool pOpen;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Begin debug tracing group. */
	deoglDebugTraceGroup(const deoglRenderThread &renderThread, const char *name, int id = 0);
	
	/** Close other group then begin debug tracing group. */
	deoglDebugTraceGroup(deoglDebugTraceGroup &closeGroup, const char *name, int id = 0);
	
	/** End debug tracing group if open. */
	~deoglDebugTraceGroup();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** End debug tracing group if open. */
	void Close();
	/*@}*/
};

#endif
