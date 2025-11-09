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

#ifndef _MEWPADD_H_
#define _MEWPADD_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeComboBoxFilterReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/event/igdeActionReference.h>

class meWorld;
class meWPAddListener;
class meWindowProperties;



/**
 * \brief Add panel.
 */
class meWPAdd : public igdeContainerScroll{
private:
	meWindowProperties &pWindowProperties;
	meWPAddListener *pListener;
	
	meWorld *pWorld;
	
	igdeActionReference pActionClassAdd;
	igdeActionReference pActionClassRemove;
	igdeActionReference pActionClassClear;
	
	igdeCheckBoxReference pChkFilterObjects;
	igdeComboBoxFilterReference pComboObjClass;
	igdeListBoxReference pListObjClasses;
	igdeCheckBoxReference pChkObjInclusive;
	
	igdeCheckBoxReference pChkRandomizeYAxis;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	meWPAdd( meWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~meWPAdd();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Get world. */
	inline meWorld *GetWorld() const{ return pWorld; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief Update parameters. */
	void UpdateParameters();
	
	/** \brief Update object filter. */
	void UpdateObjectFilter();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief Actions. */
	inline igdeAction *GetActionClassAdd() const{ return pActionClassAdd; }
	inline igdeAction *GetActionClassRemove() const{ return pActionClassRemove; }
	inline igdeAction *GetActionClassClear() const{ return pActionClassClear; }
	/*@}*/
};

#endif
