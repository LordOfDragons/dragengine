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

#ifndef _INPUTDEVICE_H_
#define _INPUTDEVICE_H_

#include "../deObject.h"
#include "../common/collection/decObjectOrderedSet.h"
#include "../common/math/decMath.h"
#include "../common/string/decString.h"
#include "../resources/image/deImage.h"
#include "../resources/model/deModel.h"
#include "../resources/skin/deSkin.h"
#include "../resources/rig/deRig.h"

class deInputDeviceButton;
class deInputDeviceAxis;
class deInputDeviceFeedback;
class deInputDeviceComponent;


/**
 * \brief Input device installed on the host system.
 * 
 * Input devices provide digital or analog input. Devices can have buttons and axes.
 * 
 * The device identifier uniquely identifies the device across different platforms for
 * the same module. It can be safely used by the scripting module to save input bindings.
 * The identifier composes only of letters, digits and underscore to allow script using
 * them as prefix. The input module should add a unique prefix to the identifier to mark
 * the identifier originating fromt his module in case different input modules are involved.
 * 
 * Devices can have axes, buttons and feedbacks. Axes receive an analog value in the range
 * from -1 to 1 for absolue axes. For relative axes like the mouse only the relative change
 * is returned. Buttons return a pressed or unpressed state. Feedbacks on the other hand
 * allow manipulating a feedback state of the device if present.
 * 
 * Devices can have a pose indicating the coordinate frame and velocities for use with VR
 * tracking. These are all the edtVR* types. If a device does not support tracking the
 * identity coordinate frame with zero velocities is returned.
 * 
 * Devices can have bone poses for use by VR controllers. To keep the system simple the bone
 * configuration is indicated using an enumeration type. This ensures a consistent bone
 * configuration developers can rely upon.
 */
class DE_DLL_EXPORT deInputDevice : public deObject{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deInputDevice> Ref;
	
	
public:
	/** \brief Device types. */
	enum eDeviceTypes{
		/** \brief Mouse. */
		edtMouse,
		
		/** \brief Keyboard. */
		edtKeyboard,
		
		/** \brief Game pad. */
		edtGamepad,
		
		/** \brief Joystick. */
		edtJoystick,
		
		/** \brief Touchpad. */
		edtTouchpad,
		
		/** \brief Touchscreen. */
		edtTouchscreen,
		
		/** \brief Racing wheel. */
		edtRacingWheel,
		
		/** \brief Generic device. */
		edtGeneric,
		
		/**
		 * \brief VR Head mounted display.
		 * \version 1.6
		 */
		edtVRHMD,
		
		/**
		 * \brief VR Right Hand Controller.
		 * \version 1.6
		 */
		edtVRRightHand,
		
		/**
		 * \brief VR Left Hand Controller.
		 * \version 1.6
		 */
		edtVRLeftHand,
		
		/**
		 * \brief VR Treadmill.
		 * \version 1.6
		 */
		edtVRTreadmill,
		
		/**
		 * \brief VR Stylue.
		 * \version 1.6
		 */
		edtVRStylus,
		
		/**
		 * \brief VR Generic Controller.
		 * \version 1.6
		 */
		edtVRController,
		
		/**
		 * \brief VR Tracker.
		 * \version 1.6
		 */
		edtVRTracker,
		
		/**
		 * \brief VR Base Station.
		 * \version 1.6
		 */
		edtVRBaseStation,
		
		/**
		 * \brief VR Eye Tracker.
		 * \version 1.12
		 */
		edtVREyeTracker
	};
	
	/**
	 * \brief Bone configurations.
	 * \version 1.6
	 */
	enum eBoneConfigurations{
		/** \brief No bones supported. */
		ebcNone,
		
		/**
		 * \brief Articulated hand bones.
		 * 
		 * Bones are defined for fingers in this order:
		 * - Wrist: 0
		 * - Thumb: bones 1 to 4
		 * - Index finger: bones 5 to 8
		 * - Middle finger: bones 9 to 12
		 * - Ring finger: bones 13 to 16
		 * - Pinky finger: bones 17 to 20
		 * 
		 * For a total of 21 bones. Each finger has 4 bones in these order:
		 * - Segment 0: inside palm
		 * - Segment 1: first finger segment
		 * - Segment 2: second finger segment
		 * - Segment 3: third finger segment
		 * 
		 * Poses are relative to the parent bone inside chains and the device coordinate
		 * frame for chain root bones.
		 */
		ebcHand
	};
	
	/**
	 * \brief Convenience enumeration for hand bones.
	 * \version 1.6
	 */
	enum eHandBones{
		ehbWrist,
		ehbThumb0,
		ehbThumb1,
		ehbThumb2,
		ehbThumb3,
		ehbIndex0,
		ehbIndex1,
		ehbIndex2,
		ehbIndex3,
		ehbMiddle0,
		ehbMiddle1,
		ehbMiddle2,
		ehbMiddle3,
		ehbRing0,
		ehbRing1,
		ehbRing2,
		ehbRing3,
		ehbPinky0,
		ehbPinky1,
		ehbPinky2,
		ehbPinky3
	};
	
	/**
	 * \brief Convenience value count of hand bones.
	 * \version 1.6
	 */
	static const int HandBoneCount = 21;
	
	/**
	 * \brief Convenience enumeration for face expressions.
	 * \version 1.12
	 * 
	 * See https://www.khronos.org/registry/OpenXR/specs/1.0/html/xrspec.html#XR_HTC_facial_tracking
	 * and https://www.khronos.org/registry/OpenXR/specs/1.0/html/xrspec.html#XR_FB_face_tracking2
	 * for description and images of face expressions. Expressions existing in one expression set
	 * are emulated in the other if possible.
	 * 
	 * Some expressions are simplified expressions. These can have detail expressions which combined
	 * match up with the simplified expression. Use only the simplified expression or the corresponding
	 * detail expressions or you get the expression applied twice. You can also apply simplified
	 * expressions last and make them overwrite detail expressions. In this case both systems can
	 * work together if it is unclear which system is used.
	 * 
	 * Expressions up to and including efeTongueMorphDownLeft are present since version 1.12 .
	 * All other expressions are present since version 1.21 .
	 */
	enum eFaceExpressions{
		efeEyeLeftBlink,
		efeEyeLeftWide,
		efeEyeLeftSqueeze,
		efeEyeLeftDown,
		efeEyeLeftUp,
		efeEyeLeftIn,
		efeEyeLeftOut,
		
		efeEyeRightBlink,
		efeEyeRightWide,
		efeEyeRightSqueeze,
		efeEyeRightDown,
		efeEyeRightUp,
		efeEyeRightIn,
		efeEyeRightOut,
		
		efeJawRight,
		efeJawLeft,
		efeJawForward,
		efeJawOpen,
		
		efeCheekPuffRight,
		efeCheekPuffLeft,
		efeCheekSuck, //<!Simplified expression of efeCheekSuckRight and efeCheekSuckLeft
		
		efeMouthApeShape,
		efeMouthUpperRight,
		efeMouthUpperLeft,
		efeMouthUpperUpRight,
		efeMouthUpperUpLeft,
		efeMouthUpperOverturn,
		efeMouthUpperInside,
		efeMouthLowerRight,
		efeMouthLowerLeft,
		efeMouthLowerDownRight,
		efeMouthLowerDownLeft,
		efeMouthLowerOverturn,
		efeMouthLowerInside,
		efeMouthLowerOverlay,
		efeMouthPout, //<!Simplified expression of efeMouthPoutRight and efeMouthPoutLeft
		efeMouthSmileRight,
		efeMouthSmileLeft,
		efeMouthSadRight,
		efeMouthSadLeft,
		
		efeTongueLeft,
		efeTongueRight,
		efeTongueUp,
		efeTongueDown,
		efeTongueRoll,
		efeTongueLongStep1,
		efeTongueLongStep2,
		efeTongueMorphUpRight,
		efeTongueMorphUpLeft,
		efeTongueMorphDownRight,
		efeTongueMorphDownLeft,
		
		efeEyeRightSquint,
		efeEyeLeftSquint,
		
		efeBrowRightDown,
		efeBrowLeftDown,
		efeBrowInnerRightUp,
		efeBrowInnerLeftUp,
		efeBrowOuterRightUp,
		efeBrowOuterLeftUp,
		
		efeLidUpperUpRight,
		efeLidUpperUpLeft,
		
		efeNoseWrinkleRight,
		efeNoseWrinkleLeft,
		
		efeCheekUpRight,
		efeCheekUpLeft,
		efeCheekSuckRight, //<!Detail expression of efeCheekSuck
		efeCheekSuckLeft, //<!Detail expression of efeCheekSuck
		
		efeChinUpperUp,
		efeChinLowerUp,
		
		efeMouthDimpleRight,
		efeMouthDimpleLeft,
		efeMouthPressRight,
		efeMouthPressLeft,
		efeMouthPoutRight, //<!Detail expression of efeCMouthPout
		efeMouthPoutLeft, //<!Detail expression of efeCMouthPout
		efeMouthStretchRight,
		efeMouthStretchLeft,
		efeMouthRight,
		efeMouthLeft,
		efeMouthUpperFunnelRight,
		efeMouthUpperFunnelLeft,
		efeMouthUpperSuckRight,
		efeMouthUpperSuckLeft,
		efeMouthLowerFunnelRight,
		efeMouthLowerFunnelLeft,
		efeMouthLowerSuckRight,
		efeMouthLowerSuckLeft,
		
		efeTongueTipInterdental,
		efeTongueTipAlveolar,
		efeTongueDorsalPalateFront,
		efeTongueDorsalPalateMid,
		efeTongueDorsalVelarBack,
		efeTongueRetreatBack,
		efeTongueOut
	};
	
	/**
	 * \brief Convenience value count of face expressions.
	 * \version 1.12
	 */
	static const int FaceExpressionCount = 51;
	
	
	
private:
	/** \brief Device identifier unique across reboots. */
	decString pID;
	
	/** \brief Display name. */
	decString pName;
	
	/** \brief Device type. */
	eDeviceTypes pType;
	
	/** \brief Model to represent the device in 3D user interfaces or NULL if not set. */
	deModel::Ref pDisplayModel;
	
	/** \brief Skin for display model or NULL if not set. */
	deSkin::Ref pDisplaySkin;
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	deImage::Ref pDisplayImage;
	
	/** \brief List of small icons of different size for use in binding displays. */
	decObjectOrderedSet pDisplayIcons;
	
	/** \brief Text to display centered across display image or icon. */
	decString pDisplayText;
	
	/** \brief Buttons. */
	deInputDeviceButton *pButtons;
	
	/** \brief Number of buttons. */
	int pButtonCount;
	
	/** \brief Axes. */
	deInputDeviceAxis *pAxes;
	
	/** \brief Number of axes. */
	int pAxisCount;
	
	/** \brief Feedbacks. */
	deInputDeviceFeedback *pFeedbacks;
	
	/** \brief Number of feedbacks. */
	int pFeedbackCount;
	
	/** \brief Components. */
	deInputDeviceComponent *pComponents;
	
	/** \brief Number of components. */
	int pComponentCount;
	
	/** \brief Bone configuration. */
	eBoneConfigurations pBoneConfiguration;
	
	/**
	 * \brief Finger tip offsets.
	 * 
	 * For use with ebcHand bone configuration. Defines the offset of the finger tip point
	 * relative to the last finger segment coordinate frame. These are typically used to
	 * figure out where finger tips touch objects in the world.
	 */
	decVector pFingerTipOffset[5];
	
	/** \brief Hand rig if ebcHand is used. */
	deRig::Ref pHandRig;
	
	/** \brief Device supports face eye expressions. */
	bool pSupportsFaceEyeExpressions;
	
	/** \brief Device supports face mouth expressions. */
	bool pSupportsFaceMouthExpressions;
	
	/** \brief Device is using hand interaction. */
	bool pUsingHandInteraction;
	
	/** \brief Model to represent the device in VR environments or nullptr if not set. */
	deModel::Ref pVRModel;
	
	/** \brief Skin for VR model or nullptr if not set. */
	deSkin::Ref pVRSkin;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create input device. */
	deInputDevice();
	
protected:
	/** \brief Clean up input device. */
	virtual ~deInputDevice();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/**
	 * \brief Device identifier.
	 * 
	 * Unique identifier for the device identifying it across removing and attaching
	 * the device to the host system. The prefix is a normalized string that contains
	 * only letters, numbers and underscores. It is suitable to be combined with
	 * button or axis identifiers to store them in config files as key bindings.
	 */
	inline const decString &GetID() const{ return pID; }
	
	/** \brief Set identifier. */
	void SetID(const char *id);
	
	/**
	 * \brief Display name.
	 * 
	 * Dispaly name is human readable like for example 'USB Gamepad #1'. The
	 * name is unique but not guaranteed to stay the same across restarting
	 * the input module or game engine.
	 */
	inline const decString &GetName() const{ return pName; }
	
	/** \brief Set Display name. */
	void SetName(const char *name);
	
	/** \brief Device type. */
	inline eDeviceTypes GetType() const{ return pType; }
	
	/** \brief Device type. */
	void SetType(eDeviceTypes type);
	
	/** \brief Model to represent the device in 3D user interfaces or NULL if not set. */
	inline const deModel::Ref &GetDisplayModel() const{ return pDisplayModel; }
	
	/** \brief Set model to represent the device in 3D user interfaces or NULL if not set. */
	void SetDisplayModel(const deModel::Ref &model);
	
	/** \brief Skin for display model or NULL if not set. */
	inline const deSkin::Ref &GetDisplaySkin() const{ return pDisplaySkin; }
	
	/** \brief Set skin for display model or NULL if not set. */
	void SetDisplaySkin(const deSkin::Ref &skin);
	
	/**
	 * \brief Image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	inline const deImage::Ref &GetDisplayImage() const{ return pDisplayImage; }
	
	/**
	 * \brief Set image to represent the device in 2D user interfaces or NULL if not set.
	 * 
	 * Large image of 128 pixels squared or larger.
	 */
	void SetDisplayImage(const deImage::Ref &image);
	
	/** \brief Count of icons representing the device in bindings. */
	int GetDisplayIconCount() const;
	
	/**
	 * \brief Icon at index representing the device in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	deImage::Ref GetDisplayIconAt(int index) const;
	
	/**
	 * \brief Add icon representing the device in bindings.
	 * 
	 * Icon is of square size and typically has a size of 16, 24, 32 or 64.
	 */
	void AddDisplayIcon(const deImage::Ref &image);
	
	/** \brief Text to display centered across display image or icon. */
	inline const decString &GetDisplayText() const{ return pDisplayText; }
	
	/** \brief Set text to display centered across display image or icon. */
	void SetDisplayText(const char *text);
	
	/**
	 * \brief Bone configuration.
	 * \version 1.6
	 */
	inline eBoneConfigurations GetBoneConfiguration() const{ return pBoneConfiguration; }
	
	/**
	 * \brief Set bone configuration.
	 * \version 1.6
	 */
	void SetBoneConfiguration(eBoneConfigurations configuration);
	
	/**
	 * \brief Finger tip offsets.
	 * \version 1.6
	 * 
	 * For use with ebcHand bone configuration. Defines the offset of the finger tip point
	 * relative to the last finger segment coordinate frame. These are typically used to
	 * figure out where finger tips touch objects in the world.
	 */
	const decVector &GetFingerTipOffset(int index) const;
	
	/**
	 * \brief Finger tip offsets.
	 * \version 1.6
	 * 
	 * For use with ebcHand bone configuration. Defines the offset of the finger tip point
	 * relative to the last finger segment coordinate frame. These are typically used to
	 * figure out where finger tips touch objects in the world.
	 */
	void SetFingerTipOffset(int index, const decVector &offset);
	
	/**
	 * \brief Hand rig if ebcHand is used.
	 * \version 1.6
	 */
	inline const deRig::Ref &GetHandRig() const{ return pHandRig; }
	
	/**
	 * \brief Set hand rig if ebcHand is used.
	 * \version 1.6
	 */
	void SetHandRig(const deRig::Ref &rig);
	
	/**
	 * \brief Device supports face eye expressions.
	 * \version 1.12
	 */
	inline bool GetSupportsFaceEyeExpressions() const{ return pSupportsFaceEyeExpressions; }
	
	/**
	 * \brief Set if device supports face eye expressions.
	 * \version 1.12
	 */
	void SetSupportsFaceEyeExpressions(bool supportsFaceEyeExpressions);
	
	/**
	 * \brief Device supports face mouth expressions.
	 * \version 1.12
	 */
	inline bool GetSupportsFaceMouthExpressions() const{ return pSupportsFaceMouthExpressions; }
	
	/**
	 * \brief Set if device supports face mouth expressions.
	 * \version 1.12
	 */
	void SetSupportsFaceMouthExpressions(bool supportsFaceMouthExpressions);
	
	/**
	 * \brief Device is using hand interaction.
	 * \version 1.28
	 */
	inline bool GetUsingHandInteraction() const{ return pUsingHandInteraction; }
	
	/**
	 * \brief Set if device is using hand interaction.
	 * \version 1.28
	 */
	void SetUsingHandInteraction(bool usingHandInteraction);
	
	/**
	 * \brief Model to represent the device in VR environments or NULL if not set.
	 * \version 1.6
	 */
	inline const deModel::Ref &GetVRModel() const{ return pVRModel; }
	
	/**
	 * \brief Set model to represent the device in VR environments or NULL if not set.
	 * \version 1.6
	 */
	void SetVRModel(const deModel::Ref &model);
	
	/**
	 * \brief Skin for VR model or NULL if not set.
	 * \version 1.6
	 */
	inline const deSkin::Ref &GetVRSkin() const{ return pVRSkin; }
	
	/**
	 * \brief Skin for VR model or NULL if not set.
	 * \version 1.6
	 */
	void SetVRSkin(const deSkin::Ref &skin);
	/*@}*/
	
	
	
	/** \name Buttons */
	/*@{*/
	/** \brief Number of buttons. */
	inline int GetButtonCount() const{ return pButtonCount; }
	
	/**
	 * \brief Set number of buttons.
	 * 
	 * Resets all buttons to default values.
	 */
	void SetButtonCount(int count);
	
	/** \brief Button at index. */
	deInputDeviceButton &GetButtonAt(int index) const;
	
	/** \brief Index of button with identifier or -1 if not found. */
	int IndexOfButtonWithID(const char *id) const;
	/*@}*/
	
	
	
	/** \name Axes */
	/*@{*/
	/** \brief Number of axes. */
	inline int GetAxisCount() const{ return pAxisCount; }
	
	/**
	 * \brief Set number of axes.
	 * 
	 * Resets all axes to default values.
	 */
	void SetAxisCount(int count);
	
	/** \brief Axis at index. */
	deInputDeviceAxis &GetAxisAt(int index) const;
	
	/** \brief Index of axis with identifier or -1 if not found. */
	int IndexOfAxisWithID(const char *id) const;
	/*@}*/
	
	
	
	/** \name Feedbacks */
	/*@{*/
	/** \brief Number of feedbacks. */
	inline int GetFeedbackCount() const{ return pFeedbackCount; }
	
	/**
	 * \brief Set number of feedbacks.
	 * 
	 * Resets all feedbacks to default values.
	 */
	void SetFeedbackCount(int count);
	
	/** \brief Feedback at index. */
	deInputDeviceFeedback &GetFeedbackAt(int index) const;
	
	/** \brief Index of feedback with identifier or -1 if not found. */
	int IndexOfFeedbackWithID(const char *id) const;
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/**
	 * \brief Number of components.
	 * \version 1.6
	 */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/**
	 * \brief Set number of components.
	 * \version 1.6
	 * 
	 * Resets all components to default values.
	 */
	void SetComponentCount(int count);
	
	/**
	 * \brief Component at index.
	 * \version 1.6
	 */
	deInputDeviceComponent &GetComponentAt(int index) const;
	
	/**
	 * \brief Index of component with identifier or -1 if not found.
	 * \version 1.6
	 */
	int IndexOfComponentWithID(const char *id) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif
