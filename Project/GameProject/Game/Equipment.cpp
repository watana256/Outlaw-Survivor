#include "Equipment.h"
#include "Game.h"
#include "Score.h"


// 武器の詳細データ(並びは EquipmentIcon::SetIndex の番号と同じ)
struct WeaponInfo {
	const char* name;
	const char* desc1;
	const char* desc2;
	int damage;
	int price;
};
static const WeaponInfo WEAPON_INFO[9] = {
	{ "斧",           "斧は投げる物",       "",       30,  100 },
	{ "ピストル",     "扱いやすい基本の銃",     "連射速度は普通",       10,  100 },
	{ "ショットガン", "近距離で高威力",         "散弾で複数に当たる",   25,  100 },
	{ "火炎瓶",       "着弾地点を燃やす",       "継続ダメージを与える", 15,  600 },
	{ "火炎放射器",   "前方に炎を放射する",     "敵を貫通する",         20, 1000 },
	{ "地雷",         "設置して敵を待ち伏せ",   "踏むと爆発する",       50,  700 },
	{ "ドローン",     "自動で敵を攻撃する",     "プレイヤーに追従する", 12, 1200 },
	{ "ステラ",       "特殊な弾を発射する",     "高い性能を持つ",       40, 1500 },
	{ "ロケット",     "爆発する弾を発射",       "範囲ダメージが大きい", 60, 2000 },
};

Equipment::Equipment()
	:Base(eTpe_Equipment)
	,m_is_show(false)
	,m_select_index(-1)
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
		CRect rect(m_get_img.m_pos - m_get_img.m_center, m_get_img.m_size);
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
	// TODO: ここに強化処理を書く
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
	int price = WEAPON_INFO[index].price;
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
	/*if (PUSH(CInput::eButton5))
	{
		m_is_show = false;
	}*/

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
	if (m_select_index >= 0)
	{
		m_select_img.SetPos(CalcIconPos(m_select_index));
		m_select_img.Draw();

		const CVector2D preview_pos(1235.0f, 345.0f);
		m_icons[m_select_index].DrawAt(preview_pos);
	}

	if (m_select_index >= 0)
	{
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
	if (m_select_index >= 0)
	{
		m_select_img.SetPos(CalcIconPos(m_select_index));
		m_select_img.Draw();

		// 右の枠の上部に、選択した武器を表示
		const CVector2D preview_pos(1235.0f, 345.0f);
		m_icons[m_select_index].DrawAt(preview_pos);

		// アイコンの下に詳細を表示
		const WeaponInfo& info = WEAPON_INFO[m_select_index];
		float x = 1060.0f;
		float y = 470.0f;
		const float line = 36.0f;
		CFont* font = FONT_T();
		font->Draw(x, y, 1, 1, 0, "%s", info.name);   // 名前(黄色)
		font->Draw(x, y + line * 1, 1, 1, 1, "%s", info.desc1);
		font->Draw(x, y + line * 2, 1, 1, 1, "%s", info.desc2);
		font->Draw(x, y + line * 3, 1, 1, 1, "攻撃力: %d", info.damage);
		if (IsFreePick(m_select_index))
			font->Draw(x, y + line * 4, 0, 1, 0, "価格: 無料(1つだけ)");
		else
			font->Draw(x, y + line * 4, 1, 1, 1, "価格: %d", info.price);
		// 購入状態の表示
		if (m_owned[m_select_index])
			font->Draw(x, y + line * 5, 0, 1, 0, "購入済み");
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
	//m_font->Draw(64, 256, 0, 0, 0, "Title");
}
