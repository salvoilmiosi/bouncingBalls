#include "globalvars.h"

#include <fstream>
#include <string>

#undef VAR
#define VAR(type, name, defValue) type name = defValue;
GLOBAL_VARS

void readOptions() {
    std::ifstream ifs(OPTIONS_FILENAME);
    std::string str;

    std::getline(ifs, str);
    if (str != OPTIONS_FIRST_LINE) return;
    std::getline(ifs, str);

    while(ifs.good() && ! ifs.eof()) {
        ifs >> str;
#undef VAR
#define VAR(type,name,defValue) if (str == #name) {ifs >> name; continue;}
        GLOBAL_VARS
    }
}

void saveOptions() {
    std::ofstream ofs(OPTIONS_FILENAME, std::ofstream::trunc|std::ofstream::out);

    ofs << OPTIONS_FIRST_LINE << "\n\n";
#undef VAR
#define VAR(type,name,defValue) ofs << #name << ' ' << name << '\n';
    GLOBAL_VARS
}
