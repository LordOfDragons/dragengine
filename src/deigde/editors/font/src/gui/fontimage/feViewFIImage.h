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

#ifndef _FEVIEWFIIMAGE_H_
#define _FEVIEWFIIMAGE_H_

#include <deigde/gui/igdeViewRenderWindow.h>

#include <dragengine/resources/canvas/deCanvasPaintReference.h>
#include <dragengine/resources/canvas/deCanvasImageReference.h>

class feWindowMain;
class feFont;



/**
 * \brief Preview font image.
 */
class feViewFIImage : public igdeViewRenderWindow{
private:
	feWindowMain &pWindowMain;
	
	feFont *pFont;
	
	int pBorderSize;
	int pZoom;
	decPoint pOffset;
	
	deCanvasPaintReference pCanvasImageBackground;
	deCanvasImageReference pCanvasFontImage;
	deCanvasPaintReference pCanvasActiveGlyph;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view font image renderer. */
	feViewFIImage( feWindowMain &windowMain );
	
	/** \brief Clean up view font image renderer. */
	virtual ~feViewFIImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	
	
	/** \brief Font. */
	inline feFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont( feFont *font );
	
	
	
	/** \brief Zoom factor in percentage. */
	inline int GetZoom() const{ return pZoom; }
	
	/** \brief Set zoom factor in percentage. */
	void SetZoom( int zoom );
	
	/** \brief Offset in pixels. */
	inline const decPoint &GetOffset() const{ return pOffset; }
	
	/** \brief Set offset in pixels. */
	void SetOffset( const decPoint &offset );
	
	
	
	/** \brief Content size. */
	decPoint GetContentSize() const;
	
	
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	
	
	/** \brief Resize canvas. */
	void ResizeCanvas();
	/*@}*/
};

#endif
