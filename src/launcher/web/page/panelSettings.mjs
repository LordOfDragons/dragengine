/**
 * Settings panel
 */
export class PanelSettings {
	/**
	 * Create settings panel.
	 * @param {String} panelId Panel identifier
	 */
	constructor (panelId) {
		this._panelId = panelId
		this._panel = document.getElementById(panelId)
		this._panelParent = this._panel.parentElement
	}
	
	/**
	 * Panel ID.
	 */
	panelId() {
		return this._panelId
	}
	
	/**
	 * Panel is visible.
	 * @returns true if panel is visible
	 */
	isPanelVisible() {
		const rect = this._panel.getBoundingClientRect()
		const parentRect = this._panelParent.getBoundingClientRect()
		return rect.bottom < parentRect.top - rect.height / 2
	}
	
	/**
	 * Hide panel.
	 */
	hidePanel() {
		const actions = [
			{transform: "translate(-50%, -100%)"}
		]
		const timing = {
			duration: 500,
			easing: 'ease-in',
			iterations: 1,
			fill: 'forwards'
		}
		this._panel.animate(actions, timing)
	}
	
	/**
	 * Show panel.
	 */
	showPanel() {
		const actions = [
			{transform: "translate(-50%, 0%)"}
		]
		const timing = {
			duration: 500,
			easing: 'ease-out',
			iterations: 1,
			fill: 'forwards'
		}
		this._panel.animate(actions, timing)
	}
	
	/**
	 * Toggle panel visibility.
	 */
	togglePanel() {
		if (this.isPanelVisible()) {
			this.hidePanel()
		} else {
			this.showPanel()
		}
	}
}
