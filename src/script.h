#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdint.h>

// ---- Strings ----
static const char s_str_0[] = "You circle from above. The wolf\nspots you. Its tail starts wagging.";
static const char s_str_1[] = "You dive. It leaps with pure joy.\nIt yelps. You win. It looks sad.";
static const char s_str_2[] = "You advance. The wolf snarls...\nthen its tail starts wagging.";
static const char s_str_3[] = "It trips over itself lunging.\nYou win. It wags the whole time.";
static const char s_str_4[] = "You cast a calming aura.\nThe wolf goes very, very still.";
static const char s_str_5[] = "It tilts its head. It whines.\nAre those eyes of hope? Oh no.";
static const char s_str_6[] = "It follows you. You shoo it.\nEventually you win, sort of.";
static const char s_str_7[] = "It wags hopefully at you.\nStill waiting for that stick.";
static const char s_str_8[] = "You dive. The Ogre's stew goes\nflying. He stares in horror.";
static const char s_str_9[] = "'My reduction! Three hours work!\nYou ANIMAL!' He charges.";
static const char s_str_10[] = "You draw your sword. The Ogre\nsniffs. 'Adventurer steak!'";
static const char s_str_11[] = "He fights and complains about\nyour 'lack of marinade.'";
static const char s_str_12[] = "You cast fire. The Ogre stops.\n'That sear! Give me the recipe!'";
static const char s_str_13[] = "He chases you for cooking tips.\nYou win. He's devastated.";
static const char s_str_14[] = "He sulks over a cold cauldron.\n'No one appreciates the craft.'";
static const char s_str_15[] = "'I challenge you to— you're\nflying. That is not permitted.'";
static const char s_str_16[] = "'Land immediately. Duelling\nrules forbid airborne opponents.'";
static const char s_str_17[] = "He refuses to engage. You wait\nhim out. He sulks. You pass.";
static const char s_str_18[] = "He bows stiffly. 'I am\noverqualified for guarding a door.'";
static const char s_str_19[] = "'Top of my class. Three dragons\nbefore breakfast. This is that.'";
static const char s_str_20[] = "He fights brilliantly while\nbeing furious about the posting.";
static const char s_str_21[] = "'Magic?' He scoffs. 'A knight\nfights with steel. Not tricks.'";
static const char s_str_22[] = "Your spell hits him squarely.\nHe stands in dignified silence.";
static const char s_str_23[] = "'I will not dignify that\nwith a reaction.' He falls.";
static const char s_str_24[] = "He stands aside, impeccably.\nStill visibly furious about it.";
static const char s_str_25[] = "'Mm? Oh yes, the demon thing.\nDo deal with Gerald first.'";
static const char s_str_26[] = "'Oh, you beat Gerald! Lovely.\nNow. Look at this diorama.'";
static const char s_str_27[] = "'Siege of Ashenvale, 1204.\nNote the pauldron detail. Note it.'";
static const char s_str_28[] = "'You may pass. I am also\nbestowing: Diorama Appreciator.'";
static const char s_str_29[] = "'Still here? Good. Have you\nconsidered this composition?'";
static const char s_str_30[] = "The skeletons are mid-argument\nabout whose femur is whose.";
static const char s_str_31[] = "You dart through while they\nsort out the bone situation.";
static const char s_str_32[] = "A skeleton lurches forward,\nwaving its sword arm menacingly.";
static const char s_str_33[] = "Its other arm falls off.\n'That's mine,' it says. It stops.";
static const char s_str_34[] = "The fight pauses so it can\nretrieve its limbs. You win.";
static const char s_str_35[] = "You cast a light spell. The\nskeleton stops. 'Is that Luminara?'";
static const char s_str_36[] = "It leans in, asks questions.\nYou defeat it mid-lecture.";
static const char s_str_37[] = "A single arm gives a thumbs up\nfrom the corner. Not its arm.";
static const char s_str_38[] = "'You are FLYING INDOORS,' the\nGenie declares. 'Not permitted.'";
static const char s_str_39[] = "'This requires Permit 22-C.\nDo you have a Permit 22-C?'";
static const char s_str_40[] = "You do not. The fight is now\nofficially about the permit.";
static const char s_str_41[] = "You find a loophole. He is\ndeeply, professionally furious.";
static const char s_str_42[] = "'Oh! A physical threat! Lovely!\nI'll need you to file Form 7-B.'";
static const char s_str_43[] = "No Form 7-B exists. You fight\nhim anyway. He seems relieved.";
static const char s_str_44[] = "'All spells require advance\nfiling in triplicate. Page six.'";
static const char s_str_45[] = "You cite Sub-Clause 9-C of\nthe Mystical Conduct Register.";
static const char s_str_46[] = "The clause doesn't exist.\nHe checks anyway. You win.";
static const char s_str_47[] = "He broods over his paperwork.\n'Loopholes everywhere. Everywhere.'";
static const char s_str_48[] = "The cave ceiling is very low.\nYou scramble. The Orc watches.";
static const char s_str_49[] = "'You look uncomfortable. That\nis valid data. Let's unpack it.'";
static const char s_str_50[] = "You lose turns to an unwanted\npep talk. Then you win.";
static const char s_str_51[] = "'Growth!' it says, falling.\n'Pure. Unfiltered. Growth!'";
static const char s_str_52[] = "'GOOD.' The Orc cracks its\nknuckles. 'Finally, a challenge.'";
static const char s_str_53[] = "You trade brutal, honest blows.\nThe Orc respects every one.";
static const char s_str_54[] = "Defeated, it nods. 'Good form.\nYour left guard. Work on it.'";
static const char s_str_55[] = "The Orc sees your staff.\n'Ha! Little stick person! Ha!'";
static const char s_str_56[] = "Your spell caves the ceiling.\nDirectly onto the Orc. Directly.";
static const char s_str_57[] = "'I see,' it says, pinned.\n'What were you FEELING there?'";
static const char s_str_58[] = "It waves from the rubble.\n'Outstanding session. Well done.'";
static const char s_str_59[] = "'You will NOT land here without\na landing permit. Form 31-L.'";
static const char s_str_60[] = "You hover. He protests. You\noutlast him on a technicality.";
static const char s_str_61[] = "The Troll raises a clipboard.\n'This is a Heritage Zone. Stop.'";
static const char s_str_62[] = "'Any combat causes structural\ndamage. Please file Form 18-T.'";
static const char s_str_63[] = "You file nothing. You fight.\nHe annotates your every move.";
static const char s_str_64[] = "Your first spell clips a column.\nThe Troll shrieks. 'IRREPLACEABLE.'";
static const char s_str_65[] = "'Eleventh century stonework.\nRemaining integrity: ZERO.'";
static const char s_str_66[] = "He documents the damage. You\nwalk past him. He doesn't notice.";
static const char s_str_67[] = "He sits filing damage reports\nin the rubble. Very thorough.";
static const char s_str_68[] = "'BEHOLD, I AM THE—' You fly\nthrough him at full speed.";
static const char s_str_69[] = "You do it again. He sags.\n'Three hundred years,' he says.";
static const char s_str_70[] = "'Three hundred years of practice.\nYou flew through me. Twice.'";
static const char s_str_71[] = "The Ghost begins his entrance.\nEerie winds. Flickering candles.";
static const char s_str_72[] = "The wail. The cold. The reveal.\nYou stare at him. Blankly.";
static const char s_str_73[] = "He stops. 'Nothing?' He checks\nhis notes. He is devastated.";
static const char s_str_74[] = "The Ghost begins his routine.\nYou raise your staff. 'Fascinating.'";
static const char s_str_75[] = "'The spectral resonance here\nis a grade-three harmonic loop.'";
static const char s_str_76[] = "He stops. Three centuries of\ncraft, peer-reviewed to death.";
static const char s_str_77[] = "He hovers, dejected.\n'I really thought the wail was good.'";
static const char s_str_78[] = "'A Wyvern! Very niche, very\ninteresting brand. I love it.'";
static const char s_str_79[] = "He tries to hand you a card.\nYou have no hands. He adapts.";
static const char s_str_80[] = "He tucks it in a wing scale.\n'Reach out. Big synergies here.'";
static const char s_str_81[] = "'Fellow swordsman!' He bows.\nA business card appears. Two cards.";
static const char s_str_82[] = "'After we're done, I'd love\nto connect. Big things ahead.'";
static const char s_str_83[] = "He fights well. He hands you\nanother card mid-fight.";
static const char s_str_84[] = "'A Mage! Excellent vertical for\nmy mage-adjacent network. Truly.'";
static const char s_str_85[] = "He pauses mid-fight to ask\nyour specialty. You blast him.";
static const char s_str_86[] = "'I'll follow up by raven.'\nHe falls, elegantly. Smiling.";
static const char s_str_87[] = "A raven arrives. It drops a\ncard. 'Great connecting today!'";
static const char s_str_88[] = "From the air: a house-sized\ndragon, lying very, very still.";
static const char s_str_89[] = "You land. 'I don't see you,'\nit says. 'I am a boulder. Leave.'";
static const char s_str_90[] = "You point out it isn't a rock.\nIts embarrassment is total.";
static const char s_str_91[] = "A boulder blocks the pass.\nYou approach. It sighs deeply.";
static const char s_str_92[] = "'I am a rock. This is a rock\narea. You are seeing nothing.'";
static const char s_str_93[] = "You fight the rock. The rock\nsighs loudly and fights back.";
static const char s_str_94[] = "A large boulder sits in your\npath. You cast Reveal. It gasps.";
static const char s_str_95[] = "'HOW DARE YOU. I was FINE.\nI was a PERFECT boulder. Fine.'";
static const char s_str_96[] = "It fights while refusing to\nacknowledge it's a dragon.";
static const char s_str_97[] = "'I am a geological feature.\nYou didn't see anything here.'";
static const char s_str_98[] = "You land near the open kiln.\nThe dragon belches. Then winces.";
static const char s_str_99[] = "'YOU! Your landing shook me!\nNow my heartburn's a full episode!'";
static const char s_str_100[] = "'I had calcium! I was FINE!\nFIGHT ME. I need to lie down.'";
static const char s_str_101[] = "The dragon is mid-glassblowing.\nIt ignores you completely.";
static const char s_str_102[] = "'ONE MOMENT. The neck on this\nvase is very, very delicate.'";
static const char s_str_103[] = "You interrupt. The vase shatters.\nThe dragon is not okay.";
static const char s_str_104[] = "You cast a fire bolt. It hits\nthe dragon's work-in-progress.";
static const char s_str_105[] = "The dragon turns very slowly.\n'That was an art piece. Mine.'";
static const char s_str_106[] = "'My fire is nuanced. Yours was\njust arson. I am appalled.'";
static const char s_str_107[] = "'That piece could have been\nbeautiful. It was almost done.'";
static const char s_str_108[] = "A sky fight at last! The dragon\nzooms past trailing rumours.";
static const char s_str_109[] = "'That Wyvern gone rogue? Is\nthat YOU? Are you that Wyvern?!'";
static const char s_str_110[] = "It stops to gossip about you.\nYou use the opening. You win.";
static const char s_str_111[] = "'Oh! Did you hear? The Demon\nLord keeps a POTTERY DIARY!'";
static const char s_str_112[] = "You ignore this. You advance.\nThe dragon is genuinely shocked.";
static const char s_str_113[] = "It keeps talking. You keep\nfighting. You win mid-sentence.";
static const char s_str_114[] = "'Oh! A Mage! I heard something\nabout you. Something JUICY.'";
static const char s_str_115[] = "You pause. '...What?' It grins.\n'Trade you. Info for info.'";
static const char s_str_116[] = "You trade. You learn his one\nweakness. Worth every secret.";
static const char s_str_117[] = "'Fine. But if you hear anything\nnew— anything— you tell me.'";
static const char s_str_118[] = "'YOU! HALT! I'm serving—' You\nfly away. The voice echoes after.";
static const char s_str_119[] = "'COME BACK. Form 4-P, cease\nand desist ALL aerial evasion—'";
static const char s_str_120[] = "You are gone. The subpoena\nlands in the swamp. Case closed.";
static const char s_str_121[] = "A hollow voice booms: 'HALT!\nYou are hereby served notice—'";
static const char s_str_122[] = "'—unauthorized use of likeness\nin all commerce. Sign here.'";
static const char s_str_123[] = "You cut through the preamble.\nAnd then through him.";
static const char s_str_124[] = "'Before we begin, I must ask:\ndid you purchase Pumpkin Spice?'";
static const char s_str_125[] = "You pause. Your breakfast was\na pumpkin spice latte. Oh no.";
static const char s_str_126[] = "The deposition takes three turns.\nYou win on a technicality.";
static const char s_str_127[] = "'This isn't over! My clerk has\nfiled a mystical appeal!'";
static const char s_str_128[] = "You dive at the Demon Lord.\nHe flinches. 'YOU! Flying youth!'";
static const char s_str_129[] = "'No respect. No gravitas at all.\nJust zooming. Entitled zooming.'";
static const char s_str_130[] = "He monologues about youth. You\nuse the opening. You press on.";
static const char s_str_131[] = "'Oh. A swordsman. Let me guess:\nprophecy? Chosen one? The sword?'";
static const char s_str_132[] = "'Eleven heroes this century.\nSame entrance. Same pose. Same.'";
static const char s_str_133[] = "You fight. He sighs the whole\ntime. His back gives out. You win.";
static const char s_str_134[] = "You begin disrupting the ritual.\nThe Demon Lord winces. 'My back.'";
static const char s_str_135[] = "'Of all the days. I have a\nwarded disc. Do you MIND?'";
static const char s_str_136[] = "You press on. He fights through\nlower back spasms. You win.";
static const char s_str_137[] = "'I yield. The pottery shop will\nbe better than this. It will.'";

static const char * const story_strings[] = {
    s_str_0,
    s_str_1,
    s_str_2,
    s_str_3,
    s_str_4,
    s_str_5,
    s_str_6,
    s_str_7,
    s_str_8,
    s_str_9,
    s_str_10,
    s_str_11,
    s_str_12,
    s_str_13,
    s_str_14,
    s_str_15,
    s_str_16,
    s_str_17,
    s_str_18,
    s_str_19,
    s_str_20,
    s_str_21,
    s_str_22,
    s_str_23,
    s_str_24,
    s_str_25,
    s_str_26,
    s_str_27,
    s_str_28,
    s_str_29,
    s_str_30,
    s_str_31,
    s_str_32,
    s_str_33,
    s_str_34,
    s_str_35,
    s_str_36,
    s_str_37,
    s_str_38,
    s_str_39,
    s_str_40,
    s_str_41,
    s_str_42,
    s_str_43,
    s_str_44,
    s_str_45,
    s_str_46,
    s_str_47,
    s_str_48,
    s_str_49,
    s_str_50,
    s_str_51,
    s_str_52,
    s_str_53,
    s_str_54,
    s_str_55,
    s_str_56,
    s_str_57,
    s_str_58,
    s_str_59,
    s_str_60,
    s_str_61,
    s_str_62,
    s_str_63,
    s_str_64,
    s_str_65,
    s_str_66,
    s_str_67,
    s_str_68,
    s_str_69,
    s_str_70,
    s_str_71,
    s_str_72,
    s_str_73,
    s_str_74,
    s_str_75,
    s_str_76,
    s_str_77,
    s_str_78,
    s_str_79,
    s_str_80,
    s_str_81,
    s_str_82,
    s_str_83,
    s_str_84,
    s_str_85,
    s_str_86,
    s_str_87,
    s_str_88,
    s_str_89,
    s_str_90,
    s_str_91,
    s_str_92,
    s_str_93,
    s_str_94,
    s_str_95,
    s_str_96,
    s_str_97,
    s_str_98,
    s_str_99,
    s_str_100,
    s_str_101,
    s_str_102,
    s_str_103,
    s_str_104,
    s_str_105,
    s_str_106,
    s_str_107,
    s_str_108,
    s_str_109,
    s_str_110,
    s_str_111,
    s_str_112,
    s_str_113,
    s_str_114,
    s_str_115,
    s_str_116,
    s_str_117,
    s_str_118,
    s_str_119,
    s_str_120,
    s_str_121,
    s_str_122,
    s_str_123,
    s_str_124,
    s_str_125,
    s_str_126,
    s_str_127,
    s_str_128,
    s_str_129,
    s_str_130,
    s_str_131,
    s_str_132,
    s_str_133,
    s_str_134,
    s_str_135,
    s_str_136,
    s_str_137,
};

// ---- Scripts ----
static const uint8_t script_WNPC_MERCHANT[] = { 0 };
static const uint8_t script_WITEM_FISH[] = { 0 };
static const uint8_t script_WENT_WEREWOLF[] = { 7, 66, 0x04, 0x00, 42, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 0, 1, 1, 6, 33, 1, 2, 1, 3, 6, 33, 1, 4, 1, 5, 1, 6, 2, 66, 0x04, 0x00, 3, 66, 0x01, 0x00, 0, 1, 7, 0 };
static const uint8_t script_WNPC_LITTLE_GIRL[] = { 0 };
static const uint8_t script_WBOSS_OGRE[] = { 7, 112, 0x04, 0x00, 40, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 8, 1, 9, 6, 31, 1, 10, 1, 11, 6, 31, 1, 12, 1, 13, 2, 112, 0x04, 0x00, 3, 112, 0x01, 0x00, 0, 1, 14, 0 };
static const uint8_t script_WNPC_OLD_MAN[] = { 0 };
static const uint8_t script_WENT_SILVER_KNIGHT[] = { 7, 68, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 15, 1, 16, 1, 17, 6, 37, 1, 18, 1, 19, 1, 20, 6, 37, 1, 21, 1, 22, 1, 23, 2, 68, 0x04, 0x00, 3, 68, 0x01, 0x00, 0, 1, 24, 0 };
static const uint8_t script_WNPC_KING[] = { 7, 68, 0x04, 0x00, 8, 1, 25, 0, 7, 41, 0x20, 0x00, 24, 1, 26, 1, 27, 1, 28, 2, 41, 0x20, 0x00, 0, 1, 29, 0 };
static const uint8_t script_WENT_SKELETON[] = { 7, 67, 0x04, 0x00, 42, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 29, 1, 30, 1, 31, 6, 33, 1, 32, 1, 33, 1, 34, 6, 33, 1, 35, 1, 36, 2, 67, 0x04, 0x00, 3, 67, 0x01, 0x00, 0, 1, 37, 0 };
static const uint8_t script_WNPC_CARPENTER[] = { 0 };
static const uint8_t script_WENT_GENIE[] = { 7, 72, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 25, 7, 32, 0x80, 0x00, 31, 1, 38, 1, 39, 1, 40, 1, 41, 6, 37, 1, 42, 1, 43, 6, 37, 1, 44, 1, 45, 1, 46, 2, 72, 0x04, 0x00, 3, 72, 0x01, 0x00, 0, 1, 47, 0 };
static const uint8_t script_WNPC_MOTHER[] = { 0 };
static const uint8_t script_WENT_ORC[] = { 7, 65, 0x04, 0x00, 48, 7, 32, 0x40, 0x00, 25, 7, 32, 0x80, 0x00, 33, 1, 48, 1, 49, 1, 50, 1, 51, 6, 39, 1, 52, 1, 53, 1, 54, 6, 39, 1, 55, 1, 56, 1, 57, 2, 65, 0x04, 0x00, 3, 65, 0x01, 0x00, 0, 1, 58, 0 };
static const uint8_t script_WNPC_FATHER[] = { 0 };
static const uint8_t script_WENT_TROLL[] = { 7, 71, 0x04, 0x00, 44, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 29, 1, 59, 1, 60, 6, 35, 1, 61, 1, 62, 1, 63, 6, 35, 1, 64, 1, 65, 1, 66, 2, 71, 0x04, 0x00, 3, 71, 0x01, 0x00, 0, 1, 67, 0 };
static const uint8_t script_WENT_GHOST[] = { 7, 70, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 68, 1, 69, 1, 70, 6, 37, 1, 71, 1, 72, 1, 73, 6, 37, 1, 74, 1, 75, 1, 76, 2, 70, 0x04, 0x00, 3, 70, 0x01, 0x00, 0, 1, 77, 0 };
static const uint8_t script_WENT_DARK_KNIGHT[] = { 7, 69, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 78, 1, 79, 1, 80, 6, 37, 1, 81, 1, 82, 1, 83, 6, 37, 1, 84, 1, 85, 1, 86, 2, 69, 0x04, 0x00, 3, 69, 0x01, 0x00, 0, 1, 87, 0 };
static const uint8_t script_WBOSS_LAND_DRAGON[] = { 7, 113, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 88, 1, 89, 1, 90, 6, 37, 1, 91, 1, 92, 1, 93, 6, 37, 1, 94, 1, 95, 1, 96, 2, 113, 0x04, 0x00, 3, 113, 0x01, 0x00, 0, 1, 97, 0 };
static const uint8_t script_WBOSS_FIRE_DRAGON[] = { 7, 115, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 98, 1, 99, 1, 100, 6, 37, 1, 101, 1, 102, 1, 103, 6, 37, 1, 104, 1, 105, 1, 106, 2, 115, 0x04, 0x00, 3, 115, 0x01, 0x00, 0, 1, 107, 0 };
static const uint8_t script_WBOSS_WIND_DRAGON[] = { 7, 114, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 108, 1, 109, 1, 110, 6, 37, 1, 111, 1, 112, 1, 113, 6, 37, 1, 114, 1, 115, 1, 116, 2, 114, 0x04, 0x00, 3, 114, 0x01, 0x00, 0, 1, 117, 0 };
static const uint8_t script_WBOSS_PUMPKIN_HEAD[] = { 7, 116, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 118, 1, 119, 1, 120, 6, 37, 1, 121, 1, 122, 1, 123, 6, 37, 1, 124, 1, 125, 1, 126, 2, 116, 0x04, 0x00, 3, 116, 0x01, 0x00, 0, 1, 127, 0 };
static const uint8_t script_WDEMONLORD[] = { 7, 127, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 128, 1, 129, 1, 130, 6, 37, 1, 131, 1, 132, 1, 133, 6, 37, 1, 134, 1, 135, 1, 136, 2, 127, 0x04, 0x00, 3, 127, 0x01, 0x00, 0, 1, 137, 0 };

// ---- Scene Mapping ----
#endif // SCRIPT_H
