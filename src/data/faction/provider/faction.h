#pragma once
#include "data/faction/quest/civilwar.h"

class faction {
    using value_string_map = std::map<FactionValue, std::string>;

public:
    static faction* get_singleton() {
        static faction singleton;
        return std::addressof(singleton);
    }

    std::string get_rank(const FactionValue a_stat) {
        if (!faction_rank_list_.contains(a_stat)) {
            return "";
        }
        return faction_rank_list_.find(a_stat)->second;
    }

    void get_factions(RE::Actor* a_actor) {
        clear_list();
        const auto actor_sex = a_actor->GetActorBase()->GetSex();
        const auto rank_default = "Member";

        constexpr auto male = RE::SEXES::SEX::kMale;
        constexpr auto female = RE::SEXES::SEX::kFemale;

        a_actor->VisitFactions([&](const RE::TESFaction* a_faction, int8_t a_rank) {
            if (a_faction && a_rank > -1) {
                const auto name(a_faction->GetName());
                const auto form_id(a_faction->GetFormID());
                auto rank_data(a_faction->rankData);

                if (!const_faction_map_.contains(form_id)) {
                    logger::trace("name {}, formId {}, rank {} not handeled"sv,
                        name,
                        string_util::int_to_hex(form_id),
                        a_rank);
                } else {
                    logger::trace("name {}, formId {}, rank {}"sv, name, string_util::int_to_hex(form_id), a_rank);
                    std::string rank;

                    for (auto it = rank_data.begin(); it != rank_data.end(); ++it) {
                        const auto index = std::distance(rank_data.begin(), it);
                        const auto i = *it;
                        if (index == a_rank) {
                            if (actor_sex == female) {
                                rank = i->femaleRankTitle;
                            }
                            if (rank.empty() || actor_sex == male) {
                                rank = i->maleRankTitle;
                            }
                            logger::trace("Name {}, Rankname {}"sv, name, rank);
                        }
                    }

                    /*if rank is empty here then we need to fill it by ourselfs*/
                    switch (const_faction_map_.find(form_id)->second) {
                        case FactionValue::darkbrotherhood:
                            rank = get_dark_brotherhood_rank();
                            break;
                        case FactionValue::greybeard:
                            rank = get_graybeard_rank();
                            break;
                        case FactionValue::imperial_legion:
                            rank = civil_war::get_singleton()->get_imperial_legion_rank();
                            break;
                        case FactionValue::stormcloaks:
                            rank = civil_war::get_singleton()->get_stormcloak_rank();
                            break;
                        case FactionValue::volkihar_vampire_clan:
                            rank = get_volkihar_vampire_clan_rank();
                            break;
                        case FactionValue::dawnguard:
                            rank = get_dawnguard_rank();
                            break;
                        case FactionValue::house_telvanni:
                            rank = get_house_telvanni_rank();
                            break;
                        default:
                            if (rank.empty()) {
                                rank = rank_default;
                            }
                            break;
                    }

                    faction_rank_list_.insert(std::pair(const_faction_map_.find(form_id)->second, rank));
                }
            }
            return false;
        });

        //MS05
        if (quest_util::is_quest_stage_complete(0x00053511, quest_util::get_as(300))) {
            //"Bard"
            faction_rank_list_.insert(std::pair(FactionValue::bard, *settings::bardRank));
        }

        logger::trace("got {} items in faction list."sv, faction_rank_list_.size());
        log_map();
    }

    void clear_list() { faction_rank_list_.clear(); }

    faction(const faction&) = delete;
    faction(faction&&) = delete;

    faction& operator=(const faction&) = delete;
    faction& operator=(faction&&) = delete;

private:
    faction() = default;

    ~faction() = default;

    value_string_map faction_rank_list_;

    //0x00072834 //blades, player might not be in there
    inline static std::map<RE::FormID, FactionValue> const_faction_map_ = { { 0x00048362, FactionValue::companions },
        { 0x0001BDB3, FactionValue::darkbrotherhood },
        { 0x0001F259, FactionValue::college_of_winterhold },
        { 0x00024029, FactionValue::orc_friend },
        { 0x00029DA9, FactionValue::thiefs_guild },
        { 0x0002BF9A, FactionValue::imperial_legion },
        { 0x0002BF9B, FactionValue::stormcloaks },
        { 0x0002C6C8, FactionValue::greybeard },
        { 0x02003376, FactionValue::volkihar_vampire_clan },
        //{ 0x02014217, FactionValue::dawnguard },
        { 0x02003375, FactionValue::dawnguard },
        { 0x04019B8A, FactionValue::house_telvanni } };

    void log_map() const {
        for (const auto& [fst, snd] : faction_rank_list_) { logger::trace("faction {}, rank {}"sv, fst, snd); }
    }

    [[nodiscard]] std::string get_dark_brotherhood_rank() const {
        auto& rank = *settings::assassinRank;
        //hail sithis
        if (quest_util::is_one_quest_stage_complete(0x0001EA59,
                std::vector{ quest_util::get_as(200), quest_util::get_as(255) })) {
            rank = *settings::listenerRank;
        }
        return rank;
    }

    static std::string get_graybeard_rank() { return *settings::ysmirRank; }

    static std::string get_volkihar_vampire_clan_rank() { return *settings::vampireLordRank; }

    static std::string get_dawnguard_rank() { return *settings::vampireHunterRank; }

    static std::string get_house_telvanni_rank() { return *settings::honoraryMemberRank; }
};
