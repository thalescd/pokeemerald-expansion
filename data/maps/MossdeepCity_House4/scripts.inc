MossdeepCity_House4_MapScripts::
	.byte 0

MossdeepCity_House4_EventScript_Woman::
	lock
	faceplayer
	goto_if_set FLAG_SYS_GAME_CLEAR, MossdeepCity_House4_EventScript_CanBattleAtSecretBases
	msgbox MossdeepCity_House4_Text_BrotherLikesToFindBases, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_CanBattleAtSecretBases::
	msgbox MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle, MSGBOX_DEFAULT
	release
	end

@ TODO: secret bases were removed; this dialogue is waiting for new lines (see plans/GROTTOS.md).
MossdeepCity_House4_EventScript_NinjaBoy::
	lock
	faceplayer
	msgbox MossdeepCity_House4_Text_MakeSecretBase, MSGBOX_DEFAULT
	release
	end

MossdeepCity_House4_EventScript_Snover::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SNOVER, CRY_MODE_NORMAL
	msgbox MossdeepCity_House4_Text_Snover, MSGBOX_DEFAULT
	waitmoncry
	release
	end

MossdeepCity_House4_Text_BrotherLikesToFindBases:
	.string "My little brother says he likes to go\n"
	.string "find people's SECRET BASES.$"

MossdeepCity_House4_Text_BrotherLikesToVisitBasesAndBattle:
	.string "My little brother says he likes to\n"
	.string "visit people's SECRET BASES and have\l"
	.string "POKéMON battles.$"

MossdeepCity_House4_Text_MakeSecretBase:
	.string "You should make a SECRET BASE\n"
	.string "somewhere. I'll go find it!$"

MossdeepCity_House4_Text_Snover:
	.string "SNOVER: Fuuu?$"

