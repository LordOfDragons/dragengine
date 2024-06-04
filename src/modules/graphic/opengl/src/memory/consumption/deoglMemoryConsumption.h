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

#ifndef _DEOGLMEMORYCONSUMPTION_H_
#define _DEOGLMEMORYCONSUMPTION_H_

#include "deoglMemoryConsumptionBufferObject.h"
#include "deoglMemoryConsumptionDeferredRendering.h"
#include "deoglMemoryConsumptionShadow.h"
#include "deoglMemoryConsumptionSkin.h"
#include "deoglMemoryConsumptionTexture.h"



/**
 * Keeps track of the memory consumption.
 */
class deoglMemoryConsumption{
public:
	deoglMemoryConsumptionTexture texture2D;
	deoglMemoryConsumptionTexture texture3D;
	deoglMemoryConsumptionTexture textureArray;
	deoglMemoryConsumptionTexture textureCube;
	
	deoglMemoryConsumptionTexture texture2DRenderable;
	deoglMemoryConsumptionTexture texture3DRenderable;
	deoglMemoryConsumptionTexture textureArrayRenderable;
	deoglMemoryConsumptionTexture textureCubeRenderable;
	
	deoglMemoryConsumptionSkin skin;
	deoglMemoryConsumptionBufferObject bufferObject;
	deoglMemoryConsumptionDeferredRendering deferredRendering;
	deoglMemoryConsumptionShadow shadow;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create memory consumption. */
	deoglMemoryConsumption();
	
	/** Clean up memory consumption. */
	~deoglMemoryConsumption();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Total memory consumption. */
	unsigned long long Total() const;
	/*@}*/
};

#endif
