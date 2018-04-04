//
// Created by toxicoverflow on 23.02.17.
//

#include "skinchanger.h"

std::unordered_map<int, cSkin> cSkinchanger::SkinList = std::unordered_map<int, cSkin>( {
	/* https://github.com/sonicrules11/CSGO-skin-ID-dumper/blob/master/item_index.txt */
	/* NOTE: Change knife model in skinchanger.h */
	// std::make_pair(WEAPON, cSkin(SKIN, WEAR, -1, Stattrak, (char*)("Name") or nullptr for no name, FLOAT)),
	// Knife
	std::make_pair(WEAPON_KNIFE, cSkin(38, -1, WEAPON_KNIFE_M9_BAYONET, 1, (char*)("twister kneiv"), 0.0001f)),
	// Pistols
	std::make_pair(WEAPON_CZ75A, cSkin(622, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_DEAGLE, cSkin(37, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_ELITE, cSkin(450, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_FIVESEVEN, cSkin(660, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_GLOCK, cSkin(586, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_HKP2000, cSkin(591, -1, -1, -1, nullptr, 0.0001f)), // P2000
	std::make_pair(WEAPON_P250, cSkin(678, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_REVOLVER, cSkin(683, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_TEC9, cSkin(179, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_USP_SILENCER, cSkin(183, -1, -1, -1, nullptr, 0.0001f)),
	// Rifles
	std::make_pair(WEAPON_AK47, cSkin(675, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_AUG, cSkin(455, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_AWP, cSkin(446, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_FAMAS, cSkin(626, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_G3SG1, cSkin(511, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_M4A1_SILENCER, cSkin(644, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_M4A1, cSkin(309, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_SCAR20, cSkin(597, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_SG556, cSkin(136, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_SSG08, cSkin(222, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_GALILAR, cSkin(398, -1, -1, -1, nullptr, 0.0001f)),
	// SMGs
	std::make_pair(WEAPON_MAC10, cSkin(38, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_MP7, cSkin(696, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_MP9, cSkin(482, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_P90, cSkin(156, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_UMP45, cSkin(688, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_BIZON, cSkin(676, -1, -1, -1, nullptr, 0.0001f)),
	// Heavy
	std::make_pair(WEAPON_MAG7, cSkin(703, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_NOVA, cSkin(537, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_SAWEDOFF, cSkin(256, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_XM1014, cSkin(654, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_M249, cSkin(496, -1, -1, -1, nullptr, 0.0001f)),
	std::make_pair(WEAPON_NEGEV, cSkin(698, -1, -1, -1, nullptr, 0.0001f)),	
});

std::unordered_map<int, const char*> cSkinchanger::ModelList;

cSkinchanger* skinchanger = new cSkinchanger;

void cSkinchanger::FrameStageNotify(ClientFrameStage_t stage) {
    if(stage == FRAME_NET_UPDATE_POSTDATAUPDATE_START){
        pLocalPlayer = (C_BaseEntity*)(pEntList->GetClientEntity(pEngine->GetLocalPlayer()));

        if(pLocalPlayer && pLocalPlayer->GetHealth() > 0){
            if(!bInit){
                Init();
                bInit = true;
            }

            ForceSkins();
        }
    }
}

void cSkinchanger::FindModels() {
    ModelList[pModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl")] = KnifeToModelMatrix[Knife].c_str();
    ModelList[pModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl")] = KnifeToModelMatrix[Knife].c_str();
}

void cSkinchanger::ForceSkins() {
    IEngineClient::player_info_t player_info;

    if(pEngine->GetPlayerInfo(pLocalPlayer->GetId(), &player_info)){

        int* pWeapons = pLocalPlayer->GetWeapons();

        C_BaseViewModel* LocalPlayerViewModel = (C_BaseViewModel*)pEntList->GetClientEntityFromHandle(pLocalPlayer->GetViewModel());

            C_BaseAttributableItem* WeaponViewModel = nullptr;

        if(LocalPlayerViewModel)
            WeaponViewModel = (C_BaseAttributableItem*)pEntList->GetClientEntityFromHandle(LocalPlayerViewModel->GetWeapon());

        C_BaseCombatWeapon* localWeapon = (C_BaseCombatWeapon*)pEntList->GetClientEntityFromHandle(pLocalPlayer->GetActiveWeapon());

        if(pWeapons){
            for(int i = 0; pWeapons[i]; i++){
                C_BaseAttributableItem* attributableItem = (C_BaseAttributableItem*)pEntList->GetClientEntityFromHandle(pWeapons[i]);

                if(attributableItem){
                    int* Definition = attributableItem->GetItemDefinitionIndex();

                    std::unordered_map<int, cSkin>::iterator SkinIter = (*Definition == WEAPON_KNIFE_T ? SkinList.find(WEAPON_KNIFE) : SkinList.find(*Definition));

                    if(SkinIter != SkinList.end()) {
                        if(*attributableItem->GetOriginalOwnerXuidLow() == player_info.xuidlow && *attributableItem->GetOriginalOwnerXuidHigh() == player_info.xuidhigh){

                            int* model_index = attributableItem->GetModelIndex();

                            std::unordered_map<int, const char*>::iterator model_iter = ModelList.find(*model_index);

                            if(model_iter != ModelList.end()){
                                *model_index = pModelInfo->GetModelIndex(model_iter->second);
                            }

                            cSkin skin = std::move(SkinIter->second);

                            if(Knife && (*Definition == WEAPON_KNIFE || *Definition == WEAPON_KNIFE_T))
                                *attributableItem->GetItemDefinitionIndex() = Knife;


                            if(skin.name){
                                sprintf(attributableItem->GetCustomName(), skin.name);
                            }

                            *attributableItem->GetItemIDHigh() = -1;
                            *attributableItem->GetFallbackPaintKit() = skin.Paintkit;
                            *attributableItem->GetFallbackStatTrak() = skin.StatTrack;
                            *attributableItem->GetEntityQuality() = 4;
                            *attributableItem->GetFallbackSeed() = skin.Seed;
                            *attributableItem->GetFallbackWear() = skin.Wear;
                            *attributableItem->GetAccountID() = player_info.xuidlow;
                        }
                    }

                    if (WeaponViewModel && WeaponViewModel == attributableItem)
                    {
                        int* model_index = ((C_BaseEntity*)LocalPlayerViewModel)->GetModelIndex();

                        std::unordered_map<int, const char*>::iterator model_iter = ModelList.find(*model_index);

                        if (model_iter != ModelList.end())
                        {
                            *model_index = pModelInfo->GetModelIndex(model_iter->second);
                        }
                    }

                }
            }

            if(LocalPlayerViewModel && localWeapon){

                int* model_index = ((C_BaseEntity*)LocalPlayerViewModel)->GetModelIndex();

                std::unordered_map<int, const char*>::iterator model_iter = ModelList.find(*((C_BaseEntity*)localWeapon)->GetModelIndex());

                if(model_iter != ModelList.end()){
                    *model_index = pModelInfo->GetModelIndex(model_iter->second);
                }
            }
        }
    }
}

void cSkinchanger::Init() {
    ModelList.clear();
    FindModels();
}

void cSkinchanger::FireEventClientSide(IGameEvent *event) {

    if (!strcmp(event->GetName(), "game_newmap"))
    {
        Init();
    }
}
