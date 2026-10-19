@ Scripts shared by the Bug Catching Contest. The gate map has the officer, the results and the rivals,
@ see data/maps/VerdanturfTown_MothGarden_Entrance/scripts.inc

BugContest_EventScript_RetirePrompt::
	lockall
	msgbox BugContest_Text_WouldYouLikeToQuit, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, YES, BugContest_EventScript_EndContest
	releaseall
	end

BugContest_EventScript_TimesUp::
	lockall
	playse SE_DING_DONG
	message BugContest_Text_TimesUp
	waitmessage
	waitbuttonpress
	goto BugContest_EventScript_EndContest
	end

BugContest_EventScript_OutOfBalls::
	lockall
	playse SE_DING_DONG
	message BugContest_Text_OutOfBalls
	waitmessage
	waitbuttonpress
	goto BugContest_EventScript_EndContest
	end

@ Judges the contest and takes the player to the gate, where the results are announced.
BugContest_EventScript_EndContest::
	special BugContest_Finish
	warp MAP_VERDANTURF_TOWN_MOTH_GARDEN_ENTRANCE, 0
	waitstate
	end

@ Announces the results and hands out the prize. Run by the gate map when VAR_BUG_CONTEST_STATE is set to results.
BugContest_EventScript_Results::
	lockall
	setvar VAR_BUG_CONTEST_STATE, BUG_CONTEST_STATE_NONE
	msgbox BugContest_Text_ReadyToJudge, MSGBOX_DEFAULT
	setvar VAR_0x8004, 2
	special BugContest_BufferPlace
	msgbox BugContest_Text_ThirdPlace, MSGBOX_DEFAULT
	setvar VAR_0x8004, 1
	special BugContest_BufferPlace
	msgbox BugContest_Text_SecondPlace, MSGBOX_DEFAULT
	setvar VAR_0x8004, 0
	special BugContest_BufferPlace
	msgbox BugContest_Text_FirstPlace, MSGBOX_DEFAULT
	special BugContest_BufferPlayerResult
	special BugContest_SetPrizeForPlace
	copyvar VAR_0x8008, VAR_RESULT
	goto_if_eq VAR_0x8005, FALSE, BugContest_EventScript_ResultsNothingCaught
	goto_if_eq VAR_0x8008, 1, BugContest_EventScript_ResultsFirstPlace
	goto_if_eq VAR_0x8008, 2, BugContest_EventScript_ResultsSecondPlace
	goto_if_eq VAR_0x8008, 3, BugContest_EventScript_ResultsThirdPlace
	msgbox BugContest_Text_DidNotPlace, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsGivePrize
	end

BugContest_EventScript_ResultsFirstPlace::
	msgbox BugContest_Text_YouPlacedFirst, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsGivePrize
	end

BugContest_EventScript_ResultsSecondPlace::
	msgbox BugContest_Text_YouPlacedSecond, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsGivePrize
	end

BugContest_EventScript_ResultsThirdPlace::
	msgbox BugContest_Text_YouPlacedThird, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsGivePrize
	end

BugContest_EventScript_ResultsNothingCaught::
	msgbox BugContest_Text_NothingCaught, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsGivePrize
	end

BugContest_EventScript_ResultsGivePrize::
	call BugContest_EventScript_GivePrize
	special BugContest_GivePlayerMon
	goto_if_eq VAR_RESULT, 1, BugContest_EventScript_ResultsMonJoinedParty
	goto_if_eq VAR_RESULT, 2, BugContest_EventScript_ResultsMonSentToBox
	goto BugContest_EventScript_ResultsDone
	end

BugContest_EventScript_ResultsMonJoinedParty::
	msgbox BugContest_Text_MonJoinedParty, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsDone
	end

BugContest_EventScript_ResultsMonSentToBox::
	msgbox BugContest_Text_PartyFullSentToBox, MSGBOX_DEFAULT
	goto BugContest_EventScript_ResultsDone
	end

BugContest_EventScript_ResultsDone::
	msgbox BugContest_Text_JoinUsNextTime, MSGBOX_DEFAULT
	releaseall
	end

@ Gives the prize held in VAR_BUG_CONTEST_PRIZE. If the bag is full the officer keeps holding it.
BugContest_EventScript_GivePrize::
	goto_if_eq VAR_BUG_CONTEST_PRIZE, ITEM_NONE, BugContest_EventScript_NoPrize
	giveitem VAR_BUG_CONTEST_PRIZE
	goto_if_eq VAR_RESULT, FALSE, BugContest_EventScript_PrizeBagFull
	setvar VAR_BUG_CONTEST_PRIZE, ITEM_NONE
	return

BugContest_EventScript_PrizeBagFull::
	msgbox BugContest_Text_WellHoldPrize, MSGBOX_DEFAULT
	return

BugContest_EventScript_NoPrize::
	return

BugContest_Text_WouldYouLikeToQuit:
	.string "Would you like to end the Bug Catching\n"
	.string "Contest right now?$"

BugContest_Text_TimesUp:
	.string "ANNOUNCER: Time's up!\p"
	.string "The Bug Catching Contest is over!$"

BugContest_Text_OutOfBalls:
	.string "ANNOUNCER: You're out of Park Balls!\p"
	.string "The Bug Catching Contest is over!$"

BugContest_Text_ReadyToJudge:
	.string "Let's judge the Bug Catching Contest!\p"
	.string "We'll announce the results now.$"

BugContest_Text_ThirdPlace:
	.string "Placing third was {STR_VAR_1},\n"
	.string "who caught a {STR_VAR_2}!\p"
	.string "The score was {STR_VAR_3} points!$"

BugContest_Text_SecondPlace:
	.string "Placing second was {STR_VAR_1},\n"
	.string "who caught a {STR_VAR_2}!\p"
	.string "The score was {STR_VAR_3} points!$"

BugContest_Text_FirstPlace:
	.string "This Bug Catching Contest winner is\n"
	.string "{STR_VAR_1}, who caught a {STR_VAR_2}!\p"
	.string "The winning score was {STR_VAR_3} points!$"

BugContest_Text_YouPlacedFirst:
	.string "{PLAYER}, you won with your\n"
	.string "{STR_VAR_2}! It scored {STR_VAR_3} points!\p"
	.string "Congratulations! Here's your prize!$"

BugContest_Text_YouPlacedSecond:
	.string "{PLAYER}, you placed second with\n"
	.string "your {STR_VAR_2}! It scored {STR_VAR_3} points!\p"
	.string "Congratulations! Here's your prize!$"

BugContest_Text_YouPlacedThird:
	.string "{PLAYER}, you placed third with\n"
	.string "your {STR_VAR_2}! It scored {STR_VAR_3} points!\p"
	.string "Congratulations! Here's your prize!$"

BugContest_Text_DidNotPlace:
	.string "{PLAYER}, your {STR_VAR_2} scored\n"
	.string "{STR_VAR_3} points.\p"
	.string "You didn't place, but here's a\n"
	.string "prize for taking part!$"

BugContest_Text_NothingCaught:
	.string "{PLAYER}, you didn't catch a Pokémon.\p"
	.string "Here's a prize for taking part!$"

BugContest_Text_WellHoldPrize:
	.string "Your Bag is full.\p"
	.string "We'll hold the prize for you. Come back\n"
	.string "when you've made room.$"

BugContest_Text_MonJoinedParty:
	.string "The {STR_VAR_1} you caught has joined\n"
	.string "your party!$"

BugContest_Text_PartyFullSentToBox:
	.string "Your party is full, so the {STR_VAR_1}\n"
	.string "you caught was sent to a Box.$"

BugContest_Text_JoinUsNextTime:
	.string "Please join us for the next contest!$"
