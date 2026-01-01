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

#ifndef _MEWPSOBJECTSHAPELISTENER_H_
#define _MEWPSOBJECTSHAPELISTENER_H_

#include "../../../../world/meWorldNotifier.h"

class meWPSObjectShape;



/**
 * \brief Object Shape Panel Listener.
 */
class meWPSObjectShapeListener : public meWorldNotifier{
public:
	typedef deTObjectReference<meWPSObjectShapeListener> Ref;
	
private:
	meWPSObjectShape &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	meWPSObjectShapeListener(meWPSObjectShape &panel);
	
protected:
	/** \brief Clean up listener. */
	~meWPSObjectShapeListener() override;
	
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Object selection changed. */
	void ObjectSelectionChanged(meWorld *world) override;
	
	/** \brief Object class changed. */
	void ObjectClassChanged(meWorld *world, meObject *object) override;
	
	/** \brief Object geometry changed. */
	void ObjectGeometryChanged(meWorld *world, meObject *object) override;
	
	/** \brief Object properties changed. */
	void ObjectPropertiesChanged(meWorld *world, meObject *object) override;
	
	/** \brief Object active property changed. */
	void ObjectActivePropertyChanged(meWorld *world, meObject *object) override;
	
	/** \brief Object shape list changed. */
	void ObjectShapeListChanged(meWorld *world) override;
	
	/** \brief Object shape selection changed. */
	void ObjectShapeSelectionChanged(meWorld *world) override;
	/*@}*/
};

#endif
