#pragma once

#include "third_party/chromium/base/basictypes.h"
#include <string>
#include <vector>
#include <list>

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
};

template<typename ValueType>
ValueType ReadValue(uint8** data, uint32* left)
{
    ValueType value;
    value = *(decltype(value)*)(*data);
    *data += sizeof(decltype(value));
    *left -= sizeof(decltype(value));
    return value;
};

#pragma pack(push, 1)
struct Header
{
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
};
#pragma pack(pop)

class Package
{
public:
    void SetHeader(const Header& header)
    {
        header_ = header;
    }
    void SetData(const std::string& data)
    {
        data_ = data;
    }

private:
    Header header_;
    std::string data_;
};

interface CommandParserInterface
{
public:
    virtual bool Parse(const std::string& json_data) = 0;

    virtual bool HandleBussiness() = 0;
};

class BufferParser
{
public:
    BufferParser(){};
    ~BufferParser(){};

    // ֻҪ��ʽ������������true
    bool HandleBuffer(const std::vector<uint8>& data)
    {
        switch (status_)
        {
        case BufferParser::ParseStatus::INIT:
            buffer_.insert(buffer_.end(), data.begin(), data.end());
            if (buffer_.size() < sizeof(Header))
                return true;

            status_ = BufferParser::ParseStatus::HEADER;
            // ������switch,�ǹ����

        case BufferParser::ParseStatus::HEADER:
        {
            uint32 left = buffer_.size();
            uint8* buffer_begin = &buffer_[0];
            if (!header_.Read(&buffer_begin, &left))
                return false;

            status_ = BufferParser::ParseStatus::DATA;
            break;
        }

        case BufferParser::ParseStatus::DATA:
        {
            if (buffer_.size() <= header_.total_length - header_.header_length)
                break;

            Package package;
            package.SetHeader(header_);
            std::string data;
            data.assign(buffer_.begin() + header_.header_length,
                buffer_.begin() + header_.total_length);

            package.SetData(data);
            package_list_.push_back(package);

            buffer_.erase(buffer_.begin(), buffer_.begin() + header_.total_length);
            break;
        }

        default:
            //DCHECK(false);
            return false;
            break;
        }
        // ��ȡheader���չ�headerָ�����ݣ�����Parse����json����

        return true;
    };

    bool GetResponses(std::vector<std::vector<uint8>>* responses)
    {
        return false;
    };

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

    // δ�չ����ݸ�ʽ�Ļ�������
    std::vector<uint8> buffer_;

    ParseStatus status_;
    Header header_;
    
    std::list<Package> package_list_;
    // �������Ժ�����ж����ظ�����, �Ѿ�������ͷ��װ����
    std::vector<std::vector<uint8>> responses_;
};

class EndPointInfoComputer
{
public:
    EndPointInfoComputer();
    ~EndPointInfoComputer();

    void SetClientVersion(const std::string& client_version);
    std::string GetClientVersion() const;

    void SetClientHash(const std::string& client_hash);
    std::string GetClientHash() const;

    void SetOsVersion(const std::string& os_version);
    std::string GetOsVersion() const;

    // ���ͷ����, ��������ţ�������Э��ͷ
    bool Make(std::string* json_data)
    {
        return true;
    }

    // ���շ�����
    bool Parse(const std::string& json_data)
    {
        return true;
    }

private:
    std::string client_version_;
    std::string client_hash_;
    std::string os_version_;
};

