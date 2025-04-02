#include "MessageFormatter.h"

using namespace logger_inner;

Message::Message(const Message& o) :
	msg{o.msg}, type{o.type}, location{o.location}, level{o.level}, thr_id{o.thr_id}, ft{o.ft} {};

Message::Message(Message&& o) :
	msg{std::move(o.msg)}, type{std::move(o.type)}, location{std::move(o.location)}, level{std::move(o.level)},
	thr_id{std::move(o.thr_id)}, ft{std::move(o.ft)} {};

Message::Message(const std::string& m, logger::MessageTypes t, const std::source_location& l,
				 const logger::LogLevel& lv, const std::string& id, const Format& _ft) :
	msg{m}, type{t}, location{l}, level{lv}, thr_id{id}, ft{_ft} {};

void Message::operator=(const Message& obj) {
	msg = obj.msg;
	type = obj.type;
	location = obj.location;
	level = obj.level;
	thr_id = obj.thr_id;
	ft = obj.ft;
}