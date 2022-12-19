/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
