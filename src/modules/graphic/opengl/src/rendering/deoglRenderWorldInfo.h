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

#ifndef _DEOGLRENDERWORLDINFO_H_
#define _DEOGLRENDERWORLDINFO_H_

#include "deoglRenderBase.h"

class deoglDebugInformation;



/**
 * \brief World renderer information for debugging purpose.
 */
class deoglRenderWorldInfo : public deoglRenderBase{
public:
	deoglDebugInformation *infoWorld;
	deoglDebugInformation *infoPrepare;
	deoglDebugInformation *infoPassMasked;
	deoglDebugInformation *infoSolidGeometry;
	deoglDebugInformation *infoReflection;
	deoglDebugInformation *infoSSR;
	deoglDebugInformation *infoSolidGeometryLights;
	deoglDebugInformation *infoTransparent;
	deoglDebugInformation *infoToneMapping;
	deoglDebugInformation *infoDebugDrawers;
	deoglDebugInformation *infoPostProcessing;
	
	deoglDebugInformation *infoSolidGeometryDetails;
	deoglDebugInformation *infoSolidGeometryClear;
	deoglDebugInformation *infoSolidGeometryDepthTask;
	deoglDebugInformation *infoSolidGeometryDepthRender;
	deoglDebugInformation *infoSolidGeometryOcclusion;
	deoglDebugInformation *infoSolidGeometryTranspCounter;
	deoglDebugInformation *infoSolidGeometrySky;
	deoglDebugInformation *infoSolidGeometryTask;
	deoglDebugInformation *infoSolidGeometryRender;
	deoglDebugInformation *infoSolidGeometryDecals;
	deoglDebugInformation *infoSolidGeometryDownsampleDepth;
	deoglDebugInformation *infoSolidGeometrySSAO;
	
	deoglDebugInformation *infoTransparentDetails;
	deoglDebugInformation *infoTransparentClear;
	deoglDebugInformation *infoTransparentDepthTask;
	deoglDebugInformation *infoTransparentDepthRender;
	deoglDebugInformation *infoTransparentCopyDepth;
	deoglDebugInformation *infoTransparentTask;
	deoglDebugInformation *infoTransparentRender;
	deoglDebugInformation *infoTransparentSSR;
	deoglDebugInformation *infoTransparentLights;
	deoglDebugInformation *infoTransparentVolumetric;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create info. */
	deoglRenderWorldInfo( deoglRenderThread &renderThread );
	
	/** \brief Clean up info. */
	virtual ~deoglRenderWorldInfo();
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** \brief Clear all. */
	void ClearAll();
	
	/** \brief Add top level debug information in the right order. */
	virtual void AddTopLevelDebugInfo();
	
	/** \brief Developer mode debug information changed. */
	virtual void DevModeDebugInfoChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
