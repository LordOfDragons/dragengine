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

#ifndef _DEOGLCAPTURECANVASLIST_H_
#define _DEOGLCAPTURECANVASLIST_H_

#include <dragengine/common/collection/decPointerSet.h>

class deGraphicOpenGl;
class deoglCaptureCanvas;



/**
 * Capture canvas list.
 */
class deoglCaptureCanvasList{
private:
	deGraphicOpenGl &pOgl;
	decPointerSet pCaptureCanvas;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create list. */
	deoglCaptureCanvasList(deGraphicOpenGl &ogl);
	
	/** Clean up list. */
	~deoglCaptureCanvasList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of capture canvas. */
	int GetCount() const;
	
	/** Capture canvas at index. */
	deoglCaptureCanvas *GetAt(int index) const;
	
	/** Add capture canvas. */
	void Add(deoglCaptureCanvas *captureCanvas);
	
	/** Remove capture canvas . */
	void Remove(deoglCaptureCanvas *captureCanvas);
	
	/** Removes all capture canvas. */
	void RemoveAll();
	
	
	
	/**
	 * Update capture canvas if required.
	 * \details For finished captures the image is created from the pixel buffer and
	 *          stored for the game scripts to retrieve. For pending captures the
	 *          capture process is started.
	 */
	void SyncToRender();
	/*@}*/
};

#endif
