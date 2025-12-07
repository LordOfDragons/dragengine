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

#ifndef _DECANVASVISITORIDENTIFY_H_
#define _DECANVASVISITORIDENTIFY_H_

#include "deCanvasVisitor.h"

class deCanvasView;


/**
 * \brief Canvas identify visitor.
 */
class DE_DLL_EXPORT deCanvasVisitorIdentify : public deCanvasVisitor{
public:
	/** \brief Canvas types. */
	enum eCanvasTypes{
		/** \brief Unknown/invalid type. */
		ectUnknown,
		
		/** \brief Image canvas. */
		ectImage,
		
		/** \brief Paint canvas. */
		ectPaint,
		
		/** \brief Canvas view canvas. */
		ectCanvasView,
		
		/** \brief Render world canvas. */
		ectRenderWorld,
		
		/** \brief Text canvas. */
		ectText,
		
		/** \brief Video player canvas. */
		ectVideoPlayer,
		
		/** \briev View canvas. */
		ectView
	};
	
	
	
private:
	deCanvas *pCanvas;
	eCanvasTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas identify visitor. */
	deCanvasVisitorIdentify();
	
	/** \brief Clean up canvas identify visitor. */
	~deCanvasVisitorIdentify() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas type. */
	inline eCanvasTypes GetType() const{ return pType; }
	
	/** \brief Canvas type is unknown. */
	inline bool IsUnknown() const{ return pType == ectUnknown; }
	
	/** \brief Canvas is an image canvas. */
	inline bool IsImage() const{ return pType == ectImage; }
	
	/** \brief Canvas is a paint canvas. */
	inline bool IsPaint() const{ return pType == ectPaint; }
	
	/** \brief Canvas is a canvas view canvas. */
	inline bool IsCanvasView() const{ return pType == ectCanvasView; }
	
	/** \brief Canvas is a render world canvas. */
	inline bool IsRenderWorld() const{ return pType == ectRenderWorld; }
	
	/** \brief Canvas is a text canvas. */
	inline bool IsText() const{ return pType == ectText; }
	
	/** \brief Canvas is a video player canvas. */
	inline bool IsVideoPlayer() const{ return pType == ectVideoPlayer; }
	
	/** \brief Canvas is a view canvas. */
	inline bool IsView() const{ return pType == ectView; }
	
	/**
	 * \brief Cast to an image canvas.
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectImage.
	 */
	deCanvasImage &CastToImage() const;
	
	/**
	 * \brief Cast to a paint canvas.
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectPaint.
	 */
	deCanvasPaint &CastToPaint() const;
	
	/**
	 * \brief Cast to a canvas view canvas.
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectCanvasView.
	 */
	deCanvasCanvasView &CastToCanvasView() const;
	
	/**
	 * \brief Cast to a render world canvas.
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectRenderWorld.
	 */
	deCanvasRenderWorld &CastToRenderWorld() const;
	
	/**
	 * \brief Cast to a text canvas. 
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectText.
	 */
	deCanvasText &CastToText() const;
	
	/**
	 * \brief Cast to a video player canvas.
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectVideoPlayer.
	 */
	deCanvasVideoPlayer &CastToVideoPlayer() const;
	
	/**
	 * \brief Cast to a view canvas.
	 * \throws deeInvalidParam Canvas is not of type eCanvasTypes::ectView.
	 */
	deCanvasView &CastToView() const;
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	void VisitCanvas(deCanvas &canvas) override;
	
	/** \brief Visit image canvas. */
	void VisitImage(deCanvasImage &canvas) override;
	
	/** \brief Visit paint canvas. */
	void VisitPaint(deCanvasPaint &canvas) override;
	
	/** \brief Visit canvas view canvas. */
	void VisitCanvasView(deCanvasCanvasView &canvas) override;
	
	/** \brief Visit render world canvas. */
	void VisitRenderWorld(deCanvasRenderWorld &canvas) override;
	
	/** \brief Visit text canvas. */
	void VisitText(deCanvasText &canvas) override;
	
	/** \brief Visit video player canvas. */
	void VisitVideoPlayer(deCanvasVideoPlayer &canvas) override;
	
	/** \brief Visit view canvas. */
	void VisitView(deCanvasView &canvas) override;
	/*@}*/
};

#endif
