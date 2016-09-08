#include "globalvars.h"

#include <cstdio>
#include <windows.h>

void optionAdd(const char *typeStr, const char *nameStr, void *varPt) {
    char msgText[128] = "";
    sprintf(msgText, "Unknown data type '%s' for %s", typeStr, nameStr);
    MessageBox(NULL, msgText, OPTIONS_FIRST_LINE, 0);
}

void optionAdd(const char *typeStr, const char *nameStr, char **varPt) {
    char msgText[128] = "";
    sprintf(msgText, "%s %s = %s", typeStr, nameStr, *((char**)varPt));
    MessageBox(NULL, msgText, OPTIONS_FIRST_LINE, 0);
}

void optionAdd(const char *typeStr, const char *nameStr, int *varPt) {
    char msgText[128] = "";
    sprintf(msgText, "%s %s = %d", typeStr, nameStr, *((int*)varPt));
    MessageBox(NULL, msgText, OPTIONS_FIRST_LINE, 0);
}

void optionAdd(const char *typeStr, const char *nameStr, unsigned int *varPt) {
    optionAdd(typeStr, nameStr, (int *)varPt);
}

void optionAdd(const char *typeStr, const char *nameStr, double *varPt) {
    char msgText[128] = "";
    sprintf(msgText, "%s %s = %f", typeStr, nameStr, *((double*)varPt));
    MessageBox(NULL, msgText, OPTIONS_FIRST_LINE, 0);
}

void optionAdd(const char *typeStr, const char *nameStr, float *varPt) {
    optionAdd(typeStr, nameStr, (double *)varPt);
}

void optionAdd(const char *typeStr, const char *nameStr, bool *varPt) {
    char msgText[128] = "";
    sprintf(msgText, "%s %s = %s", typeStr, nameStr, *((bool*)varPt)?"true":"false");
    MessageBox(NULL, msgText, OPTIONS_FIRST_LINE, 0);
}

void optionsDialog() {
#undef VAR
#define VAR(type,name,defValue) optionAdd(#type, #name, (type *) &name);
    GLOBAL_VARS
}
