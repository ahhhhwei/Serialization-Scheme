#include <iostream>
#include <fstream>
#include "contacts.pb.h"
using namespace std;
using namespace contacts;

/**
 * 新增联系人信息。
 */
void AddPeopleInfo(PeopleInfo *people_info_ptr)
{
    cout << "------------新增联系人-------------" << endl;
    cout << "请输入联系人姓名: ";
    string name;
    getline(cin, name);
    people_info_ptr->set_name(name);
    cout << "请输入联系人年龄:";
    int age;
    cin >> age;
    people_info_ptr->set_age(age);
    cin.ignore(256, '\n');
    for (int i = 1;; i++)
    {
        cout << "请输入联系人电话" << i << "(仅输入回车完成电话添加) : ";
        string number;
        getline(cin, number);
        if (number.empty())
        {
            break;
        }
        PeopleInfo_Phone *phone = people_info_ptr->add_phone();
        phone->set_number(number);
        cout << "选择此电话类型（1、移动电话    2、固定电话）：";
        int type;
        cin >> type;
        cin.ignore(256, '\n');
        switch (type)
        {
        case 1:
            phone->set_type(PeopleInfo_Phone_PhoneType::PeopleInfo_Phone_PhoneType_MP);
            break;
        case 2:
            phone->set_type(PeopleInfo_Phone_PhoneType::PeopleInfo_Phone_PhoneType_TEL);
            break;
        default:
            cout << "非法选择，使用默认值！" << endl;
            break;
        }
    }
    cout << "----------添加联系人成功-----------" << endl;
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
    Contacts contacts;
    // 尝试读取已存在的联系人信息
    fstream input(argv[1], ios::in | ios::binary);
    if (!input)
    {
        cout << argv[1] << ": 文件未找到。创建一个新文件。" << endl;
    }
    else if (!contacts.ParseFromIstream(&input))
    {
        cerr << "解析联系人信息失败。" << endl;
        input.close();
        return -1;
    }
    // 添加一个新的联系人

    AddPeopleInfo(contacts.add_contacts());
    // 将更新后的联系人信息写入磁盘文件
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!contacts.SerializeToOstream(&output))
    {
        cerr << "写入联系人信息失败。" << endl;
        input.close();
        output.close();
        return -1;
    }
    input.close();
    output.close();

    // 在程序结束时调用 ShutdownProtobufLibrary()，以释放Protocol Buffer库分配的所有全局对象。对于大多数程序来说这是不必要的，因为进程无论如何都要退出，并且操作系统会负责回收所有内存。但是，如果你使用了内存泄漏检查工具，或者编写的是可以由单个进程多次加载和卸载的库，那么你可能希望强制Protocol Buffers清理所有内容。
    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}