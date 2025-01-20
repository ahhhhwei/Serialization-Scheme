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
        if (people.has_data() && people.data().Is<Address>())
        {
            Address address;
            people.data().UnpackTo(&address);
            if (!address.home_address().empty())
            {
                cout << "家庭地址：" << address.home_address() << endl;
            }
            if (!address.unit_address().empty())
            {
                cout << "单位地址：" << address.unit_address() << endl;
            }
        }
        // if (people.has_qq()) {

        // } else if (people.has_wechat()) {

        // }
        switch (people.other_contact_case())
        {
        case contacts::PeopleInfo::OtherContactCase::kQq:
            cout << "联系人qq：" << people.qq() << endl;
            break;
        case contacts::PeopleInfo::OtherContactCase::kWechat:
            cout << "联系人wechat：" << people.wechat() << endl;
        default:
            break;
        }

        if (people.remark_size())
        {
            cout << "备注信息：" << endl;
        }
        for (auto it = people.remark().cbegin(); it != people.remark().cend(); ++it)
        {
            cout << "         " << it->first << ": " << it->second << endl;
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