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

#ifndef _PEEWINDOWCURVES_H_
#define _PEEWINDOWCURVES_H_

#include "peeWindowCurvesListener.h"
#include "../../emitter/peeEmitter.h"

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/gui/resources/igdeIcon.h>
#include <deigde/undo/igdeUndo.h>

class peeParameter;
class peeType;
class peeWindowMain;



/**
 * \brief Curves Panel.
 */
class peeWindowCurves : public igdeContainerSplitted{
public:
	typedef deTObjectReference<peeWindowCurves> Ref;
	
	/** \brief Curve to edit. */
	enum eCurves{
		ecValue,
		ecSpread,
		ecProgress,
		ecBeam
	};
	
	
	
private:
	peeWindowMain &pWindowMain;
	peeWindowCurvesListener::Ref pListener;
	
	peeEmitter::Ref pEmitter;
	
	igdeIcon::Ref pIconCurveEmpty;
	igdeIcon::Ref pIconCurveUsed;
	
	igdeListBox::Ref pListCurves;
	igdeViewCurveBezier::Ref pEditCurve;
	
	igdeUndo::Ref pUndoSetCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	peeWindowCurves(peeWindowMain &windowMain);
	
protected:
	/** \brief Clean up window. */
	virtual ~peeWindowCurves();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline peeWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Active type. */
	peeType *GetType() const;
	
	/** \brief Active parameter. */
	peeParameter *GetParameter() const;
	
	/** \brief Curve to edit. */
	eCurves GetCurve() const;
	
	/** \brief Emitter. */
	inline const peeEmitter::Ref &GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter(peeEmitter *emitter);
	
	/** \brief Update curve. */
	void UpdateCurve();
	
	/** \brief Drop progressive undo. */
	void DropProgressiveUndo();
	
	
	
private:
	void pUpdateCurveListIcons();
	/*@}*/
};

#endif
