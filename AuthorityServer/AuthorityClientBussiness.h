#pragma once

#undef max // ��Ϊ΢�����������ĳЩͷ�ļ�������max��
#undef min // ��Ϊ΢�����������ĳЩͷ�ļ�������min��

#include <vector>
#include "third_party/chromium/base/basictypes.h"
#include "third_party/chromium/base/bind.h"
#include "third_party/chromium/base/threading/non_thread_safe.h"
#include "AuthorityCommand.h"

class AuthorityClientBussiness : public base::NonThreadSafe
{
public:
    AuthorityClientBussiness();
    ~AuthorityClientBussiness();

    void SetSendDataFunction(
        base::Callback<bool(const std::vector<uint8>& data)> callback);

    bool HandleMessage(const std::vector<uint8>& data);

private:
    base::Callback<bool(const std::vector<uint8>& data)> send_data_callback_;

    BufferParser buffer_parser_;;
};

