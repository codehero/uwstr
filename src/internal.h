#define WSTR(var, str) const char var[sizeof(str) + 4] = \
		((char[]){'\xF5', '\0', sizeof(str), sizeof(str) + 1}) str
