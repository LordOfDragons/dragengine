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


#ifndef _GDEUOCENVMAPPROBESETPROPERTYNAME_H_
#define _GDEUOCENVMAPPROBESETPROPERTYNAME_H_

#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <deigde/undo/igdeUndo.h>

class gdeObjectClass;



/**
 * \brief Undo action object class environment map probe set property name.
 */
class gdeUOCEnvMapProbeSetPropertyName : public igdeUndo{
private:
	gdeObjectClass *pObjectClass;
	gdeOCEnvMapProbe *pEnvMapProbe;
	
	gdeOCEnvMapProbe::eProperties pProperty;
	decString pOldValue;
	decString pNewValue;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo action. */
	gdeUOCEnvMapProbeSetPropertyName( gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe,
		gdeOCEnvMapProbe::eProperties property, const char *newValue );
	
protected:
	/** \brief Clean up undo action. */
	virtual ~gdeUOCEnvMapProbeSetPropertyName();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif
