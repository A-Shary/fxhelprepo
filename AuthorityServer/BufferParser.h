#pragma once

#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/atomic_sequence_num.h"
#include <vector>
#include <list>
#include "MessageDefine.h"

class BufferParser
{
public:
    BufferParser();
    ~BufferParser();

    // ֻҪ��ʽ������������true
    bool HandleBuffer(const std::vector<uint8>& data);
    bool GetResponses(std::vector<std::vector<uint8>>* responses);

private:
    enum class ParseStatus
    {
        INIT = 0,
        HEADER = 1,
        DATA = 2
    };
    bool Parse(const std::vector<uint8>& data, CommandId* command_id,
        const std::string* json_data);

    bool Make(CommandId command_id, const std::string& json_data,
        std::vector<uint8>* data);

    uint32 GetSquenceNumber();

    // δ�չ����ݸ�ʽ�Ļ�������
    std::vector<uint8> buffer_;

    ParseStatus status_;
    Header header_;

    // �������Ժ�����ж����ظ�����, �Ѿ�������ͷ��װ����
    std::vector<std::vector<uint8>> responses_;

    base::AtomicSequenceNumber sequence_number_;
};
