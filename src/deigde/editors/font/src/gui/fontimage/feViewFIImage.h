/* 
 * Drag[en]gine IGDE Font Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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
