/*
 * ============================================================
 *  NX AntiCheat (NX-AC)
 *
 *  Version       : 1.0.0
 *  Developer     : RickZin021
 *  Compatibility : SA-MP 0.3.7 / open.mp
 *
 *  Copyright (c) 2026 RickZin021
 *  All rights reserved.
 * ============================================================
 */

#include <a_samp>

#define NX_AC_AUTO_CALLBACKS
#include <nx_ac>

main() {}

public OnGameModeInit() {
    NX_AC_SetWebhook("https://discord.com/api/webhooks/YOUR_WEBHOOK_HERE");
    NX_AC_EnableWebhook(true);

    NX_AC_EnableModule(NX_MODULE_VPN, false);
    NX_AC_EnableModule(NX_MODULE_SPEEDHACK, true);
    NX_AC_EnableModule(NX_MODULE_TELEPORT, true);
    NX_AC_EnableModule(NX_MODULE_AIRBREAK, true);
    NX_AC_EnableModule(NX_MODULE_WEAPON_HACK, true);
    NX_AC_EnableModule(NX_MODULE_HEALTH_HACK, true);
    NX_AC_EnableModule(NX_MODULE_RAPID_FIRE, true);
    NX_AC_EnableModule(NX_MODULE_BOT_DETECTION, true);

    NX_AC_SetRiskLimit(16);

    print("[NX-AC] AntiCheat initialized.");
    return 1;
}

public OnPlayerConnect(playerid) {
    new risk = NX_AC_GetRisk(playerid);
    if(risk > 0) {
        new str[64];
        format(str, sizeof(str), "[NX-AC] Risk residual: %d", risk);
        SendClientMessage(playerid, 0xFF0000FF, str);
    }
    return 1;
}

public OnNXDetect(playerid, detectid, risk, const reason[]) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    new str[256];
    format(str, sizeof(str),
        "[NX-AC] Detection | Player: %s | Module: %s | Risk: %d",
        name, NX_AC_GetModuleName(detectid), risk
    );

    SendClientMessageToAll(0xFF4500FF, str);
    printf(str);
    return 1;
}

public OnNXFlag(playerid, risk) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    new label[16];
    label = NX_AC_GetRiskLabel(risk);

    new str[128];
    format(str, sizeof(str), "[NX-AC] Flag | %s | Risk: %d (%s)", name, risk, label);
    printf(str);
    return 1;
}

public OnNXKick(playerid, const reason[]) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    new str[256];
    format(str, sizeof(str), "[NX-AC] KICK | Player: %s | Reason: %s", name, reason);
    SendClientMessageToAll(0xFF0000FF, str);
    printf(str);

    GameTextForPlayer(playerid, "~r~Kicked by AntiCheat", 3000, 3);
    return 1;
}

public OnNXBan(playerid, const reason[]) {
    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));

    new str[256];
    format(str, sizeof(str), "[NX-AC] BAN | Player: %s | Reason: %s", name, reason);
    SendClientMessageToAll(0xFF0000FF, str);
    printf(str);

    GameTextForPlayer(playerid, "~r~Banned by AntiCheat", 3000, 3);
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[]) {
    if(!strcmp(cmdtext, "/myrisk", true)) {
        new risk = NX_AC_GetRisk(playerid);
        new label[16];
        label = NX_AC_GetRiskLabel(risk);

        new str[64];
        format(str, sizeof(str), "[NX-AC] Your risk: %d (%s)", risk, label);
        SendClientMessage(playerid, 0xFFFFFFFF, str);
        return 1;
    }

    if(!strcmp(cmdtext, "/resetrisk", true) && IsPlayerAdmin(playerid)) {
        new str[64];
        for(new i = 0; i < MAX_PLAYERS; i++) {
            if(IsPlayerConnected(i)) {
                NX_AC_ResetRisk(i);
            }
        }
        SendClientMessage(playerid, 0x00FF00FF, "[NX-AC] All risks reset.");
        return 1;
    }

    return 0;
}
