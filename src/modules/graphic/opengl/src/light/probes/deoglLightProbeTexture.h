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

#ifndef _DEOGLLIGHTPROBETEXTURE_H_
#define _DEOGLLIGHTPROBETEXTURE_H_

#include "../../deoglBasics.h"

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglTexture;
class deoglLPTProbe;



/**
 * @brief Light Probe Texture.
 * @par Light Probes
 * each probe contains 6 rgb-color readings indicating the color hitting the probe from this direction.
 * probes are stored as rgb-float-color in the texture as a consecutive run (each of the 6 pixels stores
 * the color of one sample).
 * the probes store:
 * <ul>
 * <li>position(vector, relative to camera)</li>
 * <li>texture coordinates (2-integer) of the first (left most) pixel in the texture</li>
 * </ul>
 * 
 * @par Probe Texture
 * stores all probes for a single frame render. to produce the probe texture it is first cleared to all
 * black. then for each light rendered during solid render lighting pass after shadow casting the probe
 * texture is activated (separate FBO) and a full texture quad rendered. in the shader the probe directions
 * are tested against the light using the shadow maps as with normal lighting and the light contributions
 * added to the probe texture (GL_ONE,GL_ONE). then the defren textures are enabled to continue rendering
 * as done currently. this way light shadow maps can be used right at the time they have been produced
 * without requiring storing them or recreating them. once all lights have been processed the probes texture
 * contains a full list of probe samples.
 * 
 * @par Rendering
 * create a render task with all component faces required to be rendered using the probe texture. attach
 * only the color texture as no lighting passes are required. while rendering tap the probe samples from
 * the texture and use them to determine the lighting contribution. for this interprate the ligth samples
 * as 6 point lights arranged around the object. by adding the contributions of each direction normals
 * still have an influence. it's not physically correct but the error should be barely noticeable.
 * 
 * @par When to use
 * for transparent objects as they would cost too much speed.
 * 
 * @par Position/direction
 * for building the probes texture the position and direction for each probe are stored in an vbo.
 * 
 * @par Maximum Probe Count
 * The maximum number of probes is defined by the texture size. If more probes are required the texture size
 * is enlarged first in the X direction and then in the Y direction. The default texture size is 32x32. Here
 * a list of texture size and their maximum number of probes ( floor(width/6)*height ):
 * <ul>
 * <li>32x32 = 160 probes</li>
 * <li>64x32 = 320 probes</li>
 * <li>64x64 = 640 probes</li>
 * <li>128x64 = 1'344 probes</li>
 * <li>128x128 = 2'688 probes</li>
 * <li>256x128 = 5'376 probes</li>
 * <li>256x256 = 10'752 probes</li>
 * </ul>
 * More than 10'000 probes are usually not required but the size can grow further if required. Hence 10'000
 * probes can be calculated with a single 256x256 resolution texture which takes next to no time to process.
 * 
 */
class deoglLightProbeTexture{
private:
	struct sVBOData{
		GLushort x, y; // point position
		GLfloat px, py, pz; // probe position
		GLfloat dx, dy, dz; // probe direction
	};
	
private:
	deoglRenderThread &pRenderThread;
	
	deoglLPTProbe *pProbes;
	int pProbeCount;
	int pProbeSize;
	
	deoglTexture *pTexturePosition;
	deoglTexture *pTextureDirection;
	deoglTexture *pTextureProbes;
	int pProbesPerLine;
	int pMaxProbeCount;
	int pUsedWidth;
	int pUsedHeight;
	
	GLuint pVBO;
	GLuint pVAO;
	sVBOData *pVBOData;
	int pVBOPointCount;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new light probe texture. */
	deoglLightProbeTexture( deoglRenderThread &renderThread );
	/** Cleans up the light probe texture. */
	~deoglLightProbeTexture();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Retrieves the number of probes. */
	inline int GetProbeCount() const{ return pProbeCount; }
	/** Retrieves the probes. */
	inline const deoglLPTProbe *GetProbes() const{ return pProbes; }
	/** Retrieves the probe at the given position. */
	const deoglLPTProbe &GetProbeAt( int index );
	/** Adds a probe. */
	void AddProbe( const decVector &position );
	/** Removes all probes. */
	void RemoveAllProbes();
	
	/** Retrieves the position texture. */
	inline deoglTexture *GetTexturePosition() const{ return pTexturePosition; }
	/** Retrieves the direction texture. */
	inline deoglTexture *GetTextureDirection() const{ return pTextureDirection; }
	/** Retrieves the probes texture. */
	inline deoglTexture *GetTextureProbes() const{ return pTextureProbes; }
	/** Retrieves the number of probes per texture line. */
	inline int GetProbesPerLine() const{ return pProbesPerLine; }
	/** Retrieves the maximum number of probes this texture can hold. */
	inline int GetMaxProbeCount() const{ return pMaxProbeCount; }
	/** Retrieves the used width of the texture. */
	inline int GetUsedWidth() const{ return pUsedWidth; }
	/** Retrieves the used height of the texture. */
	inline int GetusedHeight() const{ return pUsedHeight; }
	
	/** Update the VBO and calculate texture parameters. */
	void Update();
	/** Retrieves the vbo. */
	inline GLuint GetVBO() const{ return pVBO; }
	/** Retrieves the vao. */
	inline GLuint GetVAO() const{ return pVAO; }
	/** Retrieves the number of vbo points. */
	inline int GetVBOPointCount() const{ return pVBOPointCount; }
	/*@}*/
};

#endif

