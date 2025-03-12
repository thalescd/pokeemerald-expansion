SlateportCity_House_MapScripts::
	.byte 0

SlateportCity_House_EventScript_PokefanHastyMint::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_SLATEPORT_HASTY_MINT, SlateportCity_House_EventScript_PokefanM
	msgbox SlateportCity_House_Text_NatureToDoWithStatGains, MSGBOX_NPC
	giveitem ITEM_HASTY_MINT
	setflag FLAG_RECEIVED_SLATEPORT_HASTY_MINT
	msgbox SlateportCity_House_Text_HastyMintExplanation, MSGBOX_NPC
	closemessage
	waitmovement 0
	release
	end

SlateportCity_House_EventScript_PokefanM::
	msgbox SlateportCity_House_Text_NatureToDoWithStatGains, MSGBOX_NPC
	closemessage
	waitmovement 0
	release
	end

SlateportCity_House_EventScript_Girl::
	msgbox SlateportCity_House_Text_MustBeGoingToBattleTent, MSGBOX_NPC
	end

SlateportCity_House_Text_NatureToDoWithStatGains:
	.string "My POKéMON has a HASTY nature.\p"
	.string "It has higher SPEED compared to\n"
	.string "my other POKéMON.\p"
	.string "Maybe their nature has something to\n"
	.string "do with the stat gains of POKéMON.$"

SlateportCity_House_Text_HastyMintExplanation:
	.string "If you give this to your POKéMON,\n"
	.string "they will change their natures.\p"
	.string "Pretty useful, right?$"

SlateportCity_House_Text_MustBeGoingToBattleTent:
	.string "You're a TRAINER, aren't you?\p"
	.string "Since you came to SLATEPORT CITY,\n"
	.string "you must be going to the BATTLE TENT.$"

