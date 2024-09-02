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

#ifndef _DEOGLSKINSTATECALCULATED_H_
#define _DEOGLSKINSTATECALCULATED_H_

#include <dragengine/common/math/decMath.h>

class deoglRenderThread;
class deoglTexture;
class deoglSkinState;
class deoglSkinCalculatedProperty;


/** Skin state calculated. */
class deoglSkinStateCalculated{
private:
	decColor pColor;
	deoglTexture *pTexture;
	deoglSkinCalculatedProperty *pProperty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin state calculated. */
	deoglSkinStateCalculated();
	
	/** Clean up skin state calculated. */
	~deoglSkinStateCalculated();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Calculated color. */
	inline const decColor &GetColor() const{ return pColor; }
	
	/** Set calculated color. */
	void SetColor( const decColor &color );
	
	/** Temporary texture or nullptr if not existing. */
	inline deoglTexture *GetTexture() const{ return pTexture; }
	
	/** Set temporary texture or nullptr if not existing. */
	void SetTexture( deoglTexture *texture );
	
	/** Skin calculated property. */
	deoglSkinCalculatedProperty *GetProperty() const{ return pProperty; }
	
	/** Set skin calculated property. */
	void SetProperty( deoglSkinCalculatedProperty *property );
	
	/** Update. */
	void Update( deoglSkinState &skinState );
	/*@}*/
};

#endif
