@ Moth Garden, the grass of the Bug Catching Contest.
@ The contest starts and ends at the gate, see VerdanturfTown_MothGarden_Entrance and BugContest_EventScript_*.

MothGarden_MapScripts::
	map_script MAP_SCRIPT_ON_FRAME_TABLE, MothGarden_OnFrame
	.byte 0

MothGarden_OnFrame:
	map_script_2 VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_NONE, MothGarden_EventScript_ContestNotRunning
	.2byte 0

@ Only the contest takes the player here. Anyone who walks in some other way goes back to the gate.
MothGarden_EventScript_ContestNotRunning::
	lockall
	msgbox MothGarden_Text_ContestNotRunning, MSGBOX_DEFAULT
	closemessage
	warp MAP_VERDANTURF_TOWN_MOTH_GARDEN_ENTRANCE, 0
	waitstate
	end

MothGarden_Text_ContestNotRunning:
	.string "The BUG CATCHING CONTEST isn't\n"
	.string "running. Please talk to the officer\l"
	.string "at the gate first.$"
