/* 
 * Drag[en]gine Game Engine
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
	virtual ~deCanvasVisitorIdentify();
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
	virtual void VisitCanvas( deCanvas &canvas );
	
	/** \brief Visit image canvas. */
	virtual void VisitImage( deCanvasImage &canvas );
	
	/** \brief Visit paint canvas. */
	virtual void VisitPaint( deCanvasPaint &canvas );
	
	/** \brief Visit canvas view canvas. */
	virtual void VisitCanvasView( deCanvasCanvasView &canvas );
	
	/** \brief Visit render world canvas. */
	virtual void VisitRenderWorld( deCanvasRenderWorld &canvas );
	
	/** \brief Visit text canvas. */
	virtual void VisitText( deCanvasText &canvas );
	
	/** \brief Visit video player canvas. */
	virtual void VisitVideoPlayer( deCanvasVideoPlayer &canvas );
	
	/** \brief Visit view canvas. */
	virtual void VisitView( deCanvasView &canvas );
	/*@}*/
};

#endif
