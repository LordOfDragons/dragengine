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

#ifndef _DEGLB_PANELENGINE_H_
#define _DEGLB_PANELENGINE_H_

#include <View.h>
#include <ListView.h>
#include <ScrollView.h>
#include <ColumnListView.h>
#include <ColumnTypes.h>

#include <delauncher/engine/modules/delEngineModule.h>

class deglbWindowMain;


/**
 * \brief Engine Panel.
 */
class deglbPanelEngine : public BView{
public:
	/** \brief Module list item. */
	class cModuleListItem : public BRow{
	public:
		delEngineModule::Ref module;
		
		cModuleListItem(delEngineModule *module);
		~cModuleListItem() override = default;
	};
	
	enum eMessages{
		MSG_CONTEXT_PROPS = 'pecp',
		MSG_CONTEXT_PROPS_DONE = 'pecP'
	};
	
	
private:
	deglbWindowMain *pWindowMain;
	BColumnListView *pListModules;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	deglbPanelEngine(deglbWindowMain *windowMain);
	
	/** \brief Clean up panel. */
	~deglbPanelEngine() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline deglbWindowMain *GetWindowMain() const{ return pWindowMain; }
	
	/** \brief Selected module or nullptr. */
	delEngineModule *GetSelectedModule() const;
	
	/** \brief Update module list. */
	void UpdateModuleList();
	/*@}*/
	
	
	
	/** \name BView */
	/*@{*/
	void MessageReceived(BMessage *message) override;
	
	void OnListContextMenu(cModuleListItem *item, const BPoint &where);
	void OnListInvoke(cModuleListItem *item);
	/*@}*/
	
	
	
private:
	void pShowContextMenu(BPoint screenWhere);
};

#endif
