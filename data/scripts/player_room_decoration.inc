@ Decorations in the player's room (dolls and cushions are objects that use FLAG_DECORATION_*).

PlayerRoom_EventScript_SetDecorationFlags::
	setflag FLAG_DECORATION_1
	setflag FLAG_DECORATION_2
	setflag FLAG_DECORATION_3
	setflag FLAG_DECORATION_4
	setflag FLAG_DECORATION_5
	setflag FLAG_DECORATION_6
	setflag FLAG_DECORATION_7
	setflag FLAG_DECORATION_8
	setflag FLAG_DECORATION_9
	setflag FLAG_DECORATION_10
	setflag FLAG_DECORATION_11
	setflag FLAG_DECORATION_12
	setflag FLAG_DECORATION_13
	setflag FLAG_DECORATION_14
	return

PlayerRoom_EventScript_InitDecorations::
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, 0
	special InitPlayerRoomDecorationSprites
	setvar VAR_PLAYER_ROOM_DECOR_INITIALIZED, TRUE
	end

PlayerRoom_EventScript_SetDecoration::
	setvar VAR_0x8005, 0
	special SetDecoration
	end

PlayerRoom_EventScript_PutAwayDecoration::
	setvar VAR_0x8004, 0
	goto PlayerRoom_EventScript_PutAwayDecorationLoop
	end

PlayerRoom_EventScript_PutAwayDecorationLoop::
	special PutAwayDecorationIteration
	goto_if_eq VAR_RESULT, 1, PlayerRoom_EventScript_PutAwayDecorationEnd
	addvar VAR_0x8004, 1
	goto_if_eq VAR_0x8005, 0, PlayerRoom_EventScript_PutAwayDecorationLoop
	removeobject VAR_0x8006
	setflag VAR_0x8005  @ UB: VAR_0x8005 is set to a flag by PutAwayDecorationIteration, but ScrCmd_setflag doesn't use VarGet
	goto PlayerRoom_EventScript_PutAwayDecorationLoop
	end

PlayerRoom_EventScript_PutAwayDecorationEnd::
	end
