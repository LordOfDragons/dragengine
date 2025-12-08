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

#ifndef _MEWINDOWCHANGELOG_H_
#define _MEWINDOWCHANGELOG_H_

#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/igdeIconListBox.h>

class meWindowMain;
class meWindowChangelogListener;
class meWCEntry;
class meWorld;



/**
 * \brief Changelog Window.
 */
class meWindowChangelog : public igdeContainerBorder{
private:
	meWindowMain &pWindowMain;
	meWindowChangelogListener::Ref pListener;
	
	meWorld::Ref pWorld;
	
	igdeIconListBox::Ref pListChanges;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	meWindowChangelog(meWindowMain &windowMain);
	
protected:
	/** \brief Clean up window. */
	virtual ~meWindowChangelog();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline meWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Set world. */
	void SetWorld(meWorld *world);
	
	/** \brief Update changelog list. */
	void UpdateChangelog();
	
	/** \brief Save entry. */
	void SaveEntry(meWCEntry *entry);
	
	/** \brief Save all entries. */
	void SaveAllEntries();
	/*@}*/
};

#endif
