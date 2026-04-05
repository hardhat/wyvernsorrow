# Script Writing Checklist

All dialog must fit within two short lines (~22 chars each). Each encounter needs:
an intro/taunt, a player-specific approach line, a victory line, and (where applicable)
a captive rescue line. Player-specific branching uses `IF_FLAG_GOTO` on player-identity
flags (`WFLAG_IS_SWORDSMAN`, `WFLAG_IS_MAGE`, `WFLAG_IS_WYVERN`) set at character select.

---

## Complete Encounter Map

| Location         | Enemy               | NPC Captive      | NPC Present   |
| :--------------- | :------------------ | :--------------- | :------------ |
| Forest           | WENT_WEREWOLF       | WNPC_LITTLE_GIRL | —             |
| Ogre Lair        | WBOSS_OGRE          | WNPC_OLD_MAN     | —             |
| Castle           | WENT_SILVER_KNIGHT  | —                | WNPC_KING     |
| Crypt            | WENT_SKELETON       | WNPC_CARPENTER   | —             |
| Tower            | WENT_GENIE          | WNPC_MOTHER      | —             |
| Cave             | WENT_ORC            | WNPC_FATHER      | —             |
| Ruins            | WENT_TROLL          | —                | —             |
| Shrine           | WENT_GHOST          | —                | —             |
| Harbor           | WENT_DARK_KNIGHT    | —                | —             |
| Mountain Pass    | WBOSS_LAND_DRAGON   | —                | —             |
| Volcano          | WBOSS_FIRE_DRAGON   | —                | —             |
| Sky Peak         | WBOSS_WIND_DRAGON   | —                | —             |
| Swamp            | WBOSS_PUMPKIN_HEAD  | —                | —             |
| Demon Gate       | WDEMONLORD          | —                | —             |

---

## Phase 1 — Infrastructure

- [x] **1.1** Add missing entity IDs to `OBJS` in `script_gen.py`:
  `WENT_ORC=65, WENT_WEREWOLF=66, WENT_SKELETON=67, WENT_SILVER_KNIGHT=68,
  WENT_DARK_KNIGHT=69, WENT_GHOST=70, WENT_TROLL=71, WENT_GENIE=72,
  WBOSS_FIRE_DRAGON=115, WBOSS_PUMPKIN_HEAD=116, WDEMONLORD=127`

- [x] **1.2** Add player-identity flags to `FLAGS` in `script_gen.py`:
  `WFLAG_IS_SWORDSMAN=0x0040, WFLAG_IS_MAGE=0x0080, WFLAG_IS_WYVERN=0x0100`

- [x] **1.3** Add the same three flags to `enum WORLD_FLAGS` in `src/world.h`

- [x] **1.4** Add scenes for all 14 locations to `script.json` `"scenes"` block

---

## Phase 2 — Define Personalities for Generic Enemies

These enemies have no personality notes in `npc_enemies.md` yet.
Write a short comedic twist for each before scripting their encounters.

- [x] **2.1** ORC (`WENT_ORC`) — Cave guard blocking the Father
- [x] **2.2** TROLL (`WENT_TROLL`) — Ruins obstacle
- [x] **2.3** GHOST (`WENT_GHOST`) — Shrine obstacle
- [x] **2.4** DARK KNIGHT (`WENT_DARK_KNIGHT`) — Harbor obstacle
- [x] **2.5** SILVER KNIGHT (`WENT_SILVER_KNIGHT`) — Castle obstacle
- [x] **2.6** KING (`WNPC_KING`) — Castle NPC role (helpful, hostile, or useless?)

---

## Phase 3 — Enemy Encounter Scripts (per player)

Each task = write the `script.json` entries for that enemy covering all 3 player variants.
Structure: `IF_FLAG_GOTO` branch on player identity → player approach line → shared enemy
taunt → branched victory → `END`. Also include a defeated-state (repeat visit) line.

### 3.1 Forest — Werewolf (first encounter, all paths)
- [x] **3.1a** Swordsman: moves to overpower; Werewolf wags tail mid-transformation
- [x] **3.1b** Mage: tries a calming spell; Werewolf is confused but very hopeful
- [x] **3.1c** Wyvern: circles from above; Werewolf goes berserk wanting to give chase

### 3.2 Ogre Lair — Ogre Boss (rescues Old Man)
- [x] **3.2a** Swordsman: direct duel; Ogre offended by the lack of culinary appreciation
- [x] **3.2b** Mage: fire spell; Ogre is delighted and asks for recipe tips
- [x] **3.2c** Wyvern: dive attack; Ogre complains about the dining experience being ruined

### 3.3 Castle — Silver Knight + King
- [x] **3.3a** Swordsman: formal rival duel challenge with Silver Knight; King reacts afterward
- [x] **3.3b** Mage: Silver Knight is contemptuous of magic, then humiliated by it; King reacts afterward
- [x] **3.3c** Wyvern: Silver Knight refuses to fight something "not on solid ground"; King reacts afterward
- [x] **3.3d** King dialog (all players) — depends on Phase 2.6 personality decision

### 3.4 Crypt — Skeleton (rescues Carpenter)
- [x] **3.4a** Swordsman: Skeleton tries to look threatening; wrong arm keeps falling off
- [x] **3.4b** Mage: Skeleton is fascinated by magic and briefly forgets to fight
- [x] **3.4c** Wyvern: Skeletons argue about whose leg is whose for the entire fight

### 3.5 Tower — Genie (rescues Mother)
- [x] **3.5a** Swordsman: Genie is delighted to be threatened; must file forms first
- [x] **3.5b** Mage: Genie tries to out-bureaucrat the Mage's spells; loses on a technicality
- [x] **3.5c** Wyvern: Genie rules Wyverns need a permit to fly indoors; fight is over the permit

### 3.6 Cave — Orc (rescues Father)
- [x] **3.6a** Swordsman: tough brawl; Orc respects the fight
- [x] **3.6b** Mage: Orc laughs at magic, then doesn't
- [x] **3.6c** Wyvern: tight cave disadvantages the Wyvern; has to adapt

### 3.7 Ruins — Troll (write Phase 2.2 personality first)
- [x] **3.7a** Swordsman
- [x] **3.7b** Mage
- [x] **3.7c** Wyvern

### 3.8 Shrine — Ghost (write Phase 2.3 personality first)
- [x] **3.8a** Swordsman
- [x] **3.8b** Mage
- [x] **3.8c** Wyvern

### 3.9 Harbor — Dark Knight (write Phase 2.4 personality first)
- [x] **3.9a** Swordsman
- [x] **3.9b** Mage
- [x] **3.9c** Wyvern

### 3.10 Mountain Pass — Land Dragon
- [x] **3.10a** Swordsman: tries to walk past the "boulder"; Land Dragon is offended
- [x] **3.10b** Mage: casts a reveal spell; Land Dragon is mortified
- [x] **3.10c** Wyvern: spots it from the air immediately; Dragon refuses to acknowledge this

### 3.11 Volcano — Fire Dragon
- [x] **3.11a** Swordsman: interrupts a glass-blowing session; Dragon furious about the craft
- [x] **3.11b** Mage: casts fire magic; Dragon explains why that was "artistically offensive"
- [x] **3.11c** Wyvern: crash-lands near the kiln; Dragon's heartburn flares up from the stress

### 3.12 Sky Peak — Wind Dragon
- [x] **3.12a** Swordsman: Dragon stops fighting to gossip; Swordsman refuses to engage
- [x] **3.12b** Mage: trades gossip to create an opening; uses it
- [x] **3.12c** Wyvern: natural sky fight; Dragon is obsessed with Wyvern aerial gossip

### 3.13 Swamp — Pumpkin Head
- [x] **3.13a** Swordsman: Pumpkin Head launches legal threats; Swordsman ignores them
- [x] **3.13b** Mage: accidentally says "pumpkin spice"; full legal deposition begins
- [x] **3.13c** Wyvern: Pumpkin Head tries to serve a subpoena mid-air; Wyvern just leaves

### 3.14 Demon Gate — Demon Lord (final boss)
- [x] **3.14a** Swordsman: direct confrontation; Demon Lord complains about heroic clichés
- [x] **3.14b** Mage: disrupts the ritual; Demon Lord's back pain flares at the worst moment
- [x] **3.14c** Wyvern: aerial distraction; Demon Lord whines about "entitled flying youth"

---

## Phase 4 — NPC Rescue Dialogs (per player)

Each rescued NPC reacts in character. One or two lines per player.

- [x] **4.1** Little Girl rescued — Swordsman, Mage, Wyvern (cold tactical gratitude)
- [x] **4.2** Old Man rescued — Swordsman, Mage, Wyvern (confused; tries to give the wrong item)
- [x] **4.3** Carpenter rescued — Swordsman, Mage, Wyvern (immediately lists renovation plans)
- [x] **4.4** Mother rescued — Swordsman, Mage, Wyvern (heals you, then one-ups the rescue)
- [x] **4.5** Father rescued — Swordsman, Mage, Wyvern (worried you hurt yourself doing it)

---

## Phase 5 — Town Return Dialogs (post-rescue, per NPC)

Short lines for when you speak to each NPC after they've returned to Town.

- [x] **5.1** Little Girl in Town (repeat visits)
- [x] **5.2** Old Man in Town (always references a different wrong potion)
- [x] **5.3** Carpenter in Town (always building something unnecessary)
- [x] **5.4** Mother in Town (competitive suffering triggered by your current HP)
- [x] **5.5** Father in Town (sharpens your sword; makes you promise something first)
- [x] **5.6** Merchant in Town (loyalty points arc; treats demon invasion as "supply disruption")
- [x] **5.7** King in Castle — repeat visit dialog (depends on Phase 2.6 decision)
- [x] **5.8** Add `WNPC_KING` to `OBJS` in `script_gen.py` (currently missing)

---

## Phase 6 — Integration

- [x] **6.1** Add all new scripts to `script.json`
- [x] **6.2** Add all scene-to-script mappings to `script.json` `"scenes"` block
- [x] **6.3** Run `dat_gen.py` and verify `bin/wsorrow.dat` builds without error
- [x] **6.4** Verify text lengths fit the in-game dialog box (≤36 chars per line)
