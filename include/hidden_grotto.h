#ifndef GUARD_HIDDEN_GROTTO_H
#define GUARD_HIDDEN_GROTTO_H

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
