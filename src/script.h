#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdint.h>

// ---- Strings ----
static const char s_str_0[] = "Welcome! Have you\njoined Gold-Tier yet?";
static const char s_str_1[] = "Only 9,999 points\nfor the Hero Bundle!";
static const char s_str_2[] = "The invasion? Minor\nsupply disruption. Fine.";
static const char s_str_3[] = "Your loyalty points\ndo not expire. Mostly.";
static const char s_str_4[] = "You circle from above. The wolf\nspots you. Its tail starts wagging.";
static const char s_str_5[] = "You dive. It leaps with pure joy.\nIt yelps. You win. It looks sad.";
static const char s_str_6[] = "You advance. The wolf snarls...\nthen its tail starts wagging.";
static const char s_str_7[] = "It trips over itself lunging.\nYou win. It wags the whole time.";
static const char s_str_8[] = "You cast a calming aura.\nThe wolf goes very, very still.";
static const char s_str_9[] = "It tilts its head. It whines.\nAre those eyes of hope? Oh no.";
static const char s_str_10[] = "It follows you. You shoo it.\nEventually you win, sort of.";
static const char s_str_11[] = "It wags hopefully at you.\nStill waiting for that stick.";
static const char s_str_12[] = "Air drop. Sound plan.\nI mapped it earlier.";
static const char s_str_13[] = "Crude. Effective.\nI suppose that works.";
static const char s_str_14[] = "Passable. I'd have\ntargeted the joints.";
static const char s_str_15[] = "Your performance was\nadequate. Barely.";
static const char s_str_16[] = "I've catalogued your\nfaults. Twelve so far.";
static const char s_str_17[] = "Status unchanged.\nStill watching. Always.";
static const char s_str_18[] = "You dive. The Ogre's stew goes\nflying. He stares in horror.";
static const char s_str_19[] = "'My reduction! Three hours work!\nYou ANIMAL!' He charges.";
static const char s_str_20[] = "You draw your sword. The Ogre\nsniffs. 'Adventurer steak!'";
static const char s_str_21[] = "He fights and complains about\nyour 'lack of marinade.'";
static const char s_str_22[] = "You cast fire. The Ogre stops.\n'That sear! Give me the recipe!'";
static const char s_str_23[] = "He chases you for cooking tips.\nYou win. He's devastated.";
static const char s_str_24[] = "He sulks over a cold cauldron.\n'No one appreciates the craft.'";
static const char s_str_25[] = "A dragon! Wait... er.\nA friendly one? Hello.";
static const char s_str_26[] = "Here. This... powder.\nProbably safe. Thanks.";
static const char s_str_27[] = "My hero! Here, take\nthis. It's, uh... hmm.";
static const char s_str_28[] = "Swamp Gas (Strong).\nClose enough. Thanks.";
static const char s_str_29[] = "A wizard! Here —\na vial! Take it. Uh.";
static const char s_str_30[] = "Potion of... uh...\nI forget. Still good.";
static const char s_str_31[] = "Here, take this vial.\nPotion of Fog. Mild.";
static const char s_str_32[] = "I think. It may be\nPickle Brine (Strong).";
static const char s_str_33[] = "Found another! Here.\nBaldness Cure (Dense).";
static const char s_str_34[] = "'I challenge you to— you're\nflying. That is not permitted.'";
static const char s_str_35[] = "'Land immediately. Duelling\nrules forbid airborne opponents.'";
static const char s_str_36[] = "He refuses to engage. You wait\nhim out. He sulks. You pass.";
static const char s_str_37[] = "He bows stiffly. 'I am\noverqualified for guarding a door.'";
static const char s_str_38[] = "'Top of my class. Three dragons\nbefore breakfast. This is that.'";
static const char s_str_39[] = "He fights brilliantly while\nbeing furious about the posting.";
static const char s_str_40[] = "'Magic?' He scoffs. 'A knight\nfights with steel. Not tricks.'";
static const char s_str_41[] = "Your spell hits him squarely.\nHe stands in dignified silence.";
static const char s_str_42[] = "'I will not dignify that\nwith a reaction.' He falls.";
static const char s_str_43[] = "He stands aside, impeccably.\nStill visibly furious about it.";
static const char s_str_44[] = "'Mm? Oh yes, the demon thing.\nDo deal with Gerald first.'";
static const char s_str_45[] = "'Oh, you beat Gerald! Lovely.\nNow. Look at this diorama.'";
static const char s_str_46[] = "'Siege of Ashenvale, 1204.\nNote the pauldron detail. Note it.'";
static const char s_str_47[] = "'You may pass. I am also\nbestowing: Diorama Appreciator.'";
static const char s_str_48[] = "'Still here? Good. Have you\nconsidered this composition?'";
static const char s_str_49[] = "'Three weeks. Just\nthe rust. Count each rivet.'";
static const char s_str_50[] = "The skeletons are mid-argument\nabout whose femur is whose.";
static const char s_str_51[] = "You dart through while they\nsort out the bone situation.";
static const char s_str_52[] = "A skeleton lurches forward,\nwaving its sword arm menacingly.";
static const char s_str_53[] = "Its other arm falls off.\n'That's mine,' it says. It stops.";
static const char s_str_54[] = "The fight pauses so it can\nretrieve its limbs. You win.";
static const char s_str_55[] = "You cast a light spell. The\nskeleton stops. 'Is that Luminara?'";
static const char s_str_56[] = "It leans in, asks questions.\nYou defeat it mid-lecture.";
static const char s_str_57[] = "A single arm gives a thumbs up\nfrom the corner. Not its arm.";
static const char s_str_58[] = "Out! Those rafters —\nCould you hold one up?";
static const char s_str_59[] = "Rescued! Good. Now,\ncan swords chip stone?";
static const char s_str_60[] = "Free! Now — that crack.\nBlast it. Just once.";
static const char s_str_61[] = "I built a second door.\nTo the well. Safety.";
static const char s_str_62[] = "New project: a shelf\ninside the drawbridge.";
static const char s_str_63[] = "'You are FLYING INDOORS,' the\nGenie declares. 'Not permitted.'";
static const char s_str_64[] = "'This requires Permit 22-C.\nDo you have a Permit 22-C?'";
static const char s_str_65[] = "You do not. The fight is now\nofficially about the permit.";
static const char s_str_66[] = "You find a loophole. He is\ndeeply, professionally furious.";
static const char s_str_67[] = "'Oh! A physical threat! Lovely!\nI'll need you to file Form 7-B.'";
static const char s_str_68[] = "No Form 7-B exists. You fight\nhim anyway. He seems relieved.";
static const char s_str_69[] = "'All spells require advance\nfiling in triplicate. Page six.'";
static const char s_str_70[] = "You cite Sub-Clause 9-C of\nthe Mystical Conduct Register.";
static const char s_str_71[] = "The clause doesn't exist.\nHe checks anyway. You win.";
static const char s_str_72[] = "He broods over his paperwork.\n'Loopholes everywhere. Everywhere.'";
static const char s_str_73[] = "Oh, stop. *heals*\nYou're all dramatic.";
static const char s_str_74[] = "I once flew injured.\nIn winter. Nobody saw.";
static const char s_str_75[] = "Hold still. *heals*\nPatently a scratch.";
static const char s_str_76[] = "I bled more last\nTuesday. Without help.";
static const char s_str_77[] = "Sit. *heals* You're\nquite pale, actually.";
static const char s_str_78[] = "I gave birth in\na blizzard. Twice.";
static const char s_str_79[] = "Sit down. *heals*\nYou're being dramatic.";
static const char s_str_80[] = "Last month: plague.\nAnd a splinter. Alone.";
static const char s_str_81[] = "Again? *sighs* Some\nof us never rest.";
static const char s_str_82[] = "The cave ceiling is very low.\nYou scramble. The Orc watches.";
static const char s_str_83[] = "'You look uncomfortable. That\nis valid data. Let's unpack it.'";
static const char s_str_84[] = "You lose turns to an unwanted\npep talk. Then you win.";
static const char s_str_85[] = "'Growth!' it says, falling.\n'Pure. Unfiltered. Growth!'";
static const char s_str_86[] = "'GOOD.' The Orc cracks its\nknuckles. 'Finally, a challenge.'";
static const char s_str_87[] = "You trade brutal, honest blows.\nThe Orc respects every one.";
static const char s_str_88[] = "Defeated, it nods. 'Good form.\nYour left guard. Work on it.'";
static const char s_str_89[] = "The Orc sees your staff.\n'Ha! Little stick person! Ha!'";
static const char s_str_90[] = "Your spell caves the ceiling.\nDirectly onto the Orc. Directly.";
static const char s_str_91[] = "'I see,' it says, pinned.\n'What were you FEELING there?'";
static const char s_str_92[] = "It waves from the rubble.\n'Outstanding session. Well done.'";
static const char s_str_93[] = "At that altitude?!\nYou could get a cold!";
static const char s_str_94[] = "Promise me you'll\nwear a warm scarf up.";
static const char s_str_95[] = "Oh, thank goodness.\nDid you stretch first?";
static const char s_str_96[] = "Promise me you'll\nbe home before eight.";
static const char s_str_97[] = "You're safe! Are\nthose spell burns bad?";
static const char s_str_98[] = "Promise me you'll\nwear a fireproof scarf.";
static const char s_str_99[] = "The sword. *sharpens*\nPromise me: eat first.";
static const char s_str_100[] = "And the scarf. And\nhome before eight.";
static const char s_str_101[] = "*sharpens* The scarf?\nSatisfactory. Barely.";
static const char s_str_102[] = "'You will NOT land here without\na landing permit. Form 31-L.'";
static const char s_str_103[] = "You hover. He protests. You\noutlast him on a technicality.";
static const char s_str_104[] = "The Troll raises a clipboard.\n'This is a Heritage Zone. Stop.'";
static const char s_str_105[] = "'Any combat causes structural\ndamage. Please file Form 18-T.'";
static const char s_str_106[] = "You file nothing. You fight.\nHe annotates your every move.";
static const char s_str_107[] = "Your first spell clips a column.\nThe Troll shrieks. 'IRREPLACEABLE.'";
static const char s_str_108[] = "'Eleventh century stonework.\nRemaining integrity: ZERO.'";
static const char s_str_109[] = "He documents the damage. You\nwalk past him. He doesn't notice.";
static const char s_str_110[] = "He sits filing damage reports\nin the rubble. Very thorough.";
static const char s_str_111[] = "'BEHOLD, I AM THE—' You fly\nthrough him at full speed.";
static const char s_str_112[] = "You do it again. He sags.\n'Three hundred years,' he says.";
static const char s_str_113[] = "'Three hundred years of practice.\nYou flew through me. Twice.'";
static const char s_str_114[] = "The Ghost begins his entrance.\nEerie winds. Flickering candles.";
static const char s_str_115[] = "The wail. The cold. The reveal.\nYou stare at him. Blankly.";
static const char s_str_116[] = "He stops. 'Nothing?' He checks\nhis notes. He is devastated.";
static const char s_str_117[] = "The Ghost begins his routine.\nYou raise your staff. 'Fascinating.'";
static const char s_str_118[] = "'The spectral resonance here\nis a grade-three harmonic loop.'";
static const char s_str_119[] = "He stops. Three centuries of\ncraft, peer-reviewed to death.";
static const char s_str_120[] = "He hovers, dejected.\n'I really thought the wail was good.'";
static const char s_str_121[] = "'A Wyvern! Very niche, very\ninteresting brand. I love it.'";
static const char s_str_122[] = "He tries to hand you a card.\nYou have no hands. He adapts.";
static const char s_str_123[] = "He tucks it in a wing scale.\n'Reach out. Big synergies here.'";
static const char s_str_124[] = "'Fellow swordsman!' He bows.\nA business card appears. Two cards.";
static const char s_str_125[] = "'After we're done, I'd love\nto connect. Big things ahead.'";
static const char s_str_126[] = "He fights well. He hands you\nanother card mid-fight.";
static const char s_str_127[] = "'A Mage! Excellent vertical for\nmy mage-adjacent network. Truly.'";
static const char s_str_128[] = "He pauses mid-fight to ask\nyour specialty. You blast him.";
static const char s_str_129[] = "'I'll follow up by raven.'\nHe falls, elegantly. Smiling.";
static const char s_str_130[] = "A raven arrives. It drops a\ncard. 'Great connecting today!'";
static const char s_str_131[] = "From the air: a house-sized\ndragon, lying very, very still.";
static const char s_str_132[] = "You land. 'I don't see you,'\nit says. 'I am a boulder. Leave.'";
static const char s_str_133[] = "You point out it isn't a rock.\nIts embarrassment is total.";
static const char s_str_134[] = "A boulder blocks the pass.\nYou approach. It sighs deeply.";
static const char s_str_135[] = "'I am a rock. This is a rock\narea. You are seeing nothing.'";
static const char s_str_136[] = "You fight the rock. The rock\nsighs loudly and fights back.";
static const char s_str_137[] = "A large boulder sits in your\npath. You cast Reveal. It gasps.";
static const char s_str_138[] = "'HOW DARE YOU. I was FINE.\nI was a PERFECT boulder. Fine.'";
static const char s_str_139[] = "It fights while refusing to\nacknowledge it's a dragon.";
static const char s_str_140[] = "'I am a geological feature.\nYou didn't see anything here.'";
static const char s_str_141[] = "You land near the open kiln.\nThe dragon belches. Then winces.";
static const char s_str_142[] = "'YOU! Your landing shook me!\nNow my heartburn's a full episode!'";
static const char s_str_143[] = "'I had calcium! I was FINE!\nFIGHT ME. I need to lie down.'";
static const char s_str_144[] = "The dragon is mid-glassblowing.\nIt ignores you completely.";
static const char s_str_145[] = "'ONE MOMENT. The neck on this\nvase is very, very delicate.'";
static const char s_str_146[] = "You interrupt. The vase shatters.\nThe dragon is not okay.";
static const char s_str_147[] = "You cast a fire bolt. It hits\nthe dragon's work-in-progress.";
static const char s_str_148[] = "The dragon turns very slowly.\n'That was an art piece. Mine.'";
static const char s_str_149[] = "'My fire is nuanced. Yours was\njust arson. I am appalled.'";
static const char s_str_150[] = "'That piece could have been\nbeautiful. It was almost done.'";
static const char s_str_151[] = "A sky fight at last! The dragon\nzooms past trailing rumours.";
static const char s_str_152[] = "'That Wyvern gone rogue? Is\nthat YOU? Are you that Wyvern?!'";
static const char s_str_153[] = "It stops to gossip about you.\nYou use the opening. You win.";
static const char s_str_154[] = "'Oh! Did you hear? The Demon\nLord keeps a POTTERY DIARY!'";
static const char s_str_155[] = "You ignore this. You advance.\nThe dragon is genuinely shocked.";
static const char s_str_156[] = "It keeps talking. You keep\nfighting. You win mid-sentence.";
static const char s_str_157[] = "'Oh! A Mage! I heard something\nabout you. Something JUICY.'";
static const char s_str_158[] = "You pause. '...What?' It grins.\n'Trade you. Info for info.'";
static const char s_str_159[] = "You trade. You learn his one\nweakness. Worth every secret.";
static const char s_str_160[] = "'Fine. But if you hear anything\nnew— anything— you tell me.'";
static const char s_str_161[] = "'YOU! HALT! I'm serving—' You\nfly away. The voice echoes after.";
static const char s_str_162[] = "'COME BACK. Form 4-P, cease\nand desist ALL aerial evasion—'";
static const char s_str_163[] = "You are gone. The subpoena\nlands in the swamp. Case closed.";
static const char s_str_164[] = "A hollow voice booms: 'HALT!\nYou are hereby served notice—'";
static const char s_str_165[] = "'—unauthorized use of likeness\nin all commerce. Sign here.'";
static const char s_str_166[] = "You cut through the preamble.\nAnd then through him.";
static const char s_str_167[] = "'Before we begin, I must ask:\ndid you purchase Pumpkin Spice?'";
static const char s_str_168[] = "You pause. Your breakfast was\na pumpkin spice latte. Oh no.";
static const char s_str_169[] = "The deposition takes three turns.\nYou win on a technicality.";
static const char s_str_170[] = "'This isn't over! My clerk has\nfiled a mystical appeal!'";
static const char s_str_171[] = "You dive at the Demon Lord.\nHe flinches. 'YOU! Flying youth!'";
static const char s_str_172[] = "'No respect. No gravitas at all.\nJust zooming. Entitled zooming.'";
static const char s_str_173[] = "He monologues about youth. You\nuse the opening. You press on.";
static const char s_str_174[] = "'Oh. A swordsman. Let me guess:\nprophecy? Chosen one? The sword?'";
static const char s_str_175[] = "'Eleven heroes this century.\nSame entrance. Same pose. Same.'";
static const char s_str_176[] = "You fight. He sighs the whole\ntime. His back gives out. You win.";
static const char s_str_177[] = "You begin disrupting the ritual.\nThe Demon Lord winces. 'My back.'";
static const char s_str_178[] = "'Of all the days. I have a\nwarded disc. Do you MIND?'";
static const char s_str_179[] = "You press on. He fights through\nlower back spasms. You win.";
static const char s_str_180[] = "'I yield. The pottery shop will\nbe better than this. It will.'";

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
    s_str_138,
    s_str_139,
    s_str_140,
    s_str_141,
    s_str_142,
    s_str_143,
    s_str_144,
    s_str_145,
    s_str_146,
    s_str_147,
    s_str_148,
    s_str_149,
    s_str_150,
    s_str_151,
    s_str_152,
    s_str_153,
    s_str_154,
    s_str_155,
    s_str_156,
    s_str_157,
    s_str_158,
    s_str_159,
    s_str_160,
    s_str_161,
    s_str_162,
    s_str_163,
    s_str_164,
    s_str_165,
    s_str_166,
    s_str_167,
    s_str_168,
    s_str_169,
    s_str_170,
    s_str_171,
    s_str_172,
    s_str_173,
    s_str_174,
    s_str_175,
    s_str_176,
    s_str_177,
    s_str_178,
    s_str_179,
    s_str_180,
};

// ---- Scripts ----
static const uint8_t script_WNPC_MERCHANT[] = { 7, 42, 0x20, 0x00, 14, 1, 0, 1, 1, 2, 42, 0x20, 0x00, 0, 1, 2, 1, 3, 0 };
static const uint8_t script_WITEM_FISH[] = { 0 };
static const uint8_t script_WENT_WEREWOLF[] = { 7, 66, 0x04, 0x00, 42, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 4, 1, 5, 6, 33, 1, 6, 1, 7, 6, 33, 1, 8, 1, 9, 1, 10, 2, 66, 0x04, 0x00, 3, 66, 0x01, 0x00, 0, 1, 11, 0 };
static const uint8_t script_WNPC_LITTLE_GIRL[] = { 7, 36, 0x08, 0x00, 37, 7, 32, 0x40, 0x00, 19, 7, 32, 0x80, 0x00, 23, 1, 12, 6, 25, 1, 13, 6, 25, 1, 14, 2, 36, 0x08, 0x00, 3, 36, 0x10, 0x00, 4, 36, 0, 0, 7, 36, 0x20, 0x00, 51, 1, 15, 1, 16, 2, 36, 0x20, 0x00, 0, 1, 17, 0 };
static const uint8_t script_WBOSS_OGRE[] = { 7, 112, 0x04, 0x00, 40, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 18, 1, 19, 6, 31, 1, 20, 1, 21, 6, 31, 1, 22, 1, 23, 2, 112, 0x04, 0x00, 3, 112, 0x01, 0x00, 0, 1, 24, 0 };
static const uint8_t script_WNPC_OLD_MAN[] = { 7, 40, 0x08, 0x00, 43, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 25, 1, 26, 6, 31, 1, 27, 1, 28, 6, 31, 1, 29, 1, 30, 2, 40, 0x08, 0x00, 3, 40, 0x10, 0x00, 4, 40, 0, 0, 7, 40, 0x20, 0x00, 57, 1, 31, 1, 32, 2, 40, 0x20, 0x00, 0, 1, 33, 0 };
static const uint8_t script_WENT_SILVER_KNIGHT[] = { 7, 68, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 34, 1, 35, 1, 36, 6, 37, 1, 37, 1, 38, 1, 39, 6, 37, 1, 40, 1, 41, 1, 42, 2, 68, 0x04, 0x00, 3, 68, 0x01, 0x00, 0, 1, 43, 0 };
static const uint8_t script_WNPC_KING[] = { 7, 68, 0x04, 0x00, 8, 1, 44, 0, 7, 41, 0x20, 0x00, 24, 1, 45, 1, 46, 1, 47, 2, 41, 0x20, 0x00, 0, 1, 48, 1, 49, 0 };
static const uint8_t script_WENT_SKELETON[] = { 7, 67, 0x04, 0x00, 42, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 29, 1, 50, 1, 51, 6, 33, 1, 52, 1, 53, 1, 54, 6, 33, 1, 55, 1, 56, 2, 67, 0x04, 0x00, 3, 67, 0x01, 0x00, 0, 1, 57, 0 };
static const uint8_t script_WNPC_CARPENTER[] = { 7, 39, 0x08, 0x00, 37, 7, 32, 0x40, 0x00, 19, 7, 32, 0x80, 0x00, 23, 1, 58, 6, 25, 1, 59, 6, 25, 1, 60, 2, 39, 0x08, 0x00, 3, 39, 0x10, 0x00, 4, 39, 0, 0, 7, 39, 0x20, 0x00, 49, 1, 61, 2, 39, 0x20, 0x00, 0, 1, 62, 0 };
static const uint8_t script_WENT_GENIE[] = { 7, 72, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 25, 7, 32, 0x80, 0x00, 31, 1, 63, 1, 64, 1, 65, 1, 66, 6, 37, 1, 67, 1, 68, 6, 37, 1, 69, 1, 70, 1, 71, 2, 72, 0x04, 0x00, 3, 72, 0x01, 0x00, 0, 1, 72, 0 };
static const uint8_t script_WNPC_MOTHER[] = { 7, 37, 0x08, 0x00, 43, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 73, 1, 74, 6, 31, 1, 75, 1, 76, 6, 31, 1, 77, 1, 78, 2, 37, 0x08, 0x00, 3, 37, 0x10, 0x00, 4, 37, 0, 0, 7, 37, 0x20, 0x00, 57, 1, 79, 1, 80, 2, 37, 0x20, 0x00, 0, 1, 81, 0 };
static const uint8_t script_WENT_ORC[] = { 7, 65, 0x04, 0x00, 48, 7, 32, 0x40, 0x00, 25, 7, 32, 0x80, 0x00, 33, 1, 82, 1, 83, 1, 84, 1, 85, 6, 39, 1, 86, 1, 87, 1, 88, 6, 39, 1, 89, 1, 90, 1, 91, 2, 65, 0x04, 0x00, 3, 65, 0x01, 0x00, 0, 1, 92, 0 };
static const uint8_t script_WNPC_FATHER[] = { 7, 38, 0x08, 0x00, 43, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 27, 1, 93, 1, 94, 6, 31, 1, 95, 1, 96, 6, 31, 1, 97, 1, 98, 2, 38, 0x08, 0x00, 3, 38, 0x10, 0x00, 4, 38, 0, 0, 7, 38, 0x20, 0x00, 57, 1, 99, 1, 100, 2, 38, 0x20, 0x00, 0, 1, 101, 0 };
static const uint8_t script_WENT_TROLL[] = { 7, 71, 0x04, 0x00, 44, 7, 32, 0x40, 0x00, 21, 7, 32, 0x80, 0x00, 29, 1, 102, 1, 103, 6, 35, 1, 104, 1, 105, 1, 106, 6, 35, 1, 107, 1, 108, 1, 109, 2, 71, 0x04, 0x00, 3, 71, 0x01, 0x00, 0, 1, 110, 0 };
static const uint8_t script_WENT_GHOST[] = { 7, 70, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 111, 1, 112, 1, 113, 6, 37, 1, 114, 1, 115, 1, 116, 6, 37, 1, 117, 1, 118, 1, 119, 2, 70, 0x04, 0x00, 3, 70, 0x01, 0x00, 0, 1, 120, 0 };
static const uint8_t script_WENT_DARK_KNIGHT[] = { 7, 69, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 121, 1, 122, 1, 123, 6, 37, 1, 124, 1, 125, 1, 126, 6, 37, 1, 127, 1, 128, 1, 129, 2, 69, 0x04, 0x00, 3, 69, 0x01, 0x00, 0, 1, 130, 0 };
static const uint8_t script_WBOSS_LAND_DRAGON[] = { 7, 113, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 131, 1, 132, 1, 133, 6, 37, 1, 134, 1, 135, 1, 136, 6, 37, 1, 137, 1, 138, 1, 139, 2, 113, 0x04, 0x00, 3, 113, 0x01, 0x00, 0, 1, 140, 0 };
static const uint8_t script_WBOSS_FIRE_DRAGON[] = { 7, 115, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 141, 1, 142, 1, 143, 6, 37, 1, 144, 1, 145, 1, 146, 6, 37, 1, 147, 1, 148, 1, 149, 2, 115, 0x04, 0x00, 3, 115, 0x01, 0x00, 0, 1, 150, 0 };
static const uint8_t script_WBOSS_WIND_DRAGON[] = { 7, 114, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 151, 1, 152, 1, 153, 6, 37, 1, 154, 1, 155, 1, 156, 6, 37, 1, 157, 1, 158, 1, 159, 2, 114, 0x04, 0x00, 3, 114, 0x01, 0x00, 0, 1, 160, 0 };
static const uint8_t script_WBOSS_PUMPKIN_HEAD[] = { 7, 116, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 161, 1, 162, 1, 163, 6, 37, 1, 164, 1, 165, 1, 166, 6, 37, 1, 167, 1, 168, 1, 169, 2, 116, 0x04, 0x00, 3, 116, 0x01, 0x00, 0, 1, 170, 0 };
static const uint8_t script_WDEMONLORD[] = { 7, 127, 0x04, 0x00, 46, 7, 32, 0x40, 0x00, 23, 7, 32, 0x80, 0x00, 31, 1, 171, 1, 172, 1, 173, 6, 37, 1, 174, 1, 175, 1, 176, 6, 37, 1, 177, 1, 178, 1, 179, 2, 127, 0x04, 0x00, 3, 127, 0x01, 0x00, 0, 1, 180, 0 };

// ---- Scene Mapping ----
#endif // SCRIPT_H
