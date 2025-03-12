RustboroCity_Mart_MapScripts::
	.byte 0

RustboroCity_Mart_EventScript_Clerk::
	lock
	faceplayer
	message gText_HowMayIServeYou
	waitmessage
	goto_if_unset FLAG_MET_DEVON_EMPLOYEE, RustboroCity_Mart_EventScript_PokemartBasic
	goto_if_set FLAG_MET_DEVON_EMPLOYEE, RustboroCity_Mart_EventScript_PokemartExpanded
	end

RustboroCity_Mart_EventScript_PokemartBasic::
	pokemart RustboroCity_Mart_Pokemart_Basic
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
RustboroCity_Mart_Pokemart_Basic:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	pokemartlistend

RustboroCity_Mart_EventScript_PokemartExpanded::
	pokemart RustboroCity_Mart_Pokemart_Expanded
	msgbox gText_PleaseComeAgain, MSGBOX_DEFAULT
	release
	end

	.align 2
RustboroCity_Mart_Pokemart_Expanded:
	.2byte ITEM_POKE_BALL
	.2byte ITEM_TIMER_BALL
	.2byte ITEM_REPEAT_BALL
	.2byte ITEM_POTION
	.2byte ITEM_SUPER_POTION
	.2byte ITEM_ANTIDOTE
	.2byte ITEM_PARALYZE_HEAL
	.2byte ITEM_ESCAPE_ROPE
	.2byte ITEM_REPEL
	.2byte ITEM_X_SPEED
	.2byte ITEM_X_ATTACK
	.2byte ITEM_X_DEFENSE
	pokemartlistend

RustboroCity_Mart_EventScript_PokefanF::
	msgbox RustboroCity_Mart_Text_BuyingHealsInCaseOfShroomish, MSGBOX_NPC
	end

RustboroCity_Mart_EventScript_Boy::
	msgbox RustboroCity_Mart_Text_ShouldBuySuperPotionsInstead, MSGBOX_NPC
	end

RustboroCity_Mart_EventScript_BugCatcher::
	msgbox RustboroCity_Mart_Text_GettingEscapeRopeJustInCase, MSGBOX_NPC
	end

RustboroCity_Mart_EventScript_Promotion::
	lock
	faceplayer
	goto_if_set FLAG_RECEIVED_RUSTBORO_MART_ITEM, RustboroCity_Mart_EventScript_PromotionAfter
	msgbox RustboroCity_Mart_Text_GiveItemPromotion, MSGBOX_DEFAULT
	giveitem ITEM_TM54
	setflag FLAG_RECEIVED_RUSTBORO_MART_ITEM
	msgbox RustboroCity_Mart_Text_AfterItemPromotion, MSGBOX_DEFAULT
	goto RustboroCity_Mart_EventScript_PromotionAfter
	end
	
RustboroCity_Mart_EventScript_PromotionAfter::
	msgbox RustboroCity_Mart_Text_ExplainItem, MSGBOX_DEFAULT
	closemessage
	waitmovement 0
	release
	end

RustboroCity_Mart_Text_BuyingHealsInCaseOfShroomish:
	.string "I'm buying some PARLYZ HEALS and\n"
	.string "ANTIDOTES.\p"
	.string "Just in case I run into SHROOMISH\n"
	.string "in PETALBURG WOODS.$"

RustboroCity_Mart_Text_ShouldBuySuperPotionsInstead:
	.string "My POKéMON evolved.\n"
	.string "It has a lot of HP now.\p"
	.string "I should buy SUPER POTIONS for it\n"
	.string "instead of ordinary POTIONS.$"

RustboroCity_Mart_Text_GettingEscapeRopeJustInCase:
	.string "I'm getting an ESCAPE ROPE just in\n"
	.string "case I get lost in a cave.\p"
	.string "I just need to use it to get back to\n"
	.string "the entrance.$"
	
RustboroCity_Mart_Text_GiveItemPromotion:
	.string "Excuse me, Trainer over there.\n"
	.string "Do you have a minute?\p"
	.string "You know, sometimes moves are too\n"
	.string "strong to catch a POKéMON.\p"
	.string "You end up making a Pokémon faint\n"
	.string "even when you don't want to.\p"
	.string "For such common problem, use this!$"

RustboroCity_Mart_Text_ExplainItem:
	.string "False Swipe is a very handy move.\p"
	.string "No matter how many times it's used on\n"
	.string "the opposing Pokémon, it won't cause\l"
	.string "the Pokémon to faint!$"

RustboroCity_Mart_Text_AfterItemPromotion:
	.string "I'm offering it for free right now.\n"
	.string "This is a limited-time promotion!$"
