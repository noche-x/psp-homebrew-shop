#include <iostream> //  cout, endl
#include <fstream>  //  ofstream
#include <iomanip>  //  setw

#include <signal.h>     //  signal
#include <sys/socket.h> //  network stuff
#include <arpa/inet.h>  //  network stuff
#include <unistd.h>     //  network stuff
#include <pthread.h>    //  threads

#include "json.hpp" //  json lib
#include "stardust_networking.hpp"
#include "definitions.hpp" //  log and stuff

void exit_program(int sig);

int main()
{
    signal(SIGINT, exit_program);

    std::ifstream file("apps.json");
    if (!file.good())
        ERROR("cumshot");
    nlohmann::json j;
    file >> j;
    for (int i = -1; i < j["apps"].size(); i++)
    {
        if (j["apps"][i].is_object())
        {
            std::ifstream f(j["apps"][i]["large"].get());
            if (!f.good())
            {
                std::string base_url = "http://psphbshop.s3.eu-central-1.amazonaws.com/";
                std::string req_file = base_url + j["apps"][i]["large"].get();
                LOG(req_file)
            }
        }
    }
    /*std::string file_string = "";

    file.seekg(0, std::ios::end);
    file_string.reserve(file.tellg());
    file.seekg(0, std::ios::beg);

    file_string.assign((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
*/

    return 0;
}

void exit_program(int sig)
{
    LOG("Caught SIG_TERM");

    for (auto &&i : sockets_to_close)
    {
        close(i);
    }

    exit(sig);
};