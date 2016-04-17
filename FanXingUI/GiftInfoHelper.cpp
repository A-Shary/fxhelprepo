
#include "stdafx.h"
#include "GiftInfoHelper.h"
#include "Network/EncodeHelper.h"

#include "third_party/json/json.h"
#include "third_party/chromium/base/strings/string_number_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversions.h"
#include "third_party/chromium/base/strings/utf_string_conversion_utils.h"

namespace
{
uint32 GetInt32FromJsonValue(const Json::Value& jvalue, const std::string& name)
{
    uint32 ret = 0;
    Json::Value jvdefault(Json::ValueType::objectValue);
    auto getdata = jvalue.get(name, jvdefault);
    if (getdata.isInt())
    {
        ret = getdata.asInt();
    }
    else if (getdata.isString())
    {
        base::StringToUint(getdata.asString(), &ret);
    }

    return ret;
}

double GetDoubleFromJsonValue(const Json::Value& jvalue, const std::string& name)
{
    double ret = 0;
    Json::Value jvdefault(Json::ValueType::objectValue);
    auto getdata = jvalue.get(name, jvdefault);
    if (getdata.isDouble())
    {
        ret = getdata.asDouble();
    }
    else if (getdata.isInt())
    {
        ret = getdata.asDouble();
    }
    else if (getdata.isString())
    {
        base::StringToDouble(getdata.asString(), &ret);
    }

    return ret;
}
}
GiftInfoHelper::GiftInfoHelper()
{
}


GiftInfoHelper::~GiftInfoHelper()
{
}

bool GiftInfoHelper::Initialize(const std::string& inputstr)
{
    auto pos = inputstr.find("jsonpcallback_httpvisitorfanxingkugoucomVServicesGiftServiceGiftServicegetGiftList");
    if (pos == std::string::npos)
        return false;

    pos = inputstr.find("(", pos);
    if (pos == std::string::npos)
        return false;

    auto begin = pos + 1;
    pos = inputstr.find(")",begin);
    if (pos == std::string::npos)
        return false;
    auto end = pos;

    std::string root = inputstr.substr(begin, end - begin);
    //����json����
    Json::Reader reader;
    Json::Value rootdata(Json::objectValue);
    if (!reader.parse(root, rootdata, false))
    {
        return false;
    }

    // �б�Ҫ���status��ֵ
    uint32 status = rootdata.get(std::string("status"), 0).asInt();
    if (status != 1)
    {
        return false;
    }

    Json::Value dataObject(Json::objectValue);
    dataObject = rootdata.get(std::string("data"), dataObject);
    if (dataObject.empty())
    {
        return false;
    }

    // category_
    Json::Value categoryObject(Json::arrayValue);
    categoryObject = dataObject.get("category", categoryObject);
    if (categoryObject.empty())
    {
        return false;
    }
    for (const auto& it : categoryObject)
    {
        uint32 classid = GetInt32FromJsonValue(it, "classId");
        // �Ѿ��Զ������ַ�ת��Ϊutf8, nice
        std::string className = it.get("className", "").asString(); 
        category_.insert(std::make_pair(classid, className));
    }

    // data
    Json::Value listObject(Json::arrayValue);
    listObject = dataObject.get("list", listObject);
    if (listObject.empty())
    {
        return false;
    }

    for (const auto& it : listObject)
    {
        GiftInfo giftinfo;
        giftinfo.giftid = GetInt32FromJsonValue(it, "id");
        giftinfo.giftname = it.get("name", "").asString();
        giftinfo.price = GetInt32FromJsonValue(it, "price");
        giftinfo.exchange = GetDoubleFromJsonValue(it, "exchange");
        giftinfo.category = GetInt32FromJsonValue(it, "category");
        auto find = category_.find(giftinfo.category);
        if (find!=category_.end())
        {
            giftinfo.categoryname = find->second;
        }
        giftmap_.insert(std::make_pair(giftinfo.giftid, giftinfo));
    }
    
    return true;
}

bool GiftInfoHelper::GetGiftInfo(uint32 giftid, GiftInfo* giftinfo) const
{
    const auto& it = giftmap_.find(giftid);
    if (it==giftmap_.end())
    {
        return false;
    }
    *giftinfo = it->second;
    return true;
}
