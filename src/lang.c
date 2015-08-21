#include "include/opl.h"
#include "include/lang.h"
#include "include/util.h"
#include "include/fntsys.h"
#include "include/ioman.h"

// Language support
static char *internalEnglish[LANG_STR_COUNT] = {
	"English (internal)",
	"Open PS2 Loader %s",
	"Save changes",
	"Back",
	"Network config",
	"Advanced options",
	"<no values>",
	"Settings saved...",
	"Error writing settings!",
	"Exit",
	"Settings",
	"Menu",
	"USB Games",
	"HDD Games",
	"ETH Games",
	"Apps",
	"Theme",
	"Language",
	"The system will be powered off.",
	"Exit OPL?",
	"Cancel updating?",
	"%d: HardDisk Drive not detected",
	"%d: HardDisk Drive not formatted",
	"%d: Network startup error",
	"%d: No network adaptor detected",
	"%d: Cannot connect to SMB server",
	"%d: Cannot log into SMB server",
	"%d: Cannot open SMB share",
	"%d: Cannot list SMB shares",
	"%d: Cannot list games",
	"%d: DHCP server unavailable",
	"%d: No network connection",
	"On",
	"Off",
	"OK",
	"Select",
	"Cancel",
	"Circle",
	"Cross",
	"Games List",
	"Game settings",
	"Remove all settings",
	"Removed all keys for the game",
	"Scrolling",
	"Slow",
	"Medium",
	"Fast",
	"Default menu",
	"Load from disc",
	"Please wait",
	"Error while loading the Game ID",
	"Automatic sorting",
	"Error loading the language file",
	"Disable Debug Colors",
	"No controller detected, waiting...",
	"Enable Cover Art",
	"Wide screen",
	"Power off",
	"Loading config",
	"Saving config",
	"Start device",
	"Refresh",
	"USB device start mode",
	"HDD device start mode",
	"ETH device start mode",
	"Applications start mode",
	"Auto",
	"Manual",
	"Start HDL Server",
	"HDL Server Starting...",
	"HDL Server Running...",
	"Failed to start HDL Server.",
	"HDL Server Unloading...",
	"IGR path",
	"Background color",
	"Text color",
	"IP address type",
	"Static",
	"DHCP",
	"IP address",
	"Address",
	"Mask",
	"Gateway",
	"DNS Server",
	"Port",
	"Share",
	"User",
	"Password",
	"<not set>",
	"Address type",
	"IP",
	"NetBIOS",
	"Accept",
	"Item will be permanently deleted, continue?",
	"Rename",
	"Delete",
	"Run",
	"Display settings",
	"Enable Delete and Rename actions",
	"Remember last played game",
	"Select button",
	"Error, could not run the item",
	"Test",
	"Leave empty for GUEST auth.",
	"Accurate reads",
	"Alternative data read method",
	"Unhook Syscalls",
	"0 PSS mode",
	"Emulate DVD-DL",
	"Disable IGR",
	"High module storage",
	"Hide dev9 module",
	"Changing the size will reformat the VMC",
	"Create",
	"Start",
	"Modify",
	"Abort",
	"Reset",
	"Use generic",
	"Configure VMC",
	"Name",
	"Size",
	"Status",
	"Progress",
	"VMC file exist",
	"Invalid VMC file, size is incorrect",
	"VMC file need to be created",
	"Error with VMC %s, continue with physical MC (slot %d) ?",
	"Automatic refresh",
	"About",
	"Coders",
	"Quality Assurance",
	"USB prefix path",
	"Leave empty to exit to Browser",
	"Value in minute(s), 0 to disable spin down",
	"Automatic HDD spin down",
	"Video mode",
	"Dialog color",
	"Selected color",
	"Display info page",
	"Info",
	"Custom ELF",
	"Color selection",
	"Reconnect",
	"Leave empty to list shares",
	"ETH prefix path",
	"Backspace",
	"Space",
	"Enter",
	"Mode",
	"VMC Slot 1",
	"VMC Slot 2",
	"Game ID",
	"DMA Mode",
	"V-Sync",
	"Mode 1",
	"Mode 2",
	"Mode 3",
	"Mode 4",
	"Mode 5",
	"Mode 6",
	"Mode 7",
	"Mode 8",
	"Configure GSM",
	"Ethernet link mode",
	"100Mbit full-duplex",
	"100Mbit half-duplex",
	"10Mbit full-duplex",
	"10Mbit half-duplex",
	"GSM Settings",
	"Enable GSM",
	"Toggles GSM ON or OFF",
	"VMODE",
	"Forced Custom Display Mode",
	"H-POS",
	"Horizontal Adjustment",
	"V-POS",
	"Vertical Adjustment",
	"FMV Skip",
	"Skips Full Motion Videos",
	"Show PS2RD Cheat Engine on Main Menu",
	"Cheat Settings",
	"Enable PS2RD Cheat Engine",
	"Lets PS2RD Cheat Engine patch your games",
	"PS2RD Cheat Engine Mode",
	"Auto-select or Select game cheats",
	"Auto-select cheats",
	"Select game cheats",
	"Error: failed to load Cheat File",
	"No cheats found",
	"Download defaults",
	"Network update",
	"Re-download existing records?",
	"Update failed.",
	"Failed to connect to update server.",
	"Update completed.",
	"Update cancelled.",
	"Download settings from the network?",
	"Customized Settings",
	"Downloaded Defaults",
};

static int guiLangID = 0;
static char **lang_strs = internalEnglish;
static int nValidEntries = LANG_STR_COUNT;

static int nLanguages = 0;
static language_t languages[MAX_LANGUAGE_FILES];
static char **guiLangNames;

// localised string getter
char *_l(unsigned int id) {
	return lang_strs[id];
}

static void lngFreeFromFile(void) {
	if (guiLangID == 0)
		return;

	int strId = 0;
	for(; strId < nValidEntries; strId++) {
		free(lang_strs[strId]);
		lang_strs[strId] = NULL;
	}
	free(lang_strs);
	lang_strs = NULL;
}

static int lngLoadFromFile(char* path, char *name) {
	file_buffer_t* fileBuffer = openFileBuffer(path, O_RDONLY, 1, 1024);
	if (fileBuffer) {
		// file exists, try to read it and load the custom lang
		lang_strs = (char**) malloc(LANG_STR_COUNT * sizeof(char**));

		int strId = 0;
		while (strId < LANG_STR_COUNT && readFileBuffer(fileBuffer, &lang_strs[strId])) {
			strId++;
		}
		closeFileBuffer(fileBuffer);
		
		LOG("LANG Loaded %d entries\n", strId);

		// remember how many entries were read from the file (for the free later)
		nValidEntries = strId;

		// if necessary complete lang with default internal
		while (strId < LANG_STR_COUNT) {
			LOG("LANG Default entry added: %s\n", internalEnglish[strId]);
			lang_strs[strId] = internalEnglish[strId];
			strId++;
		}

		char path[256];
		snprintf(path, sizeof(path), "%s/font_%s.ttf", gBaseMCDir, name);
		LOG("LANG Custom font path: %s\n", path);
		fntLoadDefault(path);

		return 1;
	}
	return 0;
}

char* lngGetValue(void) {
	return guiLangNames[guiLangID];
}

static int lngReadEntry(int index, const char* path, const char* separator, const char* name, unsigned int mode) {
	if (!FIO_S_ISDIR(mode)) {
		if(strstr(name, ".lng") || strstr(name, ".LNG")) {

			language_t* currLang = &languages[index];

			// filepath for this language file
			int length = strlen(path) + 1 + strlen(name) + 1;
			currLang->filePath = (char*) malloc(length * sizeof(char));
			sprintf(currLang->filePath, "%s%s%s", path, separator, name);

			// extract name for this language (will be used for the English translation)
			length = strlen(name) - 5 - 4 + 1;
			currLang->name = (char*) malloc(length * sizeof(char));
			memcpy(currLang->name, name + 5, length);
			currLang->name[length - 1] = '\0';

			/*file_buffer_t* fileBuffer = openFileBuffer(currLang->filePath, 1, 1024);
			if (fileBuffer) {
				// read localized name of language from file
				if (readLineContext(fileBuffer, &currLang->name))
					readLineContext(fileBuffer, &currLang->fontName);
				closeFileBuffer(fileBuffer);
			}*/

			index++;
		}
	}
	return index;
}

void lngInit(void) {
	fntInit();

	nLanguages = listDir(gBaseMCDir, "/", MAX_LANGUAGE_FILES, &lngReadEntry);

	// build the languages name list
	guiLangNames = (char**) malloc((nLanguages + 2) * sizeof(char**));

	// add default internal (english)
	guiLangNames[0] = internalEnglish[0];

	int i = 0;
	for (; i < nLanguages; i++) {
		guiLangNames[i + 1] =  languages[i].name;
	}

	guiLangNames[nLanguages + 1] = NULL;
}

void lngEnd(void) {
	lngFreeFromFile();

	int i = 0;
	for (; i < nLanguages; i++) {
		free(languages[i].name);
		free(languages[i].filePath);
	}

	free(guiLangNames);

	fntEnd();
}

void lngSetGuiValue(int langID) {
	if (guiLangID != langID) {

		lngFreeFromFile();

		if (langID != 0) {
			language_t* currLang = &languages[langID - 1];
			if (lngLoadFromFile(currLang->filePath, currLang->name)) {
				guiLangID = langID;
				return;
			}
		}

		lang_strs = internalEnglish;
		guiLangID = 0;
	}
}

int lngGetGuiValue(void) {
	return guiLangID;
}

int lngFindGuiID(const char* lang) {
	if (lang) {
		int i = 0;
		for (; i < nLanguages; i++) {
			if (stricmp(languages[i].name, lang) == 0)
				return i + 1; // shift for Gui id
		}
	}
	return 0;
}

char **lngGetGuiList(void) {
	return guiLangNames;
}