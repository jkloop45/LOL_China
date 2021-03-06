#include "stdafx.h"
#include "GameRes.h"
#include "Equment.h"
#include <limits>
#include <MyTools/CLLog.h>
#include <MyTools/CLPublic.h>

#define _SELF L"GameRes.cpp"

CGameRes& CGameRes::GetInstance() throw()
{
	static CGameRes GameRes;
	return GameRes;
}

CONST Point& CGameRes::GetBlueBuffPointByCamp(_In_ em_Camp emCamp) CONST throw()
{
	CONST static Point BlueCamp(3548.00f, 8206.00f, 51.19f);
	CONST static Point RedCamp(11402.00f, 6532.00f, 51.70f);
	return emCamp == em_Camp::em_Camp_Blue ? BlueCamp : RedCamp;
}

CONST Point& CGameRes::GetRedBuffPointByCamp(_In_ em_Camp emCamp) CONST throw()
{
	CONST static Point BlueCamp(7488.00f, 3716.00f, 52.00f);
	CONST static Point RedCamp(7312.00f, 11140.00f, 55.15f);
	return emCamp == em_Camp::em_Camp_Blue ? BlueCamp : RedCamp;
}

CONST Point& CGameRes::GetDroganPoint() CONST throw()
{
	CONST static Point DroganPoint(10186.00f, 4882.00f, -71.24f);
	return DroganPoint;
}

CONST Point& CGameRes::GetBaronPoint() CONST throw()
{
	CONST static Point BaronPoint(4588.0f, 9872.0f, -68.0f);
	return BaronPoint;
}

cwstring* CGameRes::GetCrystalTurretNameByCampAndType(_In_ em_Camp emCamp, _In_ em_Path_Type emPathType) CONST throw()
{
	struct tagCrystalTurret
	{
		wstring wsTurretName;
		em_Camp emCamp;
		em_Path_Type emPathType;
	};

	static CONST vector<tagCrystalTurret> vlst = {
		// 蓝方上路
		{ L"Barracks_T1_L1", em_Camp_Blue, em_Path_Type::Path_Type_Top },

		// 蓝方中路
		{ L"Barracks_T1_C1", em_Camp_Blue, em_Path_Type::Path_Type_Middle },

		// 蓝方下路
		{ L"Barracks_T1_R1", em_Camp_Blue, em_Path_Type::Path_Type_Buttom },


		// 红方上路
		{ L"Barracks_T2_L1", em_Camp_Red, em_Path_Type::Path_Type_Top },

		// 红方中路
		{ L"Barracks_T2_C1", em_Camp_Red, em_Path_Type::Path_Type_Middle },

		// 红方下路
		{ L"Barracks_T2_R1", em_Camp_Red, em_Path_Type::Path_Type_Buttom }
	};

	auto ptagCrystalTurret = CLPublic::Vec_find_if(vlst, [&emCamp, &emPathType](CONST auto& itm) {
		return itm.emCamp == emCamp && itm.emPathType == emPathType;
	});

	return ptagCrystalTurret == nullptr ? nullptr : &ptagCrystalTurret->wsTurretName;
}

em_Path_Type CGameRes::GetDefaultPathTypeByHero(_In_ em_Hero_Pro emHeroPro) CONST throw()
{
	// 设置英雄的分路的路线
	switch (emHeroPro)
	{
	case em_Hero_Pro_Ryze: // 流浪法师中单
		return em_Path_Type::Path_Type_Middle;
		Log(LOG_LEVEL_NORMAL, L"Set Current Path = Middle!");
	case em_Hero_Pro_Swain: case em_Hero_Pro_Ashe: case em_Hero_Pro_Ezreal:  case em_Hero_Pro_Chogath:  case  em_Hero_Pro_Maokai:
	case em_Hero_Pro_MissFortune: case em_Hero_Pro_Sejuani: case em_Hero_Pro_Galio: case em_Hero_Pro_KogMaw:
		Log(LOG_LEVEL_NORMAL, L"Set Current Path = Top!");
		return em_Path_Type::Path_Type_Top;
	case em_Hero_Pro_Malzahar: case em_Hero_Pro_MasterYi: case em_Hero_Pro_Graves: case em_Hero_Pro_Vayne:
	case em_Hero_Pro_Heimerdinger: case em_Hero_Pro_Nunu: case em_Hero_Pro_Garen: case em_Hero_Pro_Veigar:
	case em_Hero_Pro_Tristana: case em_Hero_Pro_Lucian: case em_Hero_Pro_Corki: case em_Hero_Pro_Caitlyn:
		Log(LOG_LEVEL_NORMAL, L"Set Current Path = Buttom!");
		return em_Path_Type::Path_Type_Buttom;
	default:
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"少年,这个英雄:%X暂时还不支持喔~!", emHeroPro);
		break;
	}
	return em_Path_Type::Path_Type_Middle;
}

CONST cwstring& CGameRes::GetBaseNameByCamp(_In_ em_Camp emCamp) CONST throw()
{
	static cwstring BlueCamp(L"HQ_T1");
	static cwstring RedCamp(L"HQ_T2");
	return emCamp == em_Camp::em_Camp_Blue ? BlueCamp : RedCamp;
}

CONST Point* CGameRes::GetNextMovePoint(_In_ em_Camp emCamp, _In_ em_Path_Type emPathType, _In_ CONST Point& CurPoint) CONST throw()
{
	auto& Vec = GetPathPointVecByCampAndPathType(emCamp, emPathType);
	if (Vec.size() == 0)
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"GetPathPointVecByCampAndPathType.size=0!");
		return nullptr;
	}

	int Index = CLPublic::GetRecentlyIndexByVec(Vec, CurPoint);
	if (Index == -1)
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"GetRecentlyIndexByVec=-1!, CurPoint=[%d,%d,%d]", CurPoint.GetX(), CurPoint.GetY(),CurPoint.GetZ());
		return nullptr;
	}

	return Index + 1 >= static_cast<decltype(Index)>(Vec.size()) ? &Vec.at(Vec.size() - 1) : &Vec.at(Index + 1);
}

CONST Point* CGameRes::GetPreviouMovePoint(_In_ em_Camp emCamp, _In_ em_Path_Type emPathType, _In_ CONST Point& CurPoint) CONST throw()
{
	auto& Vec = GetPathPointVecByCampAndPathType(emCamp, emPathType);
	if (Vec.size() == 0)
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"GetPathPointVecByCampAndPathType.size=0!");
		return nullptr;
	}

	auto Index = CLPublic::GetRecentlyIndexByVec(Vec, CurPoint);
	if (Index == -1)
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"GetRecentlyIndexByVec=-1!, CurPoint=[%d,%d,%d]", CurPoint.GetX(), CurPoint.GetY(), CurPoint.GetZ());
		return nullptr;
	}
	Log(LOG_LEVEL_NORMAL, L"GetPreviouMovePoint: Index=%d", Index);
	return Index - 1 >= 0 ? &Vec.at(Index - 1) : &Vec.at(0);
}

float CGameRes::GetHeroAttackDis(_In_ em_Hero_Pro emHeroPro) CONST throw()
{
	switch (emHeroPro)
	{
	case em_Hero_Pro::em_Hero_Pro_Ryze: case em_Hero_Pro::em_Hero_Pro_Ashe:
		return 7.0f;
	case em_Hero_Pro::em_Hero_Pro_Garen:
		return 2.0f;
	default:
		break;
	}
	return 2.0f;
}

CONST vector<Point>& CGameRes::GetPathPointVecByCampAndPathType(_In_ em_Camp emCamp, _In_ em_Path_Type emPathType) CONST throw()
{
	static CONST vector<Point> TopBlueCamp = {
		Point(604.00f, 612.00f, 183.57f),		Point(1154.00f, 3910.00f, 95.75f),		Point(1400.00f, 6086.00f, 53.76f), 
		Point(1018.00f, 9678.00f, 52.84f),		Point(1490.00f, 11490.00f, 52.84f),		Point(2224.00f, 12508.00f, 52.84f), 
		Point(3328.00f, 13268.00f, 52.84f),		Point(4828.00f, 13892.00f, 52.84f),		Point(8568.00f, 13390.00f, 52.84f), 
		Point(10836.00f, 13670.00f, 91.43f),	Point(14576.36f, 14693.83f, 171.98f)
	};
	static CONST vector<Point> MiddleBlueCamp = {
		Point(604.00f, 612.00f, 183.57f),		Point(1210.00f, 1240.00f, 95.75f),		Point(2064.00f, 2730.00f, 95.75f),
		Point(3350.00f, 3468.00f, 95.75f),		Point(4338.00f, 4412.00f, 71.06f),		Point(5472.00f, 6044.00f, 51.14f),
		Point(6784.00f, 6776.00f, 51.89f),		Point(7434.00f, 7332.00f, 53.84f),		Point(8168.00f, 8032.00f, 53.15f),
		Point(9154.00f, 9020.00f, 54.01f),		Point(10190.00f, 10432.00f, 52.30f),	Point(11362.00f, 11360.00f, 91.43f),
		Point(12630.00f, 11684.00f, 91.43f),	Point(14576.36f, 14693.83f, 171.98f)
	};
	static CONST vector<Point> ButtomBlueCamp = {
		Point(604.00f, 612.00f, 183.57f),		Point(1508.00f, 658.00f, 95.75f),		Point(2840.00f, 1200.00f, 95.75f),
		Point(3926.00f, 1216.00f, 95.75f),		Point(6516.00f, 1388.00f, 49.50f),		Point(9920.00f, 1310.00f, 51.30f),
		Point(11690.00f, 1590.00f, 50.39f),		Point(12378.00f, 2234.00f, 52.17f),		Point(13446.00f, 3576.00f, 51.37f),
		Point(13720.00f, 4948.00f, 52.76f),		Point(13402.00f, 8752.00f, 52.31f),		Point(13624.00f, 10906.00f, 91.43f),
		Point(13578.00f, 11752.00f, 91.43f),	Point(13976.00f, 13098.00f, 91.43f),	Point(14576.36f, 14693.83f, 171.98f),
	};

	static vector<Point> TopRedCamp;
	static vector<Point> MiddleRedCamp;
	static vector<Point> ButtomRedCamp;

	// Initialization Red Camp
	if (TopRedCamp.size() == 0)
	{
		TopRedCamp.reserve(TopBlueCamp.size());
		std::reverse_copy(std::begin(TopBlueCamp), std::end(TopBlueCamp), TopRedCamp.begin());
	}
	if (MiddleRedCamp.size() == 0)
	{
		MiddleRedCamp.reserve(MiddleBlueCamp.size());
		std::reverse_copy(std::begin(MiddleBlueCamp), std::end(MiddleBlueCamp), MiddleRedCamp.begin());
	}
	if (ButtomRedCamp.size() == 0)
	{
		ButtomRedCamp.reserve(ButtomBlueCamp.size());
		std::reverse_copy(std::begin(ButtomBlueCamp), std::end(ButtomBlueCamp), ButtomRedCamp.begin());
	}

	// return Camp&&PathType
	if (emCamp == em_Camp::em_Camp_Red)
	{
		switch (emPathType)
		{
		case Path_Type_Top:
			return TopRedCamp;
		case Path_Type_Middle:
			return MiddleRedCamp;
		case Path_Type_Buttom:
			return ButtomRedCamp;
		default:
			LogMsgBox(LOG_LEVEL_EXCEPTION, L"emPathType=%X", emPathType);
			break;
		}
		return TopRedCamp;
	}

	switch (emPathType)
	{
	case Path_Type_Top:
		return TopBlueCamp;
	case Path_Type_Middle:
		return MiddleBlueCamp;
	case Path_Type_Buttom:
		return ButtomBlueCamp;
	default:
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"emPathType=%X", emPathType);
		break;
	}
	return TopBlueCamp;
}

auto CGameRes::GetResEuqmentVecByType(_In_ em_ResEqument_Type emResEqumentType) CONST throw() -> CONST vector<ResEqument>*
{
	static CONST vector<HeroResEqument> HeroResEqumentVec = {
			//
		HeroResEqument{ em_ResEqument_Type_Master_Defence, {
				ResEqument{ EQUMENT_ID_蓝水晶,350 }, ResEqument{ EQUMENT_ID_速度之靴,300},
				ResEqument{ EQUMENT_ID_女神之泪,400 },ResEqument{ EQUMENT_ID_轻灵之靴,600 },
				ResEqument{ EQUMENT_ID_万世催化石, 1100 },ResEqument{ EQUMENT_ID_爆裂魔杖 , 850 },
				ResEqument{ EQUMENT_ID_时光之杖, 750 },ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 },
				ResEqument{ EQUMENT_ID_负极斗篷, 720 },ResEqument{ EQUMENT_ID_传送门 , 780 },
				ResEqument{ EQUMENT_ID_巨人腰带, 1000 },ResEqument{ EQUMENT_ID_无用大棒 , 1250 },
				ResEqument{ EQUMENT_ID_瑞莱的冰晶节杖, 1000 },ResEqument{ EQUMENT_ID_无用大棒, 1250 },
				ResEqument{ EQUMENT_ID_大天使之杖, 1100 },ResEqument{ EQUMENT_ID_爆裂魔杖, 850 },
				ResEqument{ EQUMENT_ID_虚空之杖, 2650 }}},
			//
			HeroResEqument{ em_ResEqument_Type_Master_Attack ,{
				ResEqument{ EQUMENT_ID_蓝水晶,350 }, ResEqument{ EQUMENT_ID_速度之靴,300 },
				ResEqument{ EQUMENT_ID_遗失的章节 ,550 }, ResEqument{ EQUMENT_ID_法师之靴 ,800 },
				ResEqument{ EQUMENT_ID_恶魔法典,900 }, ResEqument{ EQUMENT_ID_莫雷洛秘典 ,1100 },
				ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 }, ResEqument{ EQUMENT_ID_负极斗篷, 720 },
				ResEqument{ EQUMENT_ID_传送门 , 780 },ResEqument{ EQUMENT_ID_巨人腰带, 1000 },
				ResEqument{ EQUMENT_ID_无用大棒 , 1250 }, ResEqument{ EQUMENT_ID_瑞莱的冰晶节杖, 1000 },
				ResEqument{ EQUMENT_ID_幽魂面具,1600 }, ResEqument{ EQUMENT_ID_爆裂魔杖 ,850 },
				ResEqument{ EQUMENT_ID_兰德里的折磨, 750 }, ResEqument{ EQUMENT_ID_爆裂魔杖, 850 },
				ResEqument{ EQUMENT_ID_虚空之杖, 2650 } 
				}},


			// 攻速型ADC
		HeroResEqument{ em_ResEqument_Type_ADC_AttackSpeed ,{
					ResEqument{ EQUMENT_ID_速度之靴, 300 }, ResEqument{ EQUMENT_ID_狂战士胫甲 ,800 },
					ResEqument{ EQUMENT_ID_吸血鬼节杖, 900 }, ResEqument{ EQUMENT_ID_比尔吉沃特弯刀 ,600 },
					ResEqument{ EQUMENT_ID_反曲之弓, 1000 }, ResEqument{ EQUMENT_ID_破败王者之刃,900 },
					ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 }, ResEqument{ EQUMENT_ID_负极斗篷, 720 },
					ResEqument{ EQUMENT_ID_传送门 , 780 },ResEqument{ EQUMENT_ID_最后的轻语 ,1300 },
					ResEqument{ EQUMENT_ID_巨人的杀手,1000 }, ResEqument{ EQUMENT_ID_多米尼克领主的致意 ,400 },
					ResEqument{ EQUMENT_ID_紫雨林之拳,1200 }, ResEqument{ EQUMENT_ID_十字犒,875 },
					ResEqument{ EQUMENT_ID_冰霜之锤,1025 }, ResEqument{ EQUMENT_ID_暴风之剑 ,1300 },
					ResEqument{ EQUMENT_ID_十字犒,875 }, ResEqument{ EQUMENT_ID_灵巧披风 ,800 },
					ResEqument{ EQUMENT_ID_无尽之刃,625 }
				}},
			 

			// 飓风型ADC
		HeroResEqument{ em_ResEqument_Type_ADC_AttackDis ,{
					ResEqument{ EQUMENT_ID_速度之靴, 300 }, ResEqument{ EQUMENT_ID_狂战士胫甲 ,800 },
					ResEqument{ EQUMENT_ID_反曲之弓,1000 }, ResEqument{ EQUMENT_ID_狂热 ,1300 },
					ResEqument{ EQUMENT_ID_卢安娜的飓风,300 }, ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 },
					ResEqument{ EQUMENT_ID_负极斗篷, 720 },ResEqument{ EQUMENT_ID_传送门 , 780 },
					ResEqument{ EQUMENT_ID_暴风之剑 ,1300 }, ResEqument{ EQUMENT_ID_十字犒,875 },
					ResEqument{ EQUMENT_ID_灵巧披风 ,800 },ResEqument{ EQUMENT_ID_无尽之刃,625 },
					ResEqument{ EQUMENT_ID_最后的轻语 ,1300 },ResEqument{ EQUMENT_ID_巨人的杀手,1000 },
					ResEqument{ EQUMENT_ID_多米尼克领主的致意 ,400 },ResEqument{ EQUMENT_ID_紫雨林之拳,1200 },
					ResEqument{ EQUMENT_ID_十字犒,875 },ResEqument{ EQUMENT_ID_冰霜之锤,1025 }
				}},
			
			// 攻击力型ADC
		HeroResEqument{ em_ResEqument_Type_ADC_AttackPower ,{
					ResEqument{ EQUMENT_ID_速度之靴, 300 }, ResEqument{ EQUMENT_ID_狂战士胫甲 ,800 },
					ResEqument{ EQUMENT_ID_暴风之剑,1300 }, ResEqument{ EQUMENT_ID_考尔菲德的战锤,1100 },
					ResEqument{ EQUMENT_ID_灵巧披风,800 }, ResEqument{ EQUMENT_ID_夺萃之镰 ,400 },
					ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 }, ResEqument{ EQUMENT_ID_负极斗篷, 720 },
					ResEqument{ EQUMENT_ID_传送门 , 780 },  ResEqument{ EQUMENT_ID_狂热 ,1300 },
					ResEqument{ EQUMENT_ID_幻影之舞,1250 }, ResEqument{ EQUMENT_ID_最后的轻语 ,1300 },
					ResEqument{ EQUMENT_ID_巨人的杀手,1000 },ResEqument{ EQUMENT_ID_多米尼克领主的致意 ,400 },
					ResEqument{ EQUMENT_ID_吸血鬼节杖 ,900 }, ResEqument{ EQUMENT_ID_暴风之剑,1300 },
					ResEqument{ EQUMENT_ID_饮血剑, 1500 },
				}},
			
			
				HeroResEqument{ em_ResEqument_Type_Defence_Attack ,{
					// 半肉
					ResEqument{ EQUMENT_ID_速度之靴,300 }, ResEqument{ EQUMENT_ID_水银之靴,800 },
					ResEqument{ EQUMENT_ID_净蚀,1250 }, ResEqument{ EQUMENT_ID_考尔菲德的战锤 ,1100 },
					ResEqument{ EQUMENT_ID_黑色切割者,750 },ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 },
					ResEqument{ EQUMENT_ID_负极斗篷, 720 },ResEqument{ EQUMENT_ID_传送门 , 780 },
					ResEqument{ EQUMENT_ID_红水晶,400 }, ResEqument{ EQUMENT_ID_斑比的熔渣 , 700 },
					ResEqument{ EQUMENT_ID_锁子甲,800 }, ResEqument{ EQUMENT_ID_日炎斗篷 ,1000 },
					ResEqument{ EQUMENT_ID_红水晶,400 }, ResEqument{ EQUMENT_ID_幽魂斗篷,800 },
					ResEqument{ EQUMENT_ID_振奋铠甲,1600 }, ResEqument{ EQUMENT_ID_巨人腰带,1000 },
					ResEqument{ EQUMENT_ID_狂徒铠甲,1850 },
				}},
			
				HeroResEqument{ em_ResEqument_Type_Defence_Defence ,{
					// 纯肉
					ResEqument{ EQUMENT_ID_速度之靴,300 }, ResEqument{ EQUMENT_ID_水银之靴,800 },
					ResEqument{ EQUMENT_ID_蓝水晶, 350 }, ResEqument{ EQUMENT_ID_冰川护甲, 650 },
					ResEqument{ EQUMENT_ID_耀光,1050 }, ResEqument{ EQUMENT_ID_冰脉护手,650 },
					ResEqument{ EQUMENT_ID_猛禽斗篷 , 1200 },
					ResEqument{ EQUMENT_ID_负极斗篷, 720 },ResEqument{ EQUMENT_ID_传送门 , 780 },
					ResEqument{ EQUMENT_ID_红水晶,400 }, ResEqument{ EQUMENT_ID_斑比的熔渣 , 700 },
					ResEqument{ EQUMENT_ID_锁子甲,800 }, ResEqument{ EQUMENT_ID_日炎斗篷 ,1000 },
					ResEqument{ EQUMENT_ID_红水晶,400 }, ResEqument{ EQUMENT_ID_幽魂斗篷,800 },
					ResEqument{ EQUMENT_ID_振奋铠甲,1600 }, ResEqument{ EQUMENT_ID_巨人腰带,1000 },
					ResEqument{ EQUMENT_ID_狂徒铠甲,1850 },
				}}
	};

	auto pHeroResEqument = CLPublic::Vec_find_if(HeroResEqumentVec, [&emResEqumentType](CONST HeroResEqument& itm) {
		return itm.emResEqumentType == emResEqumentType;
	});
	return pHeroResEqument != nullptr ? &pHeroResEqument->EqumentVec : nullptr;
}

auto CGameRes::GetNextEqumentId(_In_ em_Hero_Pro emHeroPro, _In_ DWORD dwLastEqumentId) CONST throw()  -> CONST ResEqument*
{
	// Get EqumentId list by Hero
	auto emResEqumentType = GetResEqumentTypeByHero(emHeroPro);
	auto pVec = GetResEuqmentVecByType(emResEqumentType);
	if (pVec == nullptr)
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"UnExist Hero Equment:%X", emHeroPro);
		return nullptr;
	}

	if (dwLastEqumentId == NULL)
		return &*pVec->begin();

	// find current equment id in pVec
	auto& ResEqumentItr = std::find_if(pVec->begin(), pVec->end(), [&dwLastEqumentId](CONST auto& itm) {
		return itm.dwEqumentId == dwLastEqumentId;
	});
	if (ResEqumentItr == pVec->end())
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"UnExist Hero Equment:%X, ID:%X", emHeroPro, dwLastEqumentId);
		return nullptr;
	}

	if (ResEqumentItr + 1 == pVec->end())
	{
		// full equment!
		return nullptr;
	}

	// Get Next Equment
	return &*(ResEqumentItr + 1);
}

DWORD CGameRes::GetCurrentLastEqumentId(_In_ em_Hero_Pro emHeroPro, _In_ vector<CEqument>& vlst) CONST
{
	auto emResEqumentType = GetResEqumentTypeByHero(emHeroPro);
	auto pVec = GetResEuqmentVecByType(emResEqumentType);
	if (pVec == nullptr)
	{
		LogMsgBox(LOG_LEVEL_EXCEPTION, L"UnExist Hero Equment:%X", emHeroPro);
		return NULL;
	}

	if (vlst.size() == 0)
		return NULL;

	for (auto itr = pVec->rbegin(); itr != pVec->rend(); ++itr)
	{
		if (CLPublic::Vec_find_if(vlst, [&itr](CONST CEqument& itm) { return itm.GetId() == itr->dwEqumentId; }) != nullptr)
			return itr->dwEqumentId;
	}

	return NULL;
}

auto CGameRes::GetResSkillByHero(_In_ em_Hero_Pro emHeroPro) CONST throw() -> CONST ResSkill*
{
	return CLPublic::Vec_find_if(GetResSkillList(), [&emHeroPro](CONST auto& itm) { return itm.emHeroPro == emHeroPro; });
}

auto CGameRes::GetResSkillByHero(_In_ cwstring& wsHeroName) CONST throw()->CONST ResSkill*
{
	return CLPublic::Vec_find_if(GetResSkillList(), [&wsHeroName](CONST auto& itm) { return itm.HeroName == wsHeroName; });
}

em_Skill_Index CGameRes::GetSPByHeroLevel(_In_ em_Hero_Pro emHeroPro, _In_ DWORD dwLevel) CONST throw()
{
	auto fn_StudySkill_Q = [](DWORD dwLevel)
	{
		switch (dwLevel)
		{
		case 1: case 4: case 8: case 12: case 15:
			return em_Skill_Index_Q;
		case 2: case 5: case 9: case 13: case 17:
			return em_Skill_Index_W;
		case 3: case 7: case 10: case 14: case 18:
			return em_Skill_Index_E;
		case 6: case 11: case 16:
			return em_Skill_Index_R;
		default:
			break;
		}
		return em_Skill_Index_Q;
	};

	auto fn_StudySkill_W = [](DWORD dwLevel)
	{
		switch (dwLevel)
		{
		case 2: case 5: case 9: case 13: case 17:
			return em_Skill_Index_Q;
		case 1: case 4: case 8: case 12: case 15:
			return em_Skill_Index_W;
		case 3: case 7: case 10: case 14: case 18:
			return em_Skill_Index_E;
		case 6: case 11: case 16:
			return em_Skill_Index_R;
		default:
			break;
		}
		return em_Skill_Index_Q;
	};

	auto fn_StudySkill_E = [](DWORD dwLevel)
	{
		switch (dwLevel)
		{
		case 3: case 4: case 5: case 7: case 9:
			return em_Skill_Index_Q;
		case 2: case 8: case 12: case 14: case 17:
			return em_Skill_Index_W;
		case 1: case 10: case 13: case 15: case 18:
			return em_Skill_Index_E;
		case 6: case 11: case 16:
			return em_Skill_Index_R;
		default:
			break;
		}
		return em_Skill_Index_Q;
	};

	Log(LOG_LEVEL_NORMAL, L"Study Skill! Level=%d", dwLevel);
	switch (emHeroPro)
	{
	case em_Hero_Pro_Ryze: case em_Hero_Pro_Ashe: case em_Hero_Pro_Garen:
		return fn_StudySkill_Q(dwLevel);
	default:
		break;
	}

	return em_Skill_Index_Q;
	/*CONST auto& Vec = GetHeroSpVec();
	auto pHeroSp = CLPublic::Vec_find_if(Vec, [&emHeroPro](CONST auto& itm) { return itm.emHeroPro == emHeroPro; });
	return pHeroSp == nullptr ? nullptr : &pHeroSp->SkillSpVec.at(dwLevel);*/
}

CONST vector<ResSkill>& CGameRes::GetResSkillList() CONST  throw()
{
	CONST static vector<ResSkill> vlst = {
		{ em_Hero_Pro_Ryze,L"Ryze",{ em_Skill_Type::em_Skill_Type_UnDirectional , 7 },{ em_Skill_Type::em_Skill_Type_Directional, 5 },{ em_Skill_Type::em_Skill_Type_Directional, 5 },{ em_Skill_Type::em_Skill_Type_Self_UnDirectional, 10 } },
		{ em_Hero_Pro_Ashe,L"Ashe",{ em_Skill_Type::em_Skill_Type_Buff_Self , 7 },{ em_Skill_Type::em_Skill_Type_UnDirectional, 7 },{ em_Skill_Type::em_Skill_Type_UnDirectional, 7 },{ em_Skill_Type::em_Skill_Type_UnDirectional, 7 } },
		{ em_Hero_Pro_Garen,L"Garen",{ em_Skill_Type::em_Skill_Type_Buff_Self , 7 },{ em_Skill_Type::em_Skill_Type_Buff_Self, 7 },{ em_Skill_Type::em_Skill_Type_Buff_Self, 2 },{ em_Skill_Type::em_Skill_Type_Directional, 3 } },
	};

	return vlst;
}

auto CGameRes::GetResEqumentTypeByHero(_In_ em_Hero_Pro emHeroPro) CONST throw() ->em_ResEqument_Type
{
	switch (emHeroPro)
	{
	case em_Hero_Pro::em_Hero_Pro_Ryze:
		return em_ResEqument_Type_Master_Defence;
	case em_Hero_Pro::em_Hero_Pro_Ashe:
		return em_ResEqument_Type_ADC_AttackDis;
	case em_Hero_Pro::em_Hero_Pro_Garen:
		return em_ResEqument_Type_Defence_Attack;
	default:
		break;
	}
	return em_ResEqument_Type_Defence_Defence;
}

