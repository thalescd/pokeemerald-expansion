RustboroCity_Flat2_2F_MapScripts::
	.byte 0

RustboroCity_Flat2_2F_EventScript_OldMan::
	msgbox RustboroCity_Flat2_2F_Text_DevonWasTinyInOldDays, MSGBOX_NPC
	end

RustboroCity_Flat2_2F_EventScript_NinjaBoy::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_PREMIER_BALL_RUSTBORO, RustboroCity_Flat2_2F_EventScript_GavePremierBall
	msgbox RustboroCity_Flat2_2F_Text_MyDaddyMadeThisYouCanHaveIt, MSGBOX_DEFAULT
	giveitem ITEM_PREMIER_BALL
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_PREMIER_BALL_RUSTBORO
	release
	end

RustboroCity_Flat2_2F_EventScript_GavePremierBall::
	msgbox RustboroCity_Flat2_2F_Text_GoingToWorkAtDevonToo, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat2_2F_EventScript_FloatStoneGuy::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_FLOAT_STONE_RUSTBORO, RustboroCity_Flat2_2F_EventScript_GaveFloatStone
	msgbox RustboroCity_Flat2_2F_Text_GiveFloatStone, MSGBOX_DEFAULT
	giveitem ITEM_FLOAT_STONE
	goto_if_eq VAR_RESULT, FALSE, Common_EventScript_ShowBagIsFull
	setflag FLAG_RECEIVED_FLOAT_STONE_RUSTBORO
	goto RustboroCity_Flat2_2F_EventScript_GaveFloatStone

RustboroCity_Flat2_2F_EventScript_GaveFloatStone::
	msgbox RustboroCity_Flat2_2F_Text_ExplainFloatStone, MSGBOX_DEFAULT
	release
	end

RustboroCity_Flat2_2F_Text_DevonWasTinyInOldDays:
	.string "Way back in the old days, DEVON was just\n"
	.string "a teeny, tiny company.$"

RustboroCity_Flat2_2F_Text_MyDaddyMadeThisYouCanHaveIt:
	.string "My daddy's working at the CORPORATION.\p"
	.string "My daddy made this!\n"
	.string "But I can't use it, so you can have it.$"

RustboroCity_Flat2_2F_Text_GoingToWorkAtDevonToo:
	.string "My daddy's working at the CORPORATION.\p"
	.string "When I grow up, I'm going to work for\n"
	.string "DEVON, too.$"

RustboroCity_Flat2_2F_Text_GiveFloatStone:
	.string "My dad's a real ace in the DEVON\n"
	.string "CORPORATION, I'll have you know!\l"
	.string "It's no wonder I'm an ace myself!\p"
	.string "Here, I'll give you something that\n"
	.string "an ACE TRAINER like me uses!$"

RustboroCity_Flat2_2F_Text_ExplainFloatStone:
	.string "If you have a POKéMON hold a Float\n"
	.string "Stone, its weight will be halved.\p"
	.string "Although it looks like it'll be a while\n"
	.string "before you can use something like this\l"
	.string "as well as I can.$"
