#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_ALL		0xFF

#define CONFIG_OPL		0x01
#define CONFIG_LAST		0x02
#define CONFIG_APPS		0x04

#define CONFIG_FILE_NUM 3

#define CONFIG_SOURCE_DEFAULT	0
#define CONFIG_SOURCE_USER	1
#define CONFIG_SOURCE_DLOAD	2	//Downloaded from the network

#define CONFIG_ITEM_NAME			"#Name"
#define CONFIG_ITEM_LONGNAME		"#LongName"
#define CONFIG_ITEM_SIZE			"#Size"
#define CONFIG_ITEM_FORMAT			"#Format"
#define CONFIG_ITEM_MEDIA			"#Media"
#define CONFIG_ITEM_STARTUP			"#Startup"
#define CONFIG_ITEM_ALTSTARTUP		"$AltStartup"
#define CONFIG_ITEM_VMC				"$VMC"
#define CONFIG_ITEM_COMPAT			"$Compatibility"
#define CONFIG_ITEM_DMA				"$DMA"
#define CONFIG_ITEM_DNAS			"$DNAS"
#define CONFIG_ITEM_CONFIGSOURCE	"$ConfigSource"

//Note: The following is for Per-Game GSM. -Bat-

#define CONFIG_ITEM_ENABLEGSM		"$EnableGSM"
#define CONFIG_ITEM_GSMVMODE		"$GSMVMode"
#define CONFIG_ITEM_GSMXOFFSET		"$GSMXOffset"
#define CONFIG_ITEM_GSMYOFFSET		"$GSMYOffset"
#define CONFIG_ITEM_GSMSKIPVIDEOS	"$GSMSkipVideos"

#define CONFIG_KEY_NAME_LEN	32
#define CONFIG_KEY_VALUE_LEN	256

struct config_value_t {
        char key[CONFIG_KEY_NAME_LEN];
        char val[CONFIG_KEY_VALUE_LEN];

        struct config_value_t *next;
};

typedef struct {
	int type;
	struct config_value_t *head;
	struct config_value_t *tail;
	char *filename;
	int modified;
	u32 uid;
} config_set_t;

void configInit(char *prefix);
void configEnd();
config_set_t *configAlloc(int type, config_set_t *configSet, char *fileName);
void configFree(config_set_t *configSet);
config_set_t *configGetByType(int type);
int configSetStr(config_set_t* configSet, const char* key, const char* value);
int configGetStr(config_set_t* configSet, const char* key, const char** value);
void configGetStrCopy(config_set_t* configSet, const char* key, char* value, int length);
int configSetInt(config_set_t* configSet, const char* key, const int value);
int configGetInt(config_set_t* configSet, const char* key, int* value);
int configSetColor(config_set_t* configSet, const char* key, unsigned char* color);
int configGetColor(config_set_t* configSet, const char* key, unsigned char* color);
int configRemoveKey(config_set_t* configSet, const char* key);
void configMerge(config_set_t* dest, const config_set_t *source);

void configReadIP();
void configWriteIP();
void configGetDiscIDBinary(config_set_t* configSet, void* dst);

int configRead(config_set_t* configSet);
int configReadBuffer(config_set_t* configSet, const void *buffer, int size);
int configReadMulti(int types);
int configWrite(config_set_t* configSet);
int configWriteMulti(int types);
int configGetStat(config_set_t* configSet, iox_stat_t *stat);
void configClear(config_set_t* configSet);

#ifdef VMC
void configGetVMC(config_set_t* configSet, char* vmc, int length, int slot);
void configSetVMC(config_set_t* configSet, const char* vmc, int slot);
void configRemoveVMC(config_set_t* configSet, int slot);
#endif

#endif