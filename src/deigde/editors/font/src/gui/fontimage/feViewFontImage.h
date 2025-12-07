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

#ifndef _FEVIEWFONTIMAGE_H_
#define _FEVIEWFONTIMAGE_H_

#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/igdeComboBox.h>
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
	
	igdeScrollBar::Ref pSBHorizontal;
	igdeScrollBar::Ref pSBVertical;
	igdeComboBox::Ref pCBZoom;
	feViewFIImage *pViewImage;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create font image view. */
	feViewFontImage(feWindowMain &windowMain);
	
	/** \brief Clean up font image view. */
	virtual ~feViewFontImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Font. */
	inline feFont *GetFont() const{ return pFont; }
	
	/** \brief Set font. */
	void SetFont(feFont *font);
	
	/** \brief Set if rendering is enabled. */
	void SetEnableRendering(bool enable);
	
	
	
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
	void OnFrameUpdate(float elapsed);
	
	/** \brief Update scrollbar ranges. */
	void UpdateScrollbarRanges();
	
	/** \brief Update image view canvas. */
	void UpdateImageViewCanvas();
	
	/** \brief Scroll view. */
	void ScrollView();
	/*@}*/
};

#endif
