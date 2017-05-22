#pragma once

#include "third_party/chromium/base/basictypes.h"

enum class CommandId
{
    CMD_UNDEFINE = 0,

    CMD_ENDPOINT_INFO_COMPUTER = 1001,
    CMD_ENDPOINT_INFO_EXIT = 1002,
    CMD_ENDPOINT_INFO_DUMP = 1003,

    CMD_LOGIN_SUCCESS = 2001,

    CMD_AUTHORITY_REQUEST = 3001,

    CMD_OPERATION_LOG = 4001,
    CMD_HTTP_LOG = 4002,

    CMD_END
};


#pragma pack(push, 1)
class Header
{
public:
    uint16 protocol_version;
    uint16 header_length; // ��λ��32bit����
    uint32 total_length;  // ��λ���ֽ�
    uint32 sequence_number; // ����session��ʹ�õ����к�
    uint32 ack_number; // �ظ���ʹ�õ����к�
    uint32 timestamp; // ����ʱ���
    uint32 header_crc32; // ��ʱ�����

    bool Read(uint8** data, uint32* left)
    {
        if (*left < sizeof(Header))
            return false;

        protocol_version = ReadValue<uint16>(data, left);
        header_length = ReadValue<uint16>(data, left);
        total_length = ReadValue<uint32>(data, left);
        sequence_number = ReadValue<uint32>(data, left);
        ack_number = ReadValue<uint32>(data, left);
        timestamp = ReadValue<uint32>(data, left);
        header_crc32 = ReadValue<uint32>(data, left);
        return true;
    }

    bool Write(uint8** data, uint32 len)
    {
        uint32 left = len;
        left = WriteValue(data, protocol_version, left);
        left = WriteValue(data, header_length, left);
        left = WriteValue(data, total_length, left);
        left = WriteValue(data, sequence_number, left);
        left = WriteValue(data, ack_number, left);
        left = WriteValue(data, timestamp, left);
        left = WriteValue(data, header_crc32, left);
        return left == 0;
    }

private:
    template<typename ValueType>
    ValueType ReadValue(uint8** data, uint32* left)
    {
        ValueType value;
        value = *(decltype(value)*)(*data);
        *data += sizeof(decltype(value));
        *left -= sizeof(decltype(value));
        return value;
    };

    template<typename ValueType>
    uint32 WriteValue(uint8** data, ValueType value, uint32 left)
    {
        *(decltype(value)*)(*data) = value;
        *data += sizeof(decltype(value));
        left -= sizeof(decltype(value));
        return left;
    };
};
#pragma pack(pop)