/* 
 * Drag[en]gine IGDE Speech Animation Editor
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

#ifndef _SAEWPVIEWLISTENER_H_
#define _SAEWPVIEWLISTENER_H_

#include "../../sanimation/saeSAnimationListener.h"

class saeWPView;



/**
 * \brief View panel listener.
 */
class saeWPViewListener : public saeSAnimationListener{
private:
	saeWPView &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	saeWPViewListener( saeWPView &panel );
	
	/** \brief Clean up listener. */
	virtual ~saeWPViewListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Speech animation changed. */
	virtual void SAnimationChanged( saeSAnimation *sanimation );
	
	/** \brief Model changed. */
	virtual void ModelChanged( saeSAnimation *sanimation );
	
	/** \brief Sky changed. */
	virtual void SkyChanged( saeSAnimation *sanimation );
	
	/** \brief Camera changed. */
	virtual void CameraChanged( saeSAnimation *sanimation );
	/*@}*/
};

#endif
