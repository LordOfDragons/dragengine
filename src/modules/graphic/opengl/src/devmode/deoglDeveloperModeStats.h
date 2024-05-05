/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DEOGLDEVELOPERMODESTATS_H_
#define _DEOGLDEVELOPERMODESTATS_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class decUnicodeArgumentList;
class decUnicodeString;



/**
 * OpenGL Developer Mode Stats.
 */
class deoglDeveloperModeStats{
private:
	deoglRenderThread &pRenderThread;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new developer mode stats object. */
	deoglDeveloperModeStats( deoglRenderThread &renderThread );
	/** Cleans up the developer mode stats object. */
	~deoglDeveloperModeStats();
	/*@}*/
	
	/** \name Management */
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
