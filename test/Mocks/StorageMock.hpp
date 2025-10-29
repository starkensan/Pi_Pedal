#pragma once
#include <vector>
#include <string.h>
#include <stdint.h>
#include <HalStorage.hpp>

/**
 * StorageMock:
 * - 呼び出し回数・引数を記録する
 * - commit() の戻り値をテスト中に差し替え可能
 * - get()/put() はオプションでバッファにも反映できる
 *
 * 「本当にEEPROMっぽい挙動をしたい」と
 * 「呼び出し検証したい」は目的がちょっと違うので、
 * このクラスは『検証』を優先。
 */
class StorageMock : public HalStorage {
public:

    struct PutCallLog {
        int address;
        size_t size;
        // 実際に書こうとしたデータのスナップショット
        std::vector<uint8_t> bytes;
    };

    struct GetCallLog {
        int address;
        size_t size;
    };

    StorageMock()
        : begin_called(false),
          end_called(false),
          clear_called(false),
          commit_called_count(0),
          commit_return_value(true) // デフォルト成功
    {}

    virtual ~StorageMock() {}

    // ---- テストで事前に設定できるstub的な部分 ----

    // commit()が返す値をテストケースごとに変更できる
    void setCommitReturnValue(bool v) {
        commit_return_value = v;
    }

    // 内部バッファの初期サイズと初期値を決めたい場合はここ
    // （begin()を明示的に呼ぶ前にセットもできる）
    void presetBuffer(size_t size, uint8_t fill = 0xFF) {
        buffer.assign(size, fill);
    }

    // ---- HalStorageの実装（spy＋簡易スタブ）----

    void begin(size_t size) override {
        begin_called = true;
        // begin時にバッファが未準備ならここで確保
        if (buffer.size() != size) {
            buffer.assign(size, 0xFF);
        }
        begin_size = size;
    }

    void end() override {
        end_called = true;
        // 実機ならクローズ処理とか
        // テストではそのまま
    }

    bool commit() override {
        commit_called_count++;
        return commit_return_value;
    }

    void get(int address, void* data, size_t size) override {
        // 呼ばれた事実と引数を記録
        GetCallLog log;
        log.address = address;
        log.size = size;
        get_calls.push_back(log);

        // バッファから読み出す動作も一応する
        if (address < 0 ||
            (size_t)address + size > buffer.size()) {
            // 範囲外なら読めるとこだけコピー
            size_t valid = 0;
            if (address >= 0 && (size_t)address < buffer.size()) {
                valid = buffer.size() - (size_t)address;
                if (valid > size) valid = size;
                ::memcpy(data, &buffer[(size_t)address], valid);
            }
            if (valid < size) {
                ::memset((uint8_t*)data + valid, 0x00, size - valid);
            }
        } else {
            ::memcpy(data, &buffer[(size_t)address], size);
        }
    }

    void put(int address, const void* data, size_t size) override {
        // ログに積む
        PutCallLog log;
        log.address = address;
        log.size = size;
        log.bytes.resize(size);
        ::memcpy(log.bytes.data(), data, size);
        put_calls.push_back(log);

        // バッファ側も更新しておく（実機っぽさ）
        if (address >= 0) {
            size_t endPos = (size_t)address + size;
            if (endPos > buffer.size()) {
                // はみ出すなら拡張しても良いし、assertしてもよい
                buffer.resize(endPos, 0xFF);
            }
            ::memcpy(&buffer[(size_t)address], data, size);
        }
    }

    void clear() override {
        clear_called = true;
        ::memset(buffer.data(), 0xFF, buffer.size());
    }

    // ---- テストで参照する情報（アサーション用）----

    bool         begin_called;
    size_t       begin_size = 0;
    bool         end_called;
    bool         clear_called;

    int          commit_called_count;
    bool         commit_return_value;

    std::vector<uint8_t> buffer;
    std::vector<PutCallLog> put_calls;
    std::vector<GetCallLog> get_calls;
};
