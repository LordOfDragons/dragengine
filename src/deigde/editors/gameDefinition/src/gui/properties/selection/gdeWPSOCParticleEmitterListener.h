/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#ifndef _GDEWPSOCPARTICLEEMITTERLISTENER_H_
#define _GDEWPSOCPARTICLEEMITTERLISTENER_H_

#include "../../../gamedef/gdeGameDefinitionListener.h"

class gdeWPSOCParticleEmitter;



/**
 * \brief Object class particle emitter panel listener.
 */
class gdeWPSOCParticleEmitterListener : public gdeGameDefinitionListener{
private:
	gdeWPSOCParticleEmitter &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	gdeWPSOCParticleEmitterListener( gdeWPSOCParticleEmitter &panel );
	
	/** \brief Clean up listener. */
	virtual ~gdeWPSOCParticleEmitterListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object property name changed. */
	virtual void OCPropertyNameChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeProperty *property );
	
	/** \brief Object properties changed. */
	virtual void OCPropertiesChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass );
	
	/** \brief Active object class changed. */
	virtual void ActiveObjectClassChanged( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Object class particle emitters changed. */
	virtual void OCParticleEmittersChanged( gdeGameDefinition *gameDefinition, gdeObjectClass *objectClass );
	
	/** \brief Object class particle emitter changed. */
	virtual void OCParticleEmitterChanged( gdeGameDefinition *gameDefinition,
		gdeObjectClass *objectClass, gdeOCParticleEmitter *emitter );
	
	/** \brief Active object class particle emitter changed. */
	virtual void ActiveOCParticleEmitterChanged( gdeGameDefinition *gameDefinition );
	/*@}*/
};

#endif
