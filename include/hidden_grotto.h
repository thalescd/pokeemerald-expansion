#ifndef GUARD_HIDDEN_GROTTO_H
#define GUARD_HIDDEN_GROTTO_H

void SetCurrentHiddenGrotto(u32 secretBaseId);
void RefillAllHiddenGrottos(void);
void UpdateHiddenGrottoStepCounter(void);

// Specials
void SetupHiddenGrottoInterior(void);
void CreateHiddenGrottoMon(void);
void ClearHiddenGrottoMon(void);

#endif // GUARD_HIDDEN_GROTTO_H
