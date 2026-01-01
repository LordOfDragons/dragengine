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

#ifndef _CEWDSVAPREVIEW_H_
#define _CEWDSVAPREVIEW_H_

#include <deigde/gui/event/igdeMouseKeyListener.h>

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/sound/deSpeaker.h>

class ceWindowDopeSheet;
class deSound;
class deSpeaker;


/**
 * \brief Dope sheet voice audio preview.
 */
class ceWDSVAPreview : public deObject{
public:
	typedef deTObjectReference<ceWDSVAPreview> Ref;
	
private:
	ceWindowDopeSheet &pWindow;
	
	deCanvasView::Ref pCanvas;
	deCanvasImage::Ref pCanvasPreview;
	
	igdeMouseKeyListener::Ref pMouseKeyListener;
	
	float pCurTime;
	
	deSpeaker::Ref pSpeaker;
	deImage::Ref pImagePreview;
	bool pDirtyPreview;
	char *pPreviewSamples;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dope sheet lane. */
	ceWDSVAPreview(ceWindowDopeSheet &dopeSheet);
	
	/** \brief Clean up dope sheet lane. */
protected:
	~ceWDSVAPreview();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dope sheet. */
	inline ceWindowDopeSheet &GetWindow() const{ return pWindow; }
	
	/** \brief Canvas. */
	inline const deCanvasView::Ref &GetCanvas() const{ return pCanvas; }
	
	/** \brief Mouse listener. */
	inline const igdeMouseKeyListener::Ref &GetMouseKeyListener() const{ return pMouseKeyListener; }
	
	
	
	/** \brief Current time. */
	inline float GetCurTime() const{ return pCurTime; }
	
	/** \brief Set current time. */
	void SetCurTime(float time);
	
	/** \brief Set preview speaker. */
	void SetSpeaker(deSpeaker *speaker);
	
	
	
	/** \brief Play back voice audio. */
	void PlayVoiceAudio();
	
	
	
	/** \brief Dope sheet size changed. */
	void OnResize();
	
	/** \brief Action changed. */
	void OnActionChanged();
	
	/** \brief Invalidate preview. */
	void InvalidatePreview();
	
	
	
	/** \brief Create canvas. */
	void CreateCanvas();
	
	/** \brief Update canvas. */
	void UpdateCanvas();
	
	/** \brief Rebuild canvas. */
	void RebuildCanvas();
	/*@}*/
	
	
	
private:
	void UpdateVAPreviewImage();
};

#endif
