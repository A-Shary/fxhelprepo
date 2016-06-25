#include <assert.h>
#include "TcpProxyClient.h"
#include "Network/TcpClient.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"


TcpProxyClient::TcpProxyClient()
    :tcpClient_(nullptr)
{
    tcpClient_.reset(new TcpClient);
    tcpClient_->Initialize();
}

TcpProxyClient::~TcpProxyClient()
{
    tcpClient_->Finalize();
}

void TcpProxyClient::SetProxy(const std::string& ip, uint16 port)
{
    proxyIp_ = ip;
    proxyPort_ = port;
}

bool TcpProxyClient::ConnectToSocks4Proxy(const std::string& ip, uint16 port)
{
    assert(!proxyIp_.empty() && proxyPort_);
    if (!tcpClient_->Connect(proxyIp_, proxyPort_))
    {
        return false;
    }

    uint32 u32Ip = inet_addr(ip.c_str());
    char ip0 = u32Ip >> 24;
    char ip1 = u32Ip << 8 >> 24;
    char ip2 = u32Ip << 16 >> 24;
    char ip3 = u32Ip & 0xFF;
    char port0 = port >> 8;
    char port1 = port & 0xFF;
    // socks4Э��
        //+---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + .... + ---- +
        //| VN | CD | DSTPORT | DSTIP | USERID | NULL |
        //+---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- + .... + ---- +
//#of bytes :1    1      2        4      variable  1
    std::vector<char> socks4_version_send_pack;
    //VN is the SOCKS protocol version number and should be 4. CD is the
    //    SOCKS command code and should be 1 for CONNECT request.NULL is a byte
    //    of all zero bits.
    socks4_version_send_pack.push_back(0x04);
    socks4_version_send_pack.push_back(0x01);
    socks4_version_send_pack.push_back(port0);
    socks4_version_send_pack.push_back(port1);
    socks4_version_send_pack.push_back(ip3);
    socks4_version_send_pack.push_back(ip2);
    socks4_version_send_pack.push_back(ip1);
    socks4_version_send_pack.push_back(ip0);
    socks4_version_send_pack.push_back(0x00);

    if (!tcpClient_->Send(socks4_version_send_pack))
        return false;

    std::vector<char> socks4_version_recv_pack;
    if (!tcpClient_->Recv(&socks4_version_recv_pack))
        return false;

    //The SOCKS server checks to see whether such a request should be granted
    //based on any combination of source IP address, destination IP address,
    //destination port number, the userid, and information it may obtain by
    //consulting IDENT, cf. RFC 1413.  If the request is granted, the SOCKS
    //server makes a connection to the specified port of the destination host.
    //A reply packet is sent to the client when this connection is established,
    //or when the request is rejected or the operation fails. 
 //           +---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- +
 //           | VN | CD | DSTPORT | DSTIP |
 //           +---- + ---- + ---- + ---- + ---- + ---- + ---- + ---- +
 //# of bytes : 1    1      2         4
    if (socks4_version_recv_pack.size() != 8)
        return false;

    //VN is the version of the reply code and should be 0. CD is the result
    //code with one of the following values :
    if (socks4_version_recv_pack[0] != 0x00)
        return false;

    //90 : request granted
    //91 : request rejected or failed
    //92 : request rejected becasue SOCKS server cannot connect to
    //identd on the client
    //93 : request rejected because the client program and identd
    //report different user - ids
    if (socks4_version_recv_pack[1] != 0x5a) // 0x5a == 90
        return false;
    //==========================================================================

    std::string str = "<policy-file-request/>";
    std::vector<char> data;
    data.assign(str.begin(), str.end());
    data.push_back(0);

    if (!tcpClient_->Send(data))
        return false;

    std::vector<char> flashresponse;
    if (!tcpClient_->Recv(&flashresponse))
        return false;

    return true;
}

bool TcpProxyClient::ConnectToSocks5Proxy(const std::string& ip, uint16 port)
{
    assert(!proxyIp_.empty() && proxyPort_);
    if (!tcpClient_->Connect(proxyIp_, proxyPort_))
    {
        return false;
    }

    // socks5Э��
    std::vector<char> socks5_version_send_pack;
    socks5_version_send_pack.push_back(0x05);
    socks5_version_send_pack.push_back(0x03);// ����������֤��ʽ
    socks5_version_send_pack.push_back(0x00);// 0x00 ����Ҫ��֤
    socks5_version_send_pack.push_back(0x01);// 0x01 GSSAPI
    socks5_version_send_pack.push_back(0x02);// 0x02 �û�����������֤
    if (!tcpClient_->Send(socks5_version_send_pack))
        return false;
    
    std::vector<char> socks5_version_recv_pack;
    if (!tcpClient_->Recv(&socks5_version_recv_pack))
        return false;

    if (socks5_version_recv_pack.size() != 2)
        return false;

    if (socks5_version_recv_pack[0] != 0x05)
        return false;

    uint8 method = socks5_version_recv_pack[1];
    if (method!=0)
    {
        assert(false && L"��֤ʧ��");
        return false;
    }

    uint32 u32Ip = inet_addr(ip.c_str());
    
    char ip0 = u32Ip >> 24;
    char ip1 = u32Ip << 8 >> 24;
    char ip2 = u32Ip << 16 >> 24;
    char ip3 = u32Ip & 0xFF;
    char port0 = port >> 8;
    char port1 = port & 0xFF;
    u32Ip = htonl(u32Ip);
    port = htons(port);
    std::vector<char> socks5_request;
    socks5_request.push_back(0x05);
    socks5_request.push_back(0x01);// Connect����
    socks5_request.push_back(0x00);// RSV
    socks5_request.push_back(0x01);// ipv4 type
    socks5_request.push_back(ip0); // ip
    socks5_request.push_back(ip1); // ip
    socks5_request.push_back(ip2); // ip
    socks5_request.push_back(ip3); // ip
    socks5_request.push_back(port0);
    socks5_request.push_back(port1);
    if (!tcpClient_->Send(socks5_request))
        return false;

    std::vector<char> socks5_response;
    if (!tcpClient_->Recv(&socks5_response))
        return false;

    if (socks5_response.size() != 10)
        return false;

    if (socks5_response[0] != 0x05)
        return false;

    //0x00��ʾ�ɹ�
    //0x01��ͨSOCKS����������ʧ��
    //0x02���й�����������
    //0x03���粻�ɴ�
    //0x04�������ɴ�
    //0x05���ӱ���
    //0x06 TTL��ʱ
    //0x07��֧�ֵ�����
    //0x08��֧�ֵĵ�ַ����
    //0x09 - 0xFFδ����
    if (socks5_response[1] != 0x00)
        return false;

    if (socks5_response[2] != 0x00)
        return false;

    uint8 addtype = socks5_response[3];
    ip0 = socks5_response[4];
    ip1 = socks5_response[5];
    ip2 = socks5_response[6];
    ip3 = socks5_response[7];
    port0 = socks5_response[8];
    port1 = socks5_response[9];

    std::string str = "<policy-file-request/>";
    std::vector<char> data;
    data.assign(str.begin(), str.end());
    data.push_back(0);

    if (!tcpClient_->Send(data))
        return false;

    std::vector<char> flashresponse;
    if (!tcpClient_->Recv(&flashresponse))
        return false;

    return true;
}

bool TcpProxyClient::Send(const std::vector<uint8>& data)
{
    return false;
}

bool TcpProxyClient::Recv(std::vector<uint8>* buffer)
{
    return false;
}