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

#ifndef _DEOGLIMPOSTERBILLBOARD_H_
#define _DEOGLIMPOSTERBILLBOARD_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglArrayTexture;



/**
 * @brief Imposter Billboard.
 * Stores the textures and other information required for an imposter billboard.
 * Imposter billboards are typically created from skin shader driven objects but
 * can be possibly created from others. An imposter billboard composes of one or
 * more array textures storing the material properties the way a skin shader
 * expects them. The layers in the array texture correspond to views of the imposter
 * from different angles. The layout of the angles is set up by the owner of the
 * imposter billboard. Using array textures the appropriate imposter can be
 * selected for instanced geometry using the layer index.
 */
class deoglImposterBillboard{
private:
	deoglRenderThread &pRenderThread;
	
	deoglArrayTexture **pChannelTextures;
	
	decVector2 pMinExtend;
	decVector2 pMaxExtend;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new imposter billboard. */
	deoglImposterBillboard( deoglRenderThread &renderThread );
	/** Cleans up the imposter billboard. */
	~deoglImposterBillboard();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the array texture for a skin texture channel. */
	deoglArrayTexture *GetChannelTextureAt( int channel ) const;
	/** Sets if an array texture for a skin texture channel exists. */
	void EnableChannelTexture( int channel, bool enable );
	/** Determines if an array texture for a skin texture channel exists. */
	bool IsChannelTextureEnabled( int channel ) const;
	
	/** Retrieves the minimum extend. */
	inline const decVector2 &GetMinExtend() const{ return pMinExtend; }
	/** Retrieves the maximum extend. */
	inline const decVector2 &GetMaxExtend() const{ return pMaxExtend; }
	/** Sets the minimum and maximum extend. */
	void SetExtends( const decVector2 &minExtend, const decVector2 &maxExtend );
	/*@}*/
};

#endif
