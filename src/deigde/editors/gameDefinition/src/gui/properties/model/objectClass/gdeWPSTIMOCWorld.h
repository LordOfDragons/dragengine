/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _GDEWPSTIMOCWORLD_H_
#define _GDEWPSTIMOCWORLD_H_

#include "../../../../gamedef/objectClass/world/gdeOCWorld.h"

#include "gdeWPSTIMOCSubObject.h"


/**
 * \brief Object class world.
 */
class gdeWPSTIMOCWorld : public gdeWPSTIMOCSubObject{
private:
	const gdeOCWorld::Ref pWorld;
	
	
public:
	/** \brief Constructors and Destructors */
	/*@{*/
	/** \brief Create new tree item model. */
	gdeWPSTIMOCWorld(gdeWPSTreeModel &tree, gdeObjectClass *objectClass,
		gdeOCWorld *world, int index);
	
protected:
	/** \brief Clean up tree item model. */
	~gdeWPSTIMOCWorld() override = default;
	/*@}*/
	
	
	
public:
	/** \brief Management */
	/*@{*/
	/** \brief Navigation space. */
	inline const gdeOCWorld::Ref &GetOCWorld() const{ return pWorld; }
	
	/** \brief Validate and update state accordingly. */
	void Validate();
	
	/** \brief Verify if valid. */
	bool IsValid() const override;
	
	
	
	/** \brief Added to tree. */
	void OnAddedToTree() override;
	
	/** \brief User selected item. */
	void OnSelected() override;
	
	/** \brief User requests context menu for selected item. */
	void OnContextMenu(igdeMenuCascade &contextMenu) override;
	/*@}*/
};

#endif
