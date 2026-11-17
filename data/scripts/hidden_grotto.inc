@ Shared map scripts of the hidden grotto interiors (the old secret base maps).
@ Object 1 of every interior is the grotto Pokemon; its sprite comes from VAR_OBJ_GFX_ID_F.

HiddenGrotto_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, HiddenGrotto_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, HiddenGrotto_OnFrame
	map_script MAP_SCRIPT_ON_RESUME, HiddenGrotto_OnResume
	.byte 0

HiddenGrotto_OnTransition:
	special SetupHiddenGrottoInterior
	end

HiddenGrotto_OnFrame:
	map_script_2 VAR_HIDDEN_GROTTO_WALKED_IN, FALSE, HiddenGrotto_EventScript_WalkIn
	.2byte 0

@ The player arrives on the exit arrow, so step in off it.
HiddenGrotto_EventScript_WalkIn::
	applymovement LOCALID_PLAYER, HiddenGrotto_Movement_WalkIn
	waitmovement 0
	setvar VAR_HIDDEN_GROTTO_WALKED_IN, TRUE
	end

HiddenGrotto_Movement_WalkIn:
	walk_up
	walk_up
	step_end

HiddenGrotto_OnResume:
	call_if_set FLAG_SYS_CTRL_OBJ_DELETE, HiddenGrotto_EventScript_TryRemovePokemon
	end

HiddenGrotto_EventScript_TryRemovePokemon::
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_ne VAR_RESULT, B_OUTCOME_CAUGHT, Common_EventScript_NopReturn
	removeobject VAR_LAST_TALKED
	return

HiddenGrotto_EventScript_Pokemon::
	lock
	faceplayer
	special CreateHiddenGrottoMon
	goto_if_eq VAR_RESULT, FALSE, HiddenGrotto_EventScript_Release
	special ClearHiddenGrottoMon
	waitse
	playmoncry VAR_0x8004, CRY_MODE_ENCOUNTER
	delay 40
	waitmoncry
	setflag FLAG_SYS_CTRL_OBJ_DELETE
	dowildbattle
	clearflag FLAG_SYS_CTRL_OBJ_DELETE
	specialvar VAR_RESULT, GetBattleOutcome
	goto_if_eq VAR_RESULT, B_OUTCOME_CAUGHT, HiddenGrotto_EventScript_Release
	goto Common_EventScript_RemoveStaticPokemon
	end

HiddenGrotto_EventScript_Release::
	release
	end

@ Entrances: pressing A facing an entrance (or using Secret Power from the party menu) leads here.
@ The grotto is already set from the entrance's bg event. VAR_0x8007 holds the SECRET_BASE_* entrance type.
HiddenGrotto_EventScript_CheckEntrance::
	special GetHiddenGrottoEntranceType
	checkfieldmove FIELD_MOVE_SECRET_POWER
	setfieldeffectargument 0, VAR_RESULT
	buffermovename STR_VAR_2, MOVE_SECRET_POWER
	goto_if_eq VAR_0x8007, SECRET_BASE_RED_CAVE, HiddenGrotto_EventScript_Cave
	goto_if_eq VAR_0x8007, SECRET_BASE_BROWN_CAVE, HiddenGrotto_EventScript_Cave
	goto_if_eq VAR_0x8007, SECRET_BASE_BLUE_CAVE, HiddenGrotto_EventScript_Cave
	goto_if_eq VAR_0x8007, SECRET_BASE_YELLOW_CAVE, HiddenGrotto_EventScript_Cave
	goto_if_eq VAR_0x8007, SECRET_BASE_TREE, HiddenGrotto_EventScript_Tree
	goto_if_eq VAR_0x8007, SECRET_BASE_SHRUB, HiddenGrotto_EventScript_Shrub
	end

HiddenGrotto_EventScript_Cave::
	lockall
	goto_if_eq VAR_RESULT, PARTY_SIZE, HiddenGrotto_EventScript_CaveNoSecretPower
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	msgbox HiddenGrotto_Text_IndentUseSecretPower, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HiddenGrotto_EventScript_CancelOnEntrance
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_SECRET_POWER_CAVE
	waitstate
	goto HiddenGrotto_EventScript_CaveEnter
	end

HiddenGrotto_EventScript_CaveUseSecretPower::
	lockall
	dofieldeffect FLDEFF_USE_SECRET_POWER_CAVE
	waitstate
	goto HiddenGrotto_EventScript_CaveEnter
	end

HiddenGrotto_EventScript_CaveNoSecretPower::
	msgbox HiddenGrotto_Text_SmallIndentInWall, MSGBOX_DEFAULT
	releaseall
	end

HiddenGrotto_EventScript_CaveEnter::
	msgbox HiddenGrotto_Text_DiscoveredSmallCavern, MSGBOX_DEFAULT
	goto HiddenGrotto_EventScript_Enter
	end

HiddenGrotto_EventScript_Tree::
	lockall
	goto_if_eq VAR_RESULT, PARTY_SIZE, HiddenGrotto_EventScript_TreeNoSecretPower
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	msgbox HiddenGrotto_Text_TreeUseSecretPower, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HiddenGrotto_EventScript_CancelOnEntrance
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_SECRET_POWER_TREE
	waitstate
	goto HiddenGrotto_EventScript_TreeEnter
	end

HiddenGrotto_EventScript_TreeUseSecretPower::
	lockall
	dofieldeffect FLDEFF_USE_SECRET_POWER_TREE
	waitstate
	goto HiddenGrotto_EventScript_TreeEnter
	end

HiddenGrotto_EventScript_TreeNoSecretPower::
	msgbox HiddenGrotto_Text_TreeCanBeClimbed, MSGBOX_DEFAULT
	releaseall
	end

HiddenGrotto_EventScript_TreeEnter::
	msgbox HiddenGrotto_Text_VineDroppedDown, MSGBOX_DEFAULT
	goto HiddenGrotto_EventScript_Enter
	end

HiddenGrotto_EventScript_Shrub::
	lockall
	goto_if_eq VAR_RESULT, PARTY_SIZE, HiddenGrotto_EventScript_ShrubNoSecretPower
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	msgbox HiddenGrotto_Text_ClumpUseSecretPower, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, HiddenGrotto_EventScript_CancelOnEntrance
	msgbox Text_MonUsedFieldMove, MSGBOX_DEFAULT
	closemessage
	dofieldeffect FLDEFF_USE_SECRET_POWER_SHRUB
	waitstate
	goto HiddenGrotto_EventScript_ShrubEnter
	end

HiddenGrotto_EventScript_ShrubUseSecretPower::
	lockall
	dofieldeffect FLDEFF_USE_SECRET_POWER_SHRUB
	waitstate
	goto HiddenGrotto_EventScript_ShrubEnter
	end

HiddenGrotto_EventScript_ShrubNoSecretPower::
	msgbox HiddenGrotto_Text_ClumpOfGrass, MSGBOX_DEFAULT
	releaseall
	end

HiddenGrotto_EventScript_ShrubEnter::
	msgbox HiddenGrotto_Text_DiscoveredSmallEntrance, MSGBOX_DEFAULT
	goto HiddenGrotto_EventScript_Enter
	end

HiddenGrotto_EventScript_Enter::
	closemessage
	playse SE_EXIT
	setvar VAR_HIDDEN_GROTTO_WALKED_IN, FALSE
	special EnterHiddenGrotto
	waitstate
	end

HiddenGrotto_EventScript_CancelOnEntrance::
	closemessage
	releaseall
	end

HiddenGrotto_Text_TreeCanBeClimbed:
	.string "If some vines drop down, this tree can\n"
	.string "be climbed.$"

HiddenGrotto_Text_TreeUseSecretPower:
	.string "If some vines drop down, this tree can\n"
	.string "be climbed.\p"
	.string "Use the SECRET POWER?$"

HiddenGrotto_Text_VineDroppedDown:
	.string "A thick vine dropped down!$"

HiddenGrotto_Text_ClumpOfGrass:
	.string "If this clump of grass can be moved,\n"
	.string "it might be possible to go inside.$"

HiddenGrotto_Text_ClumpUseSecretPower:
	.string "If this clump of grass can be moved,\n"
	.string "it might be possible to go inside.\p"
	.string "Use the SECRET POWER?$"

HiddenGrotto_Text_DiscoveredSmallEntrance:
	.string "Discovered a small entrance!$"

HiddenGrotto_Text_SmallIndentInWall:
	.string "There's a small indent in the wall.$"

HiddenGrotto_Text_IndentUseSecretPower:
	.string "There's a small indent in the wall.\p"
	.string "Use the SECRET POWER?$"

HiddenGrotto_Text_DiscoveredSmallCavern:
	.string "Discovered a small cavern!$"
