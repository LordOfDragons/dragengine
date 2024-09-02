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

#ifndef _MEWPWORLDLISTENER_H_
#define _MEWPWORLDLISTENER_H_

#include "../../world/meWorldNotifier.h"

class meWPWorld;



/**
 * World panel listener
 */
class meWPWorldListener : public meWorldNotifier{
private:
	meWPWorld &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create listener. */
	meWPWorldListener( meWPWorld &panel );
	
	/** Clean up listener. */
	virtual ~meWPWorldListener() override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** World parameters changed. */
	virtual void WorldParametersChanged( meWorld *world ) override;
	
	/** Lighting parameters changed. */
	virtual void LightingChanged( meWorld *world ) override;
	
	/** Path find test changed. */
	virtual void PathFindTestChanged( meWorld *world ) override;
	
	/** Music changed. */
	virtual void MusicChanged( meWorld *world ) override;
	
	/** World properties changed. */
	virtual void PropertiesChanged( meWorld *world ) override;
	
	/** World active property changed. */
	virtual void ActivePropertyChanged( meWorld *world ) override;
	
	/** Object properties changed. */
	virtual void ObjectPropertiesChanged( meWorld *world, meObject *object ) override;
	
	/** Object texture properties changed. */
	virtual void ObjectTexturePropertiesChanged( meWorld *world, meObject *object,
		meObjectTexture *texture ) override;
	/*@}*/
};

#endif
