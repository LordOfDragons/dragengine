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

#ifndef _GDEWPSOCPARTICLEEMITTER_H_
#define _GDEWPSOCPARTICLEEMITTER_H_

#include "../../../gamedef/objectClass/particleemitter/gdeOCParticleEmitter.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeWindowProperties;
class gdeWPSOCParticleEmitterListener;



/**
 * \brief Object class particle emitter property panel.
 */
class gdeWPSOCParticleEmitter : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCParticleEmitterListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditPathReference pEditPath;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeTextFieldReference pEditBoneName;
	igdeCheckBoxReference pChkCasting;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	igdeComboBoxReference pCBTriggerNames;
	igdeComboBoxFilterReference pCBTriggerNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCParticleEmitter( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCParticleEmitter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or \em NULL if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or \em NULL if not set. */
	void SetGameDefinition( gdeGameDefinition *gameDefinition );
	
	
	
	/** \brief Active object class or \em NULL if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class particle emitter or \em NULL if not set. */
	gdeOCParticleEmitter *GetParticleEmitter() const;
	
	/** \brief Selected property name. */
	const gdeOCParticleEmitter::eProperties GetPropertyName() const;
	
	/** \brief Selected property name. */
	const gdeOCParticleEmitter::eTriggers GetTriggerName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update particle emitter. */
	void UpdateParticleEmitter();
	
	/** \brief Update light property name. */
	void UpdatePropertyName();
	
	/** \brief Update light trigger name. */
	void UpdateTriggerName();
	/*@}*/
};

#endif
