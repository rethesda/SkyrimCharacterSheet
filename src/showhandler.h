#pragma once
#include "scaleform/factionmenu.h"
#include "scaleform/statsmenu.h"

class ShowHandler {
    using ShowMenu = MenuUtil::ShowMenu;
public:
    static ShowHandler* GetSingleton() {
        static ShowHandler singleton;
        return std::addressof(singleton);
    }

    void CloseWindow(ShowMenu a_menu) {
        switch (a_menu) {
            case ShowMenu::mStats:
                Scaleform::StatsMenu::Close();
                break;
            case ShowMenu::mFaction:
                Scaleform::FactionMenu::Close();
                break;
        }
    }

    void SwapWindow(ShowMenu a_menu_open, ShowMenu a_menu_close) {
        CloseWindow(a_menu_close);
        ShowWindow(a_menu_open);
    }

    void CloseAllWindows() {
        CloseWindow(ShowMenu::mStats);
        CloseWindow(ShowMenu::mFaction);
    }

    void HandleMainButtonPress() {
        if (Scaleform::StatsMenu::IsMenuOpen()) {
            CloseWindow(ShowMenu::mStats);
        } else if (Scaleform::FactionMenu::IsMenuOpen()) {
            CloseWindow(ShowMenu::mFaction);
        } else {
            ShowWindow(ShowMenu::mStats);
        }
    }

    void HandleNextMenuButtonPress() {
        if (Scaleform::StatsMenu::IsMenuOpen()) {
            if (*Settings::showFactionMenu) {
                logger::debug("{} is open, open {}"sv,
                    Scaleform::StatsMenu::MENU_NAME,
                    Scaleform::FactionMenu::MENU_NAME);
                SwapWindow(ShowMenu::mFaction, ShowMenu::mStats);
            } else {
                logger::debug("{} is open, {} is disabled via config"sv,
                    Scaleform::StatsMenu::MENU_NAME,
                    Scaleform::FactionMenu::MENU_NAME);
            }
        } else if (Scaleform::FactionMenu::IsMenuOpen()) {
            logger::debug("{} is open, open {}"sv, Scaleform::FactionMenu::MENU_NAME, Scaleform::StatsMenu::MENU_NAME);
            SwapWindow(ShowMenu::mStats, ShowMenu::mFaction);
        }
    }

    bool IsMenuOpen() {
        if (Scaleform::StatsMenu::IsMenuOpen() || Scaleform::FactionMenu::IsMenuOpen()) {
            return true;
        }
        return false;
    }

private:
    void ShowWindow(ShowMenu a_menu) {
        logger::trace("Gather Value and Show Window"sv);

        switch (a_menu) {
            case ShowMenu::mStats:
                Scaleform::StatsMenu::Open();
                break;
            case ShowMenu::mFaction:
                if (*Settings::showFactionMenu) {
                    Scaleform::FactionMenu::Open();
                }
                break;
        }
    }

    ShowHandler() = default;
    ShowHandler(const ShowHandler&) = delete;
    ShowHandler(ShowHandler&&) = delete;
    ~ShowHandler() = default;

    ShowHandler& operator=(const ShowHandler&) = delete;
    ShowHandler& operator=(ShowHandler&&) = delete;
};
