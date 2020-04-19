/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SEVIEWSYNTHESIZER_H_
#define _SEVIEWSYNTHESIZER_H_

#include <deigde/gui/layout/igdeContainerBorder.h>

class seWPSource;
class seWPLink;
class seWPController;
class seWPSynthesizer;
class seWindowMain;
class seSynthesizer;



/**
 * \brief Synthesizer view.
 */
class seViewSynthesizer : public igdeContainerBorder{
private:
	seWindowMain &pWindowMain;
	
	seSynthesizer *pSynthesizer;
	
	seWPController *pWPController;
	seWPLink *pWPLink;
	seWPSource *pWPSource;
	seWPSynthesizer *pWPSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view. */
	seViewSynthesizer( seWindowMain &windowMain );
	
protected:
	/** \brief Clean up view. */
	virtual ~seViewSynthesizer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline seWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Synthesizer or \em NULL if not set. */
	inline seSynthesizer *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or \em NULL if not set. */
	void SetSynthesizer( seSynthesizer *synthesizer );
	
	/** \brief Game like frame update. */
	void OnFrameUpdate( float elapsed );
	/*@}*/
};

#endif
