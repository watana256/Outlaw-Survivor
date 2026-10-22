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

// 次のレベルに上げる費用(レベルが上がるほど高くなる)
// Lv1→2: 価格の50%, Lv2→3: 100%, Lv3→4: 150%, Lv4→5: 200%
int Equipment::GetUpgradeCost(int index) const
{
	int base = GetWeaponData(index).price;
	return base / 2 * m_level[index];
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
	return CVector2D(1450.0f + 140.0f * slot, 960.0f);
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
	if (PUSH(CInput::eMouseL))
	{
		CVector2D pos = CInput::GetMousePoint();
		CRect rect(m_end_img.m_pos, m_end_img.m_size);
		if (CCollision::CollisionRectPoint(rect, pos)) {
			m_is_show = false;   // Gameが検知してRoundIntroへ進む
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
	if (m_level[index] >= 5) return;               // 最大レベル

	int cost = GetUpgradeCost(index);
	if (Score::GetScore() < cost) return;          // スコア不足

	Score::AddScore(-cost);
	m_level[index]++;
}

void Equipment::BuyWeapon(int index)
{
	if (m_owned[index]) return;                  // 購入済みは買えない
	// 無料枠: スコアは減らさず、枠を使い切る
	if (IsFreePick(index))
	{
		m_owned[index] = true;
		m_free_available = false;
		return;
	}
	int price = GetWeaponData(index).price;
	if (Score::GetScore() < price) return;       // スコア不足

	Score::AddScore(-price);                     // スコアを減らす
	m_owned[index] = true;                       // 購入済みにする
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

	for (int i = 0; i < 9; i++) {
		EquipmentIcon& icon = m_icons[i];
		CVector2D pos = CalcIconPos(i);
		icon.SetPos(pos);
		icon.Draw();
	}

	// 選択枠と、右の枠の上部のプレビュー
	if (m_select_index >= 0)
	{
		m_select_img.SetPos(CalcIconPos(m_select_index));
		m_select_img.Draw();

		const CVector2D preview_pos(1235.0f, 345.0f);
		m_icons[m_select_index].DrawAt(preview_pos);

		// Get / UPG ボタン
		if (m_owned[m_select_index])
			m_upg_img.Draw();
		else
			m_get_img.Draw();
	}

	// 右下の装備枠3つ
	for (int i = 0; i < 3; i++)
	{
		if (m_equipped[i] >= 0)
			m_icons[m_equipped[i]].DrawAt(CalcSlotPos(i));
	}

	m_end_img.Draw();

	// アイコンの下に詳細を表示
	if (m_select_index >= 0)
	{
		const WeaponData& info = GetWeaponData(m_select_index);
		float x = 1060.0f;
		float y = 470.0f;
		const float line = 36.0f;
		CFont* font = FONT_T();
		font->Draw(x, y, 1, 1, 0, "%s", info.name);   // 名前(黄色)
		font->Draw(x, y + line * 1, 1, 1, 1, "%s", info.desc1);
		font->Draw(x, y + line * 2, 1, 1, 1, "%s", info.desc2);

		// 攻撃力(レベルに応じて変わる。強化できるときは次のレベルも表示)
		int lv = m_level[m_select_index];
		int dmg = GetWeaponDamage(m_select_index, lv);
		if (m_owned[m_select_index] && lv < 5)
			font->Draw(x, y + line * 3, 1, 1, 1, "攻撃力: %d → %d",
				dmg, GetWeaponDamage(m_select_index, lv + 1));
		else
			font->Draw(x, y + line * 3, 1, 1, 1, "攻撃力: %d", dmg);

		if (IsFreePick(m_select_index))
			font->Draw(x, y + line * 4, 0, 1, 0, "価格: 無料(1つだけ)");
		else
			font->Draw(x, y + line * 4, 1, 1, 1, "価格: %d", info.price);

		// 購入状態の表示
		if (m_owned[m_select_index])
		{
			if (m_level[m_select_index] >= 5)
				font->Draw(x, y + line * 5, 1, 1, 0, "Lv.5 (最大)");
			else
			{
				int cost = GetUpgradeCost(m_select_index);
				bool ok = Score::GetScore() >= cost;
				font->Draw(x, y + line * 5, ok ? 0.0f : 1.0f, ok ? 1.0f : 0.0f, 0,
					"Lv.%d  強化費用: %d", m_level[m_select_index], cost);
			}
		}
		else if (!IsFreePick(m_select_index) && Score::GetScore() < info.price)
			font->Draw(x, y + line * 5, 1, 0, 0, "お金が足りません");

		if (m_owned[m_select_index])
		{
			if (FindEquipSlot(m_select_index) >= 0)
				font->Draw(x, y + line * 6, 1, 1, 0, "装備中(右クリックで解除)");
			else
				font->Draw(x, y + line * 6, 1, 1, 1, "右クリックで装備");
		}
	}
}
