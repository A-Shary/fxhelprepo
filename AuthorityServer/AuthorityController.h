#pragma once
class AuthorityController
{
public:
    AuthorityController();
    ~AuthorityController();

    bool Initialize();
    void Finalize();

    bool Start();
    void Stop();

    bool AddUser();
    bool RemoveUser();
    bool QueryUser();
    bool ModifyUser();

private:
    // ���ݿ����

    // �������Ӷ���

    // Ȩ�޼�ض���

    // �����û�����
};

