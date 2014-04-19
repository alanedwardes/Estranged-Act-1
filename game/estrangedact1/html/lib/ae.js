var ae = 
{
	// Localisation Configuration
	sourcemods_dir: 'estrangedact1',
	defaultLanguage: 'english',
	availableLanguages: [
		'bulgarian',
		'brazilian',
		'danish',
		'dutch',
		'finnish',
		'french',
		'german',
		'japanese',
		'korean',
		'koreana',
		'hungarian',
		'italian',
		'norwegian',
		'polish',
		'portuguese',
		'romanian',
		'russian',
		'schinese',
		'tchinese',
		'thai',
		'spanish',
		'swedish',
		'greek',
		'czech',
		'ukrainian'
	],
	localisationSelectors: ['.ui-dialog-title', '.loc'],
	localisationError: '!!MISSING-',
	// End configuration
	currentTokens: {},
	currentLanguage: '',
	loadLanguages: function(gameRoot)
	{
		var FILE_PREFIX = '/resource/' + ae.sourcemods_dir + '_';
		var currentLanguage = ae.getPersistedValue('language');		
		if (ae.availableLanguages.indexOf(currentLanguage) > -1)
		{
			ae.element('script', {
				src: gameRoot + FILE_PREFIX + currentLanguage + '.js'
			}, document.body);
		}
		
		// ae - grab english as a fallback anyway
		ae.element('script', {
			src: gameRoot + FILE_PREFIX + ae.defaultLanguage + '.js'
		}, document.body);
	},
	callbackListeners:
	{
		enteredGamepadText: [],
		keyCodePressed: [],
		keyCodeReleased: []
	},
	executeListeners: function(id, param)
	{
		var listeners = ae.callbackListeners[id];
		for (var i = 0; i < listeners.length; i++)
		{
			listeners[i](param);
		}
	},
	engineCallbacks:
	{
		enteredGamepadText: function(text)
		{
			ae.log("ae.engineCallbacks.enteredGamepadText('" + text + "')");
			ae.executeListeners('enteredGamepadText', text);
		},
		keyCodePressed: function(keyCode)
		{
			ae.log("ae.engineCallbacks.keyCodePressed(" + keyCode + ")");
			ae.executeListeners('keyCodePressed', keyCode);
		},
		keyCodeReleased: function(keyCode)
		{
			ae.log("ae.engineCallbacks.keyCodeReleased(" + keyCode + ")");
			ae.executeListeners('keyCodeReleased', keyCode);
		}
	},
	objectKeysToLower: function(object)
	{
		// http://stackoverflow.com/a/12540603
		var key, keys = Object.keys(object);
		var n = keys.length;
		var newobj = {};
		while (n--)
		{
		  key = keys[n];
		  newobj[key.toLowerCase()] = object[key];
		}
		
		return newobj;
	},
	addLanguage: function(language, tokens)
	{
		ae.currentTokens = ae.objectKeysToLower(tokens);
		ae.currentLanguage = language;
		ae.translate(tokens, language == ae.defaultLanguage);
	},
	translate: function(tokens, fallback)
	{
		$(ae.localisationSelectors.join(', ')).each(function(){
			ae.localiseElement(this, fallback);
		});
	},
	localiseElement: function(element, fallback)
	{
		var token = $(element).text().toLowerCase();
		if (ae.currentTokens[token])
		{
			$(element).text(ae.currentTokens[token]);
			if (!fallback)
			{
				for (var locClass in ae.localisationSelectors)
				{
					$(element).removeClass(locClass.replace('.', ''));
				}
			}
		}
		
		$(element).css({ visibility: 'visible' });
	},
	controllerTextInput: function(description)
	{
		ae.log('ae.controllerTextInput("' + description + '")');
		window.location.href = "estranged://controllertextinput/" + description;
	},
	close: function()
	{
		ae.log('ae.close()');
		window.location.href = "estranged://close";
	},
	trigger: function(i)
	{
		ae.log('ae.trigger(' + i + ')');
		window.location.href = "estranged://trigger/" + i;
	},
	setPersistedValue: function(key, value)
	{
		window.location.href = "estranged://persist/" + key + "/" + value;
	},
	getPersistedValue: function(key)
	{
		var hashParts = window.location.hash.replace('#', '').split('&');
		for (var i in hashParts)
		{
			var parts = hashParts[i].split('=');
			if (parts[0] == key)
			{
				return parts[1];
			}
		}
		
		return false;
	},
	px2i: function(pixels)
	{
		return parseInt(pixels.replace('px', ''));
	},
	i2px: function(integer)
	{
		return integer + 'px';
	},
	event: function(element, event, callback)
	{
		// If we didn't supply a valid element, exit.
		if (!element) return;
		// If the W3C way...
		if (element.addEventListener)
		{
			element.addEventListener(event, callback, false);
		}
		// If the Microsoft way
		else if (element.attachEvent)
		{
			element.attachEvent('on' + event, callback);
		}
	},
	nl2br: function(text)
	{
		return text.replace("\n", '<br/>');
	},
	element: function(tag, attributes, appendTo)
	{
		var el = document.createElement(tag);
		if (typeof attributes == 'object')
		{
			for (index in attributes)
			{
				var type = typeof attributes[index];
				if (type === "object")
				{
					for (index2 in attributes[index])
					{
						el[index][index2] = attributes[index][index2];
					}
				}
				else
				{
					el[index] = attributes[index];
				}
			}
		}
		else
		{
			el.innerHTML = attributes;
		}
		if (appendTo) appendTo.appendChild(el);
		return el;
	},
	clear: function(element)
	{
		this.element('div', {style:{clear:'both'}}, element);
	},
	log: function(message)
	{
		if (ae.getPersistedValue("debug") != "true")
		{
			console.log(message);
			return;
		}
		
		var log = document.getElementById('log');
		if (log == undefined)
		{
			log = ae.element('div', { id: 'log' }, document.body);
		}
		
		var now = new Date();
		var timeStamp = '[' + now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds() + ']';
		
		var logEntry = ae.element('div', {
			className: 'logEntry',
			innerHTML: timeStamp + ' ' + message
		}, log);
		setTimeout(function(){
			log.removeChild(logEntry);
		}, 2000);
	},
	listenToExitEvents: function()
	{
		ae.callbackListeners.keyCodeReleased.push(function(keyCode){
			if (ae.isExitKeyCode(keyCode))
			{
				ae.close();
			}
		});
	},
	showKeyHint: function(key, desc)
	{
		var hint = document.getElementById('hint');
		
		if (!hint)
		{
			hint = ae.element('div', { id: 'hint' }, document.body);
		}
		
		hint.innerHTML = '';
		hint.className = 'hide';
		
		var keyElement = ae.element('span', {
			innerHTML: key,
			id: 'key'
		}, hint);
		var descElement = ae.element('span', {
			innerHTML: desc,
			className: 'loc',
			id: 'desc'
		}, hint);
		
		$(hint).fadeIn();
		
		return hint;
	},
	isExitKeyCode: function(keyCode)
	{
		switch (keyCode)
		{
			case ae.KEY_XBUTTON_B:
			case ae.KEY_XBUTTON_START:
			case ae.KEY_XBUTTON_BACK:
			case ae.KEY_ESCAPE:
			case ae.USE_KEY_CODE:
				return true;
		}
		
		return false;
	},
	showExitHint: function()
	{
		var useKey = ae.getPersistedValue('use');
		if (useKey)
		{
			return ae.showKeyHint(useKey, 'close');
		}
		
		return false;
	},
	init: function()
	{
		var useKey = ae.getPersistedValue('use');
		var useKeyCode = ae.keyStringToButtonCode(useKey);
		if (useKey && useKeyCode > 0)
		{
			ae.USE_KEY_CODE = ae.keyStringToButtonCode(useKey);	
		}
		else
		{
			ae.USE_KEY_CODE = ae.KEY_E;
		}
	
		$(document).bind('selectstart', function(e){
			e.preventDefault();
			return false;
		});
	},
	keyStringToButtonCode: function(keyString)
	{
		keyString = keyString.toString();
	
		var KEY_PREFIX = 'KEY_';
		
		for (var item in this)
		{
			if (item.substr(0, 4) == KEY_PREFIX)
			{
				// DOWNARROW is KEY_DOWN
				keyString = keyString.replace('ARROW', '');
				// KP_ is PAD_
				keyString = keyString.replace('KP_', 'PAD_');
			
				if (item.toLowerCase() == (KEY_PREFIX + keyString).toLowerCase())
				{
					return this[item];
				}
			}
		}
		
		var additionalMappings = 
		{
			'/': ae.SLASH,
			'\\': ae.BACKSLASH,
			'.': ae.KEY_PERIOD,
			',': ae.KEY_COMMA,
			'-': ae.KEY_MINUS,
			'=': ae.KEY_EQUAL
		}
		
		if (additionalMappings[keyString])
		{
			return additionalMappings[keyString];
		}
		
		return -1;
	},
	BUTTON_CODE_INVALID: -1,
	BUTTON_CODE_NONE: 0,
	KEY_NONE: 0,
	KEY_0: 1,
	KEY_1: 2,
	KEY_2: 3,
	KEY_3: 4,
	KEY_4: 5,
	KEY_5: 6,
	KEY_6: 7,
	KEY_7: 8,
	KEY_8: 9,
	KEY_9: 10,
	KEY_A: 11,
	KEY_B: 12,
	KEY_C: 13,
	KEY_D: 14,
	KEY_E: 15,
	KEY_F: 16,
	KEY_G: 17,
	KEY_H: 18,
	KEY_I: 19,
	KEY_J: 20,
	KEY_K: 21,
	KEY_L: 22,
	KEY_M: 23,
	KEY_N: 24,
	KEY_O: 25,
	KEY_P: 26,
	KEY_Q: 27,
	KEY_R: 28,
	KEY_S: 29,
	KEY_T: 30,
	KEY_U: 31,
	KEY_V: 32,
	KEY_W: 33,
	KEY_X: 34,
	KEY_Y: 35,
	KEY_Z: 36,
	KEY_PAD_0: 37,
	KEY_PAD_1: 38,
	KEY_PAD_2: 39,
	KEY_PAD_3: 40,
	KEY_PAD_4: 41,
	KEY_PAD_5: 42,
	KEY_PAD_6: 43,
	KEY_PAD_7: 44,
	KEY_PAD_8: 45,
	KEY_PAD_9: 46,
	KEY_PAD_DIVIDE: 47,
	KEY_PAD_MULTIPLY: 48,
	KEY_PAD_MINUS: 49,
	KEY_PAD_PLUS: 50,
	KEY_PAD_ENTER: 51,
	KEY_PAD_DECIMAL: 52,
	KEY_LBRACKET: 53,
	KEY_RBRACKET: 54,
	KEY_SEMICOLON: 55,
	KEY_APOSTROPHE: 56,
	KEY_BACKQUOTE: 57,
	KEY_COMMA: 58,
	KEY_PERIOD: 59,
	KEY_SLASH: 60,
	KEY_BACKSLASH: 61,
	KEY_MINUS: 62,
	KEY_EQUAL: 63,
	KEY_ENTER: 64,
	KEY_SPACE: 65,
	KEY_BACKSPACE: 66,
	KEY_TAB: 67,
	KEY_CAPSLOCK: 68,
	KEY_NUMLOCK: 69,
	KEY_ESCAPE: 70,
	KEY_SCROLLLOCK: 71,
	KEY_INSERT: 72,
	KEY_DELETE: 73,
	KEY_HOME: 74,
	KEY_END: 75,
	KEY_PAGEUP: 76,
	KEY_PAGEDOWN: 77,
	KEY_BREAK: 78,
	KEY_LSHIFT: 79,
	KEY_RSHIFT: 80,
	KEY_LALT: 81,
	KEY_RALT: 82,
	KEY_LCONTROL: 83,
	KEY_RCONTROL: 84,
	KEY_LWIN: 85,
	KEY_RWIN: 86,
	KEY_APP: 87,
	KEY_UP: 88,
	KEY_LEFT: 89,
	KEY_DOWN: 90,
	KEY_RIGHT: 91,
	KEY_F1: 92,
	KEY_F2: 93,
	KEY_F3: 94,
	KEY_F4: 95,
	KEY_F5: 96,
	KEY_F6: 97,
	KEY_F7: 98,
	KEY_F8: 99,
	KEY_F9: 100,
	KEY_F10: 101,
	KEY_F11: 102,
	KEY_F12: 103,
	KEY_CAPSLOCKTOGGLE: 104,
	KEY_NUMLOCKTOGGLE: 105,
	KEY_SCROLLLOCKTOGGLE: 106,
	KEY_XBUTTON_UP: 146,
	KEY_XBUTTON_RIGHT: 147,
	KEY_XBUTTON_DOWN: 148,
	KEY_XBUTTON_LEFT: 149,
	KEY_XBUTTON_A: 114,
	KEY_XBUTTON_B: 115,
	KEY_XBUTTON_X: 116,
	KEY_XBUTTON_Y: 117,
	KEY_XBUTTON_LEFT_SHOULDER: 118,
	KEY_XBUTTON_RIGHT_SHOULDER: 119,
	KEY_XBUTTON_BACK: 120,
	KEY_XBUTTON_START: 121,
	KEY_XBUTTON_STICK1: 121,
	KEY_XBUTTON_STICK2: 122,
	KEY_XBUTTON_LTRIGGER: 154,
	KEY_XBUTTON_RTRIGGER: 155
};

$(document).ready(function(){
	ae.init();
});