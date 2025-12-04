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

#ifndef _SEVIEWSKIN_H_
#define _SEVIEWSKIN_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>
#include <deigde/undo/igdeUndo.h>


class seWindowMain;
class seSkin;


/**
 * \brief Skin preview.
 */
class seViewSkin : public igdeViewRenderWindow{
private:
	seWindowMain &pWindowMain;
	seSkin *pSkin;
	igdeMouseCameraListenerReference pCameraInteraction;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	seViewSkin( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~seViewSkin();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Skin. */
	inline seSkin *GetSkin() const{ return pSkin; }
	
	/** \brief Set skin. */
	void SetSkin( seSkin *skin );
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	/*@}*/
};

#endif
