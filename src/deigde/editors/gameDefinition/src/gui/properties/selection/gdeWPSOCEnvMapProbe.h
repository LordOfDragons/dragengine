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

#ifndef _GDEWPSOCENVMAPPROBE_H_
#define _GDEWPSOCENVMAPPROBE_H_

#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeOCEnvMapProbe;
class gdeWindowProperties;
class gdeWPSOCEnvMapProbeListener;



/**
 * \brief Object class environment map probe property panel.
 */
class gdeWPSOCEnvMapProbe : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCEnvMapProbeListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	igdeEditVectorReference pEditScaling;
	
	igdeTextFieldReference pEditShapeInfluence;
	igdeTextFieldReference pEditShapeReflection;
	igdeTextFieldReference pEditShapeReflectionMask;
	igdeTextFieldReference pEditInfluenceBorderSize;
	igdeTextFieldReference pEditInfluencePriority;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCEnvMapProbe( gdeWindowProperties &windowMain );
	
protected:
	/** \brief Clean up panel. */
	virtual ~gdeWPSOCEnvMapProbe();
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
	
	/** \brief Active object class environment map probe or \em NULL if not set. */
	gdeOCEnvMapProbe *GetEnvMapProbe() const;
	
	/** \brief Selected property name. */
	const gdeOCEnvMapProbe::eProperties GetPropertyName() const;
	
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	
	/** \brief Update environment map probe. */
	void UpdateEnvMapProbe();
	
	/** \brief Update environment map probe property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
