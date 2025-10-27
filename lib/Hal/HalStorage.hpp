#ifndef HAL_STORAGE_HPP
#define HAL_STORAGE_HPP

#include <Arduino.h>

/**
 * @brief EEPROM互換のストレージ操作を抽象化するインターフェイス
 */
class HalStorage {
public:
    virtual ~HalStorage() {}

    /**
     * @brief ストレージの初期化
     * @param size EEPROMとして確保するサイズ（バイト単位）
     */
    virtual void begin(size_t size) = 0;

    /**
     * @brief ストレージを終了・開放（必要なら）
     */
    virtual void end() = 0;

    /**
     * @brief 書き込みを反映（コミット）
     * @return 成功したら true
     */
    virtual bool commit() = 0;

    /**
     * @brief 任意アドレスからデータを読み出す
     * @param address 開始アドレス
     * @param data 読み出し先ポインタ
     * @param size 読み出すバイト数
     */
    virtual void get(int address, void* data, size_t size) = 0;

    /**
     * @brief 任意アドレスにデータを書き込む
     * @param address 開始アドレス
     * @param data 書き込み元ポインタ
     * @param size 書き込むバイト数
     */
    virtual void put(int address, const void* data, size_t size) = 0;

    /**
     * @brief ストレージ全体を初期化（全バイトを0xFFなどにする）
     */
    virtual void clear() = 0;
};

#endif // HAL_STORAGE_HPP
