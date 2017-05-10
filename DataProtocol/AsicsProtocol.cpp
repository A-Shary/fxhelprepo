#include "AsicsProtocol.h"
#include "third_party/chromium/base/logging.h"

namespace
{
    static uint32 header_length = sizeof(Header);
}

AsicsProtocol::AsicsProtocol()
{
}


AsicsProtocol::~AsicsProtocol()
{
}

void AsicsProtocol::SetReceiveCallback()
{

}

// ��������յ�tcp�����ӹ����������ճ�������⣬��Ϻ����ݰ���ص�
bool AsicsProtocol::Receive(const std::vector<uint8>& data)
{
    if (data.empty())
        return false;

    buffer_.insert(buffer_.end(), data.begin(), data.end());


    if (header_.header_length == 0) // δ��Э��ͷ
    {
        if (buffer_.size() < header_length) // δ��Э��ͷ����
            return true;

        if (!ReadHeader())
            return false;
    }

    // �Ѷ�Э��ͷ
    // δ�����굱ǰ������
    if (buffer_.size() < header_.total_length)
        return true;
    
    if (buffer_.size() == header_.total_length)
        return ReadBody();

    // ������ݳ���Ԥ�Ƴ��ȣ�����Ϊճ�����������ģ�һ�ζ��֪꣬ͨ���
    return ReadBody();
}

bool AsicsProtocol::ReadHeader()
{

    DCHECK(header_.total_length > header_.header_length);
    return false;
}
bool AsicsProtocol::ReadBody()
{
    return false;
}

void AsicsProtocol::Finish()
{
    return;
}

void AsicsProtocol::Error()
{

}

bool AsicsProtocol::Decrypt(uint8* buffer, uint32 length)
{
    return false;
}
