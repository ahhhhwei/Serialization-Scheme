#include <iostream>
#include <fstream>
#include "contacts.pb.h"
using namespace std;
using namespace contacts;

/**
  *
  新增联系⼈
  */
void AddPeopleInfo(PeopleInfo *people_info_ptr)
{
    cout << "------------新增联系⼈-------------" << endl;
    cout << "请输⼊联系⼈姓名: ";
    string name;
    getline(cin, name);
    people_info_ptr->set_name(name);
    cout << "请输⼊联系⼈年龄:";
    int age;
    cin >> age;
    people_info_ptr->set_age(age);
    cin.ignore(256, '\n');
    for (int i = 1;; i++)
    {
        cout << "请输⼊联系⼈电话" << i << "(只输⼊回⻋完成电话新增) : ";
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
            cout << "⾮法选择，使用默认值！" << endl;
                break;
        }
    }
    cout << "----------添加联系⼈成功-----------" << endl;
}

int main(int argc, char *argv[])
{
    // GOOGLE_PROTOBUF_VERIFY_VERSION 宏:验证没有意外链接到与编译的头⽂件不兼容的库版本。如果检测到版本不匹配，程序将中⽌。注意，每个.pb.cc ⽂件在启动时都会⾃动调⽤此宏。在使⽤ C++ Protocol Buffer库之前执⾏此宏是⼀种很好的做法，但不是绝对必要的。

    GOOGLE_PROTOBUF_VERIFY_VERSION;
    if (argc != 2)
    {
        cerr << "Usage:  " << argv[0] << " CONTACTS_FILE" << endl;
        return -1;
    }
    Contacts contacts;
    // 先读取已存在的contacts
    fstream input(argv[1], ios::in | ios::binary);
    if (!input)
    {
        cout << argv[1] << ": File was not found.  Creating a new file." << endl;
    }
    else if (!contacts.ParseFromIstream(&input))
    {
        cerr << "Failed to parse contacts." << endl;
        input.close();
        return -1;
    }
    // 新增⼀个联系⼈

    AddPeopleInfo(contacts.add_contacts());
    // 向磁盘⽂件写⼊新的 contacts
    fstream output(argv[1], ios::out | ios::trunc | ios::binary);
    if (!contacts.SerializeToOstream(&output))
    {
        cerr << "Failed to write contacts." << endl;
        input.close();
        output.close();
        return -1;
    }
    input.close();
    output.close();
    // 在程序结束时调⽤ ShutdownProtobufLibrary()，为了删除Protocol Buffer库分配的所有全局对象。对于⼤多数程序来说这是不必要的，因为该过程⽆论如何都要退出，并且操作系统将负责回收其所有内存。但是，如果你使⽤了内存泄漏检查程序，该程序需要释放每个最后对象，或者你正在编写可以由单个进程多次加载和卸载的库，那么你可能希望强制使⽤Protocol Buffers来清理所有内容。

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}