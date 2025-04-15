#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <regex>

class MockSmtpServer {
public:
    enum class State {
        GREETING,
        HELO_RECEIVED,
        MAIL_RECEIVED,
        RCPT_RECEIVED,
        DATA_RECEIVED,
        QUIT
    };

    MockSmtpServer() {
        reset();
    }

    std::string getGreeting() const {
        return "220 Mock SMTP Server Ready";
    }

    std::string handleCommand(const std::string& rawCommand) {
        std::string command = trim(rawCommand);

        if (state == State::QUIT)
            return "221 Already quit";

        if (toUpper(command) == "NOOP")
            return "250 OK";

        if (toUpper(command) == "RSET") {
            reset();
            return "250 OK";
        }

        if (toUpper(command) == "QUIT") {
            state = State::QUIT;
            return "221 Bye";
        }

        if (awaitingData) {
            if (command == ".") {
                awaitingData = false;
                state = State::DATA_RECEIVED;
                return "250 OK: Message accepted";
            }
            messageBuffer.push_back(command);
            return "";  // No response until "." is received
        }

        // Handle other SMTP commands
        if (startsWithIgnoreCase(command, "HELO")) {
            return handleHelo(command);
        } else if (startsWithIgnoreCase(command, "MAIL FROM:")) {
            return handleMailFrom(command);
        } else if (startsWithIgnoreCase(command, "RCPT TO:")) {
            return handleRcptTo(command);
        } else if (startsWithIgnoreCase(command, "DATA")) {
            return handleData();
        }

        return "500 Command unrecognized";
    }

    State getState() const {
        return state;
    }

    void reset() {
        state = State::GREETING;
        from.clear();
        recipients.clear();
        messageBuffer.clear();
        awaitingData = false;
    }

private:
    State state;
    std::string from;
    std::vector<std::string> recipients;
    std::vector<std::string> messageBuffer;
    bool awaitingData = false;

    std::string handleHelo(const std::string& cmd) {
        std::smatch match;
        if (std::regex_match(cmd, match, std::regex(R"(HELO\s+(\S+))", std::regex::icase))) {
            state = State::HELO_RECEIVED;
            return "250 Hello " + match[1].str();
        }
        return "501 Syntax: HELO hostname";
    }

    std::string handleMailFrom(const std::string& cmd) {
        if (state < State::HELO_RECEIVED)
            return "503 Bad sequence of commands";

        std::smatch match;
        if (std::regex_match(cmd, match, std::regex(R"(MAIL FROM:<(.+?)>)", std::regex::icase))) {
            from = match[1];
            state = State::MAIL_RECEIVED;
            return "250 OK";
        }
        return "501 Syntax: MAIL FROM:<address>";
    }

    std::string handleRcptTo(const std::string& cmd) {
        if (state < State::MAIL_RECEIVED)
            return "503 Bad sequence of commands";

        std::smatch match;
        if (std::regex_match(cmd, match, std::regex(R"(RCPT TO:<(.+?)>)", std::regex::icase))) {
            recipients.push_back(match[1]);
            state = State::RCPT_RECEIVED;
            return "250 OK";
        }
        return "501 Syntax: RCPT TO:<address>";
    }

    std::string handleData() {
        if (state < State::RCPT_RECEIVED)
            return "503 Bad sequence of commands";
        awaitingData = true;
        return "354 End data with <CR><LF>.<CR><LF>";
    }

    static std::string trim(const std::string& str) {
        auto begin = str.find_first_not_of(" \t\r\n");
        auto end = str.find_last_not_of(" \t\r\n");
        return (begin == std::string::npos) ? "" : str.substr(begin, end - begin + 1);
    }

    static std::string toUpper(const std::string& str) {
        std::string result = str;
        for (auto& c : result) c = toupper(c);
        return result;
    }

    static bool startsWithIgnoreCase(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() &&
               toUpper(str.substr(0, prefix.size())) == toUpper(prefix);
    }
};

class SmtpServerTest : public ::testing::Test {
protected:
    MockSmtpServer server;

    void SetUp() override {
        server.reset();  // Reset state if needed
    }
};

// Connection greeting
TEST_F(SmtpServerTest, ConnectionGivesGreeting) {
    EXPECT_EQ(server.getGreeting(), "220 Mock SMTP Server Ready");
}

// HELO
TEST_F(SmtpServerTest, HeloCommandValid) {
    EXPECT_EQ(server.handleCommand("HELO example.com"), "250 Hello example.com");
}

TEST_F(SmtpServerTest, HeloCommandMissingHostname) {
    EXPECT_EQ(server.handleCommand("HELO"), "501 Syntax: HELO hostname");
}

// MAIL FROM
TEST_F(SmtpServerTest, MailFromValidAfterHelo) {
    server.handleCommand("HELO example.com");
    EXPECT_EQ(server.handleCommand("MAIL FROM:<user@example.com>"), "250 OK");
}

TEST_F(SmtpServerTest, MailFromBeforeHeloFails) {
    EXPECT_EQ(server.handleCommand("MAIL FROM:<user@example.com>"), "503 Bad sequence of commands");
}

TEST_F(SmtpServerTest, MailFromInvalidSyntax) {
    server.handleCommand("HELO example.com");
    EXPECT_EQ(server.handleCommand("MAIL FROM:user@example.com"), "501 Syntax: MAIL FROM:<address>");
}

// RCPT TO
TEST_F(SmtpServerTest, RcptToValidAfterMailFrom) {
    server.handleCommand("HELO example.com");
    server.handleCommand("MAIL FROM:<user@example.com>");
    EXPECT_EQ(server.handleCommand("RCPT TO:<recipient@example.com>"), "250 OK");
}

TEST_F(SmtpServerTest, RcptToBeforeMailFromFails) {
    server.handleCommand("HELO example.com");
    EXPECT_EQ(server.handleCommand("RCPT TO:<recipient@example.com>"), "503 Bad sequence of commands");
}

TEST_F(SmtpServerTest, RcptToInvalidSyntax) {
    server.handleCommand("HELO example.com");
    server.handleCommand("MAIL FROM:<user@example.com>");
    EXPECT_EQ(server.handleCommand("RCPT TO:recipient@example.com"), "501 Syntax: RCPT TO:<address>");
}

TEST_F(SmtpServerTest, DataBeforeRcptFails) {
    server.handleCommand("HELO example.com");
    server.handleCommand("MAIL FROM:<user@example.com>");
    EXPECT_EQ(server.handleCommand("DATA"), "503 Bad sequence of commands");
}

// QUIT
TEST_F(SmtpServerTest, QuitCommand) {
    EXPECT_EQ(server.handleCommand("QUIT"), "221 Bye");
}

// Invalid command
TEST_F(SmtpServerTest, InvalidCommand) {
    EXPECT_EQ(server.handleCommand("FOOBAR"), "500 Command unrecognized");
}

// Case Insensitivity
TEST_F(SmtpServerTest, CommandsAreCaseInsensitive) {
    EXPECT_EQ(server.handleCommand("helo example.com"), "250 Hello example.com");
}

// Reset Command
TEST_F(SmtpServerTest, RsetResetsState) {
    server.handleCommand("HELO example.com");
    server.handleCommand("MAIL FROM:<user@example.com>");
    server.handleCommand("RSET");
    EXPECT_EQ(server.handleCommand("RCPT TO:<recipient@example.com>"), "503 Bad sequence of commands");
}

// Noop command
TEST_F(SmtpServerTest, NoopCommand) {
    EXPECT_EQ(server.handleCommand("NOOP"), "250 OK");
}


// Command with trailing whitespace
TEST_F(SmtpServerTest, CommandWithTrailingWhitespace) {
    server.handleCommand("HELO example.com   ");
    EXPECT_EQ(server.handleCommand("MAIL FROM:<user@example.com>    "), "250 OK");
}

// Commands with lowercase + weird spacing
TEST_F(SmtpServerTest, CommandsWithExtraSpaces) {
    EXPECT_EQ(server.handleCommand("   helo    weirdhost.com  "), "250 Hello weirdhost.com");
}

// Empty command
TEST_F(SmtpServerTest, EmptyCommandReturnsUnrecognized) {
    EXPECT_EQ(server.handleCommand(""), "500 Command unrecognized");
}

// Command with only whitespace
TEST_F(SmtpServerTest, WhitespaceOnlyCommand) {
    EXPECT_EQ(server.handleCommand("     "), "500 Command unrecognized");
}

// Command after QUIT
TEST_F(SmtpServerTest, CommandAfterQuitIsIgnored) {
    server.handleCommand("QUIT");
    EXPECT_EQ(server.handleCommand("MAIL FROM:<stillhere@example.com>"), "221 Already quit");
}

// DATA block with only a period
TEST_F(SmtpServerTest, DataBlockWithOnlyEndMarker) {
    server.handleCommand("HELO data.com");
    server.handleCommand("MAIL FROM:<a@b.com>");
    server.handleCommand("RCPT TO:<c@d.com>");
    server.handleCommand("DATA");
    EXPECT_EQ(server.handleCommand("."), "250 OK: Message accepted");
}

// Mixed case address in MAIL FROM and RCPT TO
TEST_F(SmtpServerTest, MixedCaseAddressAccepted) {
    server.handleCommand("HELO case.com");
    EXPECT_EQ(server.handleCommand("MAIL FROM:<User@Example.Com>"), "250 OK");
    EXPECT_EQ(server.handleCommand("RCPT TO:<Recipient@Example.Com>"), "250 OK");
}

// Use of command aliases or abbreviations (should fail)
TEST_F(SmtpServerTest, CommandAbbreviationsAreNotAccepted) {
    EXPECT_EQ(server.handleCommand("HE"), "500 Command unrecognized");
    EXPECT_EQ(server.handleCommand("MAI FROM:<a@b.com>"), "500 Command unrecognized");
}

// DATA command followed by malformed input
TEST_F(SmtpServerTest, DataWithNoEndingDotFailsToFinish) {
    server.handleCommand("HELO example.com");
    server.handleCommand("MAIL FROM:<test@x.com>");
    server.handleCommand("RCPT TO:<recv@x.com>");
    server.handleCommand("DATA");
    EXPECT_EQ(server.handleCommand("This is a test"), ""); // Still waiting
    EXPECT_EQ(server.handleCommand("Another line"), "");   // Still waiting
    // no ending '.' — session stuck in DATA state
    EXPECT_EQ(server.handleCommand("MAIL FROM:<again@x.com>"), ""); // still waiting for '.'
}
