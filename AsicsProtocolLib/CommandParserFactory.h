#pragma once

#include "MessageDefine.h"
#include "CommandParserInterface.h"
#include "CommandParser.h"
#include "AsicsProtocolLib/EndPointInfoCommandParser.h"

// ֻ�������ʹ�ã�
// ��Ϊ�ͻ����ڷ���֮ǰҪ������ʱ���֪����ʲô���ͣ��յ��ظ���ʱ��Ҳͨ��serialid���ҵ�ԭ����ҵ��ص�
class CommandParserFactory
{
public:
    static CommandParserInterface* CreateCommandParser(CommandId cmd_id, uint32 session_id)
    {
        CommandParserInterface* ptr = nullptr;
        switch (cmd_id)
        {
        case CMD_ENDPOINT_INFO_COMPUTER:
            ptr = new EndPointInfoCommandParser(session_id);
            break;

        default:
            ptr = new CommandParser<CMD_UNDEFINE>(session_id);
        }

        return ptr;
    }
};
