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

#ifndef _MEWINDOWPROPERTIES_H_
#define _MEWINDOWPROPERTIES_H_

#include <deigde/gui/igdeTabBook.h>
#include <deigde/gui/properties/igdeWPUndoHistory.h>

class meWorld;
class meWindowMain;
class meWPSelection;
class meWPView;
class meWPSensors;
class meWPWorld;
class meWPHeightTerrain;
class meWPHeightTerrain;
class meWPBrowser;
class meWPAdd;



/**
 * \brief Shows Properties.
 */
class meWindowProperties : public igdeTabBook{
private:
	meWindowMain &pWindowMain;
	
	meWPSelection *pPropSelection;
	meWPView *pPropView;
	meWPWorld *pPropWorld;
	meWPSensors *pPropSensors;
	meWPHeightTerrain *pPropHT;
	igdeWPUndoHistory::Ref pPanelUndoHistory;
	meWPBrowser *pPropBrowser;
	meWPAdd *pPropAdd;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	meWindowProperties( meWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~meWindowProperties();
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set world. */
	void SetWorld( meWorld *world );
	
	/** \brief World path changed. */
	void OnWorldPathChanged();
	
	/** \brief Game project changed. */
	void OnGameProjectChanged();
	
	/** \brief Game project game definition changed. */
	void OnGameDefinitionChanged();
	
	/** \brief Browser panel. */
	inline meWPBrowser &GetBrowser() const{ return *pPropBrowser; }
	
	/** \brief Switch to browser panel. */
	void SwitchToBrowser();
};

#endif
