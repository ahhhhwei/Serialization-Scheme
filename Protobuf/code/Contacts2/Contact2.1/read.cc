#include <iostream>
#include <fstream>
#include "contacts.pb.h"
using namespace std;
using namespace contacts;

/**
 * 打印联系人列表。
 */
void PrintfContacts(const Contacts &contacts)
{
    for (int i = 0; i < contacts.contacts_size(); ++i)
    {
        const PeopleInfo &people = contacts.contacts(i);
        cout << "-----------联系人" << i + 1 << "------------" << endl;
        cout << "姓名：" << people.name() << endl;
        cout << "年龄：" << people.age() << endl;
        int j = 1;
        for (const PeopleInfo_Phone &phone : people.phone())
        {
            cout << "电话" << j++ << ": " << phone.number();
            cout << "  (" << phone.PhoneType_Name(phone.type()) << ")" << endl;
        }
    }
}

int main(int argc, char *argv[])
{
    // GOOGLE_PROTOBUF_VERIFY_VERSION 宏用于验证是否意外链接到了与编译的头文件不兼容的库版本。如果检测到版本不匹配，程序将终止。注意，每个.pb.cc 文件在启动时都会自动调用此宏。在使用 C++ Protocol Buffer库之前执行此宏是一个很好的做法，但不是绝对必要的。

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (argc != 2)
    {
        cerr << "用法:  " << argv[0] << " CONTACTS_FILE" << endl;
        return -1;
    }

    // 以二进制方式读取联系人信息
    Contacts contacts;
    fstream input(argv[1], ios::in | ios::binary);
    if (!contacts.ParseFromIstream(&input))
    {
        cerr << "解析联系人信息失败。" << endl;
        input.close();
        return -1;
    }

    // 打印联系人信息
    PrintfContacts(contacts);

    input.close();

    // 在程序结束时调用 ShutdownProtobufLibrary()，以释放Protocol Buffer库分配的所有全局对象。对于大多数程序来说这是不必要的，因为进程无论如何都要退出，并且操作系统会负责回收所有内存。但是，如果你使用了内存泄漏检查工具，或者编写的是可以由单个进程多次加载和卸载的库，那么你可能希望强制Protocol Buffers清理所有内容。

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}