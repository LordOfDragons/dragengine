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

#ifndef _IGDENVNODE_H_
#define _IGDENVNODE_H_

#include "../igdeContainer.h"
#include "../igdeWidgetReference.h"

#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class igdeNVBoard;
class igdeNVSlot;
class igdeNVNodeListener;


/**
 * \brief IGDE UI NodeView Node.
 */
class DE_DLL_EXPORT igdeNVNode : public igdeContainer{
private:
	decString pTitle;
	decString pDescription;
	bool pEnabled;
	bool pActive;
	
	decColor pBgColor;
	decColor pBorderColor;
	decColor pActiveTitleBgColor;
	decColor pInactiveTitleBgColor;
	
	igdeNVBoard *pOwnerBoard;
	decPoint pPosition;
	
	decObjectOrderedSet pSlots;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create checkbox. */
	igdeNVNode( igdeEnvironment &environment, const char *title, const char *description = "" );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeNVNode();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Title. */
	inline const decString &GetTitle() const{ return pTitle; }
	
	/** \brief Set title. */
	void SetTitle( const char *title );
	
	/** \brief Description shown in tool tips. */
	inline const decString &GetDescription() const{ return pDescription; }
	
	/** \brief Set description shown in tool tips. */
	void SetDescription( const char *description );
	
	/** \brief Node is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	/** \brief Node is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if button is active. */
	void SetActive( bool active );
	
	
	
	/** \brief Background color. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Set background color. */
	void SetBgColor( const decColor &color );
	
	/** \brief Border color. */
	inline const decColor &GetBorderColor() const{ return pBorderColor; }
	
	/** \brief Set border color. */
	void SetBorderColor( const decColor &color );
	
	/** \brief Active title background color. */
	inline const decColor &GetActiveTitleBgColor() const{ return pActiveTitleBgColor; }
	
	/** \brief Set active title background color. */
	void SetActiveTitleBgColor( const decColor &color );
	
	/** \brief Inactive title background color. */
	inline const decColor &GetInactiveTitleBgColor() const{ return pInactiveTitleBgColor; }
	
	/** \brief Set inactive title background color. */
	void SetInactiveTitleBgColor( const decColor &color );
	
	
	
	/** \brief Position. */
	inline const decPoint &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decPoint &position );
	
	/** \brief Size. */
	decPoint GetSize() const;
	
	/** \brief Owner board or NULL. */
	inline igdeNVBoard *GetOwnerBoard() const{ return pOwnerBoard; }
	
	/**
	 * \brief Set owner board or NULL.
	 * \warning For use by \em igdeNVBoard only!
	 */
	void SetOwnerBoard( igdeNVBoard *board );
	
	/** \brief Board offset changed. */
	virtual void OnBoardOffsetChanged();
	
	
	
	/** \brief Number of slots. */
	int GetSlotCount() const;
	
	/** \brief Slot at index. */
	igdeNVSlot *GetSlotAt( int index ) const;
	
	/** \brief Index of slot. */
	int IndexOfSlot( igdeNVSlot *slot ) const;
	
	/** \brief Has slot. */
	bool HasSlot( igdeNVSlot *slot ) const;
	
	/** \brief Add slot. */
	void AddSlot( igdeNVSlot *slot );
	
	/** \brief Remove slot. */
	void RemoveSlot( igdeNVSlot *slot );
	
	/** \brief Remove all slots. */
	void RemoveAllSlots();
	
	/** \brief Show context menu at position. */
	void ShowContextMenu( const decPoint &position );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeNVNodeListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeNVNodeListener *listener );
	
	/** \brief Notify listeners node has been activated. */
	virtual void NotifyActivated();
	
	/** \brief Notify listeners node has been deactivated. */
	virtual void NotifyDeactivated();
	
	/** \brief Notify listeners dragging begins. */
	virtual void NotifyDragBegin();
	
	/** \brief Notify listeners dragging in progress changing. */
	virtual void NotifyDraging();
	
	/** \brief Notify listeners dragging ended. */
	virtual void NotifyDragEnd();
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
	
	
	
protected:
	/** \brief Title changed. */
	virtual void OnTitleChanged();
	
	/** \brief Description changed. */
	virtual void OnDescriptionChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Active changed. */
	virtual void OnActiveChanged();
	
	/** \brief Colors changed. */
	virtual void OnColorsChanged();
	
	/** \brief Slots changed. */
	virtual void OnSlotsChanged();
	
	/** \brief Position changed. */
	virtual void OnPositionChanged();
	/*@}*/
};

#endif
