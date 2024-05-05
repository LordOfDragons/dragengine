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

#ifndef _DEOGLSKINMAPPED_H_
#define _DEOGLSKINMAPPED_H_

#include <dragengine/common/curve/decCurveBezierEvaluator.h>
#include <dragengine/resources/skin/deSkinMapped.h>


class deoglSkinState;
class deoglSkinStateMapped;


/**
 * Skin mapped.
 */
class deoglSkinMapped : public deObject{
public:
	typedef deTObjectReference<deoglSkinMapped> Ref;
	
private:
	const deSkinMapped &pMapped;
	decCurveBezierEvaluator pCurveEvaluator;
	int pRenderable;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create skin mapped. */
	deoglSkinMapped( const deSkinMapped &mapped );
	
protected:
	/** Clean up skin mapped. */
	virtual ~deoglSkinMapped();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Skin mapped. */
	const deSkinMapped &GetMapped() const{ return pMapped; }
	
	/** Set renderable index. */
	void SetRenderable( int renderable );
	
	/** Mapped value. */
	float Calculate( const deoglSkinState &skinState, const deoglSkinStateMapped &mapped ) const;
	/*@}*/
	
	
	
private:
	float pCalculateInputValue( const deoglSkinState &skinState, const deoglSkinStateMapped &mapped ) const;
	float pCalculateOutputValue( float inputValue ) const;
};

#endif
