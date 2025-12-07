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

#ifndef _DEDSRENDERABLEVISITORIDENTIFY_H_
#define _DEDSRENDERABLEVISITORIDENTIFY_H_

#include "deDSRenderableVisitor.h"


/**
 * \brief Identify dynamic skin renderable visitor.
 *
 * Renderable visitor providing identification. After visiting a renderable the
 * visitor can be queried for the kind of the visited renderable. Useful to
 * identify renderables and to carry out actions only for certain renderable types.
 * Furthermore save casting is provided. If the cast is not valid an exception
 * is raised.
 */
class DE_DLL_EXPORT deDSRenderableVisitorIdentify : public deDSRenderableVisitor{
public:
	/** \brief Renderable types. */
	enum eRenderableTypes{
		/** \brief Value. */
		eptValue,
		
		/** \brief Color. */
		eptColor,
		
		/** \brief Image. */
		eptImage,
		
		/** \brief Canvas. */
		eptCanvas,
		
		/** \brief Video frame. */
		eptVideoFrame,
		
		/** \brief Camera. */
		eptCamera,
		
		/** \brief Unknown. */
		eptUnknown = -1
	};
	
	
	
private:
	deDSRenderable *pRenderable;
	eRenderableTypes pType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deDSRenderableVisitorIdentify();
	
	/** \brief Clean up visitor. */
	virtual ~deDSRenderableVisitorIdentify();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Renderable type. */
	inline eRenderableTypes GetType() const{ return pType; }
	
	
	
	/** \brief Rendereable is an unknown renderable. */
	inline bool IsUnknown() const{ return pType == eptUnknown; }
	
	/** \brief Rendereable is a value renderable. */
	inline bool IsValue() const{ return pType == eptValue; }
	
	/** \brief Rendereable is a color renderable. */
	inline bool IsColor() const{ return pType == eptColor; }
	
	/** \brief Rendereable is an image renderable. */
	inline bool IsImage() const{ return pType == eptImage; }
	
	/** \brief Rendereable is a canvas renderable. */
	inline bool IsCanvas() const{ return pType == eptCanvas; }
	
	/** \brief Rendereable is a video frame renderable. */
	inline bool IsVideoFrame() const{ return pType == eptVideoFrame; }
	
	/** \brief Rendereable is a camera renderable. */
	inline bool IsCamera() const{ return pType == eptCamera; }
	
	
	
	/**
	 * \brief Cast to a value renderable.
	 * \throws deeInvalidParam Canvas is not of type eRenderableTypes::eptValue.
	 */
	deDSRenderableValue &CastToValue() const;
	
	/**
	 * \brief Cast to a color renderable.
	 * \throws deeInvalidParam Canvas is not of type eRenderableTypes::eptColor.
	 */
	deDSRenderableColor &CastToColor() const;
	
	/**
	 * \brief Cast to an image renderable.
	 * \throws deeInvalidParam Canvas is not of type eRenderableTypes::eptImage.
	 */
	deDSRenderableImage &CastToImage() const;
	
	/**
	 * \brief Cast to a canvas renderable.
	 * \throws deeInvalidParam Canvas is not of type eRenderableTypes::eptCanvas.
	 */
	deDSRenderableCanvas &CastToCanvas() const;
	
	/**
	 * \brief Cast to a video frame renderable.
	 * \throws deeInvalidParam Canvas is not of type eRenderableTypes::eptVideoFrame.
	 */
	deDSRenderableVideoFrame &CastToVideoFrame() const;
	
	/**
	 * \brief Cast to a camera renderable.
	 * \throws deeInvalidParam Canvas is not of type eRenderableTypes::eptCamera.
	 */
	deDSRenderableCamera &CastToCamera() const;
	
	
	
	/** \brief Reset visitor. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit renderable. */
	virtual void VisitRenderable(deDSRenderable &renderable);
	
	/** \brief Visit value renderable. */
	virtual void VisitValue(deDSRenderableValue &renderable);
	
	/** \brief Visit color renderable. */
	virtual void VisitColor(deDSRenderableColor &renderable);
	
	/** \brief Visit image renderable. */
	virtual void VisitImage(deDSRenderableImage &renderable);
	
	/** \brief Visit canvas renderable. */
	virtual void VisitCanvas(deDSRenderableCanvas &renderable);
	
	/** \brief Visit video frame renderable. */
	virtual void VisitVideoFrame(deDSRenderableVideoFrame &renderable);
	
	/** \brief Visit camera renderable. */
	virtual void VisitCamera(deDSRenderableCamera &renderable);
	/*@}*/
};

#endif
