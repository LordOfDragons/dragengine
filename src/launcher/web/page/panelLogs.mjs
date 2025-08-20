
/**
 * Logs panel
 */
export class PanelLogs {
	static Severity = Object.freeze({
		Info: 0,
		Warning: 1,
		Error: 2
	})
	
	
	/**
	 * Create logs panel.
	 * @param {String} panelId Panel identifier
	 */
	constructor (panelId) {
		this._panelId = panelId
		this._panel = document.getElementById(panelId)
		this._panelParent = this._panel.parentElement
		this._panelLogs = document.getElementById(`${panelId}-logs`)
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
		return rect.left < parentRect.right - rect.width / 2
	}
	
	/**
	 * Hide panel.
	 */
	hidePanel() {
		const actions = [
			{transform: "translate(100%, -50%)"}
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
			{transform: "translate(0%, -50%)"}
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
	
	/**
	 * Add logs.
	 * @param {String} severity Severity
	 * @param {String} source Source
	 * @param {String} message Message
	 */
	addLogs(severity, source, message) {
		if (severity == PanelLogs.Severity.Error) {
			console.log(`EE [${source}] ${message}`)
		} else if (severity == PanelLogs.Severity.Warning) {
			console.log(`WW [${source}] ${message}`)
		} else {
			console.log(`II [${source}] ${message}`)
		}
		
		while (this._panelLogs.childElementCount > 200) {
			//this._panelLogs.removeChild(this._panelLogs.firstChild)
			this._panelLogs.removeChild(this._panelLogs.lastChild)
		}
		
		const line = document.createElement('div')
		
		const divSource = document.createElement('div')
		divSource.classList.add('log-line-source')
		divSource.appendChild(document.createTextNode(source))
		line.appendChild(divSource)
		
		const divMessage = document.createElement('div')
		divMessage.classList.add('log-line-message')
		if (severity == PanelLogs.Severity.Error) {
			divMessage.classList.add('log-line-error')
		} else if (severity == PanelLogs.Severity.Warning) {
			divMessage.classList.add('log-line-warning')
		} else {
			divMessage.classList.add('log-line-info')
		}
		const parts = message.split('\n')
		const lastPart = parts[-1]
		for (const part of parts) {
			divMessage.appendChild(document.createTextNode(part))
			if (part !== lastPart) {
				divMessage.appendChild(document.createElement('br'))
			}
		}
		line.appendChild(divMessage)
		
		const divClear = document.createElement('div')
		divClear.classList.add('log-line-clear')
		line.appendChild(divClear)
		
		/*
		if (this._panelLogs.childElementCount > 0) {
			this._panelLogs.insertBefore(line, this._panelLogs.firstChild)
		} else {
			this._panelLogs.appendChild(line)
		}
		*/
		this._panelLogs.appendChild(line)
		
		this._panelLogs.scrollTop = this._panelLogs.scrollHeight
	}
}
