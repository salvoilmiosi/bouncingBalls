#include "globalvars.h"

#include <fstream>
#include <string>

#undef VAR
#define VAR(type, name, defValue) type name = defValue;
GLOBAL_VARS

static std::string getOptionsPath(const char *dir) {
    std::string path;
    if (dir) {
        path = dir;
        path = path.substr(0, 1 + path.find_last_of("\\/"));
    }
    path += OPTIONS_FILENAME;
    return path;
}

void readOptions(const char *dir) {
    std::ifstream ifs(getOptionsPath(dir));
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

void saveOptions(const char *dir) {
    std::ofstream ofs(getOptionsPath(dir), std::ofstream::trunc|std::ofstream::out);

    ofs << OPTIONS_FIRST_LINE << "\n\n";
#undef VAR
#define VAR(type,name,defValue) ofs << #name << ' ' << name << '\n';
    GLOBAL_VARS
}
