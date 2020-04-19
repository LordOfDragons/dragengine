/* 
 * Drag[en]gine IGDE Sky Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _SEWINDOWCURVES_H_
#define _SEWINDOWCURVES_H_

#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/curveedit/igdeViewCurveBezierReference.h>
#include <deigde/gui/layout/igdeContainerSplitted.h>
#include <deigde/undo/igdeUndoReference.h>

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
	seWindowCurvesListener *pListener;
	
	seSky *pSky;
	
	igdeViewCurveBezierReference pEditCurve;
	igdeListBoxReference pListLinks;
	
	igdeUndoReference pUndoSetCurve;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	seWindowCurves( seWindowMain &windowMain );
	
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
	void SetSky( seSky *sky );
	
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
