# Character Sheets

## Swordsman
- **Strengths**: Defense, Melee Attack
- **Weaknesses**: Speed, Ranged Attack
- **Stats**:
  - HP: 120
  - Attack: 15
  - Defense: 18
  - Speed: 8
- **Special Ability**: *Vanguard* - Reduced damage from physical frontal attacks.

## Mage
- **Strengths**: Ranged Attack, Magic Utility
- **Weaknesses**: HP, Physical Defense
- **Stats**:
  - HP: 80
  - Attack: 18 (Magic)
  - Defense: 6
  - Speed: 10
- **Special Ability**: *Arcane Veil* - Can strike enemies from a distance and bypass physical shields.

## Wyvern
- **Strengths**: Speed, Mobility
- **Weaknesses**: Crowd Control, Sustainability
- **Stats**:
  - HP: 100
  - Attack: 12
  - Defense: 10
  - Speed: 20
- **Special Ability**: *Aerial Strike* - High evasion and the ability to bypass certain terrain obstacles.


Proposal:

Core Design Idea: Position + Tempo + Stance

Instead of just:

Attack / Magic / Item

Build the system around three shared mechanics:

Range (Front / Mid / Back)

Tempo (Who controls the pace?)

Stance (Commitment vs Safety)

This creates meaningful decisions every turn — even with minimal graphics.

1️⃣ Add a Simple Range System (3 Lanes)

Each combatant exists in one of three positions:

[ FRONT ] – melee range
[ MID   ] – default range
[ BACK  ] – safe distance

Changing position costs a turn (or partial turn).

Now classes interact differently:

🗡 Swordsman (Defense-Melee)

Strong in FRONT

Can “Guard” to reduce damage and control tempo

Has counterattacks if attacked in FRONT

Weak against ranged attacks if stuck in MID/BACK

Core fantasy:

Wins by controlling space and punishing aggression.

🔮 Mage (Distance-Ranged)

Strong in BACK

Spells more powerful at distance

Weak in FRONT (casting disrupted)

Can use “Zone” spells to deny movement

Core fantasy:

Wins by controlling distance and setup.

🐉 Wyvern (Flying-Speed)

Can change range for free (once per turn)

Harder to hit in MID/BACK

Lower raw defense

Has “Dive” attacks that deal high damage but force FRONT next turn

Core fantasy:

Wins by tempo and burst mobility.

2️⃣ Introduce Tempo (Initiative Pressure)

Instead of fixed turns, add a simple “Momentum” meter.

Example:

Aggressive actions increase momentum.

Defensive actions reduce it.

If momentum hits threshold → extra action.

This makes:

Swordsman gain momentum when successfully blocking.

Mage gain momentum when exploiting weaknesses.

Wyvern gain momentum when switching positions.

Now decisions aren’t just damage — they affect pacing.

3️⃣ Add Stances (Commitment System)

Each class has 2–3 stances.

Swordsman

Guard Stance → low damage, counter chance

Assault Stance → high damage, no counter

Mage

Channel → next spell stronger

Quickcast → weaker but faster

Wyvern

Soar → hard to hit, low damage

Dive → high damage, forced FRONT next turn

Stances create:

Bluffing

Anticipation

Prediction

And they’re cheap to implement (just stat modifiers).

4️⃣ Make Enemies That Test Each Class Differently

Design enemies around forcing movement decisions.

Examples:

Archer enemy → punishes staying in BACK

Shielded enemy → immune to ranged until armor broken

Spear enemy → punishes entering FRONT

Net enemy → grounds the Wyvern

You don’t need many enemy types — just archetypes.

5️⃣ Give Each Class a Unique “Rule-Break”

This makes them feel special without huge systems.

Swordsman: Reactive Damage

Can counterattack when predicting correctly.

Mage: Status Synergy

Burn → more damage from lightning
Freeze → easier to hit
Simple bit flags can handle this.

Wyvern: Turn Manipulation

May act twice if momentum high.
Or can retreat after attacking.

These aren’t stat differences — they’re rule differences.

That’s what makes classes meaningful.

6️⃣ Keep It 8-Bit Friendly

Avoid:

Large skill trees

Complex AI

Huge status lists

Use:

Small fixed ability sets (4–6 per class)

3 position states

2 stance states

1 momentum variable

That’s extremely lightweight in RAM.

Example Turn Decision (Meaningful!)

Enemy is a spear guard in FRONT.

You are:

🗡 Swordsman

Guard (counter expected thrust)

Assault (risk heavy damage)

Retreat to MID

🔮 Mage

Stay BACK and cast

Move to MID to avoid counter magic

Channel next turn

🐉 Wyvern

Dive (burst but forced FRONT)

Soar (avoid damage)

Switch to BACK then attack

Different classes.
Different decision trees.
Same simple rule set.

What Makes This Rewarding?

Position matters.

Prediction matters.

Stances matter.

Enemies force adaptation.

Classes break rules differently.

This creates depth without complexity.

If you’d like, I can:

Design a minimal C struct layout for this system

Propose a combat loop pseudocode for your Zeal engine

Or design 6 enemy archetypes that stress-test all three classes

Given your emulator and tile engine work, this would map very cleanly onto a text-box + sprite swap battle system.

Show me the pseudocode. 

Love it. I’ll keep this 8-bit friendly, deterministic, and cheap in RAM.

We’ll assume:

1v1 combat (easy to extend later)

3 positions: FRONT, MID, BACK

2 stances per class

Simple momentum meter

Small fixed ability set

Everything is integer math. No floats. No dynamic allocation.

1️⃣ Core Data Structures
Enums
enum ClassType {
    CLASS_SWORDSMAN,
    CLASS_MAGE,
    CLASS_WYVERN
};

enum Position {
    POS_FRONT,
    POS_MID,
    POS_BACK
};

enum Stance {
    STANCE_0,
    STANCE_1
};

enum ActionType {
    ACT_ATTACK,
    ACT_SPECIAL,
    ACT_GUARD,
    ACT_MOVE_FORWARD,
    ACT_MOVE_BACK
};
Combatant Struct (Compact)

Keep it small — think 16–24 bytes total.

typedef struct {
    uint8_t classType;

    uint8_t hp;
    uint8_t maxHp;

    uint8_t atk;
    uint8_t def;
    uint8_t spd;

    uint8_t position;     // 0–2
    uint8_t stance;       // 0–1

    int8_t momentum;      // -10 to +10
    uint8_t statusFlags;  // bitfield (burn, freeze, grounded, etc.)
} Combatant;
2️⃣ Battle Loop

High-level loop:

void battleLoop(Combatant* player, Combatant* enemy)
{
    while (player->hp > 0 && enemy->hp > 0)
    {
        playerTurn(player, enemy);
        if (enemy->hp == 0) break;

        enemyTurn(enemy, player);
    }

    if (player->hp > 0)
        victory();
    else
        defeat();
}
3️⃣ Player Turn
void playerTurn(Combatant* self, Combatant* target)
{
    ActionType action = getPlayerInput();

    resolveAction(self, target, action);

    applyMomentum(self);

    checkExtraTurn(self, target);
}
4️⃣ Resolve Action

This is the heart.

void resolveAction(Combatant* self, Combatant* target, ActionType action)
{
    switch (action)
    {
        case ACT_ATTACK:
            doAttack(self, target);
            break;

        case ACT_SPECIAL:
            doSpecial(self, target);
            break;

        case ACT_GUARD:
            self->momentum -= 1;
            setGuardStance(self);
            break;

        case ACT_MOVE_FORWARD:
            if (self->position > POS_FRONT)
                self->position--;
            break;

        case ACT_MOVE_BACK:
            if (self->position < POS_BACK)
                self->position++;
            break;
    }
}
5️⃣ Damage Calculation (Position Matters)

Keep it integer and predictable.

void doAttack(Combatant* attacker, Combatant* defender)
{
    int damage = attacker->atk;

    // Range modifier
    damage += rangeModifier(attacker, defender);

    // Stance modifier
    damage += stanceModifier(attacker);

    // Defense
    damage -= defender->def;

    if (damage < 1)
        damage = 1;

    defender->hp -= damage;

    attacker->momentum += 2;
}
Range Modifier
int rangeModifier(Combatant* a, Combatant* d)
{
    int diff = a->position - d->position;

    if (a->classType == CLASS_SWORDSMAN)
    {
        if (diff == 0 && a->position == POS_FRONT)
            return +2;   // best case
        else
            return -2;
    }

    if (a->classType == CLASS_MAGE)
    {
        if (a->position == POS_BACK)
            return +2;
        if (a->position == POS_FRONT)
            return -3;
    }

    if (a->classType == CLASS_WYVERN)
    {
        return +a->spd / 2;  // speed scaling
    }

    return 0;
}
6️⃣ Special Abilities (Rule Breaking)

Each class bends rules.

void doSpecial(Combatant* a, Combatant* d)
{
    switch (a->classType)
    {
        case CLASS_SWORDSMAN:
            // Counter stance
            a->stance = STANCE_1;
            a->momentum += 1;
            break;

        case CLASS_MAGE:
            // Fire spell: ignore 50% defense
            castFire(a, d);
            break;

        case CLASS_WYVERN:
            // Dive: high damage but move to FRONT
            wyvernDive(a, d);
            break;
    }
}
Example: Wyvern Dive
void wyvernDive(Combatant* a, Combatant* d)
{
    int damage = a->atk + 4;
    d->hp -= damage;

    a->position = POS_FRONT;   // forced commitment
    a->momentum += 3;
}
7️⃣ Momentum + Extra Turn

Momentum creates clutch moments.

void applyMomentum(Combatant* c)
{
    if (c->momentum > 10) c->momentum = 10;
    if (c->momentum < -10) c->momentum = -10;
}

Extra turn trigger:

void checkExtraTurn(Combatant* self, Combatant* target)
{
    if (self->momentum >= 8)
    {
        self->momentum = 0;
        playerTurn(self, target);  // immediate extra turn
    }
}

On 8-bit you may want to set a flag instead of recursion.

8️⃣ Enemy AI (Cheap but Tactical)

No need for complex AI.

void enemyTurn(Combatant* self, Combatant* target)
{
    if (self->hp < 5)
        resolveAction(self, target, ACT_MOVE_BACK);
    else if (self->position != target->position)
        resolveAction(self, target, ACT_MOVE_FORWARD);
    else
        resolveAction(self, target, ACT_ATTACK);
}

Even simple movement makes combat feel intelligent.

9️⃣ Why This Works

This system gives:

Swordsman → wins in FRONT, counters, defensive momentum.

Mage → wants BACK, punishes approach.

Wyvern → controls tempo, burst mobility.


