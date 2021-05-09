//
// Created by kumo on 5/8/21.
//

#ifndef PEKCHAT_ACCOUNT_FUNCTION_H
#define PEKCHAT_ACCOUNT_FUNCTION_H


int register_request(const char *username, const char *password);

int login_request(const char *username, const char *password);

int login_OK();

int logout();

#endif //PEKCHAT_ACCOUNT_FUNCTION_H