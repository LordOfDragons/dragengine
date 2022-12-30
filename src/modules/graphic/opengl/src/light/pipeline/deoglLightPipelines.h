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

#ifndef _DEOGLLIGHTPIPELINES_H_
#define _DEOGLLIGHTPIPELINES_H_

#include "deoglLightPipeline.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../utils/deoglDebugNamesEnumSet.h"

#include <dragengine/deObject.h>


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelines : public deObject{
public:
	typedef deTObjectReference<deoglLightPipelines> Ref;
	
	/** Type. */
	enum eTypes{
		etNoShadow, //<! No shadow casting.
		etAmbient, //<! Ambient light only.
		etSolid1, //<! Single solid no transparent shadow.
		etSolid1Transp1, //<! Single solid and transparent shadow.
		etSolid2, //<! Double solid no transparent shadow.
		etSolid2Transp1, //<! Double solid and single transparent shadow.
		etSolid2Transp2, //<! Double solid and transparent shadow.
		etLumSolid1, //<! Luminance only single solid (unused).
		etLumSolid2, //<! Luminance only double solid (unused).
		etGIRayNoShadow, //<! GI rays no shadow casting.
		etGIRaySolid1, //<! GI rays single solid shadow.
		etGIRaySolid2 //<! GI rays double solid shadow.
	};
	
	static const deoglDebugNamesEnum DebugNamesTypes;
	
	/** Modifier. */
	enum eModifiers{
		emNoAmbient = 0x1,
		emStereo = 0x2,
		emTransparent = 0x4,
		emFlipCullFace = 0x8
	};
	
	static const deoglDebugNamesEnumSet DebugNamesModifiers;
	
	
	
protected:
	const static int TypeCount = etGIRaySolid2 + 1;
	const static int ModifiersPerType = emTransparent << 1;
	
	deoglRenderThread &pRenderThread;
	deoglLightPipeline::Ref pPipelines[ TypeCount ][ ModifiersPerType ];
	bool pPrepared;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelines( deoglRenderThread &renderThread );
	
protected:
	/** Clean up light pipeline. */
	virtual ~deoglLightPipelines();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Pipeline with type and modifiers or nullptr. */
	const deoglLightPipeline *GetWith( eTypes type, int modifiers ) const;
	
	/** Pipeline with type and modifiers throwing exception it nullptr. */
	const deoglLightPipeline &GetWithRef( eTypes type, int modifiers ) const;
	
	/** Prepare pipelines. */
	void Prepare();
	
	/** Debug name. */
	virtual const char *GetDebugName() const = 0;
	/*@}*/
	
	
	
protected:
	virtual void pPreparePipelines() = 0;
	
	void pBasePipelineConfig( deoglRenderThread &renderThread, deoglPipelineConfiguration &config );
	
	void pBasePipelineConfigGI( deoglPipelineConfiguration &config );
	
	void pCreatePipelines( deoglRenderThread &renderThread, deoglPipelineConfiguration &pipconf,
		deoglLightShaderConfig &shaconf, eTypes type, int modifierMask );
};

#endif
