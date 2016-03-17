#pragma once

#include <string>
#include <map>

class GiftInfo
{
    GiftInfo()
        :giftid(0)
        , giftname("")
        , price(0)
        , exchange(0.0)
        , category(0)
        , categoryname("")
    {

    }
    uint32 giftid;
    std::string giftname;
    uint32 price;
    double exchange;
    uint32 category;
    std::string categoryname;
};


class GiftInfoHelper
{
public:
    GiftInfoHelper();
    ~GiftInfoHelper();

    // �����ӷ�������ȡ������json����
    bool Initialize(const std::string& inputstr);
    bool GetGiftInfo(uint32 giftid, GiftInfo* giftinfo) const;

private:
    std::map<uint32, GiftInfo> giftmap_;
    std::map<uint32, std::string> category_;
};

