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

#ifndef _DEOGLRENDERWORLDINFO_H_
#define _DEOGLRENDERWORLDINFO_H_

#include "deoglRenderBase.h"

#include "../debug/deoglDebugInformation.h"



/**
 * World renderer information for debugging purpose.
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
	/** Create info. */
	deoglRenderWorldInfo(deoglRenderThread &renderThread);
	
	/** Clean up info. */
	~deoglRenderWorldInfo() override;
	/*@}*/
	
	
	
	/** \name Debug */
	/*@{*/
	/** Clear all. */
	void ClearAll();
	
	/** Add top level debug information in the right order. */
	void AddTopLevelDebugInfo() override;
	
	/** Developer mode debug information changed. */
	void DevModeDebugInfoChanged() override;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
