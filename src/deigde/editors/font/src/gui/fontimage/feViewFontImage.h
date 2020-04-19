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

#ifndef _FEVIEWFONTIMAGE_H_
#define _FEVIEWFONTIMAGE_H_

#include <deigde/gui/igdeScrollBarReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/layout/igdeContainerBorder.h>

class feWindowMain;
class feViewFIImage;
class feViewFontImageListener;
class feFont;



/**
 * \brief Font image view.
 * 
 * View showing the font image. This view is a container for the actual font image view
 * widget as well as scrollbars for altering the viewport.
 */
class feViewFontImage : public igdeContainerBorder{
private:
	feWindowMain &pWindowMain;
	feViewFontImageListener *pListener;
	feFont *pFont;
	
	decPoint pContentSize;
	decPoint pScrollOffset;
	
	igdeScrollBarReference pSBHorizontal;
	igdeScrollBarReference pSBVertical;
	igdeComboBoxReference pCBZoom;
	feViewFIImage *pViewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font image view. */
	feViewFontImage( feWindowMain &windowMain );
	
	/** \brief Clean up font image view. */
	virtual ~feViewFontImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font. */
	inline feFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont( feFont *font );
	
	/** \brief Set if rendering is enabled. */
	void SetEnableRendering( bool enable );
	
	
	
	/** \brief Game engine has been started. */
	void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	void OnBeforeEngineStop();
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief View image. */
	inline feViewFIImage &GetViewImage() const{ return *pViewImage; }
	
	
	/** \brief Game like frame update. */
	void OnFrameUpdate( float elapsed );
	
	/** \brief Update scrollbar ranges. */
	void UpdateScrollbarRanges();
	
	/** \brief Update image view canvas. */
	void UpdateImageViewCanvas();
	
	/** \brief Scroll view. */
	void ScrollView();
	/*@}*/
};

#endif
