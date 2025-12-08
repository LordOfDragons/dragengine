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

#ifndef _SEWINDOWCURVES_H_
#define _SEWINDOWCURVES_H_

#include <deigde/gui/igdeListBox.h>
#include <deigde/gui/curveedit/igdeViewCurveBezier.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/undo/igdeUndo.h>

class seWindowMain;
class seSky;
class seLink;
class seWindowCurvesListener;


/**
 * \brief Curves panel.
 */
class seWindowCurves : public igdeContainerSplitted{
private:
	seWindowMain &pWindowMain;
	seWindowCurvesListener::Ref pListener;
	
	seSky::Ref pSky;
	
	igdeViewCurveBezier::Ref pEditCurve;
	igdeListBox::Ref pListLinks;
	
	igdeUndo::Ref pUndoSetCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	seWindowCurves(seWindowMain &windowMain);
	
protected:
	/** \brief Clean up window. */
	virtual ~seWindowCurves();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Sky. */
	inline seSky *GetSky() const{ return pSky; }
	
	/** \brief Set sky. */
	void SetSky(seSky::Ref sky);
	
	/** \brief Active link or \em NULL. */
	seLink *GetLink() const;
	
	/** \brief Update curve. */
    void UpdateCurve();
	
	/** \brief Update link list. */
	void UpdateLinkList();
	
	/** \brief Select active link. */
	void SelectActiveLink();
	/*@}*/
};

#endif
