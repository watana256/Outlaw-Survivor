#pragma once
#pragma once

#define MAX_WEAPON_LEVEL 10

// レベルを 1〜最大 に収める
inline int ClampWeaponLevel(int level)
{
	if (level < 1) return 1;
	if (level > MAX_WEAPON_LEVEL) return MAX_WEAPON_LEVEL;
	return level;
}
struct WeaponData {
	const char* name;
	const char* desc1;
	const char* desc2;
	int damage;
	int price;
};