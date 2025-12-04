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

#ifndef _IGDEWPWOBJECT_H_
#define _IGDEWPWOBJECT_H_

#include <dragengine/common/math/decMath.h>

#include "../igdeButtonReference.h"
#include "../igdeCheckBoxReference.h"
#include "../igdeContainerReference.h"
#include "../igdeTextFieldReference.h"
#include "../composed/igdeEditVectorReference.h"
#include "../event/igdeActionListener.h"
#include "../event/igdeAction::Ref.h"
#include "../layout/igdeContainerFlow.h"
#include "../wrapper/igdeWObject.h"
#include "../../gamedefinition/class/igdeGDClass.h"
#include "../../undo/igdeUndo.h"
#include "../../undo/igdeUndoSystem.h"


/**
 * \brief Window properties wrapper object.
 * 
 * Provides support for configurating a wrapper object. This panel is supposed to be used
 * on a properties window typically inside a group box. An object wrapper is useful for
 * editors to provide a static environments or adding objects to an environment to test
 * features of whatever they are editing. The object wrapper makes it easy to setup an
 * object using a game definition object class. In addition properties of the wrapped object
 * can be set to configured certain object features. To use this class assign an wrapper
 * object. Whenever a change is made the set action listener is called. The object wrapper
 * object is not held in any way. The application is responsible to ensure the wrapper
 * object exists as long as it is assigned to this widget.
 */
class DE_DLL_EXPORT igdeWPWObject : public igdeContainerFlow, igdeActionListener{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<igdeWPWObject> Ref;
	
	/** \brief Undo changes done by igdeWPWObject. */
	class DE_DLL_EXPORT UndoChanges : public igdeUndo{
	public:
		/** \brief Type holding strong reference. */
		typedef deTObjectReference<UndoChanges> Ref;
		
		
	private:
		const igdeWObject::Ref pObject;
		
		igdeGDClass::Ref pGDClassOld, pGDClassNew;
		decDVector pPositionOld, pPositionNew;
		decQuaternion pOrientationOld, pOrientationNew;
		decVector pScalingOld, pScalingNew;
		
		bool pVisibleOld, pVisibleNew;
		bool pDynamicColliderOld, pDynamicColliderNew;
		
		
	public:
		/** \brief Create undo. */
		UndoChanges(igdeWObject *object);
		
	protected:
		/** \brief Clean up undo. */
		~UndoChanges() override = default;
		
		
	public:
		/** \brief Set new value. */
		void SetGDClassNew(igdeGDClass *gdClass);
		void SetPositionNew(const decDVector &position);
		void SetOrientationNew(const decQuaternion &orientation);
		void SetScalingNew(const decVector &scaling);
		void SetVisibleNew(bool visible);
		void SetDynamicColliderNew(bool dynamicCollider);
		
		/** \brief Undo. */
		void Undo() override;
		
		/** \brief Redo. */
		void Redo() override;
	};
	
	
private:
	igdeWObject::Ref pObject;
	
	igdeTextFieldReference pEditClass;
	igdeButtonReference pBtnClass;
	
	igdeEditVectorReference pEditPosition;
	igdeEditVectorReference pEditOrientation;
	igdeEditVectorReference pEditScaling;
	
	igdeCheckBoxReference pChkVisible;
	igdeCheckBoxReference pChkDynamicCollider;
	
	igdeAction::Ref pAction;
	igdeUndoSystem *pUndoSystem;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	igdeWPWObject( igdeEnvironment &environment );
	
	/** \brief Create panel. */
	igdeWPWObject( igdeEnvironment &environment, igdeAction *action );
	
	
protected:
	/** \brief Clean up panel. */
	virtual ~igdeWPWObject();
	/*@}*/
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Object or NULL if not set. */
	inline const igdeWObject::Ref &GetObject() const{ return pObject; }
	
	/** \brief Set object or NULL to clear. */
	void SetObject(igdeWObject *object);
	
	/** \brief Update widget after object changed outside. */
	void UpdateObject();
	
	
	/** \brief Action or NULL. */
	inline igdeAction *GetAction() const{ return pAction; }
	
	/** \brief Set action or NULL. */
	void SetAction( igdeAction *action );
	
	/** \brief Undo system or nullptr. */
	inline igdeUndoSystem *GetUndoSystem() const{ return pUndoSystem; }
	
	/**
	 * \brief Set undo system or nullptr.
	 * 
	 * If not nullptr an undo action is added upon changing parameters instead of applying the
	 * changes directly. This allows to undo changes done through this widget.
	 */
	void SetUndoSystem(igdeUndoSystem *undoSystem);
	
	
	/**
	 * \brief Parameters changed.
	 * 
	 * Called if user changes any of the parameters. Default implementation calls
	 * OnAction on set action object if present.
	 */
	virtual void OnAction();
	
	/** \brief Action parameters changed. */
	virtual void OnParameterChanged( igdeAction *action );
	
	/** \brief Action has been destroyed. */
	virtual void OnDestroyed( igdeAction *action );
	/*@}*/
	
	
	
private:
	void pCreateContent();
};

#endif
