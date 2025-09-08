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
	~meWPSObjectListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Trigger table changed. */
	void TriggerTableChanged( meWorld *world ) override;
	
	
	
	/** \brief Object selection changed. */
	void ObjectSelectionChanged( meWorld *world ) override;
	
	/** \brief Object changed. */
	void ObjectChanged( meWorld *world, meObject *object ) override;
	
	/** \brief Object class changed. */
	void ObjectClassChanged( meWorld *world, meObject *object ) override;
	
	/** \brief Object geometry changed. */
	void ObjectGeometryChanged( meWorld *world, meObject *object ) override;
	
	/** \brief Object properties changed. */
	void ObjectPropertiesChanged( meWorld *world, meObject *object ) override;
	
	/** \brief Object active property changed. */
	void ObjectActivePropertyChanged( meWorld *world, meObject *object ) override;
	
	/** Object attach behaviors changed. */
	void ObjectAttachBehaviorsChanged(meWorld *world, meObject *object) override;
	
	/** Object active attach behavior changed. */
	void ObjectActiveAttachBehaviorChanged(meWorld *world, meObject *object) override;
	
	/** \brief Object texture count changed. */
	void ObjectTextureCountChanged( meWorld *world, meObject *object ) override;
	
	/** \brief Object active texture changed. */
	void ObjectActiveTextureChanged( meWorld *world, meObject *object ) override;
	
	/** \brief Object texture changed. */
	void ObjectTextureChanged( meWorld *world, meObject *object, meObjectTexture *texture ) override;
	
	/** \brief Object texture properties changed. */
	void ObjectTexturePropertiesChanged( meWorld *world, meObject *object, meObjectTexture *texture ) override;
	
	/** \brief Object texture active property changed. */
	void ObjectTextureActivePropertyChanged( meWorld *world, meObject *object, meObjectTexture *texture ) override;
	/*@}*/
};

#endif

