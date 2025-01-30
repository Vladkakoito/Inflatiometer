#include <Common/Logger/Logger.h>
#include <Common/Settings.h>
#include <Defines.h>

static const char * kIniFile = MY_NAME ".ini";
static const char * kCommonIniFile = "Common.ini";
static const char * kCIniFile = "CCommon.ini";

static int PrintConfiguration(const struct TSettings * settings) {
  LOG("\n--------- Конфигурация менеджера БД ---------");
  LOG("\n---------------------------------------------");
}


int main(int argc, char * argv[]) {
  
}