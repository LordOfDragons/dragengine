/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLRCAMERA_H_
#define _DEOGLRCAMERA_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/deObject.h>

class deoglREffect;
class deoglRenderPlan;
class deoglRenderThread;
class deoglRWorld;
class deoglTexture;
class deoglVR;


/**
 * Render camera.
 */
class deoglRCamera : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	
	decDVector pPosition;
	decDMatrix pInverseCameraMatrix;
	decDMatrix pCameraMatrix;
	
	deoglTexture *pTextureToneMapParams;
	float pElapsedToneMapAdaption;
	bool pForceToneMapAdaption;
	
	bool pEnableHDRR;
	float pExposure;
	float pLowestIntensity;
	float pHighestIntensity;
	float pAdaptionTime;
	
	bool pEnableGI;
	
	deoglRenderPlan *pPlan;
	
	decObjectList pEffects;
	
	bool pInitTexture;
	
	float pLastAverageLuminance;
	bool pDirtyLastAverageLuminance;
	
	deoglVR *pVR;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render camera. */
	deoglRCamera( deoglRenderThread &renderThread );
	
	/** Clean up render camera. */
	virtual ~deoglRCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL if not set. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition( const decDVector &position );
	
	/** Camera matrix. */
	inline const decDMatrix &GetCameraMatrix() const{ return pCameraMatrix; }
	
	/** Inverse camera matrix. */
	inline const decDMatrix &GetInverseCameraMatrix() const{ return pInverseCameraMatrix; }
	
	/** Set camera matrix and calculate calvulate inverse camera matrix. */
	void SetCameraMatrices( const decDMatrix &matrix );
	
	
	
	/** Render plan. */
	inline deoglRenderPlan &GetPlan() const{ return *pPlan; }
	
	
	
	/** Tone mapping parameters texture. */
	inline deoglTexture *GetToneMapParamsTexture() const{ return pTextureToneMapParams; }
	
	/** Set tone mapping parameters texture. */
	void SetToneMapParamsTexture( deoglTexture *texture );
	
	/** Elapsed time since the last adaption of the tone mapping parameters. */
	inline float GetElapsedToneMapAdaption() const{ return pElapsedToneMapAdaption; }
	
	/** Set elapsed time since the last adaption of the tone mapping parameters. */
	void SetElapsedToneMapAdaption( float elapsed );
	
	/** Force full tone mapping adaption for the next rendering. */
	inline bool GetForceToneMapAdaption() const{ return pForceToneMapAdaption; }
	
	/** Set if a full tone mapping adaption is forced for the next rendering. */
	void SetForceToneMapAdaption( bool forceAdaption );
	
	/** Reset elapsed tone mapping adaption time. */
	void ResetElapsedToneMapAdaption();
	
	
	
	/** Enable HDRR. */
	inline bool GetEnableHDRR() const{ return pEnableHDRR; }
	
	/** Set enable HDRR. */
	void SetEnableHDRR( bool enable );
	
	/** Exposure. */
	inline float GetExposure() const{ return pExposure; }
	
	/** Set exposure. */
	void SetExposure( float exposure );
	
	/** Lowest intensity the eye can adapt to. */
	inline float GetLowestIntensity() const{ return pLowestIntensity; }
	
	/** Set lowest intensity the eye can adapt to. */
	void SetLowestIntensity( float lowestIntensity );
	
	/** Highest intensity the eye can adapt to. */
	inline float GetHighestIntensity() const{ return pHighestIntensity; }
	
	/** Set highest intensity the eye can adapt to. */
	void SetHighestIntensity( float highestIntensity );
	
	/** Adaption time of the eye in seconds. */
	inline float GetAdaptionTime() const{ return pAdaptionTime; }
	
	/** Set adaption time of the eye in seconds. */
	void SetAdaptionTime( float adaptionTime );
	
	
	
	/** Enable GI. */
	inline bool GetEnableGI() const{ return pEnableGI; }
	
	/** Set enable GI. */
	void SetEnableGI( bool enable );
	
	
	
	/** VR or nullptr. */
	inline deoglVR *GetVR() const{ return pVR; }
	
	/** Enable/Disable VR. */
	void EnableVR( bool enable );
	
	
	
	/**
	 * Last average scene luminance.
	 * 
	 * If dirty reads back the adaption parameters from the GPU and stores the last used
	 * average scene luminance. This has a slight performance impact so this method should
	 * be called only at the beginning of rendering where the GPU data has been written
	 * back in time the longest and thus stalling is not to be expected.
	 * 
	 * Time measuring shows ~0.1-0.2ms for this call to return.
	 * 
	 * If no adaption parameters are present the minimum camera intensity times the
	 * scene key constant is used.
	 * 
	 * Once the average luminance is stored the dirty flag is cleared. Thus retrieving the
	 * data from the GPU happens only the first time after the camera updated the adaption
	 * parameters on the GPU.
	 */
	float GetLastAverageLuminance();
	
	
	
	/** Number of effects. */
	int GetEffectCount() const;
	
	/** Effect at index. */
	deoglREffect &GetEffectAt( int index ) const;
	
	/** Add effect. */
	void AddEffect( deoglREffect *effect );
	
	/** Remove all effects. */
	void RemoveAllEffects();
	
	
	
	/** Update. */
	void Update( float elapsed );
	
	/** Prepare for rendering. */
	void PrepareForRender();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
