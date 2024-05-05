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

#ifndef _DEGLGAMEICON_H_
#define _DEGLGAMEICON_H_

#include "../gui/deglSharedIcon.h"

#include <delauncher/game/icon/delGameIcon.h>


/**
 * Game icon.
 */
class deglGameIcon : public delGameIcon{
public:
	/** Type holding strong reference. */
	typedef deTObjectReference<deglGameIcon> Ref;
	
	
	
private:
	deglSharedIcon::Ref pFoxIcon;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create game iconn. */
	deglGameIcon( int size, const char *path );
	
protected:
	/** Cleans up the game. */
	virtual ~deglGameIcon();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** FOX icon or nullptr if unable to load. */
	inline deglSharedIcon *GetFoxIcon() const{ return pFoxIcon; }
	
	/** FOX icon scaled to size or nullptr if unable to load. */
	deglSharedIcon::Ref GetScaledFoxIcon( int size ) const;
	/*@}*/
	
	
	
protected:
	virtual void OnContentChanged();
	
	
	
private:
	void pCreateFoxIcon();
	void pCreatePNGFoxIcon();
};

#endif
