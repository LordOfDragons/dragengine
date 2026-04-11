/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _MEPREVIEWCAMERALISTENER_H_
#define _MEPREVIEWCAMERALISTENER_H_

#include "../../world/meWorldNotifier.h"

class mePreviewCamera;


/**
 * \brief Preview Camera Listener
 */
class mePreviewCameraListener : public meWorldNotifier{
public:
	using Ref = deTObjectReference<mePreviewCameraListener>;
	
private:
	mePreviewCamera &pPreviewCamera;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	mePreviewCameraListener(mePreviewCamera &previewCamera);
	
protected:
	/** \brief Clean up listener. */
	~mePreviewCameraListener() override;
	
public:
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Editing parameters changed. */
	void EditingChanged(meWorld *world) override;
	
	/** Object added. */
	void ObjectAdded(meWorld *world, meObject *object) override;
	
	/** Object removed. */
	void ObjectRemoved(meWorld *world, meObject *object) override;
	
	/** Object class changed. */
	void ObjectClassChanged(meWorld *world, meObject *object) override;
	
	/** Object properties changed. */
	void ObjectPropertiesChanged(meWorld *world, meObject *object) override;
	
	/** Object camera show preview changed. */
	void ObjectCameraShowPreview(meWorld *world, meObject *object) override;
	/*@}*/
};

#endif
