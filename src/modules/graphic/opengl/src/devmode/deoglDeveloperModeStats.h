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

#ifndef _DEOGLDEVELOPERMODESTATS_H_
#define _DEOGLDEVELOPERMODESTATS_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class decUnicodeArgumentList;
class decUnicodeString;



/**
 * @brief OpenGL Developer Mode Stats.
 */
class deoglDeveloperModeStats{
private:
	deoglRenderThread &pRenderThread;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new developer mode stats object. */
	deoglDeveloperModeStats( deoglRenderThread &renderThread );
	/** Cleans up the developer mode stats object. */
	~deoglDeveloperModeStats();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Print stats syntax. */
	void Help( decUnicodeString &answer );
	/** Stats command. */
	void Stats( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	/** Skin stats. */
	void Skins( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Combined textures stats. */
	void CombinedTextures( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Shader sources. */
	void ShaderSources( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Shader programs. */
	void ShaderPrograms( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Skin shaders. */
	void SkinShaders( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Renderables color texture. */
	void RenderablesTexturesColor( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Renderables depth texture. */
	void RenderablesTexturesDepth( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Renderables color cube map. */
	void RenderablesCubeColor( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Renderables depth cube map. */
	void RenderablesCubeDepth( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Shared VBOs. */
	void SharedVBOs( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/** Texture units configurations. */
	void TextureUnitsConfigurations( const decUnicodeArgumentList &command, decUnicodeString &answer );
	/*@}*/
};

#endif
