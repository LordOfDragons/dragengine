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
	deoglDebugInformation::Ref infoWorld;
	deoglDebugInformation::Ref infoPassMasked;
	deoglDebugInformation::Ref infoGITraceRays;
	deoglDebugInformation::Ref infoSolidGeometry;
	deoglDebugInformation::Ref infoReflection;
	deoglDebugInformation::Ref infoSSR;
	deoglDebugInformation::Ref infoLuminancePrepare;
	deoglDebugInformation::Ref infoSolidGeometryLights;
	deoglDebugInformation::Ref infoTransparent;
	deoglDebugInformation::Ref infoToneMapping;
	deoglDebugInformation::Ref infoDebugDrawers;
	deoglDebugInformation::Ref infoPostProcessing;
	deoglDebugInformation::Ref infoDeveloperMode;
	deoglDebugInformation::Ref infoDeveloperModeTemp;
	
	deoglDebugInformation::Ref infoSolidGeometryDetails;
	deoglDebugInformation::Ref infoSolidGeometryDepthTask;
	deoglDebugInformation::Ref infoSolidGeometryDepthRender;
	deoglDebugInformation::Ref infoSolidGeometryOcclusion;
	deoglDebugInformation::Ref infoSolidGeometryTranspCounter;
	deoglDebugInformation::Ref infoSolidGeometrySky;
	deoglDebugInformation::Ref infoSolidGeometryTask;
	deoglDebugInformation::Ref infoSolidGeometryRender;
	deoglDebugInformation::Ref infoSolidGeometryDecals;
	deoglDebugInformation::Ref infoSolidGeometryDownsampleDepth;
	deoglDebugInformation::Ref infoSolidGeometrySSAO;
	
	deoglDebugInformation::Ref infoTransparentDetails;
	deoglDebugInformation::Ref infoTransparentClear;
	deoglDebugInformation::Ref infoTransparentDepthTask;
	deoglDebugInformation::Ref infoTransparentDepthRender;
	deoglDebugInformation::Ref infoTransparentCopyDepth;
	deoglDebugInformation::Ref infoTransparentTask;
	deoglDebugInformation::Ref infoTransparentRender;
	deoglDebugInformation::Ref infoTransparentSSR;
	deoglDebugInformation::Ref infoTransparentLights;
	deoglDebugInformation::Ref infoTransparentVolumetric;
	
	
	
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
