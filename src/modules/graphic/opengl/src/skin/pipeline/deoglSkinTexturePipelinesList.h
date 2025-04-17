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

#ifndef _DEOGLSKINTEXTUREPIPELINESLIST_H_
#define _DEOGLSKINTEXTUREPIPELINESLIST_H_

#include "deoglSkinTexturePipelines.h"

class deoglBatchedShaderLoading;


/**
 * Skin texture pipelines list.
 */
class deoglSkinTexturePipelinesList{
public:
	enum ePipelineTypes{
		eptComponent,
		eptBillboard,
		eptDecal,
		eptPropField,
		eptPropFieldImposter,
		eptHeightMap1,
		eptHeightMap2,
		eptParticle,
		eptParticleRibbon,
		eptParticleBeam,
		eptOutline
	};
	
	static const int PipelineTypesCount = eptOutline + 1;
	
	
	
private:
	deoglSkinTexturePipelines::Ref pPipelines[ PipelineTypesCount ];
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline. */
	deoglSkinTexturePipelinesList( const deoglSkinTexture &texture );
	
	/** Clean up skin texture pipeline. */
	~deoglSkinTexturePipelinesList();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Get pipelines. */
	deoglSkinTexturePipelines &GetAt( ePipelineTypes type ) const;
	
	/** Prepare all pipelines. */
	void Prepare( deoglBatchedShaderLoading &batched );
	/*@}*/
};

#endif
