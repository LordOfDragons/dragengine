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

#ifndef _IGDENATIVEFOXRESIZER_H_
#define _IGDENATIVEFOXRESIZER_H_

#include "foxtoolkit.h"


/**
 * \brief FOX toolkit Native resizer.
 * 
 * When dragged sends SEL_COMMAND with the drag distance as integer parameter.
 * The receiver is responsible to resize according to the dragged distance.
 */
class igdeNativeFoxResizer : public FXWindow{
	FXDECLARE(igdeNativeFoxResizer)
protected:
	   igdeNativeFoxResizer();
	
private:
	bool pIsDragging;
	FXCursor *pCursor;
	int pGripSize;
	int pGripWidth;
	FXColor pActiveColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create resizer. */
	igdeNativeFoxResizer(FXComposite *parent, FXObject *target, FXSelector selector);
	
	/** \brief Clean up resizer. */
	virtual ~igdeNativeFoxResizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Default height. */
	virtual FXint getDefaultHeight();
	
	/** \brief Dragged distance from SEL_COMMAND event data object. */
	static int SelCommandDraggedDistance(void *data);
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onPaint(FXObject*, FXSelector, void*);
	long onEnter(FXObject*, FXSelector, void*);
	long onLeave(FXObject*, FXSelector, void*);
	long onLeftMouseDown(FXObject*, FXSelector, void*);
	long onLeftMouseUp(FXObject*, FXSelector, void*);
	long onMouseMoved(FXObject*, FXSelector, void*);
	/*@}*/
};

#endif
