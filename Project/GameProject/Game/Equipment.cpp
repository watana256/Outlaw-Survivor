#include "Equipment.h"
#include "Game.h"
#include "Score.h"
#include "Weapon/Ax.h"
#include "Weapon/Pistol.h"
#include "Weapon/Shotgun.h"
#include "Weapon/Molotov.h"
#include "Weapon/Flamethrower.h"
#include "Weapon/Landmine.h"
#include "Weapon/Drone.h"
#include "Weapon/Stella.h"
#include "Weapon/Rocket.h"

// 番号(EquipmentIcon::SetIndex と同じ並び)から武器のデータを取得
static const WeaponData& GetWeaponData(int index)
{
	switch (index)
	{
	case 0: return Ax::GetData();
	case 1: return Pistol::GetData();
	case 2: return Shotgun::GetData();
	case 3: return Molotov::GetData();
	case 4: return Flamethrower::GetData();
	case 5: return Landmine::GetData();
	case 6: return Drone::GetData();
	case 7: return Stella::GetData();
	default: return Rocket::GetData();
	}
}

// 武器番号とレベルから、そのレベルの攻撃力を取得
static int GetWeaponDamage(int index, int level)
{
	switch (index)
	{
	case 0: return Ax::GetDamageAtLevel(level);
	case 1: return Pistol::GetDamageAtLevel(level);
	case 2: return Shotgun::GetDamageAtLevel(level);
	case 3: return Molotov::GetDamageAtLevel(level);
	case 4: return Flamethrower::GetDamageAtLevel(level);
	case 5: return Landmine::GetDamageAtLevel(level);
	case 6: return Drone::GetDamageAtLevel(level);
	case 7: return Stella::GetDamageAtLevel(level);
	default: return Rocket::GetDamageAtLevel(level);
	}
}
// 飛距離(弾を飛ばさない、または未対応の武器は 0)
static float GetWeaponFlyDist(int index, int level)
{
	switch (index)
	{
	case 0: return Ax::GetFlyingDistAtLevel(level);
	case 1: return Pistol::GetFlyingDistAtLevel(level);
	case 2: return Shotgun::GetFlyingDistAtLevel(level);
	case 3: return Molotov::GetFlyingDistAtLevel(level);
	case 4: return Flamethrower::GetFlyingDistAtLevel(level);
	case 6: return Drone::GetFlyingDistAtLevel(level);
	case 8: return Rocket::GetFlyingDistAtLevel(level);
	}
	return 0.0f;
}

// 弾速(弾速がない、または未対応の武器は 0)
static float GetWeaponBulletSpeed(int index, int level)
{
	switch (index)
	{
	case 0: return Ax::GetBulletSpeedAtLevel(level);
	case 1: return Pistol::GetBulletSpeedAtLevel(level);
	case 2: return Shotgun::GetBulletSpeedAtLevel(level);
	case 4: return Flamethrower::GetBulletSpeedAtLevel(level);
	case 6: return Drone::GetBulletSpeedAtLevel(level);
	}
	return 0.0f;
}

Equipment::Equipment()
	:Base(eTpe_Equipment)
	, m_is_show(false)
	, m_select_index(-1)
	, m_show_count(0)
	, m_free_available(false)
{
	m_img = COPY_RESOURCE("SHOP", CImage);

	//m_font = GET_RESOURCE("ShopFont", CFont);
	for (int i = 0; i < 9; i++) {
		EquipmentIcon& icon = m_icons[i];
		icon.SetIndex(i);
	}

	// 左下のEndボタン
	m_end_img = COPY_RESOURCE("End", CImage);
	m_end_img.SetPos(CVector2D(20.0f, 900.0f));

	m_select_img = COPY_RESOURCE("waku", CImage);
	m_select_img.SetSize(229.0f, 195.0f);
	m_select_img.SetCenter(m_select_img.GetSize() * 0.5f);

	// 右の枠の下に置くGetボタン(中心基準)
	m_get_img = COPY_RESOURCE("Get", CImage);
	m_get_img.SetSize(260.0f, 160.0f);                       // 先にサイズを決める
	m_get_img.SetCenter(m_get_img.GetSize() * 0.5f);         // その後で中心を設定
	m_get_img.SetPos(CVector2D(1235.0f, 800.0f));

	// UPGボタン(Getと同じサイズ・同じ位置)
	m_upg_img = COPY_RESOURCE("UPG", CImage);
	m_upg_img.SetSize(260.0f, 160.0f);
	m_upg_img.SetCenter(m_upg_img.GetSize() * 0.5f);
	m_upg_img.SetPos(CVector2D(1235.0f, 800.0f));

	// 「Ki」(買えない・強化できないときにボタンの真ん中へ表示)
	m_ki_img = COPY_RESOURCE("Ki", CImage);
	m_ki_img.SetSize(100.0f, 100.0f);
	m_ki_img.SetCenter(m_ki_img.GetSize() * 0.5f);
	m_ki_img.SetPos(CVector2D(1235.0f, 800.0f));

	for (int i = 0; i < 9; i++) m_owned[i] = false;
	for (int i = 0; i < 3; i++) m_equipped[i] = -1;
	for (int i = 0; i < 9; i++) m_level[i] = 1;
}
bool Equipment::IsShow() const
{
	return m_is_show;
}
void Equipment::Show()
{
	m_is_show = true;
	m_select_index = -1;
	m_free_available = (m_show_count == 0);   // 最初のShowだけ無料枠あり
	m_show_count++;
}

// 次のレベルに上げる費用 = 価格 × 現在のレベル × 2
// 例(価格500): Lv1→2 が1000, Lv2→3 が2000, Lv3→4 が3000 ...
int Equipment::GetUpgradeCost(int index) const
{
	int base = GetWeaponData(index).price;
	return base * m_level[index] * 2;
}

int Equipment::GetLevel(int index) const
{
	return m_level[index];
}

// 武器indexが何番の枠に入っているか(なければ-1)
int Equipment::FindEquipSlot(int index) const
{
	for (int i = 0; i < 3; i++)
		if (m_equipped[i] == index) return i;
	return -1;
}

// 右下の装備枠の位置(中心座標)
CVector2D Equipment::CalcSlotPos(int slot) const
{
	return CVector2D(1500.0f + 140.0f * slot, 965.0f);
}

int Equipment::GetEquipped(int slot) const
{
	return m_equipped[slot];
}

// 選択中の武器を右クリックで 装備 / 解除
void Equipment::UpdateEquip()
{
	if (m_select_index < 0) return;
	if (!PUSH(CInput::eMouseR)) return;
	if (!m_owned[m_select_index]) return;      // 未購入は装備できない

	int slot = FindEquipSlot(m_select_index);
	if (slot >= 0)
	{
		m_equipped[slot] = -1;                 // 解除
		return;
	}
	for (int i = 0; i < 3; i++)                // 空き枠に装備
	{
		if (m_equipped[i] < 0)
		{
			m_equipped[i] = m_select_index;
			return;
		}
	}
}

// 無料枠が残っていて、上段(0,1,2)の未購入アイテムなら true
bool Equipment::IsFreePick(int index) const
{
	return m_free_available && index < 3 && !m_owned[index];
}
CVector2D Equipment::CalcIconPos(int index) const
{
	CVector2D start = CVector2D(414.5f, 375.0f);
	return  start + CVector2D(229.0f * (index % 3), 195.0f * (index / 3));
}
void Equipment::UpdateSelectIcon()
{
	if (PUSH(CInput::eMouseL))
	{
		CVector2D pos = CInput::GetMousePoint();
		for (int i = 0; i < 9; i++) {
			EquipmentIcon& icon = m_icons[i];
			if (CCollision::CollisionRectPoint(icon.GetRect(), pos)) {
				m_select_index = i;
				break;
			}
		}
	}
}
void Equipment::UpdateEndButton()
{
	if (!HasEquipment()) return;   // 装備がなければ終了できない

	if (PUSH(CInput::eMouseL))
	{
		CVector2D pos = CInput::GetMousePoint();
		CRect rect(m_end_img.m_pos, m_end_img.m_size);
		if (CCollision::CollisionRectPoint(rect, pos)) {
			m_is_show = false;   // Gameが検知してラウンドへ進む
		}
	}
}

void Equipment::UpdateGetButton()
{
	if (m_select_index < 0) return;   // 武器を選んでいない間は押せない

	if (PUSH(CInput::eMouseL))
	{
		CVector2D pos = CInput::GetMousePoint();
		// 表示中のボタン(未購入ならGet、購入済みならUPG)で判定する
		CImage& btn = m_owned[m_select_index] ? m_upg_img : m_get_img;
		CRect rect(btn.m_pos - btn.m_center, btn.m_size);
		if (CCollision::CollisionRectPoint(rect, pos)) {
			if (m_owned[m_select_index])
				UpgradeWeapon(m_select_index);
			else
				BuyWeapon(m_select_index);
		}
	}
}
void Equipment::UpgradeWeapon(int index)
{
	if (!m_owned[index]) return;
	if (m_level[index] >= MAX_WEAPON_LEVEL) return;               // 最大レベル

	int cost = GetUpgradeCost(index);
	if (Score::GetScore() < cost) return;          // スコア不足

	Score::AddScore(-cost);
	m_level[index]++;
}

// 選択中の武器を、いま購入または強化できるか
bool Equipment::CanAct(int index) const
{
	if (m_owned[index])
	{
		if (m_level[index] >= MAX_WEAPON_LEVEL) return false;           // 最大レベル
		return Score::GetScore() >= GetUpgradeCost(index);
	}
	if (IsFreePick(index)) return true;                    // 無料枠
	return Score::GetScore() >= GetWeaponData(index).price;
}

// 装備枠に武器が1つでも入っているか
bool Equipment::HasEquipment() const
{
	for (int i = 0; i < 3; i++)
		if (m_equipped[i] >= 0) return true;
	return false;
}
void Equipment::BuyWeapon(int index)
{
	if (m_owned[index]) return;                  // 購入済みは買えない

	// 無料枠: スコアは減らさず、枠を使い切る
	if (IsFreePick(index))
	{
		m_owned[index] = true;
		m_free_available = false;
	}
	else
	{
		int price = GetWeaponData(index).price;
		if (Score::GetScore() < price) return;   // スコア不足

		Score::AddScore(-price);                 // スコアを減らす
		m_owned[index] = true;                   // 購入済みにする
	}

	// 装備枠に空きがあれば、自動で装備する
	for (int i = 0; i < 3; i++)
	{
		if (m_equipped[i] < 0)
		{
			m_equipped[i] = index;
			break;
		}
	}
}
bool Equipment::IsOwned(int index) const
{
	return m_owned[index];
}
void Equipment::Update()
{
	if (!m_is_show)return;

	UpdateEndButton();
	if (!m_is_show) return;   // Endを押したフレームは以降の処理を飛ばす

	UpdateGetButton();
	UpdateSelectIcon();
	UpdateEquip();
}
void Equipment::Draw()
{
	if (!m_is_show)return;
	m_img.Draw();

	// ① 武器アイコン一覧
	CFont* font_e = FONT_T();
	for (int i = 0; i < 9; i++) {
		EquipmentIcon& icon = m_icons[i];
		CVector2D pos = CalcIconPos(i);
		icon.SetPos(pos);

		// 買えない・強化できないときは暗くする(Kiは出さない)
		bool can = CanAct(i);
		if (!can) icon.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
		icon.Draw();
		icon.SetColor(1.0f, 1.0f, 1.0f, 1.0f);              // 元の色に戻す

		// 装備中の武器は、マスの右下の隅に緑の「E」を表示
		if (FindEquipSlot(i) >= 0)
			font_e->Draw(pos.x + 70.0f, pos.y + 55.0f, 0, 1, 0, "E");
	}

	// ② 選択枠と、右の枠の上部のプレビュー
	if (m_select_index >= 0)
	{
		m_select_img.SetPos(CalcIconPos(m_select_index));
		m_select_img.Draw();

		const CVector2D preview_pos(1235.0f, 345.0f);
		m_icons[m_select_index].DrawAt(preview_pos);

		// ③ Get / UPG ボタン(できないときは暗くして「Ki」を重ねる)
		CImage& btn = m_owned[m_select_index] ? m_upg_img : m_get_img;
		bool ok = CanAct(m_select_index);
		if (!ok) btn.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
		btn.Draw();
		btn.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		if (!ok)
		{
			m_ki_img.SetPos(CVector2D(1235.0f, 800.0f));
			m_ki_img.Draw();
		}
	}

	// ④ 右下の装備枠3つ
	for (int i = 0; i < 3; i++)
	{
		if (m_equipped[i] >= 0)
		{
			CVector2D slot_pos = CalcSlotPos(i);
			m_icons[m_equipped[i]].DrawFit(slot_pos, 120.0f, 80.0f);
			font_e->Draw(slot_pos.x + 44.0f, slot_pos.y + 18.0f, 0, 1, 0, "E");
		}
	}

	// Endボタン(装備がないときは暗くして、理由を表示)
	if (!HasEquipment())
	{
		m_end_img.SetColor(0.4f, 0.4f, 0.4f, 1.0f);
		m_end_img.Draw();
		m_end_img.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
		font_e->Draw(20.0f, 880.0f, 1, 0, 0, "装備を1つ以上つけてください");
	}
	else
	{
		m_end_img.Draw();
	}

	// ⑤ アイコンの下に詳細を表示
	if (m_select_index >= 0)
	{
		const WeaponData& info = GetWeaponData(m_select_index);
		float x = 1060.0f;
		float y = 470.0f;
		const float line = 30.0f;
		CFont* font = FONT_T();
		int row = 0;
		int lv = m_level[m_select_index];
		bool can_up = m_owned[m_select_index] && lv < MAX_WEAPON_LEVEL;

		font->Draw(x, y + line * row++, 1, 1, 0, "%s", info.name);   // 名前(黄色)
		font->Draw(x, y + line * row++, 1, 1, 1, "%s", info.desc1);
		font->Draw(x, y + line * row++, 1, 1, 1, "%s", info.desc2);

		// 攻撃力(強化できるときは次のレベルも表示)
		int dmg = GetWeaponDamage(m_select_index, lv);
		if (can_up)
			font->Draw(x, y + line * row++, 1, 1, 1, "攻撃力: %d → %d",
				dmg, GetWeaponDamage(m_select_index, lv + 1));
		else
			font->Draw(x, y + line * row++, 1, 1, 1, "攻撃力: %d", dmg);

		// 飛距離(0の武器は表示しない)
		float dist = GetWeaponFlyDist(m_select_index, lv);
		if (dist > 0.0f)
		{
			if (can_up)
				font->Draw(x, y + line * row++, 1, 1, 1, "飛距離: %d → %d",
					(int)dist, (int)GetWeaponFlyDist(m_select_index, lv + 1));
			else
				font->Draw(x, y + line * row++, 1, 1, 1, "飛距離: %d", (int)dist);
		}

		// 弾速(0の武器は表示しない)
		float spd = GetWeaponBulletSpeed(m_select_index, lv);
		if (spd > 0.0f)
		{
			if (can_up)
				font->Draw(x, y + line * row++, 1, 1, 1, "弾速: %d → %d",
					(int)spd, (int)GetWeaponBulletSpeed(m_select_index, lv + 1));
			else
				font->Draw(x, y + line * row++, 1, 1, 1, "弾速: %d", (int)spd);
		}

		if (IsFreePick(m_select_index))
			font->Draw(x, y + line * row++, 0, 1, 0, "価格: 無料(1つだけ)");
		else
			font->Draw(x, y + line * row++, 1, 1, 1, "価格: %d", info.price);

		// 購入状態の表示
		if (m_owned[m_select_index])
		{
			if (lv >= MAX_WEAPON_LEVEL)
				font->Draw(x, y + line * row++, 1, 1, 0, "Lv.%d (最大)", lv);
			else
			{
				int cost = GetUpgradeCost(m_select_index);
				bool afford = Score::GetScore() >= cost;
				font->Draw(x, y + line * row++, afford ? 0.0f : 1.0f, afford ? 1.0f : 0.0f, 0,
					"Lv.%d  強化費用: %d", lv, cost);
			}

			if (FindEquipSlot(m_select_index) >= 0)
				font->Draw(x, y + line * row++, 1, 1, 0, "装備中(右クリックで解除)");
			else
				font->Draw(x, y + line * row++, 1, 1, 1, "右クリックで装備");
		}
		else if (!IsFreePick(m_select_index) && Score::GetScore() < info.price)
			font->Draw(x, y + line * row++, 1, 0, 0, "お金が足りません");
	}
}

