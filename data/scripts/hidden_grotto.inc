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
	map_script_2 VAR_INIT_SECRET_BASE, 0, HiddenGrotto_EventScript_WalkIn
	.2byte 0

@ The player arrives on the exit arrow, so step in off it.
HiddenGrotto_EventScript_WalkIn::
	applymovement LOCALID_PLAYER, HiddenGrotto_Movement_WalkIn
	waitmovement 0
	setvar VAR_INIT_SECRET_BASE, 1
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
