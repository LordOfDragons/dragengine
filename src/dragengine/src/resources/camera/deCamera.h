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

#ifndef _DECAMERA_H_
#define _DECAMERA_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"

class deWorld;
class deEffect;
class deEffectChain;
class deCameraManager;
class deBaseGraphicCamera;


/**
 * \brief World camera.
 *
 * A world camera defines the camera parameters used for rendering
 * a world. Cameras have various parameters useful to alter the way
 * the world is rendered.
 *
 * The exposure is used to increase or decrease the overall luminance
 * of the rendered world. Typically this is used to simulate accomodation
 * of eyes to different levels of light luminance.
 *
 * In photography the exposure is chosen to not cause images to be overbright.
 * In games though overbrighting is often used as a gameplay element to
 * simulate per-pixel overbrighting while venturing into a room with glaring
 * light while the eyes are still accomodated to night light conditions.
 * Overbrighting can be done in two ways. For overbrighting affecting the
 * entire screen like flashbangs for example using a post processing color
 * matrix effect is the best way. For overbrighting on a per pixel basis the
 * camera object exposes two parameters. The overbright lower parameter
 * indicates the luminance of a pixel after applying the exposure where the
 * overbrighting effect takes effect. The overbright upper parameter indicates
 * the luminance where the overbright effect reaches full scale. Typically the
 * pixel in question is pushed towards white inbetween the limits. The exact
 * behavior is left to the graphcis module. If you specific control using
 * post processing effects might be better.
 * 
 * \todo Add option to define render mode:
 * - ermPerspective: Regular perspective rendering (default)
 * - ermOrthogonal: Orthogonal rendering. fov and fov ratio are used to define the render
 * size. or add new parameters
 * - ermOmniDirection: Render 360 view. output is a cube-map. for dynamic skins no addiotnal
 * work has to be done. for canvas render world the canvas has to have
 * a depth of 6. if the depth 1 one the output is converted into a
 * single image mercartor map. this is though not the default use case.
 * => another option is outputing an equirectangular image. this works well with various
 *    texture properties and especially works well with canvas systems since it is an
 *    image with depth 1 as most other images are
 */
class deCamera : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCamera> Ref;
	
	
	
private:
	decDVector pPosition;
	decQuaternion pOrientation;
	
	float pFov;
	float pFovRatio;
	float pImageDistance;
	float pViewDistance;
	
	bool pEnableHDRR;
	float pExposure;
	float pLowestIntensity;
	float pHighestIntensity;
	float pAdaptionTime;
	
	bool pEnableGI;
	
	deEffectChain *pEffects;
	
	decLayerMask pLayerMask;
	
	deBaseGraphicCamera *pPeerGraphic;
	
	deWorld *pParentWorld;
	deCamera *pLLWorldPrev;
	deCamera *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new camera object with the given resource manager. */
	deCamera( deCameraManager *manager );
	
protected:
	/**
	 * \brief Clean up camera.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCamera();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Vertical field  of view in radians for the entire view. */
	inline float GetFov() const{ return pFov; }
	
	/** \brief Set vertical field of view in radians of the entire view. */
	void SetFov( float fov );
	
	/** \brief Aspect ratio of the horizontal field of view to the vertical field of view. */
	inline float GetFovRatio() const{ return pFovRatio; }
	
	/** \brief Set aspect ratio of the horizonral field of view to the vertical field of view. */
	void SetFovRatio( float ratio );
	
	/** \brief Distance to the image plane. */
	inline float GetImageDistance() const{ return pImageDistance; }
	
	/** \brief Set distance to the image plane. */
	void SetImageDistance( float distance );
	
	/** \brief Viewing distance up to which world geometry is rendered. */
	inline float GetViewDistance() const{ return pViewDistance; }
	
	/** \brief Set view distance up to which world geometry is rendered. */
	void SetViewDistance( float distance );
	
	
	
	/** \brief Enable high definition range rendering (HDRR) if supported. */
	inline bool GetEnableHDRR() const{ return pEnableHDRR; }
	
	/** \brief Set to enable high definition range rendering (HDRR) if supported. */
	void SetEnableHDRR( bool enable );
	
	/** \brief Exposure. */
	inline float GetExposure() const{ return pExposure; }
	
	/** \brief Set exposure. */
	void SetExposure( float exposure );
	
	/** \brief Lowest intensity the eye can adapt to. */
	inline float GetLowestIntensity() const{ return pLowestIntensity; }
	
	/** \brief Set lowest intensity the eye can adapt to. */
	void SetLowestIntensity( float lowestIntensity );
	
	/** \brief Highest intensity the eye can adapt to. */
	inline float GetHighestIntensity() const{ return pHighestIntensity; }
	
	/** \brief Set highest intensity the eye can adapt to. */
	void SetHighestIntensity( float highestIntensity );
	
	/** \brief Adaption time of the eye in seconds. */
	inline float GetAdaptionTime() const{ return pAdaptionTime; }
	
	/** \brief Set adaption time of the eye in seconds. */
	void SetAdaptionTime( float adaptionTime );
	
	
	
	/** \brief Enable global illumination (GI) if supported. */
	inline bool GetEnableGI() const{ return pEnableGI; }
	
	/** \brief Set to enable global illumination (GI) if supported. */
	void SetEnableGI( bool enable );
	
	
	
	/** \brief Layer mask. Call NotifyLayerMaskChanged afterwards. */
	decLayerMask &GetLayerMask(){ return pLayerMask; }
	const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Notifies the peers that the layer mask changed. */
	void NotifyLayerMaskChanged();
	
	/** \brief Request graphic module to reset adapted intensity to optimal value. */
	void ResetAdaptedIntensity();
	/*@}*/
	
	
	
	/** \name Effects */
	/*@{*/
	/** \brief Count of effects. */
	int GetEffectCount() const;
	
	/** \brief Effect at the given index. */
	deEffect *GetEffectAt( int index ) const;
	
	/** \brief Adds an effect. */
	void AddEffect( deEffect *effect );
	
	/** \brief Removes an effect. */
	void RemoveEffect( deEffect *effect );
	
	/** \brief Removes all effects. */
	void RemoveAllEffects();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system camera peer. */
	inline deBaseGraphicCamera *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system camera peer. */
	void SetPeerGraphic( deBaseGraphicCamera *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous camera in the parent world linked list. */
	inline deCamera *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next camera in the parent world linked list. */
	void SetLLWorldPrev( deCamera *camera );
	
	/** \brief Next camera in the parent world linked list. */
	inline deCamera *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next camera in the parent world linked list. */
	void SetLLWorldNext( deCamera *camera );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
