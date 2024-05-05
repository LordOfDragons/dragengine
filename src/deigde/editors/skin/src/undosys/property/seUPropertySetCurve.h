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

#if 0
// include only once
#ifndef _SEUPROPERTYSETCURVE_H_
#define _SEUPROPERTYSETCURVE_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/curve/decCurveBezier.h>

// predefinitions
class seSkyLayer;
class seProperty;



/**
 * \brief Undo Action Set Layer Property Curve.
 */
class seUPropertySetCurve : public igdeUndo{
private:
	seSkyLayer *pLayer;
	seProperty *pProperty;
	
	decCurveBezier pOldCurve;
	decCurveBezier pNewCurve;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	seUPropertySetCurve( seSkyLayer *layer, seProperty *property );
	/** \brief Clean up undo. */
	virtual ~seUPropertySetCurve();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Sets the new curve. */
	void SetNewCurve( const decCurveBezier &curve );
	
	/** \brief Undo action. */
	virtual void Undo();
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif
#endif
