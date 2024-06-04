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

#ifndef _LPELANGPACKLISTENER_H_
#define _LPELANGPACKLISTENER_H_

#include <dragengine/deObject.h>

class lpeLangPack;
class lpeLangPackEntry;



/**
 * Language pack listener.
 */
class lpeLangPackListener : public deObject{
public:
	typedef deTObjectReference<lpeLangPackListener> Ref;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new langpack listener. */
	lpeLangPackListener();
	
protected:
	/** Cleans up the listener. */
	virtual ~lpeLangPackListener() override;
	/*@}*/
	
	
	
	
public:
	/** \name Notifications */
	/*@{*/
	/** Changed or saved state changed. */
	virtual void StateChanged( lpeLangPack *langpack );
	
	/** Undo changed. */
	virtual void UndoChanged( lpeLangPack *langpack );
	
	/** Language pack parameters changed. */
	virtual void LangPackChanged( lpeLangPack *langpack );
	
	/** Entries have been added or removed. */
	virtual void EntryStructureChanged ( lpeLangPack *langpack );
	
	/** An entry changed. */
	virtual void EntryChanged( lpeLangPack *langpack, lpeLangPackEntry *entry );
	
	/** An entry name changed. */
	virtual void EntryNameChanged( lpeLangPack *langpack, lpeLangPackEntry *entry );
	
	/** Selection of one or mote entries changed. */
	virtual void EntrySelectionChanged( lpeLangPack *langpack );
	
	/** Active entry changed. */
	virtual void ActiveEntryChanged( lpeLangPack *langpack );
	/*@}*/
};

#endif
