/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEBASEVRMODULE_H_
#define _DEBASEVRMODULE_H_

#include "../deBaseModule.h"
#include "../../../common/math/decMath.h"

class deInputDevice;
class deInputDevicePose;
class deCamera;
class deModel;
class deImage;


/**
 * \brief Base VR Module.
 * \version 1.6
 */
class DE_DLL_EXPORT deBaseVRModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseVRModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseVRModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Init the module.
	 * \returns true on success.
	 * \note To access the os object of the engine use the GetOS function.
	 */
	virtual bool Init() = 0;
	
	/** \brief Clean up module. */
	virtual void CleanUp() = 0;
	/*@}*/
	
	
	
	/** \name Runtime */
	/*@{*/
	/**
	 * \brief VR Runtime is usable.
	 * 
	 * Returns true if a call to StartRuntime() is likely to succeed or not.
	 */
	virtual bool RuntimeUsable() = 0;
	
	/**
	 * \brief Start VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various modules. VR mode can be started and
	 * stopped at any time.
	 */
	virtual void StartRuntime() = 0;
	
	/**
	 * \brief Stop VR.
	 * 
	 * VR systems typically are required to be activated before they can be used.
	 * Starting VR connects the engine to the VR system present on the host system
	 * and activates VR support on various modules. VR mode can be started and
	 * stopped at any time.
	 */
	virtual void StopRuntime() = 0;
	
	/**
	 * \brief Set camera to render on head mounted display.
	 * 
	 * If set to nullptr fades back to safe scene as defined by VR Runtime.
	 */
	virtual void SetCamera( deCamera *camera ) = 0;
	/*@}*/
	
	
	
	/** \name Devices */
	/*@{*/
	/** \brief Number of input devices. */
	virtual int GetDeviceCount() = 0;
	
	/** \brief Information for input device at index. */
	virtual deInputDevice *GetDeviceAt( int index ) = 0;
	
	/** \brief Index of device with identifier or -1 if absent. */
	virtual int IndexOfDeviceWithID( const char *id ) = 0;
	
	/** \brief Index of button with identifier on device at index or -1 if absent. */
	virtual int IndexOfButtonWithID( int device, const char *id ) = 0;
	
	/** \brief Index of axis with identifier on device at index or -1 if absent. */
	virtual int IndexOfAxisWithID( int device, const char *id ) = 0;
	
	/** \brief Index of feedback with identifier on device at index or -1 if absent. */
	virtual int IndexOfFeedbackWithID( int device, const char *id ) = 0;
	
	/** \brief Index of component with identifier on device at index or -1 if absent. */
	virtual int IndexOfComponentWithID( int device, const char *id ) = 0;
	
	/** \brief Button at index on device at index is pressed down. */
	virtual bool GetButtonPressed( int device, int button ) = 0;
	
	/** \brief Button at index on device at index is touched. */
	virtual bool GetButtonTouched( int device, int button ) = 0;
	
	/** \brief Value of axis at index on device at index. */
	virtual float GetAxisValue( int device, int axis ) = 0;
	
	/** \brief Value of feedback at index on device at index. */
	virtual float GetFeedbackValue( int device, int feedback ) = 0;
	
	/** \brief Set value of feedback at index on device at index. */
	virtual void SetFeedbackValue( int device, int feedback, float value ) = 0;
	
	/** \brief Device pose or identity if not supported. */
	virtual void GetDevicePose( int device, deInputDevicePose &pose ) = 0;
	
	/** \brief Device bone pose or identity if not supported. */
	virtual void GetDeviceBonePose( int device, int bone,
		bool withController, deInputDevicePose &pose ) = 0;
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/**
	 * \brief Add events to the VR System event queue.
	 * 
	 * This function is called before any other frame related tasks are carried out.
	 * Record changes in devices states have to be recored into a game event
	 * reported as engine input events You are expected to check message and event
	 * queues to deliver system notification (like quitting the game) to the game
	 * engine.
	 */
	virtual void ProcessEvents() = 0;
	/*@}*/
	
	
	
	/** \name Graphic Module use only */
	/*@{*/
	/**
	 * \brief VR eye.
	 * \warning For Graphic Module use only.
	 */
	enum eEye{
		evreLeft,
		evreRight
	};
	
	
	
	/**
	 * \brief VR recommended render target size.
	 * \warning For Graphic Module use only.
	 */
	virtual decPoint GetRenderSize() = 0;
	
	/**
	 * \brief VR render projection matrix parameters.
	 * \warning For Graphic Module use only.
	 * 
	 * Values are to be used directly in the projection matrix. The values also represent
	 * the half tan angles from center.
	 */
	virtual void GetProjectionParameters( eEye eye, float &left,
		float &right, float &top, float &bottom ) = 0;
	
	/**
	 * \brief VR render matrix transforming from camera space to eye space.
	 * \warning For Graphic Module use only.
	 */
	virtual decMatrix GetMatrixViewEye( eEye eye ) = 0;
	
	/**
	 * \brief VR render hidden area model or nullptr if not supported.
	 * \warning For Graphic Module use only.
	 * 
	 * If model is returned it contains a flat mesh (z=0) with one single lod containing
	 * one single double sided texture. Texture coordinates (x,y) are in the range from
	 * (0,0) to (1,1). Dummy texture coordinates are present since models require them.
	 */
	virtual deModel *GetHiddenArea( eEye eye ) = 0;
	
	/**
	 * \brief VR render distortion image or nullptr if not supported.
	 * \warning For Graphic Module use only.
	 * 
	 * Distortion image can be of any size with depth of 2. Z=0 is the U texture coordinate
	 * for the respective color and Z=1 the V texture coordinate.
	 */
	virtual deImage *GetDistortionMap( eEye eye ) = 0;
	
	/**
	 * \brief Get eye view images to use for rendering.
	 * \version 1.10
	 * \warning For Graphic Module use only.
	 * 
	 * If \em count is 0 returns the count of eye view images.
	 * 
	 * If \em count is larger than 0 \em views is filled with the eye view image handles
	 * up to \em count entries. If \em count is larger than 0 but smaller than the
	 * required count an exception is thrown. Returns the count of entries written.
	 * 
	 * \param[in] eye Eye to get view images for.
	 * \param[in] count Size of \em views array. Use 0 to query the required size.
	 * \param[out] views Array of eye view images. Pointer has to be cast to graphic api
	 *                  specific object instance pointer. Can be nullptr if \em count is 0.
	 */
	virtual int GetEyeViewImages( eEye eye, int count, void *views );
	
	/**
	 * \brief Get eye view render texture coordinates.
	 * \version 1.10
	 * \warning For Graphic Module use only.
	 * \param[in] eye Eye to get render texture coordinates for.
	 * \param[out] tcFrom Bottom left texture coordinates (range 0 to 1).
	 * \param[out] tcTo Top right texture coordinates (range 0 to 1).
	 */
	virtual void GetEyeViewRenderTexCoords( eEye eye, decVector2 &tcFrom, decVector2 &tcTo );
	
	/**
	 * \brief Begin frame.
	 * \warning For Graphic Module use only.
	 * 
	 * Graphic module has to check after this function returns if render parameters changed.
	 */
	virtual void BeginFrame() = 0;
	
	/**
	 * \brief Acquire eye view image to render into.
	 * \version 1.10
	 * \warning For Graphic Module use only.
	 * 
	 * If the VR module requires the graphic module to provide the rendered image so the
	 * VR module can do the eye view update -1 is returned.
	 * 
	 * The VR module acquires the next image to use from the list of eye view images.
	 * This list is send by notification to the graphic module. The returned index matches
	 * this list. The area to render to is also defined while sending the list and applies
	 * to all eye view images.
	 * 
	 * If -1 is returned the VR module does not support the graphic module rendering into
	 * eye images. The graphic module has to submit the images instead.
	 * 
	 * Only one image can be acquired for each eye view at the same time. The list of
	 * available eye view images is send by notification to the graphic module earlier.
	 */
	virtual int AcquireEyeViewImage( eEye eye );
	
	/**
	 * \brief Release eye view image after render into.
	 * \warning For Graphic Module use only.
	 * \version 1.10
	 */
	virtual void ReleaseEyeViewImage( eEye eye );
	
	/**
	 * \brief Submit OpenGL rendered image to the HMD.
	 * \warning For Graphic Module use only.
	 * \param[in] eye Eye to submit image to.
	 * \param[in] texture Name of Texture2D.
	 * \param[in] tcFrom Bottom left texture coordinates (range 0 to 1).
	 * \param[in] tcTo Top right texture coordinates (range 0 to 1).
	 * \param[in] distortionApplied Distortion has been already applied.
	 */
	virtual void SubmitOpenGLTexture2D( eEye eye, void *texture, const decVector2 &tcFrom,
		const decVector2 &tcTo, bool distortionApplied ) = 0;
	
	/**
	 * \brief End frame.
	 * \warning For Graphic Module use only.
	 */
	virtual void EndFrame() = 0;
	/*@}*/
};

#endif
