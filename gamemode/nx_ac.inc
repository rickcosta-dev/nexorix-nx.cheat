/*
 * ============================================================
 *  NX AntiCheat (NX-AC)
 *
 *  Version       : 1.0.0
 *  Developer     : RickZin021
 *  Language      : C++17
 *  Compatibility : SA-MP 0.3.7 / open.mp
 *
 *  Copyright (c) 2026 RickZin021
 *  All rights reserved.
 * ============================================================
 */


#if defined _nx_ac_included
    #endinput
#endif
#define _nx_ac_included

#pragma library nx_ac

#define NX_AC_VERSION "1.0.0"

#define NX_MODULE_SPEEDHACK         0
#define NX_MODULE_TELEPORT          1
#define NX_MODULE_AIRBREAK          2
#define NX_MODULE_VEHICLE_SPEED     3
#define NX_MODULE_VEHICLE_TELEPORT  4
#define NX_MODULE_WEAPON_HACK       5
#define NX_MODULE_AMMO_HACK         6
#define NX_MODULE_HEALTH_HACK       7
#define NX_MODULE_ARMOUR_HACK       8
#define NX_MODULE_MONEY_HACK        9
#define NX_MODULE_INVALID_ANIM      10
#define NX_MODULE_FAKE_LAG          11
#define NX_MODULE_PACKET_FLOOD      12
#define NX_MODULE_INVALID_SYNC      13
#define NX_MODULE_RAPID_FIRE        14
#define NX_MODULE_NO_RELOAD         15
#define NX_MODULE_VPN               16
#define NX_MODULE_MULTI_ACCOUNT     17
#define NX_MODULE_BOT_DETECTION     18

#define NX_RISK_NORMAL      0
#define NX_RISK_SUSPICIOUS  1
#define NX_RISK_DANGER      2
#define NX_RISK_CRITICAL    3

native NX_AC_EnableModule(moduleid, bool:enabled);
native NX_AC_SetRiskLimit(limit);
native NX_AC_EnableWebhook(bool:enabled);
native NX_AC_SetWebhook(const url[]);
native NX_AC_IsVPN(playerid);
native NX_AC_GetRisk(playerid);
native NX_AC_ResetRisk(playerid);

forward OnNXDetect(playerid, detectid, risk, const reason[]);
forward OnNXFlag(playerid, risk);
forward OnNXKick(playerid, const reason[]);
forward OnNXBan(playerid, const reason[]);
forward OnNXLog(level, const message[]);

#if defined NX_AC_AUTO_CALLBACKS

public OnNXDetect(playerid, detectid, risk, const reason[]) {
    #if defined NX_OnNXDetect
        return NX_OnNXDetect(playerid, detectid, risk, reason);
    #else
        return 1;
    #endif
}
#if defined NX_OnNXDetect
    forward NX_OnNXDetect(playerid, detectid, risk, const reason[]);
#endif

public OnNXFlag(playerid, risk) {
    #if defined NX_OnNXFlag
        return NX_OnNXFlag(playerid, risk);
    #else
        return 1;
    #endif
}
#if defined NX_OnNXFlag
    forward NX_OnNXFlag(playerid, risk);
#endif

public OnNXKick(playerid, const reason[]) {
    #if defined NX_OnNXKick
        return NX_OnNXKick(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined NX_OnNXKick
    forward NX_OnNXKick(playerid, const reason[]);
#endif

public OnNXBan(playerid, const reason[]) {
    #if defined NX_OnNXBan
        return NX_OnNXBan(playerid, reason);
    #else
        return 1;
    #endif
}
#if defined NX_OnNXBan
    forward NX_OnNXBan(playerid, const reason[]);
#endif

#endif

stock NX_AC_GetModuleName(moduleid) {
    new name[32];
    switch(moduleid) {
        case NX_MODULE_SPEEDHACK:        name = "SpeedHack";
        case NX_MODULE_TELEPORT:         name = "Teleport";
        case NX_MODULE_AIRBREAK:         name = "AirBreak";
        case NX_MODULE_VEHICLE_SPEED:    name = "VehicleSpeedHack";
        case NX_MODULE_VEHICLE_TELEPORT: name = "VehicleTeleport";
        case NX_MODULE_WEAPON_HACK:      name = "WeaponHack";
        case NX_MODULE_AMMO_HACK:        name = "AmmoHack";
        case NX_MODULE_HEALTH_HACK:      name = "HealthHack";
        case NX_MODULE_ARMOUR_HACK:      name = "ArmourHack";
        case NX_MODULE_MONEY_HACK:       name = "MoneyHack";
        case NX_MODULE_INVALID_ANIM:     name = "InvalidAnimation";
        case NX_MODULE_FAKE_LAG:         name = "FakeLag";
        case NX_MODULE_PACKET_FLOOD:     name = "PacketFlood";
        case NX_MODULE_INVALID_SYNC:     name = "InvalidSync";
        case NX_MODULE_RAPID_FIRE:       name = "RapidFire";
        case NX_MODULE_NO_RELOAD:        name = "NoReload";
        case NX_MODULE_VPN:              name = "VPN";
        case NX_MODULE_MULTI_ACCOUNT:    name = "MultiAccount";
        case NX_MODULE_BOT_DETECTION:    name = "BotDetection";
        default:                         name = "Unknown";
    }
    return name;
}

stock NX_AC_GetRiskLabel(risk) {
    if(risk >= 25) return "CRITICAL";
    if(risk >= 16) return "DANGER";
    if(risk >= 6)  return "SUSPICIOUS";
    return "NORMAL";
}
