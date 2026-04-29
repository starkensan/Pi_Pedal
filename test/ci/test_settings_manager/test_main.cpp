#include <unity.h>
#include <SettingsManager/SettingsManager.hpp>
#include <StorageMock.hpp>

static void test_begin_loads_defaults_when_storage_is_empty() {
    StorageMock storage;
    SettingsManager manager(storage);

    TEST_ASSERT_TRUE(manager.begin());
    TEST_ASSERT_TRUE(storage.begin_called);
    TEST_ASSERT_EQUAL(sizeof(Settings), storage.begin_size);

    const Settings& settings = manager.getAllSettings();
    TEST_ASSERT_EQUAL_UINT8(1, settings.version);
    TEST_ASSERT_FALSE(manager.getIsDirty());

    for (size_t i = 0; i < MAX_PEDALS; ++i) {
        const PedalSettings& pedal = manager.getPedalSettings(i);
        TEST_ASSERT_EQUAL(static_cast<int>(PedalMode::CC), static_cast<int>(pedal.pedalMode));
        TEST_ASSERT_EQUAL_UINT8(1, pedal.midiChannel);
        TEST_ASSERT_EQUAL_UINT8(i + 1, pedal.ccNumber);
        TEST_ASSERT_EQUAL(static_cast<int>(SwitchBehavior::MOMENTARY), static_cast<int>(pedal.switchBehavior));
    }

    TEST_ASSERT_GREATER_OR_EQUAL(1, storage.commit_called_count);
}

static void test_setters_mark_dirty_and_commit_clears_dirty() {
    StorageMock storage;
    SettingsManager manager(storage);
    TEST_ASSERT_TRUE(manager.begin());

    manager.setMidiChannel(0, 3);
    manager.setCCNumber(0, 74);
    manager.setSwitchBehavior(0, SwitchBehavior::TOGGLE);
    manager.setPedalMode(0, PedalMode::PC_NEXT);

    TEST_ASSERT_TRUE(manager.getIsDirty());
    TEST_ASSERT_EQUAL_UINT8(3, manager.getPedalSettings(0).midiChannel);
    TEST_ASSERT_EQUAL_UINT8(74, manager.getPedalSettings(0).ccNumber);
    TEST_ASSERT_EQUAL(static_cast<int>(SwitchBehavior::TOGGLE), static_cast<int>(manager.getPedalSettings(0).switchBehavior));
    TEST_ASSERT_EQUAL(static_cast<int>(PedalMode::PC_NEXT), static_cast<int>(manager.getPedalSettings(0).pedalMode));

    TEST_ASSERT_TRUE(manager.commitSettings());
    TEST_ASSERT_FALSE(manager.getIsDirty());
}

static void test_uncommit_restores_persisted_settings() {
    StorageMock storage;
    SettingsManager manager(storage);
    TEST_ASSERT_TRUE(manager.begin());

    manager.setMidiChannel(0, 7);
    TEST_ASSERT_TRUE(manager.commitSettings());
    TEST_ASSERT_EQUAL_UINT8(7, manager.getPedalSettings(0).midiChannel);

    manager.setMidiChannel(0, 11);
    TEST_ASSERT_TRUE(manager.getIsDirty());
    TEST_ASSERT_EQUAL_UINT8(11, manager.getPedalSettings(0).midiChannel);

    manager.uncommitSettings();
    TEST_ASSERT_FALSE(manager.getIsDirty());
    TEST_ASSERT_EQUAL_UINT8(7, manager.getPedalSettings(0).midiChannel);
}

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    UNITY_BEGIN();
    RUN_TEST(test_begin_loads_defaults_when_storage_is_empty);
    RUN_TEST(test_setters_mark_dirty_and_commit_clears_dirty);
    RUN_TEST(test_uncommit_restores_persisted_settings);
    return UNITY_END();
}
