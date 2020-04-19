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

#ifndef _IGDENATIVEFOXTIMER_H_
#define _IGDENATIVEFOXTIMER_H_

#include "foxtoolkit.h"


class igdeTimer;


/**
 * \brief FOX toolkit Native Timer.
 */
class igdeNativeFoxTimer : public FXObject{
	FXDECLARE( igdeNativeFoxTimer )
protected:
	   igdeNativeFoxTimer();
	
public:
	enum eFoxIDs{
		ID_TIMEOUT,
		ID_LAST
	};
	
private:
	FXApp *pApp;
	igdeTimer *pOwner;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create timer. */
	igdeNativeFoxTimer( igdeTimer &owner, FXApp *app );
	
	/** \brief Clean up timer. */
	virtual ~igdeNativeFoxTimer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Start timer. */
	virtual void StartTimer();
	
	/** \brief Stop timer. */
	virtual void StopTimer();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	long onTimeout( FXObject *sender, FXSelector selector, void *data );
	/*@}*/
};

#endif
