#include <iostream>

#include "Connection/DBConnector.hpp"
#include "Repository/MessageRepository.hpp"
#include "Repository/UserRepository.hpp"

int main()
{
	std::string cs = "postgresql://postgres:smtp!!!@localhost:5432/smtp-db";

	DBConnector dbc{cs};
	UserRepository usrrp{&dbc};
	MessageRepository msgrp{&dbc};

	auto usrs1 = usrrp.GetUsers();
	for (auto usr : usrs1)
	{
		if (usr.has_value()) std::cout << usr->id << "\t" << usr->name << "\t" << usr->password << "\n";
	}

	bool res2 = usrrp.CreateUser(User{8, "zac.chung@example.com", "pa$$w0rd"});
	std::cout << "---------------------------------------\nafter insertion of new: " << res2 << "\n";

	auto usrs6 = usrrp.GetUsers();
	for (auto usr : usrs6)
	{
		if (usr.has_value()) std::cout << usr->id << "\t" << usr->name << "\t" << usr->password << "\n";
	}

	bool res = usrrp.CreateUser(*(usrs1[2]));
	std::cout << "---------------------------------------\nafter inserstion of the same: " << res << "\n";

	auto usrs5 = usrrp.GetUsers();
	for (auto usr : usrs5)
	{
		if (usr.has_value()) std::cout << usr->id << "\t" << usr->name << "\t" << usr->password << "\n";
	}

	std::cout << "---------------------------------------\nget_by_id (valid)\n";

	auto usr2 = usrrp.GetById(1);
	// for (auto usr: usrs2) {
	if (usr2.has_value()) std::cout << usr2->id << "\t" << usr2->name << "\t" << usr2->password << "\n";
	// }

	std::cout << "---------------------------------------\nget_by_name(valid)\n";

	// auto usr3 = usrrp.get_by_name("robert.lee@example.com");
	// // for (auto usr: usrs3) {
	//     if (usr3.has_value())
	//         std::cout<<usr3->id<<"\t"<<usr3->name<<"\t"<<usr3->password<<"\n";
	// // }

	// std::cout<<"---------------------------------------\nget_by_name(invalid)\n";

	// auto usr4 = usrrp.get_by_name("yana.chyp@example.com");
	// // for (auto usr: usrs4) {
	//     if (usr4.has_value())
	//         std::cout<<usr4->id<<"\t"<<usr4->name<<"\t"<<usr4->password<<"\n";
	//     else std::cout<<"User not found"<<std::endl;
	//     // }

	// std::cout<<"---------------------------------------\nget_messages\n";

	// auto msgs1 = msgrp.get_messages();
	// for (auto msg: msgs1) {
	//     if (msg.has_value())
	//         std::cout<<"from:"<<msg->from.id<<"\tto: "<<msg->to.id<<"\n"<<msg->content<<"\n";
	// }

	// std::cout<<"---------------------------------------\nget_messages_from(valid)\n";

	// User from = *(usrs1[0]);
	// User to = *(usrs1[4]);
	// User dnex = User{7, "yana.chyp@example.com", "pa$$w0rd"};

	// auto msgs2 = msgrp.get_messages_from(from);
	// for (auto msg: msgs2) {
	//     if (msg.has_value())
	//         std::cout<<"from:"<<msg->from.id<<"\tto: "<<msg->to.id<<"\n"<<msg->content<<"\n";
	// }

	// std::cout<<"---------------------------------------\nget_messages_to(valid)\n";

	// auto msgs3 = msgrp.get_messages_to(to);
	// for (auto msg: msgs3) {
	//     if (msg.has_value())
	//         std::cout<<"from:"<<msg->from.id<<"\tto: "<<msg->to.id<<"\n"<<msg->content<<"\n";
	// }

	// std::cout<<"---------------------------------------\nget_messages_feom(invalid)\n";

	// auto msgs4 = msgrp.get_messages_from(dnex);
	// for (auto msg: msgs4) {
	//     if (msg.has_value())
	//         std::cout<<"from:"<<msg->from.id<<"\tto: "<<msg->to.id<<"\n"<<msg->content<<"\n";
	// }

	// std::cout<<"---------------------------------------\n";

	return 0;
}