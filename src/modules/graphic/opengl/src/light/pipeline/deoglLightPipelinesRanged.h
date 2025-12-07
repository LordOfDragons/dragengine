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

#ifndef _DEOGLLIGHTPIPELINESRANGED_H_
#define _DEOGLLIGHTPIPELINESRANGED_H_

#include "deoglLightPipelines.h"

class deoglRLight;
class deoglLightShaderConfig;


/**
 * Light source specific light pipelines.
 */
class deoglLightPipelinesRanged : public deoglLightPipelines{
protected:
	const deoglRLight &pLight;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create light pipeline. */
	deoglLightPipelinesRanged(const deoglRLight &light);
	
protected:
	/** Clean up light pipeline. */
	~deoglLightPipelinesRanged() override;
	/*@}*/
	
	
	
protected:
	void pSetBaseNoShadow(deoglLightShaderConfig &shaconf);
	void pSetBaseSolid1(deoglLightShaderConfig &shaconf);
	void pSetBaseSolid1Transp1(deoglLightShaderConfig &shaconf);
	void pSetBaseSolid2(deoglLightShaderConfig &shaconf);
	void pSetBaseSolid2Transp1(deoglLightShaderConfig &shaconf);
	void pSetBaseSolid2Transp2(deoglLightShaderConfig &shaconf);
	void pSetBaseLumSolid1(deoglLightShaderConfig &shaconf);
	void pSetBaseLumSolid2(deoglLightShaderConfig &shaconf);
	void pSetBaseGIRayNoShadow(deoglLightShaderConfig &shaconf);
	void pSetBaseGIRaySolid1(deoglLightShaderConfig &shaconf);
	void pSetBaseGIRaySolid2(deoglLightShaderConfig &shaconf);
	
	void pSetGI(deoglLightShaderConfig &shaconf);
	void pSetNonGI(deoglLightShaderConfig &shaconf);
};

#endif
