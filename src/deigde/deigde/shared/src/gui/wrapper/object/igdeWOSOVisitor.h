/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
