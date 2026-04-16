#include "MenuManager.hpp"
void MenuManager::init(MenuID initMenuID) {
    menuDisplay_.begin();
    currentMenu = *getMenuConfig(initMenuID);
    selected = false;
    param = 0;
    index = 0;
}

MenuConfig MenuManager::getCurrentMenu() const {
    return currentMenu;
}

void MenuManager::showPCNumber(int number) {
    PCNumber = number;
    if(currentMenu.menuID == MenuID::MAIN) {
        menuDisplay_.renderNumber(number);
    }
}

void MenuManager::enterSelectedItem() {
    if (index >= currentMenu.itemCount) {
        return; // 無効なインデックス
    }

    if(currentMenu.menuID == MenuID::MAIN) {
        currentMenu = *getMenuConfig(MenuID::SETTINGS);
        index = 0;
        menuDisplay_.render(index, selected, currentMenu);
        return;
    }
    MenuItem selectedItem = currentMenu.items[index];
    switch (selectedItem.type) {
        case MenuType::SUBMENU: {
            // サブメニューに移動
            param = selectedItem.param;
            currentMenu = *getMenuConfig(selectedItem.actionParam.submenuID);
            MemToParam();
            index = 0;
            menuDisplay_.render(index, selected, currentMenu);
            break;
        }
        case MenuType::FUNCTION: {
            // 関数を呼び出す
            if(currentMenu.menuID == MenuID::SETTINGS && index) {
                settingsManager_.FactoryReset();
            }
        }
        case MenuType::APPLY: {
            // 適用処理（必要に応じて実装）
            applySettings();
            break;
        }
        case MenuType::SAVE: {
            settingsManager_.commitSettings();
            // 保存処理（必要に応じて実装）
            break;
        }
        case MenuType::EXIT: {
            // 前のメニューに戻る
            currentMenu = *getMenuConfig(currentMenu.prevMenuID);
            index = 0;
            selected = false;
            if(currentMenu.menuID == MenuID::MAIN) {
                menuDisplay_.clear();
                menuDisplay_.renderNumber(PCNumber);
            }else{
                menuDisplay_.render(index, selected, currentMenu);
            }
            break;
        }
        case MenuType::VALUE_CHANGE: {
            // 値変更モードに入る/出る
            selected = !selected;
            menuDisplay_.render(index, selected, currentMenu);
            break;
        }
        case MenuType::NONE:
        default:
            break;
    }
}

void MenuManager::cusorUp(int value) {
    if (selected) {
        // 値変更モード
        MenuItem selectedItem = currentMenu.items[index];
        if (selectedItem.type == MenuType::VALUE_CHANGE) {
            ParamConfig* paramConfig = (ParamConfig*)(getParamConfig(selectedItem.actionParam.paramID));
            if (paramConfig) {
                int newValue = getParamValue(selectedItem.actionParam.paramID) - 1;
                if (newValue < paramConfig->minValue) {
                    newValue = paramConfig->minValue;
                }
                setParamValue(paramConfig->paramID, newValue);
                menuDisplay_.render(index, selected, currentMenu);
                
            }
        }
    } else {
        // メニュー移動モード
        index --;
        if (index < 0) {
            index = 0;
        }
        menuDisplay_.render(index, selected, currentMenu);
    }
}

void MenuManager::cusorDown(int value) {
    if (selected) {
        // 値変更モード
        MenuItem selectedItem = currentMenu.items[index];
        if (selectedItem.type == MenuType::VALUE_CHANGE) {
            ParamConfig* paramConfig = (ParamConfig*)(getParamConfig(selectedItem.actionParam.paramID));
            if (paramConfig) {
                int newValue = getParamValue(selectedItem.actionParam.paramID) + 1;
                if (newValue > paramConfig->maxValue) {
                    newValue = paramConfig->maxValue;
                }
                setParamValue(paramConfig->paramID, newValue);
                menuDisplay_.render(index, selected, currentMenu);
            }
        }
    } else {
        // メニュー移動モード
        index ++;
        if (index >= currentMenu.itemCount) {
            index = currentMenu.itemCount - 1;
        }
        menuDisplay_.render(index, selected, currentMenu);
    }
}

void MenuManager::applySettings() {
    switch (currentMenu.menuID)
    {
    case MenuID::PEDAL_ASSIGNMENT:
        settingsManager_.setPedalMode(param-1, static_cast<PedalMode>(getParamValue(ParamID::PARAM_PEDAL_MIDI_MODE)));
        settingsManager_.setMidiChannel(param-1, getParamValue(ParamID::PARAM_PEDAL_MIDI_CHANNEL));
        settingsManager_.setCCNumber(param-1, getParamValue(ParamID::PARAM_PEDAL_CC_NUMBER));
        settingsManager_.setSwitchBehavior(param-1, static_cast<SwitchBehavior>(getParamValue(ParamID::PARAM_PEDAL_SWITCH_MODE)));
        break;
    
    case MenuID::EXP_PEDAL_ASSIGNMENT:
        settingsManager_.setMidiChannel(PEDAL_COUNT, getParamValue(ParamID::PARAM_EXP_PEDAL_MIDI_CHANNEL));
        settingsManager_.setCCNumber(PEDAL_COUNT, getParamValue(ParamID::PARAM_EXP_PEDAL_CC_NUMBER));
        break;
    
    default:
        break;
    }
}

void MenuManager::MemToParam() {
    switch (currentMenu.menuID)
    {
    case MenuID::PEDAL_ASSIGNMENT:
        setParamValue(ParamID::PARAM_PEDAL_MIDI_MODE, static_cast<int>(settingsManager_.getPedalSettings(param-1).pedalMode));
        setParamValue(ParamID::PARAM_PEDAL_MIDI_CHANNEL, settingsManager_.getPedalSettings(param-1).midiChannel);
        setParamValue(ParamID::PARAM_PEDAL_CC_NUMBER, settingsManager_.getPedalSettings(param-1).ccNumber);
        setParamValue(ParamID::PARAM_PEDAL_SWITCH_MODE, static_cast<int>(settingsManager_.getPedalSettings(param-1).switchBehavior));
        
        break;
    
    case MenuID::EXP_PEDAL_ASSIGNMENT:
        setParamValue(ParamID::PARAM_EXP_PEDAL_MIDI_CHANNEL, settingsManager_.getPedalSettings(PEDAL_COUNT).midiChannel);
        setParamValue(ParamID::PARAM_EXP_PEDAL_CC_NUMBER, settingsManager_.getPedalSettings(PEDAL_COUNT).ccNumber);
        break;
    
    default:
        break;
    }
}