ProfileMan_EventScript_Man::
	lock
	faceplayer
	goto_if_set FLAG_SYS_CHAT_USED, ProfileMan_EventScript_GivenProfileBefore
	msgbox ProfileMan_Text_CollectTrainerProfiles, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeProfile
	end

ProfileMan_EventScript_AskToSeeProfile::
	msgbox ProfileMan_Text_MayISeeYourProfile, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, ProfileMan_EventScript_CreateProfile
	case 1, ProfileMan_EventScript_DeclineShowProfile
	case 2, ProfileMan_EventScript_Info
	case MULTI_B_PRESSED, ProfileMan_EventScript_DeclineShowProfile
	end

ProfileMan_EventScript_Info::
	msgbox ProfileMan_Text_EasyChatExplanation, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeProfile
	end

ProfileMan_EventScript_CreateProfile::
	msgbox ProfileMan_Text_LetsSeeItThen, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, ProfileMan_EventScript_CancelShowProfile
	goto_if_eq VAR_RESULT, 1, ProfileMan_EventScript_ShowProfile
	end

ProfileMan_EventScript_CancelShowProfile::
	msgbox ProfileMan_Text_NotIntoItRightNow, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_ShowProfile::
	setvar VAR_0x8004, 0
	special ShowEasyChatProfile
	waitmessage
	delay 80
	msgbox ProfileMan_Text_FantasticProfile, MSGBOX_DEFAULT
	msgbox ProfileMan_Text_WouldYouLikeToTrade, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, ProfileMan_EventScript_DeclineTrade
	special ChoosePartyMon
	waitstate
	goto_if_eq VAR_0x8004, PARTY_NOTHING_CHOSEN, ProfileMan_EventScript_DeclineTrade
	copyvar VAR_0x8005, VAR_0x8004
	setvar VAR_0x8004, 300
	special Special_GiveRandomMon
	goto_if_eq VAR_RESULT, SPECIES_NONE, ProfileMan_EventScript_TradeNoMon
	bufferspeciesname STR_VAR_1, VAR_RESULT
	msgbox ProfileMan_Text_TradeReceived, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_DeclineTrade::
	msgbox ProfileMan_Text_TradeDeclined, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_TradeNoMon::
	msgbox ProfileMan_Text_TradeNoMon, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_DeclineShowProfile::
	msgbox ProfileMan_Text_ImagineYouWouldHaveWonderfulProfile, MSGBOX_DEFAULT
	release
	end

ProfileMan_EventScript_GivenProfileBefore::
	msgbox ProfileMan_Text_YouHaveWonderfulSmile, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeNewProfile
	end

ProfileMan_EventScript_AskToSeeNewProfile::
	msgbox ProfileMan_Text_MayISeeYourNewProfile, MSGBOX_DEFAULT
	multichoice 17, 6, MULTI_YESNOINFO_2, FALSE
	switch VAR_RESULT
	case 0, ProfileMan_EventScript_CreateNewProfile
	case 1, ProfileMan_EventScript_DeclineNewProfile
	case 2, ProfileMan_EventScript_InfoNewProfile
	case MULTI_B_PRESSED, ProfileMan_EventScript_DeclineNewProfile
	end

ProfileMan_EventScript_InfoNewProfile::
	msgbox ProfileMan_Text_EasyChatExplanation, MSGBOX_DEFAULT
	goto ProfileMan_EventScript_AskToSeeNewProfile
	end

ProfileMan_EventScript_CreateNewProfile::
	msgbox ProfileMan_Text_EvenBetterThanLastProfile, MSGBOX_DEFAULT
	closemessage
	setvar VAR_0x8004, EASY_CHAT_TYPE_PROFILE
	call Common_ShowEasyChatScreen
	lock
	faceplayer
	goto_if_eq VAR_RESULT, 0, ProfileMan_EventScript_CancelShowProfile
	goto_if_eq VAR_RESULT, 1, ProfileMan_EventScript_ShowProfile
	end

ProfileMan_EventScript_DeclineNewProfile::
	msgbox ProfileMan_Text_LikeProfileWayItIs, MSGBOX_DEFAULT
	release
	end

@ Unused
ProfileMan_EventScript_Ret::
	return

ProfileMan_Text_CollectTrainerProfiles:
	.string "Hello there, TRAINER!\n"
	.string "You've got a wonderful smile, there.\p"
	.string "I have a hobby--collecting the profiles\n"
	.string "of POKéMON TRAINERS.$"

ProfileMan_Text_MayISeeYourProfile:
	.string "So, how about it?\n"
	.string "May I see your profile?$"

ProfileMan_Text_EasyChatExplanation:
	.string "You make your own profile by putting\n"
	.string "together four words or phrases.\p"
	.string "Here, I'll show you an example of a\n"
	.string "profile using four pieces of text.\p"
	.string "You can switch those four pieces with\n"
	.string "other text pieces any which way you\l"
	.string "like to make your own profile.\p"
	.string "There are a lot of text pieces that\n"
	.string "you can use.\p"
	.string "They are arranged in groups like\n"
	.string "POKéMON, lifestyles, and hobbies so\l"
	.string "it is easier to look them up.\p"
	.string "So, first, choose the group of text\n"
	.string "pieces to display a list of choices.\p"
	.string "Then, pick the choice you want.\p"
	.string "Repeat for the remaining text choices,\n"
	.string "and you'll have your very own profile.$"

ProfileMan_Text_LetsSeeItThen:
	.string "Yes! Thank you!\n"
	.string "So, let's see it, then.$"

ProfileMan_Text_ImagineYouWouldHaveWonderfulProfile:
	.string "Oh, no, really?\p"
	.string "I imagine someone like you would have\n"
	.string "a wonderful profile…$"

ProfileMan_Text_NotIntoItRightNow:
	.string "Oh? You're not into it right now?\p"
	.string "Well, anytime is good by me!$"

ProfileMan_Text_YouHaveWonderfulSmile:
	.string "Hello there, TRAINER!\n"
	.string "You've got a wonderful smile.$"

ProfileMan_Text_MayISeeYourNewProfile:
	.string "May I see your new profile?$"

ProfileMan_Text_EvenBetterThanLastProfile:
	.string "Yes! Thank you!\p"
	.string "I hope it's even better than the profile\n"
	.string "you showed me before.$"

ProfileMan_Text_LikeProfileWayItIs:
	.string "Oh, you like your profile the way it is.\p"
	.string "I don't blame you--it's a wonderful\n"
	.string "profile the way it is now.$"

ProfileMan_Text_FantasticProfile:
	.string "F-fantastic!\p"
	.string "Your profile, it's wonderful!\n"
	.string "It really says what you're about.\p"
	.string "Why, anyone hearing this profile would\n"
	.string "be captivated by you!\p"
	.string "Thank you!$"

ProfileMan_Text_WouldYouLikeToTrade:
	.string "Your profile really moved me…\p"
	.string "In fact, I'd love to make a trade!\n"
	.string "Would you like to exchange\l"
	.string "one of your POKéMON with me?$"

ProfileMan_Text_TradeReceived:
	.string "Wonderful! You gave me a POKéMON\n"
	.string "that matches your spirit!\p"
	.string "In return, please take\n"
	.string "{STR_VAR_1}!$"

ProfileMan_Text_TradeDeclined:
	.string "Oh, of course! No need to rush.\p"
	.string "Your profile alone was more than\n"
	.string "enough for me. Thank you!$"

ProfileMan_Text_TradeNoMon:
	.string "Hmm… I looked everywhere, but I\n"
	.string "couldn't find a fitting POKéMON\l"
	.string "to trade right now.\p"
	.string "Maybe next time!$"

@ Unused
ProfileMan_Text_YouKnowSecretSaying:
	.string "Oh?\n"
	.string "You know the secret saying!\p"
	.string "That means you're now a fellow member\n"
	.string "of the MYSTERY EVENT CLUB!$"
