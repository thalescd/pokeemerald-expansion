#ifndef GUARD_BUG_CATCHING_CONTEST_H
#define GUARD_BUG_CATCHING_CONTEST_H

struct Pokemon;

#include "constants/bug_catching_contest.h"

extern const u8 BugContest_EventScript_OutOfBalls[];

extern u8 gNumBugContestBalls;
extern u16 gBugContestStepCounter;

bool8 IsBugContestActive(void);
bool8 BugContestTakeStep(void);
void BugContestRetirePrompt(void);

// Battle
void BugContest_BuildBattleMenuText(u8 *dst);
bool8 BugContest_HasCaughtMon(void);
void BugContest_BufferSwitchPrompt(struct Pokemon *newMon);
void BugContest_StoreCaughtMon(struct Pokemon *mon);

// Whiteout
void BugContest_DoWhiteOut(void);

#endif // GUARD_BUG_CATCHING_CONTEST_H
