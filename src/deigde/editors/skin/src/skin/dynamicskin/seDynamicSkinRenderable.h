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

#ifndef _SEDYNAMICSKINRENDERABLE_H_
#define _SEDYNAMICSKINRENDERABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/video/deVideo.h>
#include <dragengine/resources/video/deVideoPlayer.h>

class seDynamicSkin;

class deEngine;



/**
 * \brief Dynamic Skin Renderable.
 */
class seDynamicSkinRenderable : public deObject{
	public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<seDynamicSkinRenderable> Ref;
	
	
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
	eRenderableTypes pRenderableType;
	
	bool pActive;
	bool pDirty;
	
	float pValue;
	float pSliderValueLower;
	float pSliderValueUpper;
	
	decColor pColor;
	
	decString pPathImage;
	deImage::Ref pEngImage;
	
	// canvas stuff
	
	decString pPathVideo;
	deVideo::Ref pEngVideo;
	deVideoPlayer::Ref pEngVideoPlayer;
	
	// camera stuff
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new renderable. */
	seDynamicSkinRenderable(deEngine *engine, const char *name = "Renderable");
	/** Cleans up the renderable. */
	virtual ~seDynamicSkinRenderable();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the engine. */
	inline deEngine *GetEngine() const{return pEngine;}
	
	/** Retrieves the parent dynamic skin or NULL if there is none. */
	inline seDynamicSkin *GetDynamicSkin() const{return pDynamicSkin;}
	/** Sets the parent dynamic skin or NULL if there is none. */
	void SetDynamicSkin(seDynamicSkin *dynamicSkin);
	
	/** Retrieves the name. */
	inline const decString &GetName() const{return pName;}
	/** Sets the name. */
	void SetName(const char *name);
	/** Retrieves the renderable type. */
	inline eRenderableTypes GetRenderableType() const{return pRenderableType;}
	/** Sets the renderable type. */
	void SetRenderableType(eRenderableTypes type);
	
	/** Determines if the renderable is the active one. */
	inline bool GetActive() const{return pActive;}
	/** Sets if the renderable is the active one. */
	void SetActive(bool active);
	
	/** Notifies the listeners that the renderable changed. */
	void NotifyChanged();
	/** Notifies the engine dynamic skin that the renderable changed. */
	void NotifyEngineDynamicSkinChanged();
	
	/** Update renderable. */
	void Update(float elapsed);
	
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
	inline float GetValue() const{return pValue;}
	/** Sets the value. */
	void SetValue(float value);
	
	/** \brief Slider lower range. */
	inline float GetSliderValueLower() const{return pSliderValueLower;}
	
	/** \brief Set slider range. */
	void SetSliderLowerRange(float range);
	
	/** \brief Slider upper range. */
	inline float GetSliderValueUpper() const{return pSliderValueUpper;}
	
	/** \brief Set slider range. */
	void SetSliderUpperRange(float range);
	/*@}*/
	
	/** \name Color Renderable */
	/*@{*/
	/** Retrieves the static color. */
	inline const decColor &GetColor() const{return pColor;}
	/** Sets the static color. */
	void SetColor(const decColor &color);
	/*@}*/
	
	/** \name Image Renderable */
	/*@{*/
	/** Retrieves the image path. */
	inline const decString &GetImagePath() const{return pPathImage;}
	/** Sets the image path. */
	void SetImagePath(const char *imagePath);
	/** Retrieves the engine image or NULL if not set. */
	inline deImage *GetEngineImage() const{return pEngImage;}
	/** Update image. */
	void UpdateImage();
	
	/** \name Canvas Renderable */
	/*@{*/
	/*@}*/
	
	/** \name Video Renderable */
	/*@{*/
	/** Retrieves the video path. */
	inline const decString &GetVideoPath() const{return pPathVideo;}
	/** Sets the video path. */
	void SetVideoPath(const char *videoPath);
	/** Retrieves the engine video or NULL if not set. */
	inline deVideo *GetEngineVideo() const{return pEngVideo;}
	/** Retrieves the engine video player or NULL if not set. */
	inline deVideoPlayer *GetEngineVideoPlayer() const{return pEngVideoPlayer;}
	/** Update video. */
	void UpdateVideo();
	
	/** \name Camera Renderable */
	/*@{*/
	/*@}*/
};

#endif
