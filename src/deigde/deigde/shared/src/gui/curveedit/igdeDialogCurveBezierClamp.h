/* 
 * Drag[en]gine IGDE
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

#ifndef _IGDEDIALOGCURVEBEZIERCLAMP_H_
#define _IGDEDIALOGCURVEBEZIERCLAMP_H_

#include "../dialog/igdeDialog.h"
#include "../igdeCheckBoxReference.h"
#include "../composed/igdeEditVector2Reference.h"



/**
 * \brief Dialog bezier curve clamp parameters.
 */
class igdeDialogCurveBezierClamp : public igdeDialog{
private:
	igdeCheckBoxReference pChkClamp;
	igdeEditVector2Reference pEditMinimum;
	igdeEditVector2Reference pEditMaximum;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialogCurveBezierClamp( igdeEnvironment &environment );
	
	/** \brief Create dialog. */
	igdeDialogCurveBezierClamp( igdeEnvironment &environment, bool clamp,
		const decVector2 &minimum, const decVector2 &maximum );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialogCurveBezierClamp();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Clamp. */
	bool GetClamp() const;
	
	/** \brief Set clamp. */
	void SetClamp( bool clamp );
	
	/** \brief Minimum clamp. */
	decVector2 GetClampMin() const;
	
	/** \brief Set minimum clamp. */
	void SetClampMin( const decVector2 &minimum );
	
	/** \brief Maximum clamp. */
	decVector2 GetClampMax() const;
	
	/** \brief Set maximum clamp. */
	void SetClampMax( const decVector2 &maximum );
	/*@}*/
	
	
	
private:
	void pCreateContent( igdeEnvironment &environment );
};

#endif
