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

#ifndef _DEOGLSKINTEXTUREPIPELINEMAP_H_
#define _DEOGLSKINTEXTUREPIPELINEMAP_H_

#include "deoglSkinTexturePipelines.h"
#include "deoglSkinTexturePipelinesList.h"


/**
 * Skin texture pipeline map.
 */
class deoglSkinTexturePipelineMap{
public:
	struct sEntry{
		deoglSkinTexturePipelinesList::ePipelineTypes list;
		deoglSkinTexturePipelines::eTypes type;
		int modifiers;
	};
	
	
	
private:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline map. */
	deoglSkinTexturePipelineMap();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Map pipeline. */
	int Map( deoglSkinTexturePipelinesList::ePipelineTypes list,
		deoglSkinTexturePipelines::eTypes type,
		deoglSkinTexturePipelines::eModifiers modifier ); 
	/*@}*/
	
	
	
private:
	int pMap[ deoglSkinTexturePipelinesList::PipelineTypesCount ]
		[ deoglSkinTexturePipelines::TypeCount ]
		[ deoglSkinTexturePipelines::ModifiersPerType ];
	
	sEntry pMapInv[ 749 ];
	
	void pInitMap();
	
	void pInitGeometry( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitGeometryDepthTest( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitMask( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitGIMaterial( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllDepth( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllCounter( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllShadow( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	void pInitAllGeometryDepthCounterShadow( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list );
	
	void pSetModified( int &index, deoglSkinTexturePipelinesList::ePipelineTypes list,
		deoglSkinTexturePipelines::eTypes type, int modifierMask );
};

#endif
