/* 
 * Drag[en]gine IGDE Conversation Editor
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

#ifndef _CERULEOFTHIRDS_H_
#define _CERULEOFTHIRDS_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>

class igdeEnvironment;


/**
 * \brief Rule of thirds aid canvas.
 */
class ceCanvasRuleOfThirdsAid{
private:
	igdeEnvironment &pEnvironment;
	deCanvasViewReference pCanvasView;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas. */
	ceCanvasRuleOfThirdsAid( igdeEnvironment &environment );
	
	/** \brief Clean up canvas. */
	~ceCanvasRuleOfThirdsAid();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Canvas to add to render window. */
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	
	
	/** \brief Update canvas size due to render window changing size. */
	void Resize();
	
	/** \brief Set visible. */
	void SetVisible( bool visible );
	/*@}*/
	
	
	
private:
	void pCreateCanvas();
	
	void pCreateAid();
	void pAddLine( int x1, int y1, int x2, int y2 );
	void pAddFocus( int x, int y );
	
	void pAddRect( int x1, int y1, int x2, int y2, const decColor &color, float transparency );
};

#endif
