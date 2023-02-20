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

#ifndef _IGDEWOSOVISITOR_H_
#define _IGDEWOSOVISITOR_H_

#include <dragengine/dragengine_export.h>


class igdeWOSubObject;
class igdeWOSOBillboard;
class igdeWOSOLight;
class igdeWOSOSpeaker;
class igdeWOSOParticleEmitter;
class igdeWOSOForceField;
class igdeWOSOEnvMapProbe;
class igdeWOSONavigationSpace;
class igdeWOSONavigationBlocker;
class igdeWOSOComponent;


/**
 * \brief Object wrapper sub object visitor.
 */
class DE_DLL_EXPORT igdeWOSOVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object wrapper sub object visitor. */
	igdeWOSOVisitor();
	
	/** \brief Clean up object wrapper sub object. */
	virtual ~igdeWOSOVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit sub object. */
	virtual void VisitSubObject( igdeWOSubObject &subObject );
	
	/** \brief Visit billboard. */
	virtual void VisitBillboard( igdeWOSOBillboard &billboard );
	
	/** \brief Visit light. */
	virtual void VisitLight( igdeWOSOLight &light );
	
	/** \brief Visit speaker. */
	virtual void VisitSpeaker( igdeWOSOSpeaker &speaker );
	
	/** \brief Visit particle emitter. */
	virtual void VisitParticleEmitter( igdeWOSOParticleEmitter &particleEmitter );
	
	/** \brief Visit particle emitter. */
	virtual void VisitForceField( igdeWOSOForceField &forceField );
	
	/** \brief Visit environment map probe. */
	virtual void VisitEnvMapProbe( igdeWOSOEnvMapProbe &envMapProbe );
	
	/** \brief Visit navigation space. */
	virtual void VisitNavigationSpace( igdeWOSONavigationSpace &navigationSpace );
	
	/** \brief Visit navigation blocker. */
	virtual void VisitNavigationBlocker( igdeWOSONavigationBlocker &navigationBlocker );
	
	/** \brief Visit component. */
	virtual void VisitComponent( igdeWOSOComponent &component );
	/*@}*/
};

#endif
