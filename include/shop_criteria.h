#ifndef GUARD_SHOP_CRITERIA_H
#define GUARD_SHOP_CRITERIA_H

void TryBuildDynamicShopItemList(const u16 **ogItemList, u16 *resultingTotal);
void TryFreeDynamicShopItemList(const u16 **ogItemList);

// Add new Criterias below!
bool32 ShopCriteriaHasPokedex(enum Item item);
bool32 ShopCriteriaAfter1Badge(enum Item item);
bool32 ShopCriteriaAfter2Badges(enum Item item);
bool32 ShopCriteriaAfter3Badges(enum Item item);
bool32 ShopCriteriaAfter4Badges(enum Item item);
bool32 ShopCriteriaAfter5Badges(enum Item item);

#endif // GUARD_SHOP_CRITERIA_H
