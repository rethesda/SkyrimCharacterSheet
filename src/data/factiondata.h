#pragma once
#include "data/faction/champion.h"
#include "data/faction/faction.h"
#include "data/faction/factionitem.h"
#include "data/faction/thane.h"
#include "settings/stats/factionsettings.h"


class FactionData {
    using FactionItemMap = std::map<FactionValue, std::unique_ptr<FactionItem>>;

public:
    static FactionData* GetSingleton() {
        static FactionData singleton;
        return std::addressof(singleton);
    }

    FactionItemMap getValuesToDisplay() {
        FactionItemMap fimp;

        auto player = RE::PlayerCharacter::GetSingleton();
        auto faction = Faction::GetSingleton();
        auto thane = Thane::GetSingleton();
        auto champion = Champion::GetSingleton();

        if (*Settings::showFactions) {
            faction->getFactions(player);
        }
        if (*Settings::showThanes) {
            thane->getRegionThanes();
        }
        if (*Settings::showChampion) {
            champion->getChampions();
        }

        auto factionSettings = FactionSetting::GetSingleton();
        auto factionSettingMap = factionSettings->load();
        logger::debug("Config Map Size is {}"sv, factionSettingMap.size());

        for (auto& element : factionSettingMap) {
            auto factionValue = element.first;
            auto factionConfig = element.second.get();

            factionConfig->logStatConfig(factionValue);

            if (!factionConfig->getShow()) {
                continue;
            }
            std::string valueText = "";

            switch (factionValue) {
                case FactionValue::companions:
                case FactionValue::darkbrotherHood:
                case FactionValue::collegeOfWinterhold:
                case FactionValue::orcFriend:
                case FactionValue::thiefsGuild:
                case FactionValue::imperialLegion:
                case FactionValue::stormcloaks:
                case FactionValue::greybeard:
                case FactionValue::bard:
                case FactionValue::volkiharVampireClan:
                case FactionValue::dawnguard:
                case FactionValue::houseTelvanni:
                    valueText = faction->getRank(factionValue);
                    break;
                case FactionValue::thaneOfEastmarch:
                case FactionValue::thaneOfFalkreath:
                case FactionValue::thaneOfHaafingar:
                case FactionValue::thaneOfHjaalmarch:
                case FactionValue::thaneOfThePale:
                case FactionValue::thaneOfTheReach:
                case FactionValue::thaneOfTheRift:
                case FactionValue::thaneOfWhiterun:
                case FactionValue::thaneOfWinterhold:
                    valueText = thane->getThane(factionValue);
                    break;
                case FactionValue::azura:
                case FactionValue::boethiah:
                case FactionValue::clavicusVile:
                case FactionValue::hermaeusMora:
                case FactionValue::hircine:
                case FactionValue::malacath:
                case FactionValue::mehrunesDagon:
                case FactionValue::mephala:
                case FactionValue::meridia:
                case FactionValue::molagBal:
                case FactionValue::namira:
                case FactionValue::nocturnal:
                case FactionValue::peryite:
                case FactionValue::sanguine:
                case FactionValue::sheogorath:
                case FactionValue::vaermina:
                    valueText = champion->getChampion(factionValue);
                    break;
                default:
                    logger::warn("unhandeled stat, name {}, displayName {}"sv,
                        factionValue,
                        factionConfig->getDisplayName());
                    break;
            }


            if (valueText != "") {
                fimp[factionValue] =
                    std::make_unique<FactionItem>(factionConfig->getDisplay(valueText), factionConfig->getMenu());
            }
        }

        logger::debug("Display Map Size is {}"sv, fimp.size());
        for (auto& element : factionSettingMap) { element.second.reset(); }
        factionSettingMap.clear();
        return fimp;
    }

private:
    FactionData() = default;
    FactionData(const FactionData&) = delete;
    FactionData(FactionData&&) = delete;

    ~FactionData() = default;

    FactionData& operator=(const FactionData&) = delete;
    FactionData& operator=(FactionData&&) = delete;
};