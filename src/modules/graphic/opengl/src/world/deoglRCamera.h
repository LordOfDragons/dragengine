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



/**
 * \brief Render camera.
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
	
	float pExposure;
	float pLowestIntensity;
	float pHighestIntensity;
	float pAdaptionTime;
	
	deoglRenderPlan *pPlan;
	
	decObjectList pEffects;
	
	bool pInitTexture;
	
	float pLastAverageLuminance;
	bool pDirtyLastAverageLuminance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create render camera. */
	deoglRCamera( deoglRenderThread &renderThread );
	
	/** \brief Clean up render camera. */
	virtual ~deoglRCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Parent world or \em NULL if not set. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL if not set. */
	void SetParentWorld( deoglRWorld *parentWorld );
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Camera matrix. */
	inline const decDMatrix &GetCameraMatrix() const{ return pCameraMatrix; }
	
	/** \brief Inverse camera matrix. */
	inline const decDMatrix &GetInverseCameraMatrix() const{ return pInverseCameraMatrix; }
	
	/** \brief Set camera matrix and calculate calvulate inverse camera matrix. */
	void SetCameraMatrices( const decDMatrix &matrix );
	
	
	
	/** \brief Render plan. */
	inline deoglRenderPlan &GetPlan() const{ return *pPlan; }
	
	
	
	/** \brief Tone mapping parameters texture. */
	inline deoglTexture *GetToneMapParamsTexture() const{ return pTextureToneMapParams; }
	
	/** \brief Set tone mapping parameters texture. */
	void SetToneMapParamsTexture( deoglTexture *texture );
	
	/** \brief Elapsed time since the last adaption of the tone mapping parameters. */
	inline float GetElapsedToneMapAdaption() const{ return pElapsedToneMapAdaption; }
	
	/** \brief Set elapsed time since the last adaption of the tone mapping parameters. */
	void SetElapsedToneMapAdaption( float elapsed );
	
	/** \brief Force full tone mapping adaption for the next rendering. */
	inline bool GetForceToneMapAdaption() const{ return pForceToneMapAdaption; }
	
	/** \brief Set if a full tone mapping adaption is forced for the next rendering. */
	void SetForceToneMapAdaption( bool forceAdaption );
	
	/** \brief Reset elapsed tone mapping adaption time. */
	void ResetElapsedToneMapAdaption();
	
	
	
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
	
	
	
	/**
	 * \brief Last average scene luminance.
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
	
	
	
	/** \brief Number of effects. */
	int GetEffectCount() const;
	
	/** \brief Effect at index. */
	deoglREffect &GetEffectAt( int index ) const;
	
	/** \brief Add effect. */
	void AddEffect( deoglREffect *effect );
	
	/** \brief Remove all effects. */
	void RemoveAllEffects();
	
	
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Prepare for rendering. */
	void PrepareForRender();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
