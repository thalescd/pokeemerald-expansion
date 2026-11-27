@ Gate of the Bug Catching Contest. The officer enters the player, announces the results and holds the prize.
@ The rivals only show up here after a contest, see BugContest_Finish in src/bug_catching_contest.c.
@
@ Map objects to place in Porymap (graphics and position are free):
@   Officer      script VerdanturfTown_MothGarden_Entrance_EventScript_Officer
@   Trigger      coord event on the tile in front of the counter, script VerdanturfTown_MothGarden_Entrance_EventScript_EntranceTrigger
@   Rival N      script VerdanturfTown_MothGarden_Entrance_EventScript_RivalN, flag FLAG_HIDE_BUG_CONTEST_RIVAL_N (N = 1 to 10)
@   Receptionist script VerdanturfTown_MothGarden_Entrance_EventScript_Receptionist, at (6,9) facing left behind the counter
@   Sign         bg event, script VerdanturfTown_MothGarden_Entrance_EventScript_RankingBoard
@ After a contest the player is walked to (4,9), in front of the receptionist, who announces the results.

VerdanturfTown_MothGarden_Entrance_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, VerdanturfTown_MothGarden_Entrance_OnTransition
	map_script MAP_SCRIPT_ON_FRAME_TABLE, VerdanturfTown_MothGarden_Entrance_OnFrame
	.byte 0

@ The rivals are only here right after a contest, so they are hidden every other time the map loads.
VerdanturfTown_MothGarden_Entrance_OnTransition:
	call_if_ne VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_RESULTS, VerdanturfTown_MothGarden_Entrance_EventScript_HideRivals
	end

VerdanturfTown_MothGarden_Entrance_EventScript_HideRivals::
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_1
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_2
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_3
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_4
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_5
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_6
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_7
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_8
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_9
	setflag FLAG_HIDE_BUG_CONTEST_RIVAL_10
	return

VerdanturfTown_MothGarden_Entrance_OnFrame:
	map_script_2 VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_RESULTS, VerdanturfTown_MothGarden_Entrance_EventScript_WalkToReceptionist
	map_script_2 VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_IN_CONTEST, VerdanturfTown_MothGarden_Entrance_EventScript_LeftContestEarly
	.2byte 0

@ Every end of a contest brings the player back on the warp at (4,21). The receptionist, at (6,9) behind the counter
@ at (5,9), announces the results to the player standing at (4,9).
VerdanturfTown_MothGarden_Entrance_EventScript_WalkToReceptionist::
	lockall
	applymovement LOCALID_PLAYER, VerdanturfTown_MothGarden_Entrance_Movement_WalkToReceptionist
	waitmovement 0
	goto BugContest_EventScript_Results
	end

VerdanturfTown_MothGarden_Entrance_Movement_WalkToReceptionist:
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	walk_up
	face_right
	step_end

VerdanturfTown_MothGarden_Entrance_EventScript_Receptionist::
	lockall
	faceplayer
	goto_if_ne VAR_BUG_CONTEST_PRIZE, ITEM_NONE, VerdanturfTown_MothGarden_Entrance_EventScript_HandOverPrize
	msgbox VerdanturfTown_MothGarden_Entrance_Text_ReceptionistInfo, MSGBOX_DEFAULT
	releaseall
	end

@ The board shows the first three places of the last contest. They are only kept in memory.
VerdanturfTown_MothGarden_Entrance_EventScript_RankingBoard::
	lockall
	special BugContest_CheckBoard
	goto_if_eq VAR_RESULT, FALSE, VerdanturfTown_MothGarden_Entrance_EventScript_BoardEmpty
	msgbox VerdanturfTown_MothGarden_Entrance_Text_BoardTitle, MSGBOX_DEFAULT
	setvar VAR_0x8004, 0
	special BugContest_BufferPlace
	msgbox VerdanturfTown_MothGarden_Entrance_Text_BoardFirst, MSGBOX_DEFAULT
	setvar VAR_0x8004, 1
	special BugContest_BufferPlace
	msgbox VerdanturfTown_MothGarden_Entrance_Text_BoardSecond, MSGBOX_DEFAULT
	setvar VAR_0x8004, 2
	special BugContest_BufferPlace
	msgbox VerdanturfTown_MothGarden_Entrance_Text_BoardThird, MSGBOX_DEFAULT
	releaseall
	end

VerdanturfTown_MothGarden_Entrance_EventScript_BoardEmpty::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_BoardEmpty, MSGBOX_DEFAULT
	releaseall
	end

@ The player walked out of the garden while the contest was still running.
VerdanturfTown_MothGarden_Entrance_EventScript_LeftContestEarly::
	lockall
	msgbox VerdanturfTown_MothGarden_Entrance_Text_WantToFinish, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BugContest_EventScript_EndContest
	msgbox VerdanturfTown_MothGarden_Entrance_Text_OkGoFinish, MSGBOX_DEFAULT
	closemessage
	warp MAP_MOTH_GARDEN, 0
	waitstate
	end

@ Talking to the officer across the counter.
VerdanturfTown_MothGarden_Entrance_EventScript_Officer::
	lockall
	faceplayer
	goto VerdanturfTown_MothGarden_Entrance_EventScript_OfferEntry
	end

@ Trigger on the tile in front of the counter. It is the only way from the doors to the garden side of the gate,
@ so everyone walking west goes through it. Walking the other way (east) does nothing.
VerdanturfTown_MothGarden_Entrance_EventScript_EntranceTrigger::
	lockall
	specialvar VAR_RESULT, GetPlayerFacingDirection
	goto_if_ne VAR_RESULT, DIR_WEST, VerdanturfTown_MothGarden_Entrance_EventScript_EntranceTriggerIgnore
	applymovement LOCALID_PLAYER, Common_Movement_WalkInPlaceFasterUp
	waitmovement 0
	goto VerdanturfTown_MothGarden_Entrance_EventScript_OfferEntry
	end

VerdanturfTown_MothGarden_Entrance_EventScript_EntranceTriggerIgnore::
	releaseall
	end

VerdanturfTown_MothGarden_Entrance_EventScript_OfferEntry::
	goto_if_ne VAR_BUG_CONTEST_PRIZE, ITEM_NONE, VerdanturfTown_MothGarden_Entrance_EventScript_HandOverPrize
	showmoneybox 0, 0
	msgbox VerdanturfTown_MothGarden_Entrance_Text_WouldYouLikeToJoin, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, VerdanturfTown_MothGarden_Entrance_EventScript_DeclineEntry
	special BugContest_CheckEntry
	goto_if_eq VAR_RESULT, 1, VerdanturfTown_MothGarden_Entrance_EventScript_FirstMonIsEgg
	goto_if_eq VAR_RESULT, 2, VerdanturfTown_MothGarden_Entrance_EventScript_FirstMonFainted
	goto_if_eq VAR_RESULT, 3, VerdanturfTown_MothGarden_Entrance_EventScript_NoRoom
	checkmoney BUG_CONTEST_ENTRY_FEE
	goto_if_eq VAR_RESULT, FALSE, VerdanturfTown_MothGarden_Entrance_EventScript_NotEnoughMoney
	getpartysize
	goto_if_gt VAR_RESULT, 1, VerdanturfTown_MothGarden_Entrance_EventScript_AskLeaveMons
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StartContest
	end

VerdanturfTown_MothGarden_Entrance_EventScript_AskLeaveMons::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_AskToUseFirstMon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, VerdanturfTown_MothGarden_Entrance_EventScript_RefusedToLeaveMons
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StartContest
	end

VerdanturfTown_MothGarden_Entrance_EventScript_StartContest::
	playse SE_SHOP
	msgbox VerdanturfTown_MothGarden_Entrance_Text_ThatWillBe5000Please, MSGBOX_DEFAULT
	removemoney BUG_CONTEST_ENTRY_FEE
	updatemoneybox
	msgbox VerdanturfTown_MothGarden_Entrance_Text_HereAreYourParkBalls, MSGBOX_DEFAULT
	playfanfare MUS_OBTAIN_ITEM
	message VerdanturfTown_MothGarden_Entrance_Text_ReceivedParkBalls
	waitfanfare
	msgbox VerdanturfTown_MothGarden_Entrance_Text_ExplainRules, MSGBOX_DEFAULT
	closemessage
	hidemoneybox
	special BugContest_Start
	warp MAP_MOTH_GARDEN, 0
	waitstate
	end

VerdanturfTown_MothGarden_Entrance_EventScript_HandOverPrize::
	call BugContest_EventScript_GivePrize
	releaseall
	end

@ Every way of not entering steps the player back from the counter, so the trigger does not fire again at once.
VerdanturfTown_MothGarden_Entrance_EventScript_DeclineEntry::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_TakePartInFuture, MSGBOX_DEFAULT
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter
	end

VerdanturfTown_MothGarden_Entrance_EventScript_RefusedToLeaveMons::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_ChooseMonAndComeBack, MSGBOX_DEFAULT
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter
	end

VerdanturfTown_MothGarden_Entrance_EventScript_FirstMonIsEgg::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_EggAsFirstMon, MSGBOX_DEFAULT
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter
	end

VerdanturfTown_MothGarden_Entrance_EventScript_FirstMonFainted::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_FirstMonCantBattle, MSGBOX_DEFAULT
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter
	end

VerdanturfTown_MothGarden_Entrance_EventScript_NoRoom::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_MakeRoom, MSGBOX_DEFAULT
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter
	end

VerdanturfTown_MothGarden_Entrance_EventScript_NotEnoughMoney::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_NotEnoughMoney, MSGBOX_DEFAULT
	goto VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter
	end

VerdanturfTown_MothGarden_Entrance_EventScript_StepBackFromCounter::
	closemessage
	hidemoneybox
	applymovement LOCALID_PLAYER, VerdanturfTown_MothGarden_Entrance_Movement_StepBackFromCounter
	waitmovement 0
	releaseall
	end

VerdanturfTown_MothGarden_Entrance_Movement_StepBackFromCounter:
	walk_right
	step_end

@ Rival N talks about their result. gSpecialVar_0x8004 holds the rival index, 0 to 9.
VerdanturfTown_MothGarden_Entrance_EventScript_Rival1::
	setvar VAR_0x8004, 0
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival2::
	setvar VAR_0x8004, 1
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival3::
	setvar VAR_0x8004, 2
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival4::
	setvar VAR_0x8004, 3
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival5::
	setvar VAR_0x8004, 4
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival6::
	setvar VAR_0x8004, 5
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival7::
	setvar VAR_0x8004, 6
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival8::
	setvar VAR_0x8004, 7
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival9::
	setvar VAR_0x8004, 8
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_Rival10::
	setvar VAR_0x8004, 9
	goto VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival
	end

VerdanturfTown_MothGarden_Entrance_EventScript_TalkToRival::
	lock
	faceplayer
	special BugContest_BufferRival
	goto_if_eq VAR_RESULT, FALSE, VerdanturfTown_MothGarden_Entrance_EventScript_RivalNotCompeting
	msgbox VerdanturfTown_MothGarden_Entrance_Text_RivalResult, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_MothGarden_Entrance_EventScript_RivalNotCompeting::
	msgbox VerdanturfTown_MothGarden_Entrance_Text_RivalNotCompeting, MSGBOX_DEFAULT
	release
	end

VerdanturfTown_MothGarden_Entrance_Text_WouldYouLikeToJoin:
	.string "Welcome to the\n"
	.string "BUG CATCHING CONTEST!\p"
	.string "Catch the best bug POKéMON in the\n"
	.string "MOTH GARDEN and compete for prizes.\p"
	.string "The entry fee is ¥5,000.\n"
	.string "Would you like to take part?$"

VerdanturfTown_MothGarden_Entrance_Text_AskToUseFirstMon:
	.string "Only your first POKéMON takes part.\p"
	.string "We'll hold on to the rest of your\n"
	.string "party until the CONTEST is over.\p"
	.string "Is that all right?$"

VerdanturfTown_MothGarden_Entrance_Text_ThatWillBe5000Please:
	.string "That will be ¥5,000, please.$"

VerdanturfTown_MothGarden_Entrance_Text_HereAreYourParkBalls:
	.string "Here are your PARK BALLS.$"

VerdanturfTown_MothGarden_Entrance_Text_ReceivedParkBalls:
	.string "{PLAYER} received 20 PARK BALLS!$"

VerdanturfTown_MothGarden_Entrance_Text_ExplainRules:
	.string "You start with 20 PARK BALLS for\n"
	.string "catching POKéMON in the MOTH GARDEN.\p"
	.string "The CONTEST is over when you run\n"
	.string "out of PARK BALLS, or when you've\l"
	.string "walked 250 steps.\p"
	.string "You can only keep one POKéMON.\n"
	.string "The rest are released.\p"
	.string "Whoever catches the best POKéMON\n"
	.string "wins. Good luck!$"

VerdanturfTown_MothGarden_Entrance_Text_TakePartInFuture:
	.string "Please take part in the next one!$"

VerdanturfTown_MothGarden_Entrance_Text_ChooseMonAndComeBack:
	.string "Choose the POKéMON you want to use\n"
	.string "and come back.$"

VerdanturfTown_MothGarden_Entrance_Text_EggAsFirstMon:
	.string "Your first POKéMON is an EGG.\p"
	.string "Please put a POKéMON that can battle\n"
	.string "first and come back.$"

VerdanturfTown_MothGarden_Entrance_Text_FirstMonCantBattle:
	.string "Your first POKéMON can't battle.\p"
	.string "Please heal it or choose another one\n"
	.string "and come back.$"

VerdanturfTown_MothGarden_Entrance_Text_MakeRoom:
	.string "Your party and your BOXES are full.\p"
	.string "Please make room for the POKéMON\n"
	.string "you may catch and come back.$"

VerdanturfTown_MothGarden_Entrance_Text_NotEnoughMoney:
	.string "You don't have enough money.$"

VerdanturfTown_MothGarden_Entrance_Text_WantToFinish:
	.string "Do you want to finish the CONTEST now?$"

VerdanturfTown_MothGarden_Entrance_Text_OkGoFinish:
	.string "Okay! Go catch more bugs!$"

VerdanturfTown_MothGarden_Entrance_Text_RivalResult:
	.string "{STR_VAR_1}:\n"
	.string "I caught a {STR_VAR_2}.\p"
	.string "It scored {STR_VAR_3} points.$"

VerdanturfTown_MothGarden_Entrance_Text_RivalNotCompeting:
	.string "I wasn't in this CONTEST.\n"
	.string "Maybe next time!$"

VerdanturfTown_MothGarden_Entrance_Text_ReceptionistInfo:
	.string "The results of the last CONTEST are\n"
	.string "posted on the board.\p"
	.string "If you want to take part, please\n"
	.string "talk to the officer at the counter.$"

VerdanturfTown_MothGarden_Entrance_Text_BoardEmpty:
	.string "BUG CATCHING CONTEST\n"
	.string "No CONTEST has been held yet.$"

VerdanturfTown_MothGarden_Entrance_Text_BoardTitle:
	.string "BUG CATCHING CONTEST\n"
	.string "Results of the last CONTEST$"

VerdanturfTown_MothGarden_Entrance_Text_BoardFirst:
	.string "1st: {STR_VAR_1}\n"
	.string "{STR_VAR_2}, {STR_VAR_3} points$"

VerdanturfTown_MothGarden_Entrance_Text_BoardSecond:
	.string "2nd: {STR_VAR_1}\n"
	.string "{STR_VAR_2}, {STR_VAR_3} points$"

VerdanturfTown_MothGarden_Entrance_Text_BoardThird:
	.string "3rd: {STR_VAR_1}\n"
	.string "{STR_VAR_2}, {STR_VAR_3} points$"
