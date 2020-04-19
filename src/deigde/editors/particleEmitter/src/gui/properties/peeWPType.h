/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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

#ifndef _PEEWPTYPE_H_
#define _PEEWPTYPE_H_

#include <deigde/gui/igdeTextFieldReference.h>
#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeCheckBoxReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeListBoxReference.h>
#include <deigde/gui/igdeSpinTextFieldReference.h>
#include <deigde/gui/composed/igdeEditPathReference.h>
#include <deigde/gui/layout/igdeContainerScroll.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/resources/igdeIconReference.h>

class peeEmitter;
class peeType;
class peeParameter;
class peeWindowProperties;
class peeWPTypeListener;



/**
 * \brief Type Panel.
 */
class peeWPType : public igdeContainerScroll{
private:
	peeWindowProperties &pWindowProperties;
	peeWPTypeListener *pListener;
	
	peeEmitter *pEmitter;
	
	igdeIconReference pIconUnused;
	igdeIconReference pIconUsed;
	
	igdeActionReference pActionType;
	igdeActionReference pActionTypeAdd;
	igdeActionReference pActionTypeRemove;
	igdeActionReference pActionTypeRename;
	
	igdeComboBoxReference pCBType;
	igdeButtonReference pBtnType;
	
	igdeTextFieldReference pEditBurstLifetime;
	igdeCheckBoxReference pChkEmitBurst;
	
	igdeEditPathReference pEditSkin;
	igdeEditPathReference pEditModel;
	igdeEditPathReference pEditModelSkin;
	igdeComboBoxReference pCBCastFrom;
	igdeComboBoxReference pCBSimType;
	igdeCheckBoxReference pChkIntervalAsDistance;
	igdeEditPathReference pEditPathTrailEmitter;
	igdeComboBoxReference pCBTrailController;
	igdeTextFieldReference pEditTrailTargetController;
	
	igdeTextFieldReference pEditPhysicsSize;
	igdeComboBoxReference pCBCollisionResponse;
	igdeEditPathReference pEditPathCollisionEmitter;
	igdeTextFieldReference pEditEmitMinImpulse;
	igdeComboBoxReference pCBEmitController;
	igdeTextFieldReference pEditEmitTargetController;
	
	igdeListBoxReference pListParams;
	
	igdeTextFieldReference pEditParamValue;
	igdeTextFieldReference pEditParamSpread;
	igdeComboBoxReference pCBParamCtrlValue;
	igdeComboBoxReference pCBParamCtrlSpread;
	
	bool pPreventUpdate;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create panel. */
	peeWPType( peeWindowProperties &windowProperties );
	
protected:
	/** \brief Clean up panel. */
	virtual ~peeWPType();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Window properties. */
	inline peeWindowProperties &GetWindowProperties() const{ return pWindowProperties; }
	
	/** \brief Emitter. */
	inline peeEmitter *GetEmitter() const{ return pEmitter; }
	
	/** \brief Set emitter. */
	void SetEmitter( peeEmitter *emitter );
	
	/** \brief Active type. */
	peeType *GetType() const;
	
	/** \brief Active parameter. */
	peeParameter *GetParameter() const;
	
	/** \brief Update type list. */
	void UpdateTypeList();
	
	/** \brief Change selection to active type. */
	void SelectActiveType();
	
	/** \brief Update emitter. */
	void UpdateEmitter();
	
	/** \brief Update type. */
	void UpdateType();
	
	/** \brief Update controller list. */
	void UpdateControllerList();
	
	/** \brief Change selection to active trail type. */
	void SelectActiveTrailController();
	
	/** \brief Change selection to active emit type. */
	void SelectActiveEmitController();
	
	/** \brief Change selection to active parameter. */
	void SelectActiveParameter();
	
	/** \brief Update active parameter. */
	void UpdateParameter();
	
	/** \brief Update parameter usage icon. */
	void UpdateParameterUsage( const peeParameter &parameter );
	
	
	
	/** \brief Actions. */
	inline igdeAction* GetActionTypeAdd() const{ return pActionTypeAdd; }
	inline igdeAction* GetActionTypeRemove() const{ return pActionTypeRemove; }
	inline igdeAction* GetActionTypeRename() const{ return pActionTypeRename; }
	/*@}*/
};

#endif
