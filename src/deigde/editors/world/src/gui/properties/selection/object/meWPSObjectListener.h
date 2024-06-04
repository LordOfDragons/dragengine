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

#ifndef _MEWPSOBJECTLISTENER_H_
#define _MEWPSOBJECTLISTENER_H_

#include "../../../../world/meWorldNotifier.h"

class meWPSObject;



/**
 * \brief Object panel listener.
 */
class meWPSObjectListener : public meWorldNotifier{
private:
	meWPSObject &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPSObjectListener( meWPSObject &panel );
	
	/** \brief Clean up listener. */
	virtual ~meWPSObjectListener();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Trigger table changed. */
	virtual void TriggerTableChanged( meWorld *world );
	
	
	
	/** \brief Object selection changed. */
	virtual void ObjectSelectionChanged( meWorld *world );
	
	/** \brief Object changed. */
	virtual void ObjectChanged( meWorld *world, meObject *object );
	
	/** \brief Object class changed. */
	virtual void ObjectClassChanged( meWorld *world, meObject *object );
	
	/** \brief Object geometry changed. */
	virtual void ObjectGeometryChanged( meWorld *world, meObject *object );
	
	/** \brief Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object );
	
	/** \brief Object active property changed. */
	virtual void ObjectActivePropertyChanged( meWorld *world, meObject *object );
	
	/** \brief Object texture count changed. */
	virtual void ObjectTextureCountChanged( meWorld *world, meObject *object );
	
	/** \brief Object active texture changed. */
	virtual void ObjectActiveTextureChanged( meWorld *world, meObject *object );
	
	/** \brief Object texture changed. */
	virtual void ObjectTextureChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	
	/** \brief Object texture properties changed. */
	virtual void ObjectTexturePropertiesChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	
	/** \brief Object texture active property changed. */
	virtual void ObjectTextureActivePropertyChanged( meWorld *world, meObject *object, meObjectTexture *texture );
	/*@}*/
};

#endif
