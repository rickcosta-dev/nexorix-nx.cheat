# NX AntiCheat (NX-AC)

**Versão:** 1.0.0  
**Desenvolvido por:** Nexorix  
**Compatibilidade:** SA-MP 0.3.7 · open.mp  
**Linguagem:** C++17  

---

## O que é

NX-AC é um anti-cheat profissional para servidores SA-MP e open.mp. Toda detecção acontece no servidor.

O sistema analisa os sync packets enviados pelo cliente via RakNet, valida o comportamento temporal do jogador e aplica heurísticas para identificar trapaças com baixo índice de falsos positivos.

---

## Como funciona

```
Cliente envia sync packet (OnFoot / InCar / Bullet / RPC)
                ↓
RakNet Hook intercepta o pacote
                ↓
PacketValidator verifica integridade dos dados
                ↓
SyncValidator atualiza o estado do jogador
                ↓
Módulos analisam o histórico de amostras
                ↓
Detecção → Risk Score acumulado
                ↓
EventDispatcher → Callbacks Pawn + Webhook + Logs
                ↓
Risk >= threshold → Kick / Ban automático
```

---

## Instalação Rápida

### 1. Instalar no servidor

```
samp-server/
├── plugins/
│   └── nx_ac.dll          (Windows)
├── config/
│   └── nx_ac.json         ← configuração principal
└── pawno/
    └── include/
        └── nx_ac.inc      ← include Pawn
```

### 2. Registrar no servidor

**SA-MP — server.cfg:**
```
plugins nx_ac
```

**open.mp — config.json:**
```json
{
  "pawn": {
    "legacy_plugins": ["nx_ac"]
  }
}
```

### 3. Incluir no seu script

```pawn
#define NX_AC_AUTO_CALLBACKS
#include <nx_ac>
```

---

## Configuração

Edite `config/nx_ac.json` para ajustar o comportamento:

```json
{
  "modules": {
    "speedhack": true,
    "teleport": true,
    "airbreak": true,
    "weapon_hack": true,
    "health_hack": true,
    "rapid_fire": true,
    "vpn": false
  },
  "risk": {
    "kick_threshold": 16,
    "ban_threshold": 25,
    "decay_interval_seconds": 60,
    "decay_amount": 1
  },
  "webhook": {
    "enabled": false,
    "url": "https://discord.com/api/webhooks/..."
  },
  "logs": {
    "console_logs": true,
    "file_logs": true,
    "log_file": "logs/nx_ac.log"
  }
}
```

---

## Sistema de Risk Score

O NX-AC **nunca bane instantaneamente**. Cada detecção adiciona pontos ao score do jogador. O score decai com o tempo.

| Score | Nível      | Ação automática |
|-------|------------|-----------------|
| 0–5   | Normal     | Nenhuma         |
| 6–15  | Suspeito   | Log + Flag      |
| 16–24 | Perigo     | Kick            |
| 25+   | Crítico    | Ban             |

Score padrão por módulo:

| Módulo          | Score |
|-----------------|-------|
| PacketFlood     | +10   |
| InvalidSync     | +10   |
| WeaponHack      | +8    |
| HealthHack      | +7    |
| AmmoHack        | +6    |
| ArmourHack      | +6    |
| AirBreak        | +5    |
| SpeedHack       | +5    |
| NoReload        | +5    |
| RapidFire       | +4    |
| VehicleSpeed    | +4    |
| Teleport        | +3    |
| VehicleTeleport | +3    |
| FakeLag         | +3    |
| VPN             | +2    |
| InvalidAnim     | +2    |

---

## Módulos de Detecção

| ID  | Módulo              | Descrição                                          |
|-----|---------------------|----------------------------------------------------|
| 0   | SpeedHack           | Velocidade a pé acima do máximo com compensação de lag |
| 1   | Teleport            | Deslocamento instantâneo a pé                      |
| 2   | AirBreak            | Movimento vertical anormal sem veículo             |
| 3   | VehicleSpeedHack    | Veículo acima da velocidade máxima do modelo       |
| 4   | VehicleTeleport     | Teleporte dentro de veículo                        |
| 5   | WeaponHack          | Arma com ID fora do range válido (0–46)            |
| 6   | AmmoHack            | Munição negativa, acima de 9999 ou inválida        |
| 7   | HealthHack          | Health fora de 0–100 ou regeneração anormal        |
| 8   | ArmourHack          | Armour fora de 0–100                               |
| 9   | MoneyHack           | Ganho de dinheiro acima do threshold legítimo      |
| 10  | InvalidAnimation    | Animação com ID fora do range 0–1811               |
| 11  | FakeLag             | Ausência de updates ou posição congelada           |
| 12  | PacketFlood         | Envio de pacotes acima do limite por segundo       |
| 13  | InvalidSync         | Pacote com dados inválidos (NaN, Inf, quaternion)  |
| 14  | RapidFire           | Disparo mais rápido que o ciclo da arma            |
| 15  | NoReload            | Disparo sem recarga                                |
| 16  | VPN                 | IP identificado como VPN/Proxy (requer API key)    |
| 17  | MultiAccount        | Múltiplos jogadores com o mesmo serial (GPCI)      |
| 18  | BotDetection        | Comportamento robótico via análise estatística     |

Veja [`docs/MODULES.md`](docs/MODULES.md) para detalhes de cada algoritmo.

---

## API Pawn

### Natives

```pawn
NX_AC_EnableModule(moduleid, bool:enabled)  // liga/desliga módulo
NX_AC_SetRiskLimit(limit)                   // threshold de kick
NX_AC_EnableWebhook(bool:enabled)           // liga/desliga webhook
NX_AC_SetWebhook(const url[])               // define URL do webhook
NX_AC_IsVPN(playerid)                       // 1 se VPN detectado
NX_AC_GetRisk(playerid)                     // score atual
NX_AC_ResetRisk(playerid)                   // zera o score
```

### Callbacks

```pawn
public OnNXDetect(playerid, detectid, risk, const reason[])
public OnNXFlag(playerid, risk)
public OnNXKick(playerid, const reason[])
public OnNXBan(playerid, const reason[])
public OnNXLog(level, const message[])
```

Veja [`include/README.md`](include/README.md) para documentação completa da API.

---

## Webhook (Discord)

Quando habilitado, o plugin envia notificações HTTP POST assíncronas. A thread principal nunca é bloqueada — o envio usa uma fila com worker thread dedicada e retry automático.

Payload de exemplo:
```json
{
  "event": "detection",
  "player": "NomeJogador",
  "ip": "191.xxx.xxx.xxx",
  "reason": "SpeedHack",
  "risk": 5,
  "timestamp": 1700000000
}
```

Veja [`docs/WEBHOOK.md`](docs/WEBHOOK.md) para detalhes.

---

## Exemplos

| Arquivo                              | Descrição                              |
|--------------------------------------|----------------------------------------|
| `examples/basic_gamemode.pwn`        | Integração em gamemode com callbacks   |
| `examples/filterscript_example.pwn`  | Integração como filterscript com admin |

Veja [`examples/README.md`](examples/README.md) para guia de uso dos exemplos.

---

## Estrutura do Projeto

```
nx-ac/
├── plugin/
│   ├── core/           # Plugin principal, Config, RiskSystem, PlayerData, EventDispatcher
│   ├── modules/        # Detectores independentes (um arquivo por módulo)
│   ├── hooks/          # RakNetHook, SyncHook, RPCHook
│   ├── network/        # PacketValidator, SyncValidator
│   ├── webhook/        # WebhookManager, WebhookQueue (async)
│   ├── utils/          # Logger, Timer, MathUtils, StringUtils
│   ├── events/         # EventTypes e mapeamento de IDs
│   ├── sampapi/        # Headers SA-MP / AMX
│   └── main.cpp        # Entry point do plugin (Load/Unload/AmxLoad)
├── include/
│   └── nx_ac.inc       # Interface Pawn completa
├── config/
│   └── nx_ac.json      # Configuração principal
├── examples/           # Exemplos prontos de uso
├── docs/               # Documentação técnica
├── logs/               # Logs gerados em runtime
└── CMakeLists.txt      # Build system
```

---

## Compilar

Requisitos: CMake 3.16+, C++17 compiler, Git.

```bash
# Linux
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Windows (MSVC)
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -A Win32
cmake --build . --config Release
```

Dependências baixadas automaticamente: `nlohmann/json 3.11.3`, `cpr 1.10.5`.

---

## Licença

Propriedade da **Nexorix**. Todos os direitos reservados.
