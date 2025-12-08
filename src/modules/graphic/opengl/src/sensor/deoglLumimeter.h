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

#ifndef _DEOGLLUMIMETER_H_
#define _DEOGLLUMIMETER_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicLumimeter.h>
#include <dragengine/common/math/decMath.h>

class deoglRLumimeter;

class deGraphicOpenGl;
class deLumimeter;



/**
 * Lumimeter peer.
 */
class deoglLumimeter : public deBaseGraphicLumimeter{
public:
	deGraphicOpenGl &pOgl;
	const deLumimeter &pLumimeter;
	
	deoglRLumimeter::Ref pRLumimeter;
	
	float pLuminance;
	decColor pColor;
	
	bool pDirtyLumimeter;
	bool pDirtyOctree;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create lumimeter peer. */
	deoglLumimeter(deGraphicOpenGl &ogl, const deLumimeter &lumimeter);
	
	/** Clean up lumimeter peer. */
	virtual ~deoglLumimeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Lumimeter. */
	inline const deLumimeter &GetLumimeter() const{ return pLumimeter; }
	
	
	
	/** Render lumimter. */
	inline deoglRLumimeter *GetRLumimeter() const{ return pRLumimeter; }
	
	/** Update render thread counterpart if required. */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Position changed. */
	virtual void PositionChanged();
	
	/** Direction changed. */
	virtual void DirectionChanged();
	
	/** Cone parameter changed. */
	virtual void ConeParameterChanged();
	/*@}*/
	
	
	
	/** \name Measuring */
	/*@{*/
	/** Measures the luminance. */
	virtual float MeasureLuminance();
	
	/** Measures the color. */
	virtual decColor MeasureColor();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
