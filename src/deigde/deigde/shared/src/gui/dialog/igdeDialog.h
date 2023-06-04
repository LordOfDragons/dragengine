/* 
 * Drag[en]gine IGDE
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _IGDEDIALOG_H_
#define _IGDEDIALOG_H_

#include <stddef.h>

#include "../igdeWindow.h"
#include "../igdeWidgetReference.h"
#include "../event/igdeAction.h"

#include <dragengine/common/string/decString.h>

class igdeWidget;
class igdeContainerReference;



/**
 * \brief IGDE UI Dialog.
 * 
 * Accepts one child widget which is usually of container type. Supports convenience
 * method to add content using a button bar at the bottom.
 */
class DE_DLL_EXPORT igdeDialog : public igdeWindow{
public:
	/**
	 * \brief Accept dialog action calling Accept() on dialog.
	 */
	class DE_DLL_EXPORT AcceptDialog : public igdeAction{
	private:
		igdeDialog &pDialog;
		
	public:
		AcceptDialog( igdeDialog &dialog, const char *text, igdeIcon *icon = NULL );
		virtual ~AcceptDialog();
		
		virtual void OnAction();
	};
	
	
	/**
	 * \brief Cancel dialog action calling Cancel() on dialog.
	 */
	class DE_DLL_EXPORT CancelDialog : public igdeAction{
	private:
		igdeDialog &pDialog;
		
	public:
		CancelDialog( igdeDialog &dialog, const char *text, igdeIcon *icon = NULL );
		virtual ~CancelDialog();
		
		virtual void OnAction();
	};
	
	
	
private:
	igdeWidget *pOwner;
	bool pAccepted;
	decPoint pInitialSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dialog. */
	igdeDialog( igdeEnvironment &environment, const char *title,
		igdeIcon *icon = NULL, bool canResize = true );
	
	
	
protected:
	/** \brief Clean up dialog. */
	virtual ~igdeDialog();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Initial size when shown or (0,0) to choose automatically. */
	inline const decPoint &GetInitialSize() const{ return pInitialSize; }
	
	/** \brief Set initial size when shown or (0,0) to choose automatically. */
	void SetInitialSize( const decPoint &initialSize );
	
	/**
	 * \brief Default action changed requiring dialog to update.
	 * 
	 * Call this method if you change the default value of actions attached to buttons
	 * in the dialog.
	 */
	virtual void DefaultActionChanged();
	
	
	
	/** \brief Add child. */
	virtual void AddChild( igdeWidget *child );
	
	
	
	/**
	 * \brief Add button bar with content above.
	 * 
	 * Adds an igdeWindowBorder widget as dialog content. Adds to the bottom area
	 * a container with a separator and a button bar. Adds to the center a container
	 * the content container. Content and button bar widget have to be created by the
	 * user. CreateButtonBar() can be used to create the button bar easily.
	 */
	void AddContent( igdeWidget *content, igdeWidget *buttonBar );
	
	/**
	 * \brief Add button bar with content above with header.
	 * 
	 * Adds an igdeWindowBorder widget as dialog content. Adds to the bottom area
	 * a container with a separator and a button bar. Adds to the center a container
	 * the content container. Adds the header to the top area. Content, header and
	 * button bar widget have to be created by the user. CreateButtonBar() can be
	 * used to create the button bar easily.
	 */
	void AddContent( igdeWidget *content, igdeWidget *header, igdeWidget *buttonBar );
	
	/**
	 * \brief Add button bar with content above with header and optional side panels.
	 * 
	 * Adds an igdeWindowBorder widget as dialog content. Adds to the bottom area
	 * a container with a separator and a button bar. Adds to the center a container
	 * the content container. Adds the header to the top area. If left and right side
	 * panels exist they are added to the left or right respectively. Content, header,
	 * side panels and button bar widget have to be created by the user. CreateButtonBar()
	 * can be used to create the button bar easily.
	 */
	void AddContent( igdeWidget *content, igdeWidget *header, igdeWidget *leftPanel,
		igdeWidget *rightPanel, igdeWidget *buttonBar );
	
	/**
	 * \brief Create button bar using actions.
	 */
	void CreateButtonBar( igdeContainerReference &buttonBar, igdeAction *action );
	
	void CreateButtonBar( igdeContainerReference &buttonBar,
		igdeAction *action1, igdeAction *action2 );
	
	void CreateButtonBar( igdeContainerReference &buttonBar,
		igdeAction *action1, igdeAction *action2, igdeAction *action3 );
	
	void CreateButtonBar( igdeContainerReference &buttonBar,
		igdeAction **actions, int actionCount );
	
	/**
	 * \brief Create button bar with action CancelDialog.
	 */
	void CreateButtonBar( igdeContainerReference &buttonBar, const char *text );
	
	/**
	 * \brief Create button bar with action AcceptDialog and CancelDialog.
	 */
	void CreateButtonBar( igdeContainerReference &buttonBar,
		const char *textAccept, const char *textCancel );
	
	
	
	/**
	 * \brief Run dialog modal.
	 * \returns \em true if dialog has been accepted or \em false if cancelled.
	 */
	bool Run( igdeWidget *owner );
	
	
	
	/**
	 * \brief Accept dialog.
	 * 
	 * Dialog has been accepted by the user. Subclass has to do the required actions
	 * then super call to continue closing. If the dialog has been successfully accepted
	 * and super call has been done \em true has to be returned. If accepting failed and
	 * the dialog should stay open \em false has to be returned.
	 * 
	 * Default implementation closes the dialog and returns \em true. Run() will exit
	 * with return value \em true.
	 */
	virtual bool Accept();
	
	/**
	 * \brief Close dialog.
	 * 
	 * Changes made by the user will be discarded. Dialog will be closed.
	 * 
	 * Default implementation closes the dialog and returns \em true. Run() will exit
	 * with return value \em false.
	 */
	virtual bool Cancel();
	
	/** \brief Close dialog and makes Run() exit with accepted value. */
	virtual void CloseDialog( bool accepted );
	
	
	
	/**
	 * \brief Init running.
	 * 
	 * Called by Run() after dialog is created. Subclass can implement to init controls
	 * after the dialog is visible on screen.
	 */
	virtual void OnDialogShown();
	
	/**
	 * \brief Game like frame update while running modal.
	 * 
	 * Called while the dialog is inside Run(). Can be used to do updating on a regular basis.
	 */
	virtual void OnFrameUpdate();
	/*@}*/
	
	
	
	/**
	 * \name IGDE Internal Use Only
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	/*@{*/
	/**
	 * \brief Create native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void CreateNativeWidget();
	
	/**
	 * \brief Destroy native widget.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void DestroyNativeWidget();
	/*@}*/
	
	
	
protected:
	void pAddContent( igdeWidget *content, igdeWidget *header, igdeWidget *leftPanel,
		igdeWidget *rightPanel, igdeWidget *buttonBar );
	
	/**
	 * \brief Window title changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnTitleChanged();
	
	/**
	 * \brief Window icon changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnIconChanged();
	
	/**
	 * \brief Window size changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnSizeChanged();
	
	/**
	 * \brief Window position changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	virtual void OnPositionChanged();
	
	/** \brief Visible changed. */
	virtual void OnVisibleChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	/*@}*/
};

#endif
