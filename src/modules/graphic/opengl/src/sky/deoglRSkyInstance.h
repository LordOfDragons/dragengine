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

#ifndef _DEOGLRSKYINSTANCE_H_
#define _DEOGLRSKYINSTANCE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoglRWorld;
class deoglRenderThread;
class deoglRSky;
class deoglRSkyInstanceLayer;

class deSkyInstance;



/**
 * \brief Render sky instance.
 */
class deoglRSkyInstance : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRWorld *pParentWorld;
	
	deoglRSky *pRSky;
	
	int pOrder;
	decLayerMask pLayerMask;
	
	float *pControllerStates;
	int pControllerStateCount;
	
	deoglRSkyInstanceLayer **pLayers;
	int pLayerCount;
	
	float pTotalSkyLightIntensity;
	float pTotalSkyAmbientIntensity;
	decColor pTotalSkyLightColor;
	
	float pEnvMapTimer;
	
	bool pSkyNeedsUpdate;
	bool pWorldMarkedRemove;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sky instance. */
	deoglRSkyInstance( deoglRenderThread &renderThread );
	
	/** \brief Clean up render sky instance. */
	virtual ~deoglRSkyInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Parent world or \em NULL. */
	inline deoglRWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( deoglRWorld *world );
	
	
	
	/** \brief Render sky or \em NULL. */
	inline deoglRSky *GetRSky() const{ return pRSky; }
	
	/**
	 * \brief Set render sky or \em NULL.
	 * 
	 * Called during synchronization time.
	 */
	void SetRSky( deoglRSky *rsky );
	
	
	
	/** \brief Rendering order. */
	inline int GetOrder() const{ return pOrder; }
	
	/** \brief Set rendering order. */
	void SetOrder( int order );
	
	/** \brief Layer mask. */
	inline  const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Number of controller states. */
	inline int GetControllerStateCount() const{ return pControllerStateCount; }
	
	/** \brief Controller state at index. */
	float GetControllerStateAt( int index ) const;
	
	/**
	 * \brief Update controller states.
	 * 
	 * Called during synchronization time.
	 */
	void UpdateControllerStates( const deSkyInstance &instance );
	
	
	
	/** \brief Number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** \brief Layer at index. */
	deoglRSkyInstanceLayer &GetLayerAt( int index ) const;
	
	/** \brief Rebuild layers. */
	void RebuildLayers();
	
	/** \brief Update layers. */
	void UpdateLayers();
	
	
	
	/** \brief Total sky light intensity. */
	inline float GetTotalSkyLightIntensity() const{ return pTotalSkyLightIntensity; }
	
	/** \brief Total sky ambient only light intensity. */
	inline float GetTotalSkyAmbientIntensity() const{ return pTotalSkyAmbientIntensity; }
	
	/** \brief Total sky light color. */
	inline const decColor &GetTotalSkyLightColor() const{ return pTotalSkyLightColor; }
	
	
	
	/** \brief Environment map timer. */
	inline float GetEnvironmentMapTimer() const{ return pEnvMapTimer; }
	
	/** \brief Set environment map timer. */
	void SetEnvironmentMapTimer( float timer );
	
	
	
	/** \brief Prepare for rendering. */
	void PrepareForRender();
	
	/** \brief Notifiy world about changes in the sky if required. */
	void NotifySkyChanged();
	
	
	
	/** \brief Prepare for quick disposal of sky instance. */
	void PrepareQuickDispose();
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
	/*@}*/
};

#endif
