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

#ifndef _DEOGLRSKY_H_
#define _DEOGLRSKY_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/deObject.h>

class deoglRSkyLayer;
class deoglRSkyLink;
class deoglRenderThread;

class deSky;



/**
 * \brief Render sky.
 */
class deoglRSky : public deObject{
private:
	deoglRenderThread &pRenderThread;
	
	deoglRSkyLink **pLinks;
	int pLinkCount;
	
	deoglRSkyLayer **pLayers;
	int pLayerCount;
	
	decColor pBgColor;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new render sky. */
	deoglRSky( deoglRenderThread &renderThread );
	
	/** \brief Clean up render sky. */
	~deoglRSky();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	
	
	/** \brief Background color of the sky. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Set background color of the sky. */
	void SetBgColor( const decColor &color );
	
	
	
	/** \brief Number of layers. */
	inline int GetLayerCount() const{ return pLayerCount; }
	
	/** \brief Get link at index. */
	const deoglRSkyLink &GetLinkAt( int index ) const;
	
	/**
	 * \brief Rebuild links.
	 * 
	 * \note Called from main thread during synchronoization.
	 */
	void RebuildLinks( const deSky &sky );
	
	
	
	/** \brief Layer at index. */
	deoglRSkyLayer &GetLayerAt( int index ) const;
	
	/**
	 * \brief Rebuild layers.
	 * 
	 * \note Called from main thread during synchronoization.
	 */
	void RebuildLayers( const deSky &sky );
	/*@}*/
};

#endif
