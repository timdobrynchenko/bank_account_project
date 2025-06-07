#include "Account.hpp"
#include <sstream>
#include <vector>

static std::vector<std::string> split(const std::string& s, char delim=';')
{
    std::vector<std::string> out; std::string part; std::stringstream ss(s);
    while(std::getline(ss, part, delim)) out.push_back(part);
    return out;
}

Account Account::fromCSV(const std::string& line)
{
    auto v = split(line);
    if(v.size() != 6) throw std::runtime_error("Bad CSV line: "+line);

    Account a;
    a.id      = v[0];
    a.fio     = v[1];
    a.balance = std::stof(v[2]);
    a.rate    = std::stof(v[3]);
    a.open    = CDate::parse(v[4]);
    a.hasCard = (v[5]=="1" || v[5]=="true");
    return a;
}

std::string Account::toCSV() const
{
    std::ostringstream os;
    os << id << ';' << fio << ';'
       << std::fixed << std::setprecision(2) << balance << ';'
       << rate << ';' << open.str() << ';' << (hasCard?1:0);
    return os.str();
}
