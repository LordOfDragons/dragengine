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

#ifndef _DEDSLOADINGSCREEN_H_
#define _DEDSLOADINGSCREEN_H_

#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasText.h>

class deScriptingDragonScript;


/**
 * Loading screen. Used while scripts are loaded and compiled before the game can take over
 * the screen. Shows a black screen with an image and text.
 */
class dedsLoadingScreen : public deObject{
public:
	typedef deTObjectReference<dedsLoadingScreen> Ref;
	
	
private:
	deScriptingDragonScript &pDS;
	deCanvasPaint::Ref pCanvasBackground;
	deCanvasImage::Ref pCanvasImage;
	deCanvasText::Ref pCanvasText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create loading screen. */
	dedsLoadingScreen( deScriptingDragonScript &ds );
	
protected:
	/** Clean up loading screen. */
	virtual ~dedsLoadingScreen();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Update loading screen. */
	void Update();
	/*@}*/
	
	
	
private:
	void pCreateScreen();
	void pCleanUp();
};

#endif
