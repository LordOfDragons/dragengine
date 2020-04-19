/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEWPVIEWLISTENER_H_
#define _SEWPVIEWLISTENER_H_

#include "../../skin/seSkinListener.h"

class seWPView;



/**
 * \brief View panel listener.
 */
class seWPViewListener : public seSkinListener{
private:
	seWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seWPViewListener( seWPView &panel );
	
	/** \brief Clean up listener. */
	virtual ~seWPViewListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Skin parameters changed. */
	virtual void SkinChanged( seSkin *skin );
	
	/** \brief The sky changed. */
	virtual void SkyChanged( seSkin *skin );
	
	/** \brief The environment component changed. */
	virtual void EnvObjectChanged( seSkin *skin );
	
	/** \brief The view changed. */
	virtual void ViewChanged( seSkin *skin );
	
	/** \brief The camera changed. */
	virtual void CameraChanged( seSkin *skin );
	/*@}*/
};

#endif
