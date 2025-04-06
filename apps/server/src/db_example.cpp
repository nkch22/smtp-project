#include <iostream>

#include "Database.hpp"
#include "../Mailbox.hpp"

int main() {
    std::string cs = "postgresql://postgres:aerosmith@localhost:5432/smtp";

    Database db(cs);

    std::cout<<"db created\n";

    // User user1(7, "zack.chung@example.com", "safepassword");
    // bool res1 = db.CreateUser(user1);
    // std::cout<<"user created: "<<res1<<"\n";

    User user2(3, "aliceinchains@example.com", "rottenapple");
    bool res2 = db.UpdateUser(user2);
    // std::cout<<"user updated: "<<res2<<"\n";

    bool res22 = db.UpdateUser(User(4, "carol.white@example.com", "strongpass789"));

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

