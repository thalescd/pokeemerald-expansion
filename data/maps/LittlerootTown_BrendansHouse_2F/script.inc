PlayersHouse_2F_EventScript_StarterRegionChooser::
# 2 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	lock
# 3 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	message PlayersHouse_2F_EventScript_StarterRegionChooser_Text_0
# 5 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	multichoicegrid 25, 2, MULTI_REGION, 3, 0
# 6 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	closemessage
# 7 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	setvar VAR_STARTER_REGION, var ( VAR_RESULT )
# 8 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	release
	return


PlayersHouse_2F_EventScript_StarterRegionChooser_Text_0:
# 3 "data/maps/LittlerootTown_BrendansHouse_2F/script.pory"
	.string "Favorite starter region$"
