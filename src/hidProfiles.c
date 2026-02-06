#include <hidProfiles.h>

// SDL axis index -> HID Generic Desktop usage
static const uint16_t sdlAxisToHidUsage[] = {
    HID_USAGE_X, // a0 -> X
    HID_USAGE_Y, // a1 -> Y
    HID_USAGE_Z, // a2 -> Z
    HID_USAGE_RX, // a3 -> Rx
    HID_USAGE_RY, // a4 -> Ry
    HID_USAGE_RZ, // a5 -> Rz
};

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

// Parse 4 hex chars in little-endian to uint16_t
static uint16_t parseHex(const char *s) {
    if (!s || strlen(s) < 4) return 0;
    // Read the two bytes separately and swap
    unsigned int lo, hi;
    if (sscanf(s, "%2x%2x", &lo, &hi) != 2) return 0;
    return (uint16_t)((hi << 8) | lo);
}

void parse_vid_pid(const char *guid, uint16_t *vid, uint16_t *pid) {
    if (!guid || strlen(guid) < 32) {
        *vid = 0;
        *pid = 0;
        return;
    }

    *vid = parseHex(guid + 8);   // bytes 8-11
    *pid = parseHex(guid + 16);  // bytes 16-19
}

void parseMappingToken(HidRecord *dev, const char *token) {
    char logical[32], hid[32];
    if (sscanf(token, "%31[^:]:%31s", logical, hid) != 2)
        return;

    // AXES
    if (hid[0] == 'a') {
        InputAxis ax = parseAxisName(logical);
        int sdlAxis = atoi(hid + 1);
        if (sdlAxis < 0 || sdlAxis >= (int)(sizeof(sdlAxisToHidUsage)/sizeof(sdlAxisToHidUsage[0])))
            return;

        AxisMapping *m = &dev->axes[dev->axisCount++];
        m->mappedEnum = ax;
        m->usage      = sdlAxisToHidUsage[sdlAxis];
        m->capIndex   = -1; // resolved later
    }

    // BUTTONS
    if (hid[0] == 'b') {
        InputButton btn = parseButtonName(logical);
        if (btn == MAP_UNUSED) return;

        ButtonMapping *m = &dev->buttons[dev->buttonCount++];
        m->mappedEnum  = btn;
        m->buttonIndex = atoi(hid + 1); // b0 -> 0, b1 -> 1
        m->usage       = 0;             // resolved later
    }
}

void readLines(FILE *fp, HidRecord *dev) {
    /* Not very optimal, but it is a plaintext file and i'd rather do
    anything else than setup a data dictionary so I can have O(1) complexity
    for the people testing this app with the Hatsune Miku Sho PS3 Controller*/ 

    char line[512];
    while (fgets(line, sizeof(line), fp)) {
        if (line[0] == '#' || line[0] == '\n') continue;

        // Check if this line matches our device
        uint16_t vid, pid;
        parse_vid_pid(line, &vid, &pid);
        if (vid != dev->vendorID || pid != dev->productID) continue;

        // assign as a token when params match
        char *tokens = strchr(line, ','); // comma after GUID
        if (!tokens) continue;
        tokens++;

        // Split by commas or spaces and map each token
        char *tok = strtok(tokens, ",");
        while (tok) {
            parseMappingToken(dev, tok);
            tok = strtok(NULL, ",");
        }

        break;
    }
}

void buildHIDMap(HidRecord *dev) {
    dev->axisCount = 0;
    dev->buttonCount = 0;
    dev->dpadCount = 4;

    dev->dpads[0] = (DpadMapping){ INPUT_DPAD_UP,    BTN_DPAD_UP };
    dev->dpads[1] = (DpadMapping){ INPUT_DPAD_DOWN,  BTN_DPAD_DOWN };
    dev->dpads[2] = (DpadMapping){ INPUT_DPAD_LEFT,  BTN_DPAD_LEFT };
    dev->dpads[3] = (DpadMapping){ INPUT_DPAD_RIGHT, BTN_DPAD_RIGHT };
    // Load the game controller DB file provided by SDL
    FILE *fp = fopen("gamecontrollerdb.txt", "r");
    if (!fp) return;
    // search for the line containing our VID/PID via the GUID that prefixes the DB
    readLines(fp, dev);
    fclose(fp);
}