#include <hidProfiles.h>

static void clearMaps(HidRecord *dev) {
    for (int i = 0; i < MAX_USAGES; i++) {
        dev->axisMap[i] = -1;
        dev->buttonMap[i] = -1;
    }
}

InputButton parseButtonName(const char *s) {
    if (!strcmp(s, "a")) return INPUT_BTN_A;
    if (!strcmp(s, "b")) return INPUT_BTN_B;
    if (!strcmp(s, "x")) return INPUT_BTN_X;
    if (!strcmp(s, "y")) return INPUT_BTN_Y;
    if (!strcmp(s, "back")) return INPUT_BTN_BACK;
    if (!strcmp(s, "start")) return INPUT_BTN_START;
    if (!strcmp(s, "leftshoulder")) return INPUT_BTN_LB;
    if (!strcmp(s, "rightshoulder")) return INPUT_BTN_RB;
    if (!strcmp(s, "leftstick")) return INPUT_BTN_LS;
    if (!strcmp(s, "rightstick")) return INPUT_BTN_RS;
    return MAP_UNUSED;
}

InputAxis parseAxisName(const char *s) {
    if (!strcmp(s, "leftx")) return INPUT_AXIS_LEFT_X;
    if (!strcmp(s, "lefty")) return INPUT_AXIS_LEFT_Y;
    if (!strcmp(s, "rightx")) return INPUT_AXIS_RIGHT_X;
    if (!strcmp(s, "righty")) return INPUT_AXIS_RIGHT_Y;
    if (!strcmp(s, "lefttrigger")) return INPUT_AXIS_LT;
    if (!strcmp(s, "righttrigger")) return INPUT_AXIS_RT;
    return MAP_UNUSED;
}

InputDpad parseDpadName(const char *s) {
    if (!strcmp(s, "dpup")) return INPUT_DPAD_UP;
    if (!strcmp(s, "dpdown")) return INPUT_DPAD_DOWN;
    if (!strcmp(s, "dpleft")) return INPUT_DPAD_LEFT;
    if (!strcmp(s, "dpdright")) return INPUT_DPAD_RIGHT;
    return MAP_UNUSED;
}

static uint16_t parseHex(const char *s) {
    uint16_t value = 0;
    sscanf(s, "%4hx", &value);
    return value;
}

void parse_vid_pid(const char *guid, uint16_t *vid, uint16_t *pid) {
    /* sample SDL GUID standard for future reference: 
        03000000300f00000a01000000000000

        Byte 8 starts the VID (300f), byte 16 starts the PID (0a01)*/
    *vid = parseHex(guid + 8);
    *pid = parseHex(guid + 16);
}

void buildHIDMap(HidRecord *dev) {
    clearMaps(dev);

    // TODO: Load the game controller DB file
    //FILE *fp = fopen("gamecontrollerdb.txt");
    // TODO: search for the line containing our VID/PID via the GUID that prefixes the DB
    // TODO: Parse the data after the GUID
    // TODO: Map the controller state based on the parsed data
}