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

#ifndef _IGDENVBOARD_H_
#define _IGDENVBOARD_H_

#include "../igdeContainer.h"
#include "../igdeWidgetReference.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/collection/decObjectOrderedSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/string/decString.h>


class igdeNVNode;
class igdeNVLink;
class igdeNVSlot;
class igdeNVBoardListener;


/**
 * \brief IGDE UI NodeView Board.
 */
class DE_DLL_EXPORT igdeNVBoard : public igdeContainer{
private:
	decColor pBgColor;
	bool pEnabled;
	decPoint pOffset;
	
	decObjectOrderedSet pNodes;
	decObjectOrderedSet pLinks;
	deObjectReference pActiveNode;
	
	decObjectOrderedSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create checkbox. */
	igdeNVBoard( igdeEnvironment &environment );
	
	
	
protected:
	/**
	 * \brief Clean up widget.
	 * \note Subclasses should set their destructor protected too to avoid users
	 *       accidently deleting a reference counted object through the object
	 *       pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~igdeNVBoard();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Background color. */
	inline const decColor &GetBgColor() const{ return pBgColor; }
	
	/** \brief Set background color. */
	void SetBgColor( const decColor &color );
	
	/** \brief Button is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if button is enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Size. */
	decPoint GetSize() const;
	
	/** \brief Offset of board content relative to board center. */
	inline const decPoint &GetOffset() const{ return pOffset; }
	
	/** \brief Set offset of board content relative to board center. */
	void SetOffset( const decPoint &offset );
	
	/** \brief Clear board. */
	void Clear();
	
	
	
	/** \brief Number of nodes. */
	int GetNodeCount() const;
	
	/** \brief Node at index. */
	igdeNVNode *GetNodeAt( int index ) const;
	
	/** \brief Has node. */
	bool HasNode( igdeNVNode *node ) const;
	
	/** \brief Add node. */
	void AddNode( igdeNVNode *node );
	
	/** \brief Remove node. */
	void RemoveNode( igdeNVNode *node );
	
	/** \brief Remove all nodes. */
	void RemoveAllNodes();
	
	/** \brief Active node. or NULL */
	inline igdeNVNode *GetActiveNode() const{ return ( igdeNVNode* )( deObject* )pActiveNode; }
	
	/** \brief Set active node or NULL. */
	void SetActiveNode( igdeNVNode *node );
	
	
	
	/** \brief Number of links. */
	int GetLinkCount() const;
	
	/** \brief Link at index. */
	igdeNVLink *GetLinkAt( int index ) const;
	
	/** \brief Link between source and target or NULL if not linked. */
	igdeNVLink *GetLinkBetween( igdeNVSlot *source, igdeNVSlot *target ) const;
	
	/** \brief Has link. */
	bool HasLink( igdeNVLink *link ) const;
	
	/** \brief Has link between source and target. */
	bool HasLinkBetween( igdeNVSlot *source, igdeNVSlot *target ) const;
	
	/** \brief Can link. */
	bool CanLink( igdeNVSlot *source, igdeNVSlot *target );
	
	/** \brief Add link. */
	igdeNVLink *AddLink( igdeNVSlot *source, igdeNVSlot *target );
	
	/** \brief Remove link. */
	void RemoveLink( igdeNVLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Remove all links with sockets belonging to node. */
	void RemoveAllNodeLinks( igdeNVNode *node );
	
	/** \brief Link closest to position inside range in board coordinate system or NULL. */
	igdeNVLink *ClosestLinkNear( const decPoint &position, float range = 6.0f ) const;
	
	/** \brief Show context menu at position. */
	void ShowContextMenu( const decPoint &position );
	
	
	
	/** \brief Add listener. */
	void AddListener( igdeNVBoardListener *listener );
	
	/** \brief Remove listener. */
	void RemoveListener( igdeNVBoardListener *listener );
	
	/** \brief Notify link added. */
	void NotifyLinkAdded( igdeNVLink *link );
	
	/** \brief Notify link removed. */
	void NotifyLinkRemoved( igdeNVSlot *source, igdeNVSlot *target );
	
	/** \brief Notify all links removed. */
	void NotifyAllLinksRemoved();
	
	/** \brief Notify board offset changed. */
	void NotifyOffsetChanged();
	
	/** \brief Notify board size changed. */
	void NotifySizeChanged();
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
	
	/**
	 * \brief Notify nodes board offset changed.
	 * \warning IGDE Internal Use Only. Do not use.
	 */
	void NotifyNodesOffsetChanged();
	
	
	
protected:
	/** \brief Colors changed. */
	virtual void OnColorsChanged();
	
	/** \brief Enabled changed. */
	virtual void OnEnabledChanged();
	
	/** \brief Offset changed. */
	virtual void OnOffsetChanged();
	
	/** \brief Nodes changed. */
	virtual void OnNodesChanged();
	
	/** \brief Links changed. */
	virtual void OnLinksChanged();
	/*@}*/
};

#endif
