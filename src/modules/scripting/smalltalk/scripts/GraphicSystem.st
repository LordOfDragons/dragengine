" Smalltalk Class DEGraphicSystem
|
| Copyright (C) 2015, Plüss Roland ( roland@rptd.ch )
|
| This program is free software; you can redistribute it and/or 
| modify it under the terms of the GNU General Public License 
| as published by the Free Software Foundation; either 
| version 2 of the License, or (at your option) any later 
| version.
| 
| This program is distributed in the hope that it will be useful,
| but WITHOUT ANY WARRANTY; without even the implied warranty of
| MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
| GNU General Public License for more details.
| 
| You should have received a copy of the GNU General Public License
| along with this program; if not, write to the Free Software
| Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA."

Object subclass: DEGraphicSystem [
	<comment: 'I am the graphic system. I am a singleton.'>
	<shape: #byte>
	
	DEGraphicSystem class [
		| scripting |
		
		new [
			self error: #shouldNotImplement
		]
		
		windowWidth [
			<category: 'accessory'>
			<cCall: 'DEGraphicSystem.getWindowWidth' returning: #int args: #(#self)>
		]
		
		windowHeight [
			<category: 'accessors'>
			<cCall: 'DEGraphicSystem.getWindowHeight' returning: #int args: #(#self)>
		]
		
		windowWidth: anInteger height: anInteger2 [
			<category: 'mutators'>
			<cCall: 'DEGraphicSystem.setWindowSize' returning: #void args: #(#self #int #int)>
		]
		
		windowTitle: aString [
			<category: 'mutators'>
			<cCall: 'DEGraphicSystem.setWindowTitle' returning: #void args: #(#self #string)>
		]
		
		primaryCanvas [
			<category: 'rendering'>
			<cCall: 'DEGraphicSystem.getPrimaryCanvas' returning: #smalltalk args: #(#self)>
		]
	]
]
