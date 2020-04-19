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
class deDSRenderableVisitorIdentify : public deDSRenderableVisitor{
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
	virtual void VisitRenderable( deDSRenderable &renderable );
	
	/** \brief Visit value renderable. */
	virtual void VisitValue( deDSRenderableValue &renderable );
	
	/** \brief Visit color renderable. */
	virtual void VisitColor( deDSRenderableColor &renderable );
	
	/** \brief Visit image renderable. */
	virtual void VisitImage( deDSRenderableImage &renderable );
	
	/** \brief Visit canvas renderable. */
	virtual void VisitCanvas( deDSRenderableCanvas &renderable );
	
	/** \brief Visit video frame renderable. */
	virtual void VisitVideoFrame( deDSRenderableVideoFrame &renderable );
	
	/** \brief Visit camera renderable. */
	virtual void VisitCamera( deDSRenderableCamera &renderable );
	/*@}*/
};

#endif
