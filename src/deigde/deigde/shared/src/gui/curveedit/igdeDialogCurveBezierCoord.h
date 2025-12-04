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

#ifndef _IGDEDIALOGCURVEBEZIERCOORD_H_
#define _IGDEDIALOGCURVEBEZIERCOORD_H_

#include "../dialog/igdeDialog.h"
#include "../composed/igdeEditVector2.h"

#include <dragengine/common/curve/decCurveBezierPoint.h>



/**
 * \brief Dialog bezier curve coordinates.
 */
class DE_DLL_EXPORT igdeDialogCurveBezierCoord : public igdeDialog{
private:
	decCurveBezierPoint pPoint;
	igdeEditVector2::Ref pEditPoint;
	igdeEditVector2::Ref pEditHandle1;
	igdeEditVector2::Ref pEditHandle2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogCurveBezierCoord( igdeEnvironment &environment );
	
	/** \brief Create dialog. */
	igdeDialogCurveBezierCoord( igdeEnvironment &environment, const decCurveBezierPoint &point );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogCurveBezierCoord();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Point. */
	inline const decCurveBezierPoint &GetPoint() const{ return pPoint; }
	
	/** \brief Set point. */
	void SetPoint( const decCurveBezierPoint &point );
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeEnvironment &environment );
};

#endif
