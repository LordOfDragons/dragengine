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

#ifndef _SEVIEWCONSTRUCTED_H_
#define _SEVIEWCONSTRUCTED_H_

#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/igdeSpinTextField.h>
#include <deigde/gui/layout/igdeContainerBorder.h>

#include <dragengine/common/math/decMath.h>

class seSkin;
class seWindowMain;
class seViewConstructedView;
class seViewConstructedListener;



/**
 * \brief Constructed property preview container.
 */
class seViewConstructed : public igdeContainerBorder{
private:
	seWindowMain &pWindowMain;
	seViewConstructedListener *pListener;
	
	decPoint pContentSize;
	decPoint pScrollOffset;
	
	igdeScrollBar::Ref pSBHorizontal;
	igdeScrollBar::Ref pSBVertical;
	igdeComboBox::Ref pCBZoom;
	igdeSpinTextField::Ref pSpinLayer;
	seViewConstructedView *pViewNode;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create constructed property view. */
	seViewConstructed(seWindowMain &windowMain);
	
protected:
	/** \brief Clean up constructed property view. */
	virtual ~seViewConstructed();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Skin. */
	seSkin *GetSkin() const;
	
	/** \brief Set skin. */
	void SetSkin(seSkin *skin);
	
	/** \brief Constructed view. */
	inline seViewConstructedView &GetViewNode() const{ return *pViewNode; }
	
	
	
	/** \brief Set if rendering is enabled. */
	void SetEnableRendering(bool enable);
	
	
	
	/** \brief Game engine has been started. */
	void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	void OnBeforeEngineStop();
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	/** \brief View scrolled. */
	void OnScrolled();
	
	
	
	/** \brief Update scrollbar ranges. */
	void UpdateScrollbarRanges();
	
	/** \brief Update layer ranges. */
	void UpdateLayerRanges();
	
	/** \brief Update active layer. */
	void UpdateActiveLayer();
	/*@}*/
};

#endif
