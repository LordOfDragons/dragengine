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

#ifndef _DEOGLRRDSRENDERABLE_H_
#define _DEOGLRRDSRENDERABLE_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>

class deoglCubeMap;
class deoglTexture;
class deoglRDynamicSkin;
class deoglRenderPlan;
class deoglRenderPlanMasked;



/**
 * Render dynamic skin renderable.
 */
class deoglRDSRenderable : public deObject{
public:
	enum eType{
		etValue,
		etColor,
		etImage,
		etVideoFrame,
		etCanvas,
		etCamera
	};
	
	
	
private:
	const eType pType;
	decString pName;
	deoglRDynamicSkin &pDynamicSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin renderable. */
	deoglRDSRenderable( eType type, deoglRDynamicSkin &dynamicSkin );
	
	/** Clean up render dynamic skin renderable. */
	virtual ~deoglRDSRenderable();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Type. */
	inline const eType GetType() const{ return pType; }
	
	/** Dynamic skin. */
	inline deoglRDynamicSkin &GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** Name. */
	inline const decString &GetName() const{ return pName; }
	
	/** Set name. */
	void SetName( const char *name );
	
	/** Prepare for render. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask ) = 0;
	
	/** Render if required. */
	virtual void Render( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * Get value if support or default value.
	 * \details Default implementation returns default value.
	 */
	virtual float GetRenderValue( float defaultValue );
	
	/**
	 * Get color if support or default color.
	 * \details Default implementation returns default value.
	 */
	virtual decColor GetRenderColor( const decColor &defaultColor );
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	
	/**
	 * Get cubemap to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglCubeMap *GetRenderCubeMap();
	
	/**
	 * Get render plan to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglRenderPlan *GetRenderPlan() const;
	/*@}*/
};

#endif
