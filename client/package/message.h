//
// Created by kumo on 5/7/21.
//

#ifndef PEKCHAT_MESSAGE_H
#define PEKCHAT_MESSAGE_H


#define INT_NUMBER 10
#define STR_NUMBER 4
#define STR_LENGTH 20
#define MESSAGE_LENGTH 1024

#define CONTENT_LENGTH MESSAGE_LENGTH - 4 * INT_NUMBER - STR_NUMBER * STR_LENGTH

#define TYPE 0
#define STATE 1
#define SOURCE 2
#define DESTINATION 3
#define SEQ 4  // number of the content, for example, id of the message
#define ID_TOKEN 5
#define TIME_SERVER 6

//===TYPE===
#define REGISTER_REQUEST 1
#define REGISTER_RESPONSE 2
#define LOGIN_REQUEST 3
#define LOGIN_RESPONSE 4  // server to client
#define LOGIN_OK 9  // client to server, last time
#define CHAT_SEND 5  // client send chat to friend
#define CHAT_RECV 6  // server notice client or init when login
#define FRIEND_RECV 7  // server notice client for friend or init when login
#define FRIEND_LOG_CHANGE 8  // server notice client when someone login or logout
#define INIT_COMPLETE 10  // server to client, only when init
#define LOGOUT 11  // client to server. remember, even without this message, user may log out
#define TIME_REQUEST 12  // client to server, get the server time
#define TIME_RESPONSE 13  // server to client, return the server time

class message {
public:
    int integer[INT_NUMBER]{};  // 10 int
    char strs[STR_NUMBER][STR_LENGTH]{};  // 4 short strings
    char content[CONTENT_LENGTH]{};

    int fd;

public:
    message();

    message(int t);

    void serialize(char *buf);

    int set_int(int n, int value);

    int set_str(int n, const char *s);

    int set_fd(int n);

    int get_fd();

    int set_content(const char *s);

    int get_int(int n, int *tar);

    int get_int(int n);

    int get_str(int n, char *s);

    char *get_str(int n);

    void parse(char *buf);
};

#endif //PEKCHAT_MESSAGE_H