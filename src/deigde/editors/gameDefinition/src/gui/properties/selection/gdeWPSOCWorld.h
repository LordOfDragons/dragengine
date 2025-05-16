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

#ifndef _GDEWPSOCWORLD_H_
#define _GDEWPSOCWORLD_H_

#include "../../../gamedef/objectClass/world/gdeOCWorld.h"

#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/composed/igdeEditVectorReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>

class gdeGameDefinition;
class gdeObjectClass;
class gdeWindowProperties;
class gdeWPSOCWorldListener;


/**
 * \brief Object class world property panel.
 */
class gdeWPSOCWorld : public igdeContainerScroll{
private:
	gdeWindowProperties &pWindowProperties;
	gdeWPSOCWorldListener *pListener;
	
	gdeGameDefinition *pGameDefinition;
	
	igdeEditPathReference pEditPath;
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditRotation;
	
	igdeComboBoxReference pCBPropertyNames;
	igdeComboBoxFilterReference pCBPropertyNameTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	gdeWPSOCWorld(gdeWindowProperties &windowMain);
	
protected:
	/** \brief Clean up panel. */
	~gdeWPSOCWorld() override;
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Game definition or nullptr if not set. */
	inline gdeGameDefinition *GetGameDefinition() const{ return pGameDefinition; }
	
	/** \brief Set game definition or nullptr if not set. */
	void SetGameDefinition(gdeGameDefinition *gameDefinition);
	
	
	/** \brief Active object class or nullptr if absent. */
	gdeObjectClass *GetObjectClass() const;
	
	/** \brief Active object class navigation space or nullptr if not set. */
	gdeOCWorld *GetWorld() const;
	
	/** \brief Selected property name. */
	const gdeOCWorld::eProperties GetPropertyName() const;
	
	
	/** \brief Update object class property list. */
	void UpdatePropertyList();
	
	
	/** \brief Update navigation space. */
	void UpdateWorld();
	
	/** \brief Update navigation space property name. */
	void UpdatePropertyName();
	/*@}*/
};

#endif
