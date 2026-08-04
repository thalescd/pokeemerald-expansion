RustboroCity_DevonCorp_2F_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, RustboroCity_DevonCorp_2F_OnTransition
	.byte 0

RustboroCity_DevonCorp_2F_OnTransition:
	call_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_SetFossilReady
	end

RustboroCity_DevonCorp_2F_EventScript_SetFossilReady::
	setvar VAR_FOSSIL_RESURRECTION_STATE, 2
	return

RustboroCity_DevonCorp_2F_EventScript_TalkToPokemonScientist::
	lock
	faceplayer
	call_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_SetFossilReady
	msgbox RustboroCity_DevonCorp_2F_Text_DeviceForTalkingToPokemon, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_BallScientist::
	lock
	faceplayer
	call_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_SetFossilReady
	goto_if_set FLAG_MET_DEVON_EMPLOYEE, RustboroCity_DevonCorp_2F_EventScript_DevelopedBalls
	msgbox RustboroCity_DevonCorp_2F_Text_DevelopingNewBalls, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_DevelopedBalls::
	msgbox RustboroCity_DevonCorp_2F_Text_WeFinallyMadeNewBalls, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_PokenavScientist::
	lock
	faceplayer
	call_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_SetFossilReady
	goto_if_set FLAG_RECEIVED_POKENAV, RustboroCity_DevonCorp_2F_EventScript_HasPokenav
	msgbox RustboroCity_DevonCorp_2F_Text_IMadePokenav, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_HasPokenav::
	msgbox RustboroCity_DevonCorp_2F_Text_WowThatsAPokenav, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_PokemonDreamsScientist::
	lock
	faceplayer
	call_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_SetFossilReady
	msgbox RustboroCity_DevonCorp_2F_Text_DeviceToVisualizePokemonDreams, MSGBOX_DEFAULT
	release
	end

@ VAR_WHICH_FOSSIL_REVIVED / the dynamic fossil menu both use these ids.
@ FOSSIL_ID_UNKNOWN is the only one whose species isn't decided until pickup.
	.set FOSSIL_ID_CANCEL,      0
	.set FOSSIL_ID_ROOT,        1
	.set FOSSIL_ID_CLAW,        2
	.set FOSSIL_ID_UNKNOWN,     3
	.set FOSSIL_ID_OLD_AMBER,   4

RustboroCity_DevonCorp_2F_EventScript_FossilScientist::
	lock
	faceplayer
	goto_if_eq VAR_FOSSIL_RESURRECTION_STATE, 2, RustboroCity_DevonCorp_2F_EventScript_FossilMonReady
	goto_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_StillRegenerating
	msgbox RustboroCity_DevonCorp_2F_Text_DevelopDeviceToResurrectFossils, MSGBOX_DEFAULT
	call RustboroCity_DevonCorp_2F_EventScript_CountFossils
	goto_if_eq VAR_0x8005, 0, RustboroCity_DevonCorp_2F_EventScript_NoFossilToRevive
	goto RustboroCity_DevonCorp_2F_EventScript_NoticeFossil
	end

RustboroCity_DevonCorp_2F_EventScript_NoFossilToRevive::
	release
	end

@ Tallies how many revivable fossils the player is carrying into VAR_0x8005,
@ and remembers the last one seen in VAR_0x8006 so a lone fossil needs no menu.
RustboroCity_DevonCorp_2F_EventScript_CountFossils::
	setvar VAR_0x8005, 0
	setvar VAR_0x8006, FOSSIL_ID_CANCEL
	checkitem ITEM_ROOT_FOSSIL
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_TallyRootFossil
	checkitem ITEM_CLAW_FOSSIL
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_TallyClawFossil
	checkitem ITEM_UNKNOWN_FOSSIL
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_TallyUnknownFossil
	checkitem ITEM_OLD_AMBER
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_TallyOldAmber
	return

RustboroCity_DevonCorp_2F_EventScript_TallyRootFossil::
	addvar VAR_0x8005, 1
	setvar VAR_0x8006, FOSSIL_ID_ROOT
	return

RustboroCity_DevonCorp_2F_EventScript_TallyClawFossil::
	addvar VAR_0x8005, 1
	setvar VAR_0x8006, FOSSIL_ID_CLAW
	return

RustboroCity_DevonCorp_2F_EventScript_TallyUnknownFossil::
	addvar VAR_0x8005, 1
	setvar VAR_0x8006, FOSSIL_ID_UNKNOWN
	return

RustboroCity_DevonCorp_2F_EventScript_TallyOldAmber::
	addvar VAR_0x8005, 1
	setvar VAR_0x8006, FOSSIL_ID_OLD_AMBER
	return

RustboroCity_DevonCorp_2F_EventScript_NoticeFossil::
	closemessage
	playse SE_PIN
	applymovement LOCALID_FOSSIL_SCIENTIST, Common_Movement_ExclamationMark
	waitmovement 0
	applymovement LOCALID_FOSSIL_SCIENTIST, Common_Movement_Delay48
	waitmovement 0
	msgbox RustboroCity_DevonCorp_2F_Text_WantToBringFossilBackToLife, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_DevonCorp_2F_EventScript_DeclineGiveFossil
	goto_if_ge VAR_0x8005, 2, RustboroCity_DevonCorp_2F_EventScript_ChooseFossil
	copyvar VAR_0x8004, VAR_0x8006
	goto RustboroCity_DevonCorp_2F_EventScript_GiveFossil
	end

RustboroCity_DevonCorp_2F_EventScript_ChooseFossil::
	message RustboroCity_DevonCorp_2F_Text_ManyFossilsPickOne
	waitmessage
	checkitem ITEM_ROOT_FOSSIL
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_PushRootFossil
	checkitem ITEM_CLAW_FOSSIL
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_PushClawFossil
	checkitem ITEM_UNKNOWN_FOSSIL
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_PushUnknownFossil
	checkitem ITEM_OLD_AMBER
	call_if_eq VAR_RESULT, TRUE, RustboroCity_DevonCorp_2F_EventScript_PushOldAmber
	dynmultipush gText_Exit, FOSSIL_ID_CANCEL
@ No pushed id matches when the player has no Root Fossil, which leaves the
@ cursor on row 0 -- either way it opens on the first fossil, never on EXIT.
	dynmultistack 0, 0, FALSE, 6, FALSE, FOSSIL_ID_ROOT, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, RustboroCity_DevonCorp_2F_EventScript_CancelFossilSelect
	goto_if_eq VAR_RESULT, FOSSIL_ID_CANCEL, RustboroCity_DevonCorp_2F_EventScript_CancelFossilSelect
	copyvar VAR_0x8004, VAR_RESULT
	goto RustboroCity_DevonCorp_2F_EventScript_GiveFossil
	end

RustboroCity_DevonCorp_2F_EventScript_PushRootFossil::
	bufferitemname STR_VAR_1, ITEM_ROOT_FOSSIL
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, FOSSIL_ID_ROOT
	return

RustboroCity_DevonCorp_2F_EventScript_PushClawFossil::
	bufferitemname STR_VAR_1, ITEM_CLAW_FOSSIL
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, FOSSIL_ID_CLAW
	return

RustboroCity_DevonCorp_2F_EventScript_PushUnknownFossil::
	bufferitemname STR_VAR_1, ITEM_UNKNOWN_FOSSIL
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, FOSSIL_ID_UNKNOWN
	return

RustboroCity_DevonCorp_2F_EventScript_PushOldAmber::
	bufferitemname STR_VAR_1, ITEM_OLD_AMBER
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, FOSSIL_ID_OLD_AMBER
	return

@ VAR_0x8004 holds the chosen fossil id.
RustboroCity_DevonCorp_2F_EventScript_GiveFossil::
	switch VAR_0x8004
	case FOSSIL_ID_ROOT, RustboroCity_DevonCorp_2F_EventScript_TakeRootFossil
	case FOSSIL_ID_CLAW, RustboroCity_DevonCorp_2F_EventScript_TakeClawFossil
	case FOSSIL_ID_UNKNOWN, RustboroCity_DevonCorp_2F_EventScript_TakeUnknownFossil
	case FOSSIL_ID_OLD_AMBER, RustboroCity_DevonCorp_2F_EventScript_TakeOldAmber
	end

RustboroCity_DevonCorp_2F_EventScript_TakeRootFossil::
	bufferitemname STR_VAR_1, ITEM_ROOT_FOSSIL
	removeitem ITEM_ROOT_FOSSIL
	setvar VAR_WHICH_FOSSIL_REVIVED, FOSSIL_ID_ROOT
	goto RustboroCity_DevonCorp_2F_EventScript_FossilHandedOver
	end

RustboroCity_DevonCorp_2F_EventScript_TakeClawFossil::
	bufferitemname STR_VAR_1, ITEM_CLAW_FOSSIL
	removeitem ITEM_CLAW_FOSSIL
	setvar VAR_WHICH_FOSSIL_REVIVED, FOSSIL_ID_CLAW
	goto RustboroCity_DevonCorp_2F_EventScript_FossilHandedOver
	end

RustboroCity_DevonCorp_2F_EventScript_TakeUnknownFossil::
	bufferitemname STR_VAR_1, ITEM_UNKNOWN_FOSSIL
	removeitem ITEM_UNKNOWN_FOSSIL
	setvar VAR_WHICH_FOSSIL_REVIVED, FOSSIL_ID_UNKNOWN
	goto RustboroCity_DevonCorp_2F_EventScript_FossilHandedOver
	end

RustboroCity_DevonCorp_2F_EventScript_TakeOldAmber::
	bufferitemname STR_VAR_1, ITEM_OLD_AMBER
	removeitem ITEM_OLD_AMBER
	setvar VAR_WHICH_FOSSIL_REVIVED, FOSSIL_ID_OLD_AMBER
	goto RustboroCity_DevonCorp_2F_EventScript_FossilHandedOver
	end

RustboroCity_DevonCorp_2F_EventScript_FossilHandedOver::
	msgbox RustboroCity_DevonCorp_2F_Text_HandedFossilToResearcher, MSGBOX_DEFAULT
	setvar VAR_FOSSIL_RESURRECTION_STATE, 1
	release
	end

RustboroCity_DevonCorp_2F_EventScript_DeclineGiveFossil::
	msgbox RustboroCity_DevonCorp_2F_Text_OhIsThatSo, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_CancelFossilSelect::
	release
	end

RustboroCity_DevonCorp_2F_EventScript_StillRegenerating::
	msgbox RustboroCity_DevonCorp_2F_Text_FossilRegeneratorTakesTime, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_FossilMonReady::
	switch VAR_WHICH_FOSSIL_REVIVED
	case FOSSIL_ID_ROOT, RustboroCity_DevonCorp_2F_EventScript_LileepReady
	case FOSSIL_ID_CLAW, RustboroCity_DevonCorp_2F_EventScript_AnorithReady
	case FOSSIL_ID_UNKNOWN, RustboroCity_DevonCorp_2F_EventScript_AskForGuess
	case FOSSIL_ID_OLD_AMBER, RustboroCity_DevonCorp_2F_EventScript_AerodactylReady
	end

RustboroCity_DevonCorp_2F_EventScript_LileepReady::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_LILEEP
	goto RustboroCity_DevonCorp_2F_EventScript_RevealFossilMon
	end

RustboroCity_DevonCorp_2F_EventScript_AnorithReady::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_ANORITH
	goto RustboroCity_DevonCorp_2F_EventScript_RevealFossilMon
	end

RustboroCity_DevonCorp_2F_EventScript_AerodactylReady::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_AERODACTYL
	goto RustboroCity_DevonCorp_2F_EventScript_RevealFossilMon
	end

RustboroCity_DevonCorp_2F_EventScript_RevealFossilMon::
	bufferspeciesname STR_VAR_2, VAR_TEMP_TRANSFERRED_SPECIES
	msgbox RustboroCity_DevonCorp_2F_Text_FossilizedMonBroughtBackToLife, MSGBOX_DEFAULT
	goto RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMon
	end

@ The Unknown Fossil could have been any of four Pokemon, so the researcher lets
@ the player call it. Whatever they pick is what the machine "produced" -- and
@ declining to guess (VAR_0x8005) rolls one at random instead.
RustboroCity_DevonCorp_2F_EventScript_AskForGuess::
	setvar VAR_0x8005, 0
	goto_if_set FLAG_GUESSED_FOSSIL_MON, RustboroCity_DevonCorp_2F_EventScript_AskForGuessAgain
	message RustboroCity_DevonCorp_2F_Text_CareToGuessWhatCameOut
	goto RustboroCity_DevonCorp_2F_EventScript_ShowGuessMenu
	end

RustboroCity_DevonCorp_2F_EventScript_AskForGuessAgain::
	message RustboroCity_DevonCorp_2F_Text_YouKnowTheRoutineByNow
	goto RustboroCity_DevonCorp_2F_EventScript_ShowGuessMenu
	end

RustboroCity_DevonCorp_2F_EventScript_ShowGuessMenu::
	waitmessage
	bufferspeciesname STR_VAR_1, SPECIES_OMANYTE
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, SPECIES_OMANYTE
	bufferspeciesname STR_VAR_1, SPECIES_KABUTO
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, SPECIES_KABUTO
	bufferspeciesname STR_VAR_1, SPECIES_TIRTOUGA
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, SPECIES_TIRTOUGA
	bufferspeciesname STR_VAR_1, SPECIES_ARCHEN
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuBufferedName, SPECIES_ARCHEN
	dynmultipush RustboroCity_DevonCorp_2F_Text_MenuNoIdea, SPECIES_NONE
	dynmultistack 0, 0, FALSE, 6, FALSE, SPECIES_OMANYTE, DYN_MULTICHOICE_CB_NONE
	goto_if_eq VAR_RESULT, MULTI_B_PRESSED, RustboroCity_DevonCorp_2F_EventScript_PostponeGuess
	goto_if_eq VAR_RESULT, SPECIES_NONE, RustboroCity_DevonCorp_2F_EventScript_NoGuess
	copyvar VAR_TEMP_TRANSFERRED_SPECIES, VAR_RESULT
	goto RustboroCity_DevonCorp_2F_EventScript_GuessWasRight
	end

RustboroCity_DevonCorp_2F_EventScript_PostponeGuess::
	msgbox RustboroCity_DevonCorp_2F_Text_TakeYourTimeItIsNotGoingAnywhere, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_NoGuess::
	setvar VAR_0x8005, 1
	random 4
	switch VAR_RESULT
	case 0, RustboroCity_DevonCorp_2F_EventScript_RolledOmanyte
	case 1, RustboroCity_DevonCorp_2F_EventScript_RolledKabuto
	case 2, RustboroCity_DevonCorp_2F_EventScript_RolledTirtouga
	case 3, RustboroCity_DevonCorp_2F_EventScript_RolledArchen
	end

RustboroCity_DevonCorp_2F_EventScript_RolledOmanyte::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_OMANYTE
	goto RustboroCity_DevonCorp_2F_EventScript_GuessWasRight
	end

RustboroCity_DevonCorp_2F_EventScript_RolledKabuto::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_KABUTO
	goto RustboroCity_DevonCorp_2F_EventScript_GuessWasRight
	end

RustboroCity_DevonCorp_2F_EventScript_RolledTirtouga::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_TIRTOUGA
	goto RustboroCity_DevonCorp_2F_EventScript_GuessWasRight
	end

RustboroCity_DevonCorp_2F_EventScript_RolledArchen::
	setvar VAR_TEMP_TRANSFERRED_SPECIES, SPECIES_ARCHEN
	goto RustboroCity_DevonCorp_2F_EventScript_GuessWasRight
	end

@ Branches on FLAG_GUESSED_FOSSIL_MON, not FLAG_RECEIVED_REVIVED_FOSSIL_MON:
@ the latter is already set by a plain Root/Claw Fossil revival, which would make
@ the researcher congratulate a repeat guess the player has never made. The flag
@ is only raised on the way out, once this fossil's dialogue has been chosen.
RustboroCity_DevonCorp_2F_EventScript_GuessWasRight::
	bufferspeciesname STR_VAR_2, VAR_TEMP_TRANSFERRED_SPECIES
	goto_if_eq VAR_0x8005, 1, RustboroCity_DevonCorp_2F_EventScript_NoGuessReveal
	goto_if_set FLAG_GUESSED_FOSSIL_MON, RustboroCity_DevonCorp_2F_EventScript_GuessWasRightAgain
	msgbox RustboroCity_DevonCorp_2F_Text_ThatsCorrectYoureGoodAtThis, MSGBOX_DEFAULT
	goto RustboroCity_DevonCorp_2F_EventScript_MarkGuessDone
	end

RustboroCity_DevonCorp_2F_EventScript_GuessWasRightAgain::
	msgbox RustboroCity_DevonCorp_2F_Text_CorrectYouKnowSomething, MSGBOX_DEFAULT
	goto RustboroCity_DevonCorp_2F_EventScript_MarkGuessDone
	end

RustboroCity_DevonCorp_2F_EventScript_NoGuessReveal::
	msgbox RustboroCity_DevonCorp_2F_Text_FairEnoughLetsHaveALook, MSGBOX_DEFAULT
	goto RustboroCity_DevonCorp_2F_EventScript_MarkGuessDone
	end

@ Declining to guess still counts as having been through the ritual, so the
@ researcher doesn't re-explain it next time.
RustboroCity_DevonCorp_2F_EventScript_MarkGuessDone::
	setflag FLAG_GUESSED_FOSSIL_MON
	goto RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMon
	end

@ Shared by every fossil. VAR_TEMP_TRANSFERRED_SPECIES holds the species, and is
@ also what Common_EventScript_TransferredToPC reads, so it must outlive the
@ common gift scripts -- VAR_0x8004 would not, they overwrite it.
RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMon::
	givemon VAR_TEMP_TRANSFERRED_SPECIES, 20
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PARTY, RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMonParty
	goto_if_eq VAR_RESULT, MON_GIVEN_TO_PC, RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMonPC
	goto Common_EventScript_NoMoreRoomForPokemon
	end

RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMonParty::
	call RustboroCity_DevonCorp_2F_EventScript_ReceivedFossilMonFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_DevonCorp_2F_EventScript_FinishReceivingFossilMon
	call Common_EventScript_GetGiftMonPartySlot
	call Common_EventScript_NameReceivedPartyMon
	goto RustboroCity_DevonCorp_2F_EventScript_FinishReceivingFossilMon
	end

RustboroCity_DevonCorp_2F_EventScript_ReceiveFossilMonPC::
	call RustboroCity_DevonCorp_2F_EventScript_ReceivedFossilMonFanfare
	msgbox gText_NicknameThisPokemon, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, RustboroCity_DevonCorp_2F_EventScript_TransferFossilMonToPC
	call Common_EventScript_NameReceivedBoxMon
	goto RustboroCity_DevonCorp_2F_EventScript_TransferFossilMonToPC
	end

RustboroCity_DevonCorp_2F_EventScript_TransferFossilMonToPC::
	call Common_EventScript_TransferredToPC
	goto RustboroCity_DevonCorp_2F_EventScript_FinishReceivingFossilMon
	end

RustboroCity_DevonCorp_2F_EventScript_ReceivedFossilMonFanfare::
	bufferspeciesname STR_VAR_2, VAR_TEMP_TRANSFERRED_SPECIES
	playfanfare MUS_OBTAIN_ITEM
	message RustboroCity_DevonCorp_2F_Text_ReceivedMonFromResearcher
	waitmessage
	waitfanfare
	bufferspeciesname STR_VAR_1, VAR_TEMP_TRANSFERRED_SPECIES
	return

RustboroCity_DevonCorp_2F_EventScript_FinishReceivingFossilMon::
	setvar VAR_FOSSIL_RESURRECTION_STATE, 0
	setflag FLAG_RECEIVED_REVIVED_FOSSIL_MON
	release
	end

RustboroCity_DevonCorp_2F_EventScript_MatchCallScientist::
	lock
	faceplayer
	call_if_eq VAR_FOSSIL_RESURRECTION_STATE, 1, RustboroCity_DevonCorp_2F_EventScript_SetFossilReady
	goto_if_ge VAR_RUSTBORO_CITY_STATE, 6, RustboroCity_DevonCorp_2F_EventScript_WorkOnNext
	msgbox RustboroCity_DevonCorp_2F_Text_DevelopNewPokenavFeature, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_EventScript_WorkOnNext::
	msgbox RustboroCity_DevonCorp_2F_Text_WhatToWorkOnNext, MSGBOX_DEFAULT
	release
	end

RustboroCity_DevonCorp_2F_Text_DeviceForTalkingToPokemon:
	.string "We're developing a device for talking\n"
	.string "with POKéMON.\p"
	.string "But we haven't had much success…$"

RustboroCity_DevonCorp_2F_Text_DevelopingNewBalls:
	.string "I'm developing new kinds of\n"
	.string "POKé BALLS…\p"
	.string "But I haven't made much headway…$"

RustboroCity_DevonCorp_2F_Text_WeFinallyMadeNewBalls:
	.string "We finally made new kinds of\n"
	.string "POKé BALLS!\p"
	.string "The REPEAT BALL makes it easier to\n"
	.string "catch POKéMON you've caught before.\p"
	.string "The TIMER BALL gets better at catching\n"
	.string "POKéMON the longer a battle runs.\p"
	.string "Both are proudly developed by\n"
	.string "the DEVON CORPORATION.\p"
	.string "Please give them a try!$"

RustboroCity_DevonCorp_2F_Text_IMadePokenav:
	.string "I made the POKéNAV!\p"
	.string "As an engineer, I feel blessed to have\n"
	.string "made something so great!$"

RustboroCity_DevonCorp_2F_Text_WowThatsAPokenav:
	.string "Oh, wow!\n"
	.string "That's a POKéNAV!\p"
	.string "It came about as a result of our\n"
	.string "PRESIDENT's desire to learn about\l"
	.string "the feelings of POKéMON.\p"
	.string "Would you like me to describe its\n"
	.string "features in detail?\p"
	.string "No, no. I think you'll find out just by\n"
	.string "trying the POKéNAV out.$"

RustboroCity_DevonCorp_2F_Text_DeviceToVisualizePokemonDreams:
	.string "I'm trying to develop a device that\n"
	.string "visually reproduces the dreams of\l"
	.string "POKéMON…\p"
	.string "But it's not going well.$"

RustboroCity_DevonCorp_2F_Text_DevelopDeviceToResurrectFossils:
	.string "I've been trying to develop a device\n"
	.string "that resurrects POKéMON from fossils…\p"
	.string "And, it's working!$"

RustboroCity_DevonCorp_2F_Text_WantToBringFossilBackToLife:
	.string "Wait! That thing you have there…\n"
	.string "Is that a POKéMON fossil?\p"
	.string "Would you like to bring that POKéMON\n"
	.string "back to life?\p"
	.string "I can with my newly developed\n"
	.string "FOSSIL REGENERATOR.$"

RustboroCity_DevonCorp_2F_Text_OhIsThatSo:
	.string "Oh, is that so?\p"
	.string "DEVON's technological expertise\n"
	.string "is outstanding, I tell you.$"

RustboroCity_DevonCorp_2F_Text_ManyFossilsPickOne:
	.string "Oh, now that's a surprise!\n"
	.string "You're carrying more than one fossil?\p"
	.string "Unfortunately, my machine can only\n"
	.string "regenerate one POKéMON at a time.\p"
	.string "Would you like to pick one of your\n"
	.string "fossils for regeneration?$"

@ Used as the label for every entry of the dynamic fossil and guess menus.
@ dynmultipush expands placeholders as the option is pushed, so buffering the
@ name right before each push keeps the menu in sync with the item and species
@ tables instead of duplicating their names here.
RustboroCity_DevonCorp_2F_Text_MenuBufferedName:
	.string "{STR_VAR_1}$"

RustboroCity_DevonCorp_2F_Text_MenuNoIdea:
	.string "I have no idea$"

RustboroCity_DevonCorp_2F_Text_CareToGuessWhatCameOut:
	.string "Thanks for waiting!\p"
	.string "Your fossilized POKéMON has been\n"
	.string "brought back to life!\p"
	.string "This one was worn far too smooth for\n"
	.string "me to identify beforehand…\p"
	.string "So! Care to guess what came out?$"

RustboroCity_DevonCorp_2F_Text_YouKnowTheRoutineByNow:
	.string "Thanks for waiting!\p"
	.string "Another worn-out fossil, another\n"
	.string "POKéMON back among the living!\p"
	.string "You know the routine by now.\n"
	.string "What's your guess?$"

RustboroCity_DevonCorp_2F_Text_ThatsCorrectYoureGoodAtThis:
	.string "That's correct!\p"
	.string "The fossil was an ancient POKéMON.\n"
	.string "{STR_VAR_2}, it was!\p"
	.string "You're good at this!$"

@ Deliberately avoids claiming the player guessed right "again" -- they may have
@ declined to guess on their previous fossil.
RustboroCity_DevonCorp_2F_Text_CorrectYouKnowSomething:
	.string "Correct!\p"
	.string "{STR_VAR_2}, just as you called it…\p"
	.string "I'm beginning to suspect you know\n"
	.string "something I don't.$"

RustboroCity_DevonCorp_2F_Text_FairEnoughLetsHaveALook:
	.string "Fair enough!\n"
	.string "Let's have a look…\p"
	.string "The fossil was an ancient POKéMON.\n"
	.string "{STR_VAR_2}, it was!$"

RustboroCity_DevonCorp_2F_Text_TakeYourTimeItIsNotGoingAnywhere:
	.string "Take your time.\p"
	.string "It's not as though it's going\n"
	.string "anywhere after all these years.$"

RustboroCity_DevonCorp_2F_Text_HandedFossilToResearcher:
	.string "Excellent!\n"
	.string "Let's do this right away.\p"
	.string "{PLAYER} handed the {STR_VAR_1} to\n"
	.string "the DEVON RESEARCHER.$"

RustboroCity_DevonCorp_2F_Text_FossilRegeneratorTakesTime:
	.string "The FOSSIL REGENERATOR, which I made,\n"
	.string "is incredible.\p"
	.string "But it has one drawback--it takes\n"
	.string "a long time to work.\p"
	.string "So, uh… How about you go for a stroll\n"
	.string "and look around for a while?$"

RustboroCity_DevonCorp_2F_Text_FossilizedMonBroughtBackToLife:
	.string "Thanks for waiting!\p"
	.string "Your fossilized POKéMON has been\n"
	.string "brought back to life!\p"
	.string "The fossil was an ancient POKéMON.\n"
	.string "{STR_VAR_2}, it was!$"

RustboroCity_DevonCorp_2F_Text_ReceivedMonFromResearcher:
	.string "{PLAYER} received {STR_VAR_2} from\n"
	.string "the DEVON RESEARCHER.$"

@ Unused
RustboroCity_DevonCorp_2F_Text_TooManyPokemon:
	.string "Uh-oh, you've got too many POKéMON.\n"
	.string "You have no room for this one.$"

RustboroCity_DevonCorp_2F_Text_DevelopNewPokenavFeature:
	.string "I'm trying to develop a new feature\n"
	.string "for the POKéNAV…\p"
	.string "But it's not going well.$"

RustboroCity_DevonCorp_2F_Text_WhatToWorkOnNext:
	.string "Well, now what shall I work on\n"
	.string "developing next?\p"
	.string "Our company allows us to make our\n"
	.string "inspirations into reality.\p"
	.string "One couldn't ask for a better\n"
	.string "environment as an engineer.$"

