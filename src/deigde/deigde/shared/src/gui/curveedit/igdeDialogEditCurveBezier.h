/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _IGDEDIALOGEDITCURVEBEZIER_H_
#define _IGDEDIALOGEDITCURVEBEZIER_H_

#include "igdeViewCurveBezier.h"
#include "../dialog/igdeDialog.h"

#include <dragengine/common/curve/decCurveBezierPoint.h>


/**
 * \brief Dialog edit bezier curve.
 */
class DE_DLL_EXPORT igdeDialogEditCurveBezier : public igdeDialog{
public:
	/** \brief Type holding strong reference. */
	using Ref = deTObjectReference<igdeDialogEditCurveBezier>;
	
	
private:
	igdeViewCurveBezier::Ref pViewCurveBezier;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogEditCurveBezier(igdeEnvironment &environment);
	
	/** \brief Create dialog. */
	igdeDialogEditCurveBezier(igdeEnvironment &environment, const decCurveBezier &curve);
	
	
protected:
	/** \brief Clean up dialog. */
	~igdeDialogEditCurveBezier() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Curve.. */
	const decCurveBezier &GetCurve() const;
	
	/** \brief Set curve. */
	void SetCurve(const decCurveBezier &curve);
	
	/** \brief Curve editor. */
	inline const igdeViewCurveBezier::Ref &GetViewCurveBezier() const{ return pViewCurveBezier; }
	
	/**
	 * \brief Init running.
	 * 
	 * Called by Run() after dialog is created. Subclass can implement to init controls
	 * after the dialog is visible on screen.
	 */
	void OnDialogShown() override;
	/*@}*/
	
	
private:
	void pCreateContent(igdeEnvironment &environment);
};

#endif
