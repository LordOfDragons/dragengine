/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDECAMERA_H_
#define _IGDECAMERA_H_

#include <dragengine/resources/camera/deCameraReference.h>
#include <dragengine/resources/world/deWorldReference.h>
#include <dragengine/common/curve/decCurveBezier.h>
#include <dragengine/common/string/decString.h>
#include <dragengine/common/math/decMath.h>

class deCamera;
class deEngine;
class deWorld;



/**
 * \brief Camera for rendering a world.
 *
 * Manages a camera including the wrapped engine camera. Provides also
 * some simple unproject methods to determine a ray shot into the world
 * as seen by this camera. The camera is managed in a lazy way. Hence
 * the engine is not create or updated until it is acquired by the user.
 */
class DE_DLL_EXPORT igdeCamera{
private:
	deEngine *pEngine;
	deCameraReference pEngCamera;
	deWorldReference pEngWorld;
	
	decString pName;
	decDVector pPosition;
	decVector pOrientation;
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
	
	float pWhiteIntensity;
	float pBloomIntensity;
	float pBloomStrength;
	float pBloomBlend;
	float pBloomSize;
	
	decCurveBezier pToneMapCurve;
	
	float pDistance;
	
	decDMatrix pViewMatrix;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create camera. */
	igdeCamera( deEngine *engine );
	
	/** \brief Clean up camera object. */
	virtual ~igdeCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Game engine. */
	inline deEngine *GetEngine() const{ return pEngine; }
	
	/** \brief Eengine camera. */
	inline deCamera *GetEngineCamera() const{ return pEngCamera; }
	
	/** \brief World the camera is attached to. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	
	/** \brief Attach camera to world. */
	void SetEngineWorld( deWorld *world );
	
	/** \brief Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set name. */
	void SetName( const char *name );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation of the camera. */
	inline const decVector &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation of the camera. */
	void SetOrientation( const decVector &orientation );
	
	/** \brief Field of view in radians. */
	inline float GetFov() const{ return pFov; }
	
	/** \brief Set field of view in radians. */
	void SetFov( float fov );
	
	/** \brief Aspect ratio of the horizontal field of view to the vertical field of view. */
	inline float GetFovRatio() const{ return pFovRatio; }
	
	/** \brief Set aspect ratio of the horizonral field of view to the vertical field of view. */
	void SetFovRatio( float ratio );
	
	/** \brief Distance to the image plane. */
	inline float GetImageDistance() const{ return pImageDistance; }
	
	/** \brief Set distance to the image plane. */
	void SetImageDistance( float distance );
	
	/** \brief View distance. */
	inline float GetViewDistance() const{ return pViewDistance; }
	
	/** \brief Set view distance. */
	void SetViewDistance( float viewDistance );
	
	
	
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
	
	/** \brief Distance of camera to the center point along the view direction. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance of camera to the center point along the view direction. */
	void SetDistance( float distance );
	
	
	
	/** \brief Enable global illumination (GI) if supported. */
	inline bool GetEnableGI() const{ return pEnableGI; }
	
	/** \brief Set to enable global illumination (GI) if supported. */
	void SetEnableGI( bool enable );
	
	
	
	/**
	 * \brief White intensity multiplier.
	 * \version 1.21
	 */
	inline float GetWhiteIntensity() const{ return pWhiteIntensity; }
	
	/**
	 * \brief Set white intensity multiplier.
	 * \version 1.21
	 */
	void SetWhiteIntensity( float intensity );
	
	/**
	 * \brief Bloom intensity multiplier.
	 * \version 1.21
	 */
	inline float GetBloomIntensity() const{ return pBloomIntensity; }
	
	/**
	 * \brief Set bloom intensity multiplier.
	 * \version 1.21
	 */
	void SetBloomIntensity( float intensity );
	
	/**
	 * \brief Bloom strength as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	inline float GetBloomStrength() const{ return pBloomStrength; }
	
	/**
	 * \brief Set bloom strength as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	void SetBloomStrength( float strength );
	
	/**
	 * \brief Bloom blend as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	inline float GetBloomBlend() const{ return pBloomBlend; }
	
	/**
	 * \brief Set bloom blend as multiplier of intensity beyond bloom intensity.
	 * \version 1.21
	 */
	void SetBloomBlend( float blend );
	
	/**
	 * \brief Bloom size as percentage of screen width.
	 * \version 1.21
	 */
	inline float GetBloomSize() const{ return pBloomSize; }
	
	/**
	 * \brief Bloom size as percentage of screen width.
	 * \version 1.21
	 */
	void SetBloomSize( float size );
	
	
	
	/**
	 * \brief Custom tone mapping curve or empty curve to disable.
	 * \version 1.21
	 */
	inline const decCurveBezier &GetToneMapCurve() const{ return pToneMapCurve; }
	
	/**
	 * \brief Set custom tone mapping curve or empty curve to disable.
	 * \version 1.21
	 * \note If enabled make sure to set the matching white intensity as it defaults to 4.
	 */
	void SetToneMapCurve( const decCurveBezier &curve );
	
	
	
	/** \brief View matrix. */
	inline const decDMatrix &GetViewMatrix() const{ return pViewMatrix; }
	
	/**
	 * \brief Direction of ray shot from camera position through point on image plane.
	 * 
	 * The width and height are the size of the screen.
	 */
	decVector GetDirectionFor( int width, int height, int x, int y ) const;
	
	/** \brief Reset camera. */
	virtual void Reset();
	
	/** \brief World changed. */
	virtual void WorldChanged();
	
	/** \brief Geometry changed. */
	virtual void GeometryChanged();
	
	/** \brief Parameter changed. */
	virtual void ParameterChanged();
	
	/** \brief Adaption changed. */
	virtual void AdaptionChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateCameraPosition();
	void pUpdateViewMatrix();
};

#endif
