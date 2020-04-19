/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEDYNAMICSKINRENDERABLE_H_
#define _SEDYNAMICSKINRENDERABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/video/deVideoReference.h>
#include <dragengine/resources/video/deVideoPlayerReference.h>

class seDynamicSkin;

class deEngine;



/**
 * \brief Dynamic Skin Renderable.
 */
class seDynamicSkinRenderable : public deObject{
public:
	/** Renderable types. */
	enum eRenderableTypes{
		/** Value. */
		ertValue,
		/** Color. */
		ertColor,
		/** Image. */
		ertImage,
		/** Canvas. */
		ertCanvas,
		/** Video frame. */
		ertVideoFrame,
		/** Camera. */
		ertCamera,
		/** Dummy entry with the number renderable types. */
		ERT_COUNT
	};
	
private:
	deEngine *pEngine;
	
	seDynamicSkin *pDynamicSkin;
	
	decString pName;
	int pRenderableType;
	
	bool pActive;
	bool pDirty;
	
	float pValue;
	float pSliderValueLower;
	float pSliderValueUpper;
	
	decColor pColor;
	
	decString pPathImage;
	deImageReference pEngImage;
	
	// canvas stuff
	
	decString pPathVideo;
	deVideoReference pEngVideo;
	deVideoPlayerReference pEngVideoPlayer;
	
	// camera stuff
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable. */
	seDynamicSkinRenderable( deEngine *engine, const char *name = "Renderable" );
	/** Cleans up the renderable. */
	virtual ~seDynamicSkinRenderable();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** Retrieves the parent dynamic skin or NULL if there is none. */
	inline seDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	/** Sets the parent dynamic skin or NULL if there is none. */
	void SetDynamicSkin( seDynamicSkin *dynamicSkin );
	
	/** Retrieves the name. */
	inline const decString &GetName() const{ return pName; }
	/** Sets the name. */
	void SetName( const char *name );
	/** Retrieves the renderable type. */
	inline int GetRenderableType() const{ return pRenderableType; }
	/** Sets the renderable type. */
	void SetRenderableType( int type );
	
	/** Determines if the renderable is the active one. */
	inline bool GetActive() const{ return pActive; }
	/** Sets if the renderable is the active one. */
	void SetActive( bool active );
	
	/** Notifies the listeners that the renderable changed. */
	void NotifyChanged();
	/** Notifies the engine dynamic skin that the renderable changed. */
	void NotifyEngineDynamicSkinChanged();
	
	/** Update renderable. */
	void Update( float elapsed );
	
	/** Create value renderable. */
	void CreateRenderableValue();
	/** Create color renderable. */
	void CreateRenderableColor();
	/** Create image renderable. */
	void CreateRenderableImage();
	/** Create canvas renderable. */
	void CreateRenderableCanvas();
	/** Create video frame renderable. */
	void CreateRenderableVideoFrame();
	/** Create camera renderable. */
	void CreateRenderableCamera();
	/*@}*/
	
	/** \name Value Renderable */
	/*@{*/
	/** Retrieves the value. */
	inline float GetValue() const{ return pValue; }
	/** Sets the value. */
	void SetValue( float value );
	
	/** \brief Slider lower range. */
	inline float GetSliderValueLower() const{ return pSliderValueLower; }
	
	/** \brief Set slider range. */
	void SetSliderLowerRange( float range );
	
	/** \brief Slider upper range. */
	inline float GetSliderValueUpper() const{ return pSliderValueUpper; }
	
	/** \brief Set slider range. */
	void SetSliderUpperRange( float range );
	/*@}*/
	
	/** \name Color Renderable */
	/*@{*/
	/** Retrieves the static color. */
	inline const decColor &GetColor() const{ return pColor; }
	/** Sets the static color. */
	void SetColor( const decColor &color );
	/*@}*/
	
	/** \name Image Renderable */
	/*@{*/
	/** Retrieves the image path. */
	inline const decString &GetImagePath() const{ return pPathImage; }
	/** Sets the image path. */
	void SetImagePath( const char *imagePath );
	/** Retrieves the engine image or NULL if not set. */
	inline deImage *GetEngineImage() const{ return pEngImage; }
	/** Update image. */
	void UpdateImage();
	
	/** \name Canvas Renderable */
	/*@{*/
	/*@}*/
	
	/** \name Video Renderable */
	/*@{*/
	/** Retrieves the video path. */
	inline const decString &GetVideoPath() const{ return pPathVideo; }
	/** Sets the video path. */
	void SetVideoPath( const char *videoPath );
	/** Retrieves the engine video or NULL if not set. */
	inline deVideo *GetEngineVideo() const{ return pEngVideo; }
	/** Retrieves the engine video player or NULL if not set. */
	inline deVideoPlayer *GetEngineVideoPlayer() const{ return pEngVideoPlayer; }
	/** Update video. */
	void UpdateVideo();
	
	/** \name Camera Renderable */
	/*@{*/
	/*@}*/
};

#endif
