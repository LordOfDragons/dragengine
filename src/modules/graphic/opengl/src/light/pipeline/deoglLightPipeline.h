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

#ifndef _DEOGLLIGHTPIPELINE_H_
#define _DEOGLLIGHTPIPELINE_H_

#include "../shader/deoglLightShader.h"

class deoglPipeline;


/**
 * Light pipeline.
 */
class deoglLightPipeline : public deObject{
public:
	typedef deTObjectReference<deoglLightPipeline> Ref;
	
	
	
private:
	const deoglPipeline * const pPipeline;
	const deoglLightShader::Ref pShader;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipeline( const deoglPipeline *pipeline, const deoglLightShader::Ref &shader );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglLightPipeline();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pipeline. */
	inline const deoglPipeline *GetPipeline() const{ return pPipeline; }
	
	/** Shader. */
	inline const deoglLightShader::Ref &GetShader() const{ return pShader; }
	/*@}*/
};

#endif
