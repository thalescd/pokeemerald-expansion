LilycoveCity_DepartmentStoreRooftop_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, LilycoveCity_DepartmentStoreRooftop_OnTransition
	.byte 0

LilycoveCity_DepartmentStoreRooftop_OnTransition:
	getpokenewsactive POKENEWS_LILYCOVE
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_DepartmentStoreRooftop_EventScript_ShowSaleWoman
	call_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStoreRooftop_EventScript_HideSaleWoman
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_ShowSaleWoman::
	clearflag FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_SALE_WOMAN
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_HideSaleWoman::
	setflag FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_SALE_WOMAN
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_SaleWoman::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	pokemartdecoration LilycoveCity_DepartmentStoreRooftop_PokemartDecor_ClearOutSale
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
LilycoveCity_DepartmentStoreRooftop_PokemartDecor_ClearOutSale:
	.2byte DECOR_MUD_BALL
	.2byte DECOR_FENCE_LENGTH
	.2byte DECOR_FENCE_WIDTH
	.2byte DECOR_TIRE
	.2byte DECOR_BREAKABLE_DOOR
	.2byte DECOR_SOLID_BOARD
	.2byte DECOR_SAND_ORNAMENT
	.2byte DECOR_STAND
	.2byte DECOR_SLIDE
	.2byte DECOR_TV
	.2byte DECOR_ROUND_TV
	.2byte DECOR_CUTE_TV
	.2byte DECOR_WAILMER_DOLL
	.2byte DECOR_RHYDON_DOLL
	.2byte 0
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_Man::
	lock
	faceplayer
	getpokenewsactive POKENEWS_LILYCOVE
	call_if_eq VAR_RESULT, TRUE, LilycoveCity_DepartmentStoreRooftop_EventScript_ManClearOutSale
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_SetDatesForClearOutSales, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_ManClearOutSale::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_BeenWaitingForClearOutSale, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_ThirstyMan::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_BoneDryThirsty, MSGBOX_NPC
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_VendingMachine::
	lockall
	message LilycoveCity_DepartmentStoreRooftop_Text_WhichDrinkWouldYouLike
	waitmessage
	showmoneybox 0, 0
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseDrink
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseDrink::
	multichoice 16, 0, MULTI_VENDING_MACHINE, FALSE
	copyvar VAR_TEMP_1, VAR_RESULT
	switch VAR_TEMP_1
	case 0, LilycoveCity_DepartmentStoreRooftop_EventScript_FreshWater
	case 1, LilycoveCity_DepartmentStoreRooftop_EventScript_SodaPop
	case 2, LilycoveCity_DepartmentStoreRooftop_EventScript_Lemonade
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_DecidedAgainstBuyingDrink, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_ExitVendingMachine
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_FreshWater::
	setvar VAR_TEMP_0, ITEM_FRESH_WATER
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_TryBuyDrink
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_SodaPop::
	setvar VAR_TEMP_0, ITEM_SODA_POP
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_TryBuyDrink
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_Lemonade::
	setvar VAR_TEMP_0, ITEM_LEMONADE
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_TryBuyDrink
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_CheckMoneyFreshWater::
	checkmoney 200
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_CheckMoneySodaPop::
	checkmoney 300
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_CheckMoneyLemonade::
	checkmoney 350
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_RemoveMoneyFreshWater::
	removemoney 200
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_RemoveMoneySodaPop::
	removemoney 300
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_RemoveMoneyLemonade::
	removemoney 350
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_TryBuyDrink::
	call_if_eq VAR_TEMP_1, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_CheckMoneyFreshWater
	call_if_eq VAR_TEMP_1, 1, LilycoveCity_DepartmentStoreRooftop_EventScript_CheckMoneySodaPop
	call_if_eq VAR_TEMP_1, 2, LilycoveCity_DepartmentStoreRooftop_EventScript_CheckMoneyLemonade
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStoreRooftop_EventScript_NotEnoughMoneyForDrink
	checkitemspace VAR_TEMP_0
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStoreRooftop_EventScript_NoRoomForDrink
	call_if_eq VAR_TEMP_1, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_RemoveMoneyFreshWater
	call_if_eq VAR_TEMP_1, 1, LilycoveCity_DepartmentStoreRooftop_EventScript_RemoveMoneySodaPop
	call_if_eq VAR_TEMP_1, 2, LilycoveCity_DepartmentStoreRooftop_EventScript_RemoveMoneyLemonade
	updatemoneybox
	bufferitemname STR_VAR_1, VAR_TEMP_0
	playse SE_VEND
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_CanOfDrinkDroppedDown, MSGBOX_DEFAULT
	additem VAR_TEMP_0
	bufferitemname STR_VAR_2, VAR_TEMP_0
	bufferstdstring STR_VAR_3, STDSTRING_ITEMS
	msgbox gText_PutItemInPocket, MSGBOX_DEFAULT
	random 64  @ 1/64 chance of an additional drink dropping
	goto_if_ne VAR_RESULT, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseNewDrink
	checkitemspace VAR_TEMP_0
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStoreRooftop_EventScript_NoRoomForDrink
	playse SE_VEND
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ExtraCanOfDrinkDroppedDown, MSGBOX_DEFAULT
	additem VAR_TEMP_0
	bufferitemname STR_VAR_2, VAR_TEMP_0
	bufferstdstring STR_VAR_3, STDSTRING_ITEMS
	msgbox gText_PutItemInPocket, MSGBOX_DEFAULT
	random 64  @ 1/64 * the prev 1/64 chance of a third additional drink dropping, ~ 0.02% chance
	goto_if_ne VAR_RESULT, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseNewDrink
	checkitemspace VAR_TEMP_0
	goto_if_eq VAR_RESULT, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_NoRoomForDrink
	playse SE_VEND
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ExtraCanOfDrinkDroppedDown, MSGBOX_DEFAULT
	additem VAR_TEMP_0
	bufferitemname STR_VAR_2, VAR_TEMP_0
	bufferstdstring STR_VAR_3, STDSTRING_ITEMS
	msgbox gText_PutItemInPocket, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseNewDrink
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseNewDrink::
	message LilycoveCity_DepartmentStoreRooftop_Text_WhichDrinkWouldYouLike
	waitmessage
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_ChooseDrink
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_NotEnoughMoneyForDrink::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_NotEnoughMoney, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_ExitVendingMachine
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_NoRoomForDrink::
	msgbox gText_TheBagIsFull, MSGBOX_DEFAULT
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_ExitVendingMachine
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_ExitVendingMachine::
	hidemoneybox
	releaseall
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_Reporter::
	lockall
	applymovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER, Common_Movement_FaceRight
	applymovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN, Common_Movement_FaceLeft
	waitmovement 0
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ReporterBroadcast, MSGBOX_DEFAULT
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ReporterMicCrackled, MSGBOX_DEFAULT
	faceplayer
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ReporterAskColor, MSGBOX_DEFAULT
	multichoice 22, 8, MULTI_TV_LATI, TRUE
	copyvar VAR_0x8004, VAR_RESULT
	special InitRoamer
	copyvar VAR_ROAMER_POKEMON, VAR_RESULT
	call_if_eq VAR_ROAMER_POKEMON, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_BufferRed
	call_if_ne VAR_ROAMER_POKEMON, 0, LilycoveCity_DepartmentStoreRooftop_EventScript_BufferBlue
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ReporterHeardLoudAndClear, MSGBOX_DEFAULT
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_ReporterWrapUp, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN, LilycoveCity_DepartmentStoreRooftop_Movement_CameramanExit
	applymovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER, LilycoveCity_DepartmentStoreRooftop_Movement_ReporterExit
	waitmovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN
	playse SE_EXIT
	removeobject LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN
	waitmovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER
	playse SE_EXIT
	removeobject LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER
	setflag FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER
	releaseall
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_BufferRed::
	bufferstring STR_VAR_1, LilycoveCity_DepartmentStoreRooftop_Text_Red
	return

LilycoveCity_DepartmentStoreRooftop_EventScript_BufferBlue::
	bufferstring STR_VAR_1, LilycoveCity_DepartmentStoreRooftop_Text_Blue
	return

@ The stairs are at the top right (13, 3). The cameraman goes first so the reporter
@ doesn't walk into him, and the player talking to the reporter is standing at (11, 4).
LilycoveCity_DepartmentStoreRooftop_Movement_CameramanExit:
	walk_up
	walk_right
	walk_up
	step_end

LilycoveCity_DepartmentStoreRooftop_Movement_ReporterExit:
	delay_16
	walk_right
	walk_right
	walk_up
	walk_up
	step_end

LilycoveCity_DepartmentStoreRooftop_EventScript_Cameraman::
	lock
	faceplayer
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_CameramanDontLookAtMe, MSGBOX_DEFAULT
	closemessage
	applymovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN, Common_Movement_FaceDown
	waitmovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_CAMERAMAN
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_Enthusiast::
	lock
	goto_if_unset FLAG_HIDE_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_REPORTER, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastBeingInterviewed
	faceplayer
	goto_if_set FLAG_RECEIVED_EON_FLUTE, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastAfterFlute
	checkspecies SPECIES_LATIAS
	goto_if_eq VAR_RESULT, TRUE, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastFluteReacts
	checkspecies SPECIES_LATIOS
	goto_if_eq VAR_RESULT, TRUE, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastFluteReacts
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastAboutEonPokemon, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastBeingInterviewed::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastBeingInterviewed, MSGBOX_DEFAULT
	release
	end

@ The flute only reacts to a LATIAS or LATIOS. Whichever one the player shows decides which one it calls
LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastFluteReacts::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastFluteReacts, MSGBOX_DEFAULT
	special ChoosePartyMon
	goto_if_ge VAR_0x8004, PARTY_SIZE, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastNotNow
	specialvar VAR_RESULT, ScriptGetPartyMonSpecies
	goto_if_eq VAR_RESULT, SPECIES_LATIAS, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastShownLatias
	goto_if_eq VAR_RESULT, SPECIES_LATIOS, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastShownLatios
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastNotThatOne, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastNotNow::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastNotNow, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastShownLatias::
	setflag FLAG_EON_LATI
	bufferstring STR_VAR_1, LilycoveCity_DepartmentStoreRooftop_Text_Latias
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastGiveFlute
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastShownLatios::
	clearflag FLAG_EON_LATI
	bufferstring STR_VAR_1, LilycoveCity_DepartmentStoreRooftop_Text_Latios
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastGiveFlute
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastGiveFlute::
	playse SE_PIN
	applymovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_ENTHUSIAST, Common_Movement_ExclamationMark
	waitmovement LOCALID_LILYCOVE_DEPARTMENT_STORE_ROOFTOP_ENTHUSIAST
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastFluteSinging, MSGBOX_DEFAULT
	giveitem ITEM_EON_FLUTE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_EON_FLUTE
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastIslandHint, MSGBOX_DEFAULT
	release
	end

@ With both in the party the player can pick which one the flute calls
LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastAfterFlute::
	checkspecies SPECIES_LATIAS
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastChat
	checkspecies SPECIES_LATIOS
	goto_if_eq VAR_RESULT, FALSE, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastChat
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastBothLati, MSGBOX_DEFAULT
	multichoice 20, 6, MULTI_EON_FLUTE_LATI, FALSE
	switch VAR_RESULT
	case 0, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastCallLatias
	case 1, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastCallLatios
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastTakeYourTime, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastCallLatias::
	setflag FLAG_EON_LATI
	bufferstring STR_VAR_1, LilycoveCity_DepartmentStoreRooftop_Text_Latias
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastFluteWillCall
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastCallLatios::
	clearflag FLAG_EON_LATI
	bufferstring STR_VAR_1, LilycoveCity_DepartmentStoreRooftop_Text_Latios
	goto LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastFluteWillCall
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastFluteWillCall::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastFluteCalls, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastChat::
	goto_if_set FLAG_LANDMARK_SOUTHERN_ISLAND, LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastBeenToIsland
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastHowsTheFlute, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_EventScript_EnthusiastBeenToIsland::
	msgbox LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastBeenToIsland, MSGBOX_DEFAULT
	release
	end

LilycoveCity_DepartmentStoreRooftop_Text_SetDatesForClearOutSales:
	.string "Don't they have set dates for their\n"
	.string "clear-out sales?\p"
	.string "I watch TV, but they never show any\n"
	.string "commercials.$"

LilycoveCity_DepartmentStoreRooftop_Text_BeenWaitingForClearOutSale:
	.string "Yes! I've been waiting a long time for\n"
	.string "this clear-out sale.\p"
	.string "They have items that you can only get\n"
	.string "here. I'm going to load up, that I am!$"

LilycoveCity_DepartmentStoreRooftop_Text_BoneDryThirsty:
	.string "Ohh… I'm bone-dry thirsty!$"

LilycoveCity_DepartmentStoreRooftop_Text_WhichDrinkWouldYouLike:
	.string "It's a VENDING MACHINE.\n"
	.string "Which drink would you like?$"

LilycoveCity_DepartmentStoreRooftop_Text_CanOfDrinkDroppedDown:
	.string "Clang!\p"
	.string "A can of {STR_VAR_1} dropped down.$"

LilycoveCity_DepartmentStoreRooftop_Text_ExtraCanOfDrinkDroppedDown:
	.string "Clang!\p"
	.string "Score! An extra can of {STR_VAR_1}\n"
	.string "dropped down!$"

LilycoveCity_DepartmentStoreRooftop_Text_NotEnoughMoney:
	.string "Not enough money…$"

LilycoveCity_DepartmentStoreRooftop_Text_DecidedAgainstBuyingDrink:
	.string "Decided against buying a drink.$"

LilycoveCity_DepartmentStoreRooftop_Text_Red:
	.string "RED$"

LilycoveCity_DepartmentStoreRooftop_Text_Blue:
	.string "BLUE$"

LilycoveCity_DepartmentStoreRooftop_Text_Latias:
	.string "LATIAS$"

LilycoveCity_DepartmentStoreRooftop_Text_Latios:
	.string "LATIOS$"

LilycoveCity_DepartmentStoreRooftop_Text_ReporterBroadcast:
	.string "We bring you this emergency\n"
	.string "news flash!\p"
	.string "In various HOENN locales, there have\n"
	.string "been reports of a BZZT…colored\l"
	.string "POKéMON in flight.\p"
	.string "The identity of this POKéMON is\n"
	.string "currently unknown.$"

LilycoveCity_DepartmentStoreRooftop_Text_ReporterMicCrackled:
	.string "Hm? Cut, cut! Did the mic crackle\n"
	.string "again?!$"

LilycoveCity_DepartmentStoreRooftop_Text_ReporterAskColor:
	.string "You there, TRAINER! Did you hear\n"
	.string "what I just said?\p"
	.string "What color was that POKéMON?$"

LilycoveCity_DepartmentStoreRooftop_Text_ReporterHeardLoudAndClear:
	.string "{STR_VAR_1}, you say? So you heard it\n"
	.string "loud and clear!\p"
	.string "Then it must be coming through fine\n"
	.string "on the broadcast, too. Phew!$"

LilycoveCity_DepartmentStoreRooftop_Text_ReporterWrapUp:
	.string "Well, isn't that something!\n"
	.string "There are still unknown POKéMON.\p"
	.string "Thank you for your time, sir!\n"
	.string "We now return you to the regular\l"
	.string "program!$"

LilycoveCity_DepartmentStoreRooftop_Text_CameramanDontLookAtMe:
	.string "Don't look at me. I only point\n"
	.string "the camera.$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastBeingInterviewed:
	.string "Shh! I'm being interviewed!$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastAboutEonPokemon:
	.string "Did you hear the report? A glowing\n"
	.string "POKéMON streaking across HOENN like\l"
	.string "a neon jet!\p"
	.string "Those are the EON POKéMON,\n"
	.string "LATIAS and LATIOS!\p"
	.string "They move from place to place\n"
	.string "in the blink of an eye!\p"
	.string "I would love to see them up close,\n"
	.string "at least once.\p"
	.string "I even got a weird flute that's said\n"
	.string "to be able to call them, but it\l"
	.string "seems broken…$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastFluteReacts:
	.string "Hm? What's that sound?\n"
	.string "The flute… it's glowing!\p"
	.string "It's reacting to something! One of\n"
	.string "your POKéMON, maybe? Show me!$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastNotThatOne:
	.string "Hm… The flute doesn't react to that\n"
	.string "one. It must be another.$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastNotNow:
	.string "Oh… Maybe next time, then.$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastFluteSinging:
	.string "The flute is singing! It's a\n"
	.string "{STR_VAR_1}!\p"
	.string "It was never broken. It just needed\n"
	.string "to be close to one of them!\p"
	.string "I can't believe I'm this close to a\n"
	.string "{STR_VAR_1}! My hands are shaking!\p"
	.string "You must be a truly skilled TRAINER\n"
	.string "to have earned such a magnificent\l"
	.string "POKéMON's trust!\p"
	.string "Take it, {PLAYER}. It belongs with\n"
	.string "someone who can actually fly with it.$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastIslandHint:
	.string "I heard there's an island far to the\n"
	.string "south that no ferry reaches.\p"
	.string "Maybe the flute knows the way.$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastHowsTheFlute:
	.string "How's the flute treating you?\p"
	.string "I still wonder about that island far\n"
	.string "to the south…$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastBeenToIsland:
	.string "You've been to the island, haven't\n"
	.string "you? I can see it in your eyes.\p"
	.string "Nothing beats a story like that!$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastBothLati:
	.string "Both a LATIAS and a LATIOS?!\n"
	.string "The flute is going crazy!\p"
	.string "It can only call one at a time.\n"
	.string "Which one should answer?$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastFluteCalls:
	.string "Understood! From now on, the flute\n"
	.string "will call {STR_VAR_1}.$"

LilycoveCity_DepartmentStoreRooftop_Text_EnthusiastTakeYourTime:
	.string "Take your time. Decide when you're\n"
	.string "ready.$"
