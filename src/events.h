#pragma once

namespace Events {
    class KeyManager : public RE::BSTEventSink<RE::InputEvent*> {
    public:
        using EventResult = RE::BSEventNotifyControl;

        static KeyManager* GetSingleton();

        auto ProcessEvent(RE::InputEvent* const* a_event, RE::BSTEventSource<RE::InputEvent*>* a_eventSource)
            -> EventResult;

        void SetKey(uint64_t a_key);

        static void Sink();

        uint32_t GetGamepadIndex(RE::BSWin32GamepadDevice::Key a_key);

        enum : uint32_t {
            kInvalid = static_cast<uint32_t>(-1),
            kKeyboardOffset = 0,
            kMouseOffset = 256,
            kGamepadOffset = 266
        };
        KeyManager() = default;
        KeyManager(const KeyManager&) = delete;
        KeyManager(KeyManager&&) = delete;
        virtual ~KeyManager() = default;

        KeyManager& operator=(const KeyManager&) = delete;
        KeyManager& operator=(KeyManager&&) = delete;

        uint64_t _key = kInvalid;
    };

    class MenuHandler : public RE::BSTEventSink<RE::MenuOpenCloseEvent> {
    public:
        using EventResult = RE::BSEventNotifyControl;

        static MenuHandler* GetSingleton();

        static void Sink();

        auto ProcessEvent(RE::MenuOpenCloseEvent const* a_event,
            [[maybe_unused]] RE::BSTEventSource<RE::MenuOpenCloseEvent>* a_eventSource) -> EventResult;

        MenuHandler() = default;
        MenuHandler(const MenuHandler&) = delete;
        MenuHandler(MenuHandler&&) = delete;
        virtual ~MenuHandler() = default;

        MenuHandler& operator=(const MenuHandler&) = delete;
        MenuHandler& operator=(MenuHandler&&) = delete;
    };

    void SinkEventHandlers();
}