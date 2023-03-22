/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLSKINTEXTUREPIPELINE_H_
#define _DEOGLSKINTEXTUREPIPELINE_H_

#include <dragengine/deObject.h>

class deoglPipeline;
class deoglSkinShader;


/**
 * Skin texture pipeline.
 */
class deoglSkinTexturePipeline : public deObject{
public:
	typedef deTObjectReference<deoglSkinTexturePipeline> Ref;
	
	
	
private:
	const deoglPipeline * const pPipeline;
	deoglSkinShader *pShader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglSkinTexturePipeline( const deoglPipeline *pipeline, deoglSkinShader *shader );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglSkinTexturePipeline();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pipeline. */
	inline const deoglPipeline *GetPipeline() const{ return pPipeline; }
	
	/** Shader. */
	inline deoglSkinShader *GetShader() const{ return pShader; }
	/*@}*/
};

#endif
