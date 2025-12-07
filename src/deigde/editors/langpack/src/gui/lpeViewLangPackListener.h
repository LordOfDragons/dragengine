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

#ifndef _LPEVIEWLANGPACKLISTENER_H_
#define _LPEVIEWLANGPACKLISTENER_H_

#include "../langpack/lpeLangPackListener.h"

class lpeViewLangPack;



/**
 * \brief Language pack view listener.
 */
class lpeViewLangPackListener : public lpeLangPackListener{
private:
	lpeViewLangPack &pPanel;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	lpeViewLangPackListener(lpeViewLangPack &window);
	
	/** \brief Clean up listener. */
	~lpeViewLangPackListener();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Language pack parameters changed. */
	virtual void LangPackChanged(lpeLangPack *langpack);
	
	
	
	/** \brief Entries have been added or removed. */
	virtual void EntryStructureChanged (lpeLangPack *langpack);
	
	/** \brief An entry changed. */
	virtual void EntryChanged(lpeLangPack *langpack, lpeLangPackEntry *entry);
	
	/** \brief An entry name changed. */
	virtual void EntryNameChanged(lpeLangPack *langpack, lpeLangPackEntry *entry);
	
	/** \brief Selection of one or mote entries changed. */
	virtual void EntrySelectionChanged(lpeLangPack *langpack);
	
	/** \brief Active entry changed. */
	virtual void ActiveEntryChanged(lpeLangPack *langpack);
	/*@}*/
};

#endif
