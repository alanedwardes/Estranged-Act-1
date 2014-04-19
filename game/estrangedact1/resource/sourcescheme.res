///////////////////////////////////////////////////////////
// Tracker scheme resource file
//
// sections:
//		Colors			- all the colors used by the scheme
//		BaseSettings	- contains settings for app to use to draw controls
//		Fonts			- list of all the fonts used by app
//		Borders			- description of all the borders
//
///////////////////////////////////////////////////////////
Scheme
{
	Colors
	{
		// General Colours
		White					"255 255 255 255"
		OffWhite				"216 216 216 255"
		DullWhite				"142 142 142 255"
		Orange					"255 155 0   255"
		TransparentBlack		"0   0   0   128"
		Shadow					"0   0   0   64"
		Black					"0   0   0   255"
		Blank					"0   0   0   0"
		
		// Estranged Colour Scheme
		DarkRed					"90  0  0  255"
		LightRed				"200 50 50 255"
		LightRedTrans			"200 50 50 32"
		VeryDarkRed				"64  0  0  255"
		PanelLightRed			"79  28 28 255"
		PanelVeryLightRed		"150 54 54 255"
		
		LightSlateGray			"100 105 105 255"
		SlateGray				"045 055 055 255"
		DarkSlateGray			"030 040 040 255"
	}

	BaseSettings
	{
		Border.Bright						Shadow
		Border.Dark							Shadow
		Border.Selection					Blank
		
		HTML.BgColor						SlateGray
		
		Button.TextColor					White
		Button.BgColor						PanelLightRed
		Button.ArmedTextColor				White
		Button.ArmedBgColor					PanelVeryLightRed
		Button.DepressedTextColor			White
		Button.DepressedBgColor				VeryDarkRed
		Button.FocusBorderColor				Blank

		CheckButton.TextColor				White
		CheckButton.SelectedTextColor		White
		CheckButton.BgColor					TransparentBlack
		CheckButton.Border1  				PanelLightRed
		CheckButton.Border2  				PanelLightRed
		CheckButton.Check					White
		CheckButton.HighlightFgColor		OffWhite
		CheckButton.ArmedBgColor			Blank
		CheckButton.DepressedBgColor		Blank
		CheckButton.DisabledBgColor	   		Black
				
		ComboBoxButton.ArrowColor			DullWhite
		ComboBoxButton.ArmedArrowColor		White
		ComboBoxButton.BgColor				Blank
		ComboBoxButton.DisabledBgColor		Blank

		Frame.TitleTextInsetX				10
		Frame.ClientInsetX					5
		Frame.ClientInsetY					5
		Frame.BgColor						SlateGray
		Frame.OutOfFocusBgColor				DarkSlateGray
		Frame.FocusTransitionEffectTime		0
		Frame.TransitionEffectTime			0
		Frame.AutoSnapRange					0
		FrameGrip.Color1					LightSlateGray
		FrameGrip.Color2					DarkSlateGray
		FrameTitleButton.FgColor			PanelVeryLightRed
		FrameTitleButton.BgColor			Blank
		FrameTitleButton.DisabledFgColor	LightSlateGray
		FrameTitleButton.DisabledBgColor	Blank
		FrameSystemButton.FgColor			Blank
		FrameSystemButton.BgColor			Blank
		FrameSystemButton.Icon				titlebaricon
		FrameSystemButton.DisabledIcon		titlebardisabledicon
		FrameTitleBar.Font					FrameTitle
		FrameTitleBar.TextColor				White
		FrameTitleBar.BgColor				Blank
		FrameTitleBar.DisabledTextColor		LightSlateGray
		FrameTitleBar.DisabledBgColor		Blank

		GraphPanel.FgColor					White
		GraphPanel.BgColor					TransparentBlack

		Label.TextDullColor					DullWhite
		Label.TextColor						OffWhite
		Label.TextBrightColor				White
		Label.SelectedTextColor				White
		Label.BgColor						Blank
		Label.DisabledFgColor1				DarkSlateGray
		Label.DisabledFgColor2				LightSlateGray

		ListPanel.TextColor					OffWhite
		ListPanel.TextBgColor				Blank
		ListPanel.BgColor					TransparentBlack
		ListPanel.SelectedTextColor			White
		ListPanel.SelectedBgColor			PanelLightRed
		ListPanel.SelectedOutOfFocusBgColor	VeryDarkRed
		ListPanel.EmptyListInfoTextColor	OffWhite

		Panel.FgColor						DullWhite
		Panel.BgColor						Blank

		ProgressBar.FgColor					LightSlateGray
		ProgressBar.BgColor					Blank

		PropertySheet.TextColor				OffWhite
		PropertySheet.SelectedTextColor		White
		PropertySheet.TransitionEffectTime	0

		RadioButton.TextColor				DullWhite
		RadioButton.SelectedTextColor		White

		RichText.TextColor					OffWhite
		RichText.BgColor					TransparentBlack
		RichText.SelectedTextColor			White
		RichText.SelectedBgColor			PanelLightRed

		ScrollBar.Wide						16
		ScrollBarButton.FgColor				White
		ScrollBarButton.BgColor				PanelLightRed
		ScrollBarButton.ArmedFgColor		White
		ScrollBarButton.ArmedBgColor		PanelVeryLightRed
		ScrollBarButton.DepressedFgColor	VeryDarkRed
		ScrollBarButton.DepressedBgColor	Blank

		ScrollBarSlider.FgColor				LightSlateGray
		ScrollBarSlider.BgColor				Blank

		SectionedListPanel.HeaderTextColor	White
		SectionedListPanel.HeaderBgColor	Blank
		SectionedListPanel.DividerColor		Black
		SectionedListPanel.TextColor		DullWhite
		SectionedListPanel.BrightTextColor	OffWhite
		SectionedListPanel.BgColor			TransparentBlack
		SectionedListPanel.SelectedTextColor			White
		SectionedListPanel.SelectedBgColor				PanelLightRed
		SectionedListPanel.OutOfFocusSelectedTextColor	OffWhite
		SectionedListPanel.OutOfFocusSelectedBgColor	VeryDarkRed

		Slider.NobColor						LightSlateGray
		Slider.TextColor					OffWhite
		Slider.TrackColor					PanelLightRed
		Slider.DisabledTextColor1			Label.DisabledFgColor1
		Slider.DisabledTextColor2			Label.DisabledFgColor2

		TextEntry.TextColor					OffWhite
		TextEntry.BgColor					TransparentBlack
		TextEntry.CursorColor				OffWhite
		TextEntry.DisabledTextColor			DullWhite
		TextEntry.DisabledBgColor			Blank
		TextEntry.SelectedTextColor			White
		TextEntry.SelectedBgColor			PanelLightRed
		TextEntry.OutOfFocusSelectedBgColor	VeryDarkRed
		TextEntry.FocusEdgeColor			White

		ToggleButton.SelectedTextColor		White

		Tooltip.TextColor					White
		Tooltip.BgColor						LightSlateGray

		TreeView.BgColor					TransparentBlack

		WizardSubPanel.BgColor				Blank
		
		Menu.TextColor						OffWhite
		Menu.BgColor						SlateGray
		Menu.ArmedTextColor					White
		Menu.ArmedBgColor					PanelLightRed
		Menu.TextInset						2

		Console.TextColor					OffWhite
		Console.DevTextColor				White
	}

	//////////////////////// BITMAP FONT FILES /////////////////////////////
	//
	// Bitmap Fonts are ****VERY*** expensive static memory resources so they are purposely sparse
	BitmapFontFiles
	{
	}

	//////////////////////// FONTS /////////////////////////////
	//
	// describes all the fonts
	Fonts
	{
		// fonts are used in order that they are listed
		// fonts listed later in the order will only be used if they fulfill a range not already filled
		// if a font fails to load then the subsequent fonts will replace
		// fonts are used in order that they are listed
		"DebugFixed"
		{
			// Not sure where this is used
			"1"
			{
				"name"		"Inconsolata"
				"tall"		"13"
				"antialias" "1"
			}
		}
		// fonts are used in order that they are listed
		"DebugFixedSmall"
		{
			// Not sure where this is used
			"1"
			{
				"name"		"Inconsolata"
				"tall"		"13"
				"antialias" "1"
			}
		}
		"DefaultFixedOutline"
		{
			// Not sure where this is used
			"1"
			{
				"name"		 "Inconsolata"
				"tall"		 "13"
				"tall_lodef" "15"
				"tall_hidef" "20"
				"outline"	 "1"
			}
		}
		"ConsoleText"
		{
			"1"
			{
				"name"		"Inconsolata"
				"tall"		"13"
				"antialias"	"1"
			}
		}
		"DefaultFixed"
		{
			"1"
			{
				"name"		"Inconsolata"
				"tall"		"13"
				"antialias" "1"
			}
		}

		"DefaultFixedDropShadow"
		{
			"1"
			{
				"name"		"Inconsolata"
				"tall"		"13"
				"antialias" "1"
				"dropshadow" "1"
			}
		}
		"DefaultSmall"
		{
			// Column headings for tables,
			// third party technology credits screen
			// console suggestions
			"1"
			{
				"name"		"Inconsolata"
				"antialias"	"1"
				"tall"		"13"
			}
		}
		"DefaultSmallDropShadow"
		{
			// Used in the top right of the screen
			// for developer output ("Stuck on x", "source engine build y")
			"1"
			{
				"name"		"Inconsolata"
				"tall"		"13"
				"weight"	"0"
				"dropshadow" "1"
				"antialias" "1"
			}
		}
		"Default"
		{
			// Buttons, labels, text etc
			"1"
			{
				"name"		"Noto Sans"
				"antialias"	"1"
				"dropshadow" "1"
				"tall"		"17"
			}
		}
		"DefaultBold"
		{
			// Unsure where this is used
			"1"
			{
				"name"		"Noto Sans"
				"antialias"	"1"
				"dropshadow" "1"
				"tall"		"17"
				"weight"	"1000"
			}
		}
		"DefaultUnderline"
		{
			// Used for links. I removed the
			// underline because it looks awful.
			"1"
			{
				"name"		"Noto Sans"
				"antialias"	"1"
				"tall"		"17"
			}
		}
		"DefaultVerySmall"
		{
			// LOW --- HIGH slider labels,
			// Section headings for the keyboard
			// options screen.
			"1"
			{
				"name"		"Noto Sans"
				"antialias"	"1"
				"tall"		"13"
				"weight"	"0"
			}
		}
		"DefaultLarge"
		{
			// No idea where this is used
			"1"
			{
				"name"		"Noto Sans"
				"antialias"	"1"
				"tall"		"21"
				"weight"	"0"
			}
		}
		"UiBold"
		{
			// CHAPTER 1 etc on the new game
			// dialogue
			"1"
			{
				"name"		"Noto Sans"
				"antialias"	"1"
				"tall"		"17"
				"dropshadow" "1"
			}
		}
		"FrameTitle"
		{
			// Title of VGUI popups
			"1"
			{
				"name"		"League Gothic"
				"tall"		"23"
				"antialias"	"1"
				"dropshadow" "1"
			}
		}
		"EstrangedMenuHeader"
		{
			"1"
			{
				"name"		"League Gothic"
				"tall"		"50"
				"dropshadow""1"
				"antialias" "1"
				"yres"	"480 599"
			}
			"2"
			{
				"name"		"League Gothic"
				"tall"		"65"
				"dropshadow""1"
				"antialias" "1"
				"yres"	"600 767"
			}
			"3"
			{
				"name"		"League Gothic"
				"tall"		"80"
				"dropshadow""1"
				"antialias" "1"
				"yres"	"768 1023"
			}
			"4"
			{
				"name"		"League Gothic"
				"tall"		"95"
				"dropshadow""1"
				"antialias" "1"
				"yres"	"1024 1199"
			}
			"5"
			{
				"name"		"League Gothic"
				"tall"		"110"
				"dropshadow""1"
				"antialias" "1"
				"yres"	"1200 6000"
			}
		}
		"EstrangedMenuItem"
		{
			"1"
			{
				"name"		"Linden Hill"
				"tall"		"20"
				"dropshadow""1"
				"antialias" "1"
				"yres"		"480 599"
			}
			"2"
			{
				"name"		"Linden Hill"
				"tall"		"30"
				"dropshadow""1"
				"antialias" "1"
				"yres"		"600 767"
			}
			"3"
			{
				"name"		"Linden Hill"
				"tall"		"40"
				"dropshadow""1"
				"antialias" "1"
				"yres"		"768 1023"
			}
			"4"
			{
				"name"		"Linden Hill"
				"tall"		"50"
				"dropshadow""1"
				"antialias" "1"
				"yres"		"1024 1199"
			}
			"5"
			{
				"name"		"Linden Hill"
				"tall"		"60"
				"dropshadow""1"
				"antialias" "1"
				"yres"		"1200 6000"
			}
		}
		// this is the symbol font
		"Marlett"
		{
			"1"
			{
				"name"		"Marlett"
				"tall"		"14"
				"weight"	"0"
				"symbol"	"1"
			}
		}

		"Trebuchet24"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"24"
				"weight"	"900"
			}
		}

		"Trebuchet20"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"20"
				"weight"	"900"
			}
		}

		"Trebuchet18"
		{
			"1"
			{
				"name"		"Trebuchet MS"
				"tall"		"18"
				"weight"	"900"
			}
		}
	}

	//
	//////////////////// BORDERS //////////////////////////////
	//
	// describes all the border types
	Borders
	{
		BaseBorder		DepressedBorder
		ButtonBorder	RaisedBorder
		ComboBoxBorder	DepressedBorder
		MenuBorder		RaisedBorder
		BrowserBorder	DepressedBorder
		PropertySheetBorder	RaisedBorder

		FrameBorder
		{
			"backgroundtype" "0"
		}
		
		BrowserBorder
		{
			"backgroundtype" "0"
		}

		DepressedBorder
		{
			"inset" "0 0 1 1"
			Left
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 1"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}
		}
		RaisedBorder
		{
			"inset" "0 0 1 1"
			Left
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 1"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 1"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}
		}
		
		TitleButtonBorder
		{
			"backgroundtype" "0"
		}

		TitleButtonDisabledBorder
		{
			"backgroundtype" "0"
		}

		TitleButtonDepressedBorder
		{
			"backgroundtype" "0"
		}

		ScrollBarButtonBorder
		{
			"inset" "2 2 0 0"
			Left
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 1"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}
		}
		
		ScrollBarButtonDepressedBorder
		{
			"inset" "2 2 0 0"
			Left
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 1"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}
		}

		TabBorder
		{
			"inset" "0 0 1 1"
			Left
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 1"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}

		}

		TabActiveBorder
		{
			"inset" "0 0 1 0"
			Left
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}

		}


		ToolTipBorder
		{
			"inset" "0 0 1 0"
			Left
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}
		}

		// this is the border used for default buttons (the button that gets pressed when you hit enter)
		ButtonKeyFocusBorder
		{
			"inset" "0 0 1 1"
			Left
			{
				"1"
				{
					"color" "Border.Selection"
					"offset" "0 0"
				}
				"2"
				{
					"color" "Border.Bright"
					"offset" "0 1"
				}
			}
			Top
			{
				"1"
				{
					"color" "Border.Selection"
					"offset" "0 0"
				}
				"2"
				{
					"color" "Border.Bright"
					"offset" "1 0"
				}
			}
			Right
			{
				"1"
				{
					"color" "Border.Selection"
					"offset" "0 0"
				}
				"2"
				{
					"color" "Border.Dark"
					"offset" "1 0"
				}
			}
			Bottom
			{
				"1"
				{
					"color" "Border.Selection"
					"offset" "0 0"
				}
				"2"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}
		}

		ButtonDepressedBorder
		{
			"inset" "2 1 1 1"
			Left
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 1"
				}
			}

			Right
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "1 0"
				}
			}

			Top
			{
				"1"
				{
					"color" "Border.Dark"
					"offset" "0 0"
				}
			}

			Bottom
			{
				"1"
				{
					"color" "Border.Bright"
					"offset" "0 0"
				}
			}
		}
	}

	//////////////////////// CUSTOM FONT FILES /////////////////////////////
	//
	// specifies all the custom (non-system) font files that need to be loaded to service the above described fonts
	CustomFontFiles
	{
		"1"		"resource/leaguegothic.otf"
		"2"		"resource/lindenhill.otf"
		"3"		"resource/inconsolata.otf"
		"4"		"resource/marlett.ttf"
		"5"		"resource/notosans.ttf"
	}
}
