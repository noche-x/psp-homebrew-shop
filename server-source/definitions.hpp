#pragma once
#include <iostream>
#include <vector>

#define LOG(x) std::cout << "[+] " << x << std::endl;
#define INFO(x) std::cout << "[*] " << x << std::endl;
#define ERROR(x) std::cout << "[!!!] " << x << std::endl;

#define IF(statement, conti, error, ret) \
    if (statement)                       \
    {                                    \
        ERROR(error);                    \
        return ret;                      \
    }                                    \
    else if (conti != "")                \
        LOG(conti);

enum PacketIDS
{
    NONE = 0,
    GET_APPS = 300,
    SEND_APPS = 301,
    GET_FILE = 302,
    SEND_FILE = 303 
};

std::vector<int> sockets_to_close;