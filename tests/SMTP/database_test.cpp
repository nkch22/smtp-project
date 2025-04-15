#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <optional>
#include <memory>
#include <set>

// ========== INTERFACE ==========
struct User {
    int id;
    std::string email;
};

struct Mail {
    int id;
    int userId;
    std::string content;
};

class IDatabase {
public:
    virtual ~IDatabase() = default;

    // User operations
    virtual int addUser(const std::string& email) = 0;
    virtual std::optional<User> getUserById(int id) const = 0;
    virtual std::optional<User> getUserByEmail(const std::string& email) const = 0;
    virtual bool removeUser(int id) = 0;
    virtual std::vector<User> getAllUsers() const = 0;

    // Mail operations
    virtual int addMail(int userId, const std::string& content) = 0;
    virtual std::vector<Mail> getMailsByUserId(int userId) const = 0;
    virtual bool removeMail(int mailId) = 0;
    virtual std::vector<Mail> getAllMails() const = 0;

    // Maintenance
    virtual void clear() = 0;
    virtual int generateId() = 0;
};

// ========== FAKE DATABASE IMPLEMENTATION ==========
class FakeDatabase : public IDatabase {
public:
    int addUser(const std::string& email) override {
        int id = generateId();
        users.push_back({id, email});
        return id;
    }

    std::optional<User> getUserById(int id) const override {
        for (const auto& u : users)
            if (u.id == id) return u;
        return std::nullopt;
    }

    std::optional<User> getUserByEmail(const std::string& email) const override {
        for (const auto& u : users)
            if (u.email == email) return u;
        return std::nullopt;
    }

    bool removeUser(int id) override {
        for (auto it = users.begin(); it != users.end(); ++it) {
            if (it->id == id) {
                users.erase(it);
                return true;
            }
        }
        return false;
    }

    std::vector<User> getAllUsers() const override {
        return users;
    }

    int addMail(int userId, const std::string& content) override {
        int id = generateId();
        mails.push_back({id, userId, content});
        return id;
    }

    std::vector<Mail> getMailsByUserId(int userId) const override {
        std::vector<Mail> result;
        for (const auto& m : mails)
            if (m.userId == userId)
                result.push_back(m);
        return result;
    }

    bool removeMail(int mailId) override {
        for (auto it = mails.begin(); it != mails.end(); ++it) {
            if (it->id == mailId) {
                mails.erase(it);
                return true;
            }
        }
        return false;
    }

    std::vector<Mail> getAllMails() const override {
        return mails;
    }

    void clear() override {
        users.clear();
        mails.clear();
        nextId = 1;
    }

    int generateId() override {
        return nextId++;
    }

private:
    std::vector<User> users;
    std::vector<Mail> mails;
    int nextId = 1;
};

// ========== TEST SUITE ==========
class DatabaseTest : public ::testing::Test {
protected:
    std::unique_ptr<IDatabase> db;

    void SetUp() override {
        db = std::make_unique<FakeDatabase>();
    }

    void TearDown() override {
        db.reset();
    }
};

// --- USER TESTS ---
TEST_F(DatabaseTest, AddUserReturnsValidId) {
    int id = db->addUser("test@example.com");
    EXPECT_GT(id, 0);
}

TEST_F(DatabaseTest, GetUserByIdReturnsCorrectUser) {
    int id = db->addUser("abc@def.com");
    auto user = db->getUserById(id);
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(user->email, "abc@def.com");
}

TEST_F(DatabaseTest, GetUserByEmailReturnsCorrectUser) {
    db->addUser("unique@email.com");
    auto user = db->getUserByEmail("unique@email.com");
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(user->email, "unique@email.com");
}

TEST_F(DatabaseTest, GetNonexistentUserByIdReturnsNullopt) {
    auto user = db->getUserById(999);
    EXPECT_FALSE(user.has_value());
}

TEST_F(DatabaseTest, GetNonexistentUserByEmailReturnsNullopt) {
    auto user = db->getUserByEmail("missing@none.com");
    EXPECT_FALSE(user.has_value());
}

TEST_F(DatabaseTest, AddMultipleUsersCreatesUniqueIds) {
    std::set<int> ids;
    for (int i = 0; i < 100; ++i) {
        int id = db->addUser("user" + std::to_string(i) + "@mail.com");
        ids.insert(id);
    }
    EXPECT_EQ(ids.size(), 100);
}

TEST_F(DatabaseTest, GetAllUsersReturnsAllAdded) {
    db->addUser("a@x.com");
    db->addUser("b@y.com");
    db->addUser("c@z.com");
    auto all = db->getAllUsers();
    EXPECT_EQ(all.size(), 3);
}

TEST_F(DatabaseTest, RemoveUserWorks) {
    int id = db->addUser("temp@delete.com");
    EXPECT_TRUE(db->removeUser(id));
    EXPECT_FALSE(db->getUserById(id).has_value());
}

TEST_F(DatabaseTest, RemoveNonexistentUserFails) {
    EXPECT_FALSE(db->removeUser(123456));
}

TEST_F(DatabaseTest, RemoveUserDoesNotAffectOthers) {
    int id1 = db->addUser("user1@mail.com");
    int id2 = db->addUser("user2@mail.com");

    db->removeUser(id1);
    auto user2 = db->getUserById(id2);
    ASSERT_TRUE(user2.has_value());
    EXPECT_EQ(user2->email, "user2@mail.com");
}

// --- MAIL TESTS ---
TEST_F(DatabaseTest, AddMailReturnsValidId) {
    int userId = db->addUser("mailbox@in.com");
    int mailId = db->addMail(userId, "Hello");
    EXPECT_GT(mailId, 0);
}

TEST_F(DatabaseTest, GetMailsByUserReturnsCorrectData) {
    int id = db->addUser("x@x.com");
    db->addMail(id, "msg1");
    db->addMail(id, "msg2");
    auto mails = db->getMailsByUserId(id);
    EXPECT_EQ(mails.size(), 2);
}

TEST_F(DatabaseTest, GetMailsForUserWithNoneReturnsEmpty) {
    int id = db->addUser("nobody@nowhere.com");
    auto mails = db->getMailsByUserId(id);
    EXPECT_TRUE(mails.empty());
}

TEST_F(DatabaseTest, MailContentIsStoredCorrectly) {
    int id = db->addUser("store@mail.com");
    db->addMail(id, "This is a test email.");
    auto mails = db->getMailsByUserId(id);
    ASSERT_FALSE(mails.empty());
    EXPECT_EQ(mails[0].content, "This is a test email.");
}

TEST_F(DatabaseTest, RemoveMailWorks) {
    int uid = db->addUser("x@x.com");
    int mid = db->addMail(uid, "Bye");
    EXPECT_TRUE(db->removeMail(mid));
}

TEST_F(DatabaseTest, RemoveMailActuallyDeletesIt) {
    int uid = db->addUser("x@x.com");
    int mid = db->addMail(uid, "Bye");
    db->removeMail(mid);
    auto mails = db->getMailsByUserId(uid);
    EXPECT_TRUE(mails.empty());
}

TEST_F(DatabaseTest, DeleteNonexistentMailFails) {
    EXPECT_FALSE(db->removeMail(404));
}

TEST_F(DatabaseTest, GetAllMailsReturnsAll) {
    int uid = db->addUser("x@x.com");
    db->addMail(uid, "1");
    db->addMail(uid, "2");
    db->addMail(uid, "3");
    EXPECT_EQ(db->getAllMails().size(), 3);
}

// --- MISC TESTS ---
TEST_F(DatabaseTest, GenerateIdReturnsUniqueIds) {
    std::set<int> ids;
    for (int i = 0; i < 1000; ++i) {
        ids.insert(db->generateId());
    }
    EXPECT_EQ(ids.size(), 1000);
}

TEST_F(DatabaseTest, ClearResetsState) {
    db->addUser("clear@test.com");
    db->addMail(1, "wipe me");
    db->clear();
    EXPECT_TRUE(db->getAllUsers().empty());
    EXPECT_TRUE(db->getAllMails().empty());
}

// --- STRESS / EDGE TESTS ---
TEST_F(DatabaseTest, StressAddAndRemoveUsers) {
    for (int i = 0; i < 200; ++i)
        db->addUser("stress" + std::to_string(i) + "@test.com");

    for (int i = 1; i <= 100; ++i)
        db->removeUser(i);

    EXPECT_EQ(db->getAllUsers().size(), 100);
}

TEST_F(DatabaseTest, MassMailAddRemoveTest) {
    int uid = db->addUser("mass@load.com");
    for (int i = 0; i < 500; ++i)
        db->addMail(uid, "message " + std::to_string(i));

    for (const auto& mail : db->getAllMails())
        db->removeMail(mail.id);

    EXPECT_TRUE(db->getAllMails().empty());
}

class SMTPDataTest : public ::testing::Test {
protected:
    std::unique_ptr<IDatabase> db;
    void SetUp() override {
        db = std::make_unique<FakeDatabase>();
    }
};

// Mass add users
TEST_F(SMTPDataTest, Add1000UsersUniqueEmails) {
    std::set<int> ids;
    for (int i = 0; i < 1000; ++i) {
        int id = db->addUser("user" + std::to_string(i) + "@domain.com");
        ids.insert(id);
    }
    EXPECT_EQ(ids.size(), 1000);
    EXPECT_EQ(db->getAllUsers().size(), 1000);
}

// Add, then remove, then re-add same email
TEST_F(SMTPDataTest, ReAddAfterRemoveUser) {
    int id1 = db->addUser("reused@email.com");
    db->removeUser(id1);
    int id2 = db->addUser("reused@email.com");
    EXPECT_NE(id1, id2);
}

// Get nonexistent user multiple ways
TEST_F(SMTPDataTest, NulloptForInvalidUsers) {
    EXPECT_FALSE(db->getUserById(5000).has_value());
    EXPECT_FALSE(db->getUserByEmail("no@no.com").has_value());
}

// Mass add and remove users
TEST_F(SMTPDataTest, BulkUserInsertDelete) {
    std::vector<int> ids;
    for (int i = 0; i < 500; ++i)
        ids.push_back(db->addUser("bulk" + std::to_string(i) + "@x.com"));
    for (int i = 0; i < 250; ++i)
        db->removeUser(ids[i]);
    EXPECT_EQ(db->getAllUsers().size(), 250);
}

// Heavy mail test
TEST_F(SMTPDataTest, Add1000MailsToOneUser) {
    int uid = db->addUser("spam@bot.com");
    for (int i = 0; i < 1000; ++i)
        db->addMail(uid, "msg #" + std::to_string(i));
    auto mails = db->getMailsByUserId(uid);
    EXPECT_EQ(mails.size(), 1000);
}

// Mass remove mails
TEST_F(SMTPDataTest, RemoveAllMailsFromUser) {
    int uid = db->addUser("mail@test.com");
    std::vector<int> mids;
    for (int i = 0; i < 300; ++i)
        mids.push_back(db->addMail(uid, "text " + std::to_string(i)));
    for (int mid : mids)
        db->removeMail(mid);
    EXPECT_TRUE(db->getMailsByUserId(uid).empty());
}

// Mail content integrity
TEST_F(SMTPDataTest, MailContentPreserved) {
    int uid = db->addUser("c@c.com");
    db->addMail(uid, "keep this text safe");
    auto mails = db->getMailsByUserId(uid);
    ASSERT_EQ(mails.size(), 1);
    EXPECT_EQ(mails[0].content, "keep this text safe");
}

// ID generation stress test
TEST_F(SMTPDataTest, GenerateThousandIdsUnique) {
    std::set<int> ids;
    for (int i = 0; i < 1000; ++i)
        ids.insert(db->generateId());
    EXPECT_EQ(ids.size(), 1000);
}

// Test clear
TEST_F(SMTPDataTest, ClearResetsAllData) {
    int uid = db->addUser("x@y.com");
    db->addMail(uid, "test");
    db->clear();
    EXPECT_TRUE(db->getAllUsers().empty());
    EXPECT_TRUE(db->getAllMails().empty());
}

// Boundary ID test
TEST_F(SMTPDataTest, VeryHighIdAccessFails) {
    for (int i = 0; i < 100; ++i)
        db->addUser("id" + std::to_string(i) + "@test.com");
    EXPECT_FALSE(db->getUserById(999999).has_value());
}

// Mail to multiple users
TEST_F(SMTPDataTest, DistributeMailsEvenly) {
    std::vector<int> uids;
    for (int i = 0; i < 10; ++i)
        uids.push_back(db->addUser("u" + std::to_string(i) + "@d.com"));
    for (int i = 0; i < 100; ++i)
        db->addMail(uids[i % 10], "m" + std::to_string(i));
    for (int uid : uids)
        EXPECT_EQ(db->getMailsByUserId(uid).size(), 10);
}

// Mail removal integrity
TEST_F(SMTPDataTest, SelectiveMailDeletion) {
    int uid = db->addUser("delete@test.com");
    std::vector<int> mids;
    for (int i = 0; i < 50; ++i)
        mids.push_back(db->addMail(uid, "m" + std::to_string(i)));
    for (int i = 0; i < 25; ++i)
        db->removeMail(mids[i]);
    auto mails = db->getMailsByUserId(uid);
    EXPECT_EQ(mails.size(), 25);
}

// Performance-like bulk insert
TEST_F(SMTPDataTest, Insert10000UsersAndMails) {
    for (int i = 0; i < 1000; ++i) {
        int id = db->addUser("huge" + std::to_string(i) + "@x.com");
        for (int j = 0; j < 10; ++j)
            db->addMail(id, "m" + std::to_string(j));
    }
    EXPECT_EQ(db->getAllUsers().size(), 1000);
    EXPECT_EQ(db->getAllMails().size(), 10000);
}

// Simulated concurrent inserts
TEST_F(SMTPDataTest, SimulateMultithreadedAdd) {
    std::mutex m;
    auto addUsers = [this, &m](int start) {
        for (int i = start; i < start + 100; ++i) {
            std::lock_guard<std::mutex> lock(m);
            db->addUser("thr" + std::to_string(i) + "@t.com");
        }
    };
    std::thread t1(addUsers, 0);
    std::thread t2(addUsers, 100);
    t1.join();
    t2.join();
    EXPECT_EQ(db->getAllUsers().size(), 200);
}

// Mail ID uniqueness
TEST_F(SMTPDataTest, UniqueMailIds) {
    int uid = db->addUser("mail@id.com");
    std::set<int> mids;
    for (int i = 0; i < 300; ++i)
        mids.insert(db->addMail(uid, "x"));
    EXPECT_EQ(mids.size(), 300);
}

// Repeated add/remove users
TEST_F(SMTPDataTest, AddRemoveCycleStability) {
    for (int cycle = 0; cycle < 10; ++cycle) {
        std::vector<int> ids;
        for (int i = 0; i < 50; ++i)
            ids.push_back(db->addUser("cyc" + std::to_string(cycle) + "_" + std::to_string(i) + "@t.com"));
        for (int id : ids)
            db->removeUser(id);
    }
    EXPECT_TRUE(db->getAllUsers().empty());
}


// Edge case: empty email
TEST_F(SMTPDataTest, AddUserWithEmptyEmail) {
    int id = db->addUser("");
    auto user = db->getUserById(id);
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(user->email, "");
}

// Edge case: long email
TEST_F(SMTPDataTest, AddUserWithLongEmail) {
    std::string longEmail(500, 'a');
    longEmail += "@domain.com";
    int id = db->addUser(longEmail);
    auto user = db->getUserById(id);
    ASSERT_TRUE(user.has_value());
    EXPECT_EQ(user->email, longEmail);
}

// Add same email twice
TEST_F(SMTPDataTest, AddDuplicateEmails) {
    int id1 = db->addUser("dupe@test.com");
    int id2 = db->addUser("dupe@test.com");
    EXPECT_NE(id1, id2);
    auto all = db->getAllUsers();
    EXPECT_EQ(all.size(), 2);
}

// Remove mail before user
TEST_F(SMTPDataTest, RemoveMailsThenUser) {
    int uid = db->addUser("delcombo@test.com");
    int mid1 = db->addMail(uid, "msg1");
    int mid2 = db->addMail(uid, "msg2");
    db->removeMail(mid1);
    db->removeMail(mid2);
    EXPECT_TRUE(db->removeUser(uid));
    EXPECT_TRUE(db->getMailsByUserId(uid).empty());
}

// Interleave user/mail add/remove
TEST_F(SMTPDataTest, InterleaveUserMailOps) {
    int uid1 = db->addUser("x@x.com");
    int uid2 = db->addUser("y@y.com");
    db->addMail(uid1, "mail1");
    db->removeUser(uid2);
    db->addMail(uid1, "mail2");
    EXPECT_EQ(db->getMailsByUserId(uid1).size(), 2);
}

// Exotic email formats
TEST_F(SMTPDataTest, AddUsersWithOddEmails) {
    std::vector<std::string> emails = {
        "a@b",
        "a+b@c.com",
        "\"weird\"@quote.org",
        "dot..dot@invalid.com",
        "emoji@🐍.dev"
    };
    for (const auto& e : emails)
        db->addUser(e);
    EXPECT_EQ(db->getAllUsers().size(), emails.size());
}

// Large mail content
TEST_F(SMTPDataTest, AddHugeMailContent) {
    int uid = db->addUser("huge@mail.com");
    std::string bigBody(100000, 'z');
    db->addMail(uid, bigBody);
    auto mails = db->getMailsByUserId(uid);
    ASSERT_EQ(mails.size(), 1);
    EXPECT_EQ(mails[0].content.size(), 100000);
}

// Idempotent user deletion
TEST_F(SMTPDataTest, RemoveUserTwice) {
    int uid = db->addUser("once@only.com");
    EXPECT_TRUE(db->removeUser(uid));
    EXPECT_FALSE(db->removeUser(uid));
}

// Add users and delete in reverse
TEST_F(SMTPDataTest, ReverseDeleteOrder) {
    std::vector<int> ids;
    for (int i = 0; i < 10; ++i)
        ids.push_back(db->addUser("z" + std::to_string(i) + "@z.com"));
    std::reverse(ids.begin(), ids.end());
    for (int id : ids)
        EXPECT_TRUE(db->removeUser(id));
    EXPECT_TRUE(db->getAllUsers().empty());
}

// User mails stay after unrelated mail deletion
TEST_F(SMTPDataTest, MailDeletionDoesNotAffectOthers) {
    int u1 = db->addUser("a@a.com");
    int u2 = db->addUser("b@b.com");
    int m1 = db->addMail(u1, "x");
    int m2 = db->addMail(u2, "y");
    db->removeMail(m1);
    auto mailsU2 = db->getMailsByUserId(u2);
    ASSERT_EQ(mailsU2.size(), 1);
    EXPECT_EQ(mailsU2[0].id, m2);
}

// Mail from same text but different users
TEST_F(SMTPDataTest, DuplicateMailContentAcrossUsers) {
    int u1 = db->addUser("same1@test.com");
    int u2 = db->addUser("same2@test.com");
    db->addMail(u1, "same");
    db->addMail(u2, "same");
    EXPECT_EQ(db->getMailsByUserId(u1).size(), 1);
    EXPECT_EQ(db->getMailsByUserId(u2).size(), 1);
}

// Reuse ID post-clear
TEST_F(SMTPDataTest, IDAfterClearRestarts) {
    int id1 = db->addUser("first@x.com");
    db->clear();
    int id2 = db->addUser("second@x.com");
    EXPECT_EQ(id2, 1); // ID restarts
}

// Random insert/delete pattern
TEST_F(SMTPDataTest, RandomPatternStressTest) {
    std::vector<int> ids;
    for (int i = 0; i < 100; ++i)
        ids.push_back(db->addUser("r" + std::to_string(i) + "@x.com"));
    for (int i = 0; i < 100; i += 2)
        db->removeUser(ids[i]);
    for (int i = 100; i < 150; ++i)
        db->addUser("r" + std::to_string(i) + "@x.com");
    EXPECT_EQ(db->getAllUsers().size(), 100);
}
