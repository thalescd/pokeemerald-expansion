#ifndef GUARD_HIDDEN_GROTTO_H
#define GUARD_HIDDEN_GROTTO_H

// Each grotto type has its own encounter table per route (grotto_<type>_mons in wild_encounters.json).
enum GrottoType
{
    GROTTO_TYPE_CAVE,
    GROTTO_TYPE_TREE,
    GROTTO_TYPE_SHRUB,
};

void SetCurrentHiddenGrotto(u32 secretBaseId);
void SetCurrentHiddenGrottoFromPosition(const struct MapPosition *position, const struct MapEvents *events);
void ToggleHiddenGrottoEntranceMetatile(void);
bool32 CurMapIsHiddenGrotto(void);
void InitHiddenGrottoMetatiles(void);
void RefillAllHiddenGrottos(void);
void UpdateHiddenGrottoStepCounter(void);

// Specials
void GetHiddenGrottoEntranceType(void);
void EnterHiddenGrotto(void);
void SetupHiddenGrottoInterior(void);
void CreateHiddenGrottoMon(void);
void ClearHiddenGrottoMon(void);

#endif // GUARD_HIDDEN_GROTTO_H
