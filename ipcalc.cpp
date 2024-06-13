#include <iostream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <math.h>

std::string yellow = "\033[33m";
std::string blue = "\033[34m";
std::string reset = "\033[0m";

unsigned int ipToInteger(const std::string& ip) {
    std::vector<int> octets;
    std::stringstream ss(ip);
    std::string octet;
    while (std::getline(ss, octet, '.')) {
        octets.push_back(std::stoi(octet));
    }
    
    unsigned int result = 0;
    for (int i = 0; i < 4; ++i) {
        result += (octets[i] << (8 * (3 - i)));
    }
    return result;
}

std::vector<int> splitString(const std::string& input, char delimiter) {
    std::vector<int> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(std::stoi(token));
    }

    return tokens;
}

std::vector<std::string> splitString1(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

unsigned int getNetworkAddress(const int& baseIP, int subnetMask) {
    return (baseIP & ((unsigned int)(-1) << (32 - subnetMask)));
}

unsigned int getBroadcastAddress(const int& baseIP, int subnetMask) {
    int mask = 32 - subnetMask;

    unsigned int tmp = baseIP >> mask;
    tmp = tmp << mask;

    unsigned int a = ~((unsigned int)(-1) << mask);

    return (tmp | a);
}

std::string integerToIP(unsigned int integer) {
    return std::to_string((integer >> 24) & 0xFF) + "." +
           std::to_string((integer >> 16) & 0xFF) + "." +
           std::to_string((integer >> 8) & 0xFF) + "." +
           std::to_string(integer & 0xFF);
}


bool check_ip_address(std::vector<int> ip) {
    for (int i = 0; i < ip.size(); i++) {
        if (ip[i] > 255) {
            return false;
        }
    }
    return true;
}

void print(std::vector<int> ip) {
    for (int i = 0; i < 4; i++) {
        if (i == 3) {
            std::cout << yellow << std::bitset<8>(ip[i]) << reset;
        } else {
            std::cout << yellow << std::bitset<8>(ip[i]) << reset << ".";
        }
    }
}

unsigned int get_netmask(int mask) {
    return ~((unsigned int)(-1) >> mask);
}

unsigned int get_wildcart(int mask) {
    return ~(get_netmask(mask));
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./ipcalc <ip_address>/<mask>" << std::endl;
        return -1;
    }

    std::string subnet_mask = argv[1];
    std::vector<std::string> splited_subnet_mask = splitString1(subnet_mask, '/'); // 0 element is ip, 1 is mask

    // parse mask
    int mask = std::stoi(splited_subnet_mask[1]);
    int bytes = 32 - mask;

    if (mask < 8){
        std::cerr << "Mask gotta be great than 8."<< std::endl;
        return -1;
    }

    // parse ip
    const std::string ip = splited_subnet_mask[0];
    const int ip_int = ipToInteger(ip);
    std::vector<int> splited_ip = splitString(ip, '.');

    // check ip is good
    if (!check_ip_address(splited_ip)) {
        std::cerr << "Ip address is incorrect." << std::endl;
        return -1;
    }

    // print address in string and in byte
    std::cout << "Address: " << blue << ip << reset << "\t\t";
    print(splited_ip); // function for print vector in byte-like
    std::cout << std::endl;

    std::string netmask = integerToIP(get_netmask(mask));
    std::string wildcart = integerToIP(get_wildcart(mask));

    std::cout << "Netmask: " << blue << netmask << " = " << mask << reset << "\t\t";
    print(splitString(netmask, '.'));
    std::cout << std::endl;

    std::cout << "Wildcart: " << blue << wildcart << reset << "\t\t";
    print(splitString(wildcart, '.'));
    std::cout << "\n=>\n";

    // get addresses (network adn boardcast)
    std::string network_address = integerToIP(getNetworkAddress(ip_int, mask));
    std::string boardcast_address = integerToIP(getBroadcastAddress(ip_int, mask));

    std::string HostMin = integerToIP(getNetworkAddress(ip_int, mask)+1);
    std::string HostMax = integerToIP(getBroadcastAddress(ip_int, mask)-1);

    // print network ip
    std::cout << "Network: " << blue << network_address << "/" << mask << reset << "\t\t";
    print(splitString(network_address, '.'));
    std::cout << std::endl;

    std::cout << "HostMin: " << blue << HostMin << reset << "\t\t";
    print(splitString(HostMin, '.'));
    std::cout << std::endl;

    std::cout << "HostMax: " << blue << HostMax << reset << "\t";
    print(splitString(HostMax, '.'));
    std::cout << std::endl;

    // print boardcast ip
    std::cout << "Boardcast: " << blue << boardcast_address  << reset << "\t";
    print(splitString(boardcast_address, '.'));
    std::cout << std::endl;

    // get total ips (without 2 boardcast and networks ip)
    long long int total_ips = pow(2, bytes) - 2;
    std::cout << "Hosts/Net: " << blue << total_ips << reset << "\n\n";

    return 0;
}