#include <iostream>

#include "Database.hpp"
#include "SMTP/Mailbox.hpp"

int main() {
    std::string cs = "postgresql://username:password@localhost:5432/smtp";

    Database db(cs);

    std::cout<<"Database object created\n";

    User user1(7, "zack.chung@example.com", "safepassword");
    // bool res1 = db.CreateUser(user1);
    // std::cout<<"user created: "<<res1<<"\n";

    User user2(3, "aliceinchains@example.com", "rottenapple");
    User user3(4, "carol.white@example.com", "strongpass789");

    auto mails = db.GetSentMails(user2);
    
    for (const auto& mail_opt : mails) {
        if (mail_opt) {  // Check if optional has value
            IMail* mail_ptr = mail_opt.value();
            if (mail_ptr) {
                if (Message* msg = dynamic_cast<Message*>(mail_ptr)) {
                    std::cout << "Message content: " << msg->content << "\n";
                } else {
                    std::cout << "Not a Message type\n";
                }
            }
        }
    }

    // std::vector<std::optional<IMail*>> mails = db.GetReceivedMails(user2);
    // std::cout<<"mails.size : "<<mails.size()<<"\n";

    // for (std::optional<IMail*> mail: mails) {
    //     // if (mail) {
    //         // auto pmail = mail.value();
    //         // std::cout<<"mail: "<<pmail<<"\t\ttype of mail: "<<typeid(pmail).name()<<"\n";
    //         // Message* msg = dynamic_cast<Message*>(pmail);

    //         // std::cout<<"msg: "<<"\n";
    //         // if (msg) std::cout<<msg->content<<"\n";
    //         // else {std::cout<<"not a message\n";}

    //         std::cout<<"IMail*: ";
    //         IMail* val = mail.value();
    //         std::cout<<val<<"\n";
    //         std::cout<<"type of val: "<<typeid(val).name()<<"\n";
    //         if (val) {
    //             std::cout<<"Message*: ";
    //             if (Message* msg = dynamic_cast<Message*>(val)) {
    //                 std::cout<<msg<<"\n";
    //                 std::cout<<"main.value->content: "<<msg->content<<"\n";
    //             }
    //             else {std::cout<<"not a message\n";}
    //         }
        // }
    // }

    // Message message1(user2, user3, "Hello, how are you?");
    // bool res3 = db.CreateMail(message1);
    // std::cout<<"message1 created: "<<res3<<"\n";

    // Message message2(user3, user2, "Hello, fine, and you?");
    // bool res4 = db.CreateMail(message2);
    // std::cout<<"message2 created: "<<res3<<"\n";

    // bool res2 = db.UpdateUser(user2);
    // std::cout<<"user updated: "<<res2<<"\n";

    // bool res22 = db.UpdateUser(User(4, "carol.white@example.com", "strongpass789"));

    // User user3(5, "aliceinchains@example.com", "sludgefactory");
    // bool res3 = db.RemoveUser(user3);
    // std::cout<<"user removed: "<<res3<<"\n";

    // SMTP::Mailbox mb("zack.chung@example.com");
    // auto user4 = db.ReadUser(mb);
    // if (user4.has_value()) {
    //     std::cout<<"user4 found\n";
    //     std::cout<<"value().get(): "<<user4.value().get()<<"\n";
    //     User* puser = dynamic_cast<User*>(user4.value().get());
    //     std::cout<<"pointer to user: "<<puser<<"\n";
    //     std::cout<<"user4: "<<puser->id<<"\t"<<puser->name<<"\t"<<puser->password<<"\n";
    // }
    // else {std::cout<<"user4 not found\n";}
    
    



    return 0;
}

