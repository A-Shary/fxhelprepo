//#include <windows.h>
#include <assert.h>
char ReadByte(char ** src)
{
    char b = **src++;
    return b;
}
unsigned short Read2Byte(char ** src)
{
    unsigned short *sp = (unsigned short*)(*src);
    *src += 2;
    return *sp;
}
unsigned int ANSI2USC(char ** src)
{
    char b1 = ReadByte(src);
    if (b1 <= 0x7F)
    {
        return b1;
    }

    //���ֻ�������ֽڱ�ʾ
    char b2 = ReadByte(src);
    return b2 << 2 | b1;
}
unsigned int UTF162UCS(char ** src)
{
    // UTF16ʹ��1-2��16bit���ݱ�ʾUCS
    // �ж���ʹ��1��16bit����2��16bit;
    unsigned short lead = Read2Byte(src);
    if (lead >= 0xD800 && lead <= 0xDFFF)// 2��16bit��ʾ����
    {
        unsigned short trail = Read2Byte(src);
        if (trail >= 0xDC00 && trail <= 0xDFFF)
        {
            lead -= 0xD800;
            trail -= 0xDC00;
            return (lead << 16) | (trail);
        }
        else
        {
            // ����0xDC00-0xDFFF��Χ�ڵĺ�β���������ǲ��Ϸ�������
            // ��δ���ο�RFC�پ���
            assert(0);
            return 0;
        }

    }
    else// 1��16bit��ʾ����,
    {
        // ����Ҫ�����ת��,ֱ�Ӿ���UCS��
        return lead;
    }
}
unsigned int UTF82UCS(char ** src)
{
    char b1 = ReadByte(src);
    int ucs = 0;
    if (b1 >> 7 == 0x0)//0000 0000-0000 007F | 0xxxxxxx
    {
        return b1;
    }
    else if (b1 >> 5 == 0x6)//0000 0080-0000 07FF | 110xxxxx 10xxxxxx
    {
        char b2 = ReadByte(src);
        if (b2 >> 6 != 0x2)// �ڶ��ֽڲ���10xxxxxx��ʽ����Ϊ�Ǳ������
        {
            return 0;
        }
        return b1 << 4 | b2;
    }
    else if (b1 >> 4 == 0xE)//0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
    {
        char b2 = ReadByte(src);
        if (b2 >> 6 != 0x2)// �ڶ��ֽڲ���10xxxxxx��ʽ����Ϊ�Ǳ������
        {
            return 0;
        }
        char b3 = ReadByte(src);
        if (b3 >> 6 != 0x2)// �����ֽڲ���10xxxxxx��ʽ����Ϊ�Ǳ������
        {
            return 0;
        }
        return b1 << 8 | b2 << 4 | b3;
    }
    else if (b1 >> 3 == 0x1E)//0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
    {
        char b2 = ReadByte(src);
        if (b2 >> 6 != 0x2)// �ڶ��ֽڲ���10xxxxxx��ʽ����Ϊ�Ǳ������
        {
            return 0;
        }
        char b3 = ReadByte(src);
        if (b3 >> 6 != 0x2)// �����ֽڲ���10xxxxxx��ʽ����Ϊ�Ǳ������
        {
            return 0;
        }
        char b4 = ReadByte(src);
        if (b4 >> 6 != 0x2)// �����ֽڲ���10xxxxxx��ʽ����Ϊ�Ǳ������
        {
            return 0;
        }
        return b1 << 12 | b2 << 8 | b3 << 4 | b4;
    }
    else
    {
        assert(0);
    }

    return 0;
}

// rfc 3629 
//Char. number range | UTF-8 octet sequence
//(hexadecimal) | (binary)
//--------------------+---------------------------------------------
//0000 0000-0000 007F | 0xxxxxxx
//0000 0080-0000 07FF | 110xxxxx 10xxxxxx
//0000 0800-0000 FFFF | 1110xxxx 10xxxxxx 10xxxxxx
//0001 0000-0010 FFFF | 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

// encode format for new extend data
// 0020 0000-03FF FFFF | 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
// 0400 0000-7FFF FFFF | 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
int UCS2UTF8(unsigned int ucs, char * dest)
{
    char *p = dest;
    // ���ucs��ֵ, ʹ��UTF8��������ʾ����
    if (ucs <= 0x7f)
    {
        if (!p)return 1;
        *p++ = ucs & 0x7f;
    }
    else if (ucs <= 0x7ff)
    {
        if (!p)return 2;
        *p++ = 0xC0 | (ucs >> 6);
        *p++ = 0x08 | (ucs & 0x3f);
    }
    else if (ucs <= 0xFFFF)
    {
        if (!p)return 3;
        *p++ = 0xE0 | (ucs >> 12);
        *p++ = 0x80 | ((ucs >> 6) & 0x3f);
        *p++ = 0x80 | (ucs & 0x3f);
    }
    else if (ucs <= 0x10FFFF)
    {
        if (!p)return 4;
        *p++ = 0xF0 | (ucs >> 18);
        *p++ = 0x80 | ((ucs >> 12) & 0x3f);
        *p++ = 0x80 | ((ucs >> 6) & 0x3f);
        *p++ = 0x80 | (ucs & 0x3f);
    }
    else if (ucs <= 0x3ffffff)//��Щֵ����RFC3629�ﶨ��,�����Ҿ���ҲӦ�ô���
    {
        if (!p)return 5;
        *p++ = 0xF8 | (ucs >> 24);
        *p++ = 0x80 | ((ucs >> 18) & 0x3f);
        *p++ = 0x80 | ((ucs >> 12) & 0x3f);
        *p++ = 0x80 | ((ucs >> 6) & 0x3f);
        *p++ = 0x80 | (ucs & 0x3f);
    }
    else if (ucs <= 0x7fffffff)//��Щֵ����RFC3629�ﶨ��,�����Ҿ���ҲӦ�ô���
    {
        if (!p)return 6;
        *p++ = 0xF8 | (ucs >> 30);
        *p++ = 0x80 | ((ucs >> 24) & 0x3f);
        *p++ = 0x80 | ((ucs >> 18) & 0x3f);
        *p++ = 0x80 | ((ucs >> 12) & 0x3f);
        *p++ = 0x80 | ((ucs >> 6) & 0x3f);
        *p++ = 0x80 | (ucs & 0x3f);
    }
    else//����0-0x7fffffff��Χ�ڵ���ֵ,��Ϊ���ǺϷ���ucs
    {
        // Ĭ�ϴ���Ϊ0���Ǻ����أ��ο�RFC��������
        assert(0);
    }
    return p - dest;
}

int UCS2UTF16(unsigned int ucs, char * dest)
{
    char *p = dest;
    if (ucs <= 0xFFFF)
    {
        if (!p)return 2;
        *p++ = ucs;
        *p++ = 0;
    }
    else if (ucs <= 0x10FFFF)
    {
        if (!p)return 4;
        ucs -= 0x10000;
        unsigned short us = (ucs & 0x3FF) | 0xDC00;//low surrogate
        *p++ = us >> 8;
        *p++ = us & 0xFF;
        us = (ucs >> 10) | 0xD800;//high surrogate
        *p++ = us >> 8;
        *p++ = us & 0xFF;
    }
    else // RFC 2781 UTF16��˵��û�������˷�Χ�Ĵ���,����ΪӦ����Ҫ����
    {
        assert(0);
    }
    return p - dest;
}

int UTF162UTF8(char* src, char * dest)
{
    //read utf16
    int ucs = 0;
    char * utf16 = src;
    if (!dest)
    {
        // �������destΪ��,��ֻΪ���Ա��UTF8��ĳ����Է�������ڴ�
        int count = 0;
        while (ucs = UTF162UCS(&utf16))
        {
            count += UCS2UTF8(ucs, 0);
        }
        return count + 1;
    }

    char * utf8 = dest;
    while (ucs = UTF162UCS(&utf16))
    {
        utf8 += UCS2UTF8(ucs, utf8);
    }
    *utf8++ = 0;
    return utf8 - dest;
}

int UTF82UTF16(char* src, char * dest)
{
    //read utf8
    int ucs = 0;
    char * utf8 = src;
    if (!dest)
    {
        // �������destΪ��,��ֻΪ���Ա��UTF8��ĳ����Է�������ڴ�
        int count = 0;
        while (ucs = UTF82UCS(&utf8))
        {
            count += UCS2UTF16(ucs, 0);
        }
        return count + 2;
    }

    char * utf16 = dest;
    while (ucs = UTF82UCS(&utf8))
    {
        utf8 += UCS2UTF16(ucs, utf16);
    }
    *utf16++ = 0;
    *utf16++ = 0;
    return utf16 - dest;
}

// ������Ҫ�����ֽڵ�ANSI�ַ��ı���ʱ����������һ�������������ֽڴ洢��
// ������Ҫ�����ֽڵ�ANSI�ַ�������ʱ���룬Ҫ������˳���Ĵ���ʽ���ɲ���scanf
// ������Ҫ���������ֽ����ϱ�ʾ�ı������֣�ʹ��ANSI�޷���ʾ�����Ǳ������ᰲ�ų�0x3f3f��������ʽ
int ANSI2UTF8(char * src, char * dest)
{
    int ucs = 0;
    char * ansi = src;
    if (!dest)
    {
        int count = 0;
        while (ucs = ANSI2USC(&ansi))
        {
            count += UCS2UTF8(ucs, 0);
        }
        return count + 1;
    }

    char * utf8 = dest;
    while (ucs = ANSI2USC(&ansi))
    {
        ansi += UCS2UTF8(ucs, utf8);
    }
    *utf8++ = 0;
    return utf8 - dest;
}

int ANSI2UTF16(char * src, char * dest)
{
    int ucs = 0;
    char * ansi = src;
    if (!dest)
    {
        int count = 0;
        while (ucs = ANSI2USC(&ansi))
        {
            count += UCS2UTF16(ucs, 0);
        }
        return count + 2;
    }

    char * utf16 = dest;
    while (ucs = ANSI2USC(&ansi))
    {
        ansi += UCS2UTF16(ucs, utf16);
    }
    *utf16++ = 0;
    *utf16++ = 0;
    return utf16 - dest;
}