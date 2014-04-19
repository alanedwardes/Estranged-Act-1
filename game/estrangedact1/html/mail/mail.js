var blockAllInput = false;

function StartMail()
{
	for (var i = 0; i < icons.length; i++)
	{
		icons[i].create(i);
	}
	
	var bodyWidth = $(document.body).width();
	var bodyHeight = $(document.body).height();
	var dialogWidth = bodyWidth / 1.25;
	var dialogHeight = bodyHeight / 1.25;
	var dialogPosition = [(bodyWidth / 2) - (dialogWidth / 2), (bodyHeight / 2) - (dialogHeight / 2)];
	
	$(".dialog").dialog({
		autoOpen: false,
		modal: true,
		closeOnEscape: false,
		draggable: false,
		resizable: false,
		width: dialogWidth,
		height: dialogHeight,
		position: dialogPosition
	});
	
	$("button").button();
	
	var cards = $('.card');
	var emails = $('.email');
	
	cards.each(function(i, card){
		$(card).click(function(e){
			$('.card').removeClass('selected');
			$(this).addClass('selected');
			$('.email').hide();
			$(emails.get(i)).show();
		});
	});
	
	// Make sure the first is always selected
	cards.first().click();
	
	ae.callbackListeners.keyCodeReleased.push(function(keyCode){
		if (blockAllInput)
		{
			return;
		}
	
		var emailVisible = $('.card').is(":visible");
		if (emailVisible)
		{
			switch (keyCode)
			{
				case ae.KEY_UP:
				case ae.KEY_XBUTTON_UP:
					$('.card.selected').prev('.card').click();
					return;
				case ae.KEY_DOWN:
				case ae.KEY_XBUTTON_DOWN:
					$('.card.selected').next('.card').click();
					return;
			}
		}
		
		var anyDialoguesVisible = $('.ui-dialog').is(":visible");
		if (!anyDialoguesVisible)
		{
			var selected = $('.icon.selected');
			var iconNumber = selected.get(0).iconNumber;
			var icon = icons[iconNumber];
			
			switch (keyCode)
			{
				case ae.KEY_UP:
				case ae.KEY_XBUTTON_UP:
					if (icon.neighbors.up) icon.neighbors.up.select();
					return;
				case ae.KEY_DOWN:
				case ae.KEY_XBUTTON_DOWN:
					if (icon.neighbors.down) icon.neighbors.down.select();
					return;
				case ae.KEY_LEFT:
				case ae.KEY_XBUTTON_LEFT:
					if (icon.neighbors.left) icon.neighbors.left.select();
					return;
				case ae.KEY_RIGHT:
				case ae.KEY_XBUTTON_RIGHT:
					if (icon.neighbors.right) icon.neighbors.right.select();
					return;
				case ae.KEY_ENTER:
				case ae.KEY_XBUTTON_A:
					icon.open();
					return;
			}
		}
		
		if (ae.isExitKeyCode(keyCode))
		{
			var hasOpenDialog = false;
			$(".ui-dialog-content").each(function(){
				var isModal = $(this).dialog("option", "dialogClass");
				var isOpen = $(this).dialog("isOpen");
				if (isOpen && !isModal)
				{
					hasOpenDialog = true;
					if (keyCode != ae.USE_KEY_CODE)
					{
						$(this).dialog("close");
					}
				}
			});
			
			if (!hasOpenDialog)
			{
				ae.close();
			}
		}
	});
}

function Icon(title, id, icon, x, y, hideInitially)
{
	this.title = title;
	this.id = id;
	this.icon = icon;
	this.x = x;
	this.y = y;
	this.hideInitially = hideInitially;
	this.neighbors = { up: 0, down: 0, left: 0, right: 0 };
	this.domElement = null;
	
	this.create = function(number)
	{
		var self = this;
		this.domElement = ae.element('a', {
			className: number == 0 ? 'icon selected' : 'icon',
			href: '#',
			iconNumber: number,
			id: 'icon-' + this.id,
			style: {
				left: ae.i2px(this.x),
				top: ae.i2px(this.y)
			}
		}, document.body);
		
		if (this.hideInitially)
		{
			this.domElement.style.display = 'none';
		}
		
		$(this.domElement).click(function(e){
			e.preventDefault();
			self.select();
			return true;
		});
		
		$(this.domElement).dblclick(function(e){
			e.preventDefault();
			self.open();
			return true;
		});
		
		ae.element('img', { src: this.icon }, this.domElement);
		ae.element('span', { className: 'loc', innerHTML: this.title }, this.domElement);
	}
	
	this.select = function()
	{
		$('.icon').removeClass('selected');
		$(this.domElement).addClass('selected');
	}
	
	this.open = function()
	{
		this.select();
		$('#dialog-' + this.id).dialog("open");
	}
}