#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <mutex>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
std::mutex cout_mutex;

void scan_port(const std::string& host, int port) {
    int sockfd;
    struct sockaddr_in server_addr;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::lock_guard<std::mutex> guard(cout_mutex);
        std::cerr << "Ошибка при создании сокета\n";
        return;
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        std::lock_guard<std::mutex> guard(cout_mutex);
        std::cerr << "Ошибка при преобразовании адреса\n";
        close(sockfd);
        return;
    }
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    setsockopt(sockfd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0) {
        std::lock_guard<std::mutex> guard(cout_mutex);
        std::cout << "[+] Порт " << port << " открыт на хосте " << host << "\n";
    } else {
        std::lock_guard<std::mutex> guard(cout_mutex);
        std::cout << "[-] Порт " << port << " закрыт на хосте " << host << "\n";
    }
    close(sockfd);
}

void scan_ports(const std::string& host, const std::vector<int>& ports) {
    std::vector<std::thread> threads;
    for (int port : ports) {
        threads.emplace_back(scan_port, host, port);
    }

    for (std::thread& t : threads) {
        t.join();
    }
}

int main(int argc, char* argv[]) {
    std::string ip;
    std::vector<int> ports = {22, 80, 443, 8080};

    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("ip", po::value<std::string>(&ip)->required(), "set IP address")
            ("ports", po::value<std::vector<int>>()->multitoken()->default_value(ports, "22 80 443 8080"), "set ports");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);

        if (vm.count("help")) {
            std::cout << desc << "\n";
            return 1;
        }

        po::notify(vm);

        if (vm.count("ip")) {
            ip = vm["ip"].as<std::string>();
        }

        if (vm.count("ports")) {
            ports = vm["ports"].as<std::vector<int>>();
        }
    } catch (const po::error& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    std::cout << "\nСканирование хоста " << ip << "...\n";
    scan_ports(ip, ports);

    return 0;
}

