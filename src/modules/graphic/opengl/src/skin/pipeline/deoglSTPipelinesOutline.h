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

#ifndef _DEOGLSTPIPELINESOUTLINE_H_
#define _DEOGLSTPIPELINESOUTLINE_H_

#include "deoglSkinTexturePipelines.h"


/**
 * Outline skin texture pipelines.
 */
class deoglSTPipelinesOutline : public deoglSkinTexturePipelines{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin texture pipeline. */
	deoglSTPipelinesOutline(const deoglSkinTexture &texture);
	
protected:
	/** Clean up skin texture pipeline. */
	~deoglSTPipelinesOutline() override;
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Debug name. */
	const char *GetDebugName() const override;
	/*@}*/
	
	
	
protected:
	void pPreparePipelines(const ChannelInfo &cinfo, deoglBatchedShaderLoading &batched) override;
	
	virtual void pPipelineConfigGeometry(deoglPipelineConfiguration &config);
	virtual void pPipelineConfigDepth(deoglPipelineConfiguration &config);
	virtual void pPipelineConfigDepthReversed(deoglPipelineConfiguration &config);
	virtual void pPipelineConfigCounter(deoglPipelineConfiguration &config);
	
	void pSetBase(deoglSkinShaderConfig &config) override;
	void pSetGeometry(deoglSkinShaderConfig &config, const ChannelInfo &cinfo) override;
	void pSetDepth(deoglSkinShaderConfig &config, const ChannelInfo &cinfo) override;
	void pSetCounter(deoglSkinShaderConfig &config, const ChannelInfo &cinfo) override;
};

#endif
