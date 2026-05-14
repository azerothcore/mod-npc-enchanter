/*

# Enchanter NPC #

_This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))_

### Data ###
------------------------------------------------------------------------------------------------------------------
- Type: NPC (ID: 601015)
- Script: npc_enchantment
- Config: Yes
- SQL: No


### Version ###
------------------------------------------------------------------------------------------------------------------
- v2019.04.15 - Ported to AC by gtao725 (https://github.com/gtao725/)
- v2019.02.21 - Add AI/Phrases/Emotes, Update Menu
- v2018.12.05 - Fix broken menu. Replace 'Enchant Weapon' function. Add creature AI and creature text.
- v2018.12.01 - Update function, Add icons, Fix typos, Add a little personality (Emotes don't always work)
- v2017.08.08 - Release


### CREDITS
------------------------------------------------------------------------------------------------------------------
![Styx](https://stygianthebest.github.io/assets/img/avatar/avatar-128.jpg "Styx")
![StygianCore](https://stygianthebest.github.io/assets/img/projects/stygiancore/StygianCore.png "StygianCore")

##### This module was created for [StygianCore](https://rebrand.ly/stygiancoreproject). A World of Warcraft 3.3.5a Solo/LAN repack by StygianTheBest | [GitHub](https://rebrand.ly/stygiangithub) | [Website](https://rebrand.ly/stygianthebest))

#### Additional Credits

- [Blizzard Entertainment](http://blizzard.com)
- [TrinityCore](https://github.com/TrinityCore/TrinityCore/blob/3.3.5/THANKS)
- [SunwellCore](http://www.azerothcore.org/pages/sunwell.pl/)
- [AzerothCore](https://github.com/AzerothCore/azerothcore-wotlk/graphs/contributors)
- [OregonCore](https://wiki.oregon-core.net/)
- [Wowhead.com](http://wowhead.com)
- [OwnedCore](http://ownedcore.com/)
- [ModCraft.io](http://modcraft.io/)
- [MMO Society](https://www.mmo-society.com/)
- [AoWoW](https://wotlk.evowow.com/)
- [More credits are cited in the sources](https://github.com/StygianTheBest)

### LICENSE
------------------------------------------------------------------------------------------------------------------
This code and content is released under the [GNU AGPL v3](https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3).

*/

#include "Cell.h"
#include "CellImpl.h"
#include "Chat.h"
#include "CombatAI.h"
#include "Configuration/Config.h"
#include "DBCStores.h"
#include "DBCStructure.h"
#include "GameEventMgr.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "GameObject.h"
#include "InstanceScript.h"
#include "ObjectMgr.h"
#include "PassiveAI.h"
#include "Player.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "SpellAuraEffects.h"
#include "Unit.h"

enum Enchants
{
    ENCHANT_WEP_BERSERKING = 3789,
    ENCHANT_WEP_BLADE_WARD = 3869,
    ENCHANT_WEP_BLOOD_DRAINING = 3870,
    ENCHANT_WEP_ACCURACY = 3788,
    ENCHANT_WEP_AGILITY_1H = 1103,
    ENCHANT_WEP_SPIRIT = 3844,
    ENCHANT_WEP_BATTLEMASTER = 2675,
    ENCHANT_WEP_BLACK_MAGIC = 3790,
    ENCHANT_WEP_ICEBREAKER = 3239,
    ENCHANT_WEP_LIFEWARD = 3241,
    ENCHANT_WEP_MIGHTY_SPELL_POWER = 3834, // One-hand
    ENCHANT_WEP_EXECUTIONER = 3225,
    ENCHANT_WEP_POTENCY = 3833,
    ENCHANT_WEP_TITANGUARD = 3851,
    ENCHANT_2WEP_MASSACRE = 3827,
    ENCHANT_2WEP_SCOURGEBANE = 3247,
    ENCHANT_2WEP_GIANT_SLAYER = 3251,
    ENCHANT_2WEP_GREATER_SPELL_POWER = 3854,
    ENCHANT_2WEP_AGILITY = 2670,
    ENCHANT_2WEP_MONGOOSE = 2673,

    ENCHANT_SHIELD_DEFENSE = 1952,
    ENCHANT_SHIELD_INTELLECT = 1128,
    ENCHANT_SHIELD_RESILIENCE = 3229,
    ENCHANT_SHIELD_BLOCK = 2655,
    ENCHANT_SHIELD_STAMINA = 1071,
    ENCHANT_SHIELD_TOUGHSHIELD = 2653,
    ENCHANT_SHIELD_TITANIUM_PLATING = 3849,

    ENCHANT_HEAD_BLISSFUL_MENDING = 3819,
    ENCHANT_HEAD_BURNING_MYSTERIES = 3820,
    ENCHANT_HEAD_DOMINANCE = 3796,
    ENCHANT_HEAD_SAVAGE_GLADIATOR = 3842,
    ENCHANT_HEAD_STALWART_PROTECTOR = 3818,
    ENCHANT_HEAD_TORMENT = 3817,
    ENCHANT_HEAD_TRIUMPH = 3795,
    ENCHANT_HEAD_ECLIPSED_MOON = 3815,
    ENCHANT_HEAD_FLAME_SOUL = 3816,
    ENCHANT_HEAD_FLEEING_SHADOW = 3814,
    ENCHANT_HEAD_FROSTY_SOUL = 3812,
    ENCHANT_HEAD_TOXIC_WARDING = 3813,

    ENCHANT_SHOULDER_MASTERS_AXE = 3835,
    ENCHANT_SHOULDER_MASTERS_CRAG = 3836,
    ENCHANT_SHOULDER_MASTERS_PINNACLE = 3837,
    ENCHANT_SHOULDER_MASTERS_STORM = 3838,
    ENCHANT_SHOULDER_GREATER_AXE = 3808,
    ENCHANT_SHOULDER_GREATER_CRAG = 3809,
    ENCHANT_SHOULDER_GREATER_GLADIATOR = 3852,
    ENCHANT_SHOULDER_GREATER_PINNACLE = 3811,
    ENCHANT_SHOULDER_GREATER_STORM = 3810,
    ENCHANT_SHOULDER_DOMINANCE = 3794,
    ENCHANT_SHOULDER_TRIUMPH = 3793,

    ENCHANT_CLOAK_DARKGLOW_EMBROIDERY = 3728,
    ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730,
    ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722,
    ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE = 3859,
    ENCHANT_CLOAK_WISDOM = 3296,
    ENCHANT_CLOAK_TITANWEAVE = 1951,
    ENCHANT_CLOAK_SPELL_PIERCING = 3243,
    ENCHANT_CLOAK_SHADOW_ARMOR = 3256,
    ENCHANT_CLOAK_MIGHTY_ARMOR = 3294,
    ENCHANT_CLOAK_MAJOR_AGILITY = 1099,
    ENCHANT_CLOAK_GREATER_SPEED = 3831,

    ENCHANT_LEG_EARTHEN = 3853,
    ENCHANT_LEG_FROSTHIDE = 3822,
    ENCHANT_LEG_ICESCALE = 3823,
    ENCHANT_LEG_BRILLIANT_SPELLTHREAD = 3719,
    ENCHANT_LEG_SAPPHIRE_SPELLTHREAD = 3721,
    ENCHANT_LEG_DRAGONSCALE = 3331,
    ENCHANT_LEG_WYRMSCALE = 3332,

    ENCHANT_GLOVES_GREATER_BLASTING = 3249,
    ENCHANT_GLOVES_ARMSMAN = 3253,
    ENCHANT_GLOVES_CRUSHER = 1603,
    ENCHANT_GLOVES_AGILITY = 3222,
    ENCHANT_GLOVES_PRECISION = 3234,
    ENCHANT_GLOVES_EXPERTISE = 3231,
    ENCHANT_GLOVES_HYPERSPEED = 3604,

    ENCHANT_BRACERS_MAJOR_STAMINA = 3850,
    ENCHANT_BRACERS_SUPERIOR_SP = 2332,
    ENCHANT_BRACERS_GREATER_ASSUALT = 3845,
    ENCHANT_BRACERS_MAJOR_SPIRT = 1147,
    ENCHANT_BRACERS_EXPERTISE = 3231,
    ENCHANT_BRACERS_GREATER_STATS = 2661,
    ENCHANT_BRACERS_INTELLECT = 1119,
    ENCHANT_BRACERS_FURL_ARCANE = 3763,
    ENCHANT_BRACERS_FURL_FIRE = 3759,
    ENCHANT_BRACERS_FURL_FROST = 3760,
    ENCHANT_BRACERS_FURL_NATURE = 3762,
    ENCHANT_BRACERS_FURL_SHADOW = 3761,
    ENCHANT_BRACERS_FURL_ATTACK = 3756,
    ENCHANT_BRACERS_FURL_STAMINA = 3757,
    ENCHANT_BRACERS_FURL_SPELLPOWER = 3758,

    ENCHANT_CHEST_POWERFUL_STATS = 3832,
    ENCHANT_CHEST_SUPER_HEALTH = 3297,
    ENCHANT_CHEST_GREATER_MAINA_REST = 2381,
    ENCHANT_CHEST_EXCEPTIONAL_RESIL = 3245,
    ENCHANT_CHEST_GREATER_DEFENSE = 1953,

    ENCHANT_BOOTS_GREATER_ASSULT = 1597,
    ENCHANT_BOOTS_TUSKARS_VITLIATY = 3232,
    ENCHANT_BOOTS_SUPERIOR_AGILITY = 983,
    ENCHANT_BOOTS_GREATER_SPIRIT = 1147,
    ENCHANT_BOOTS_GREATER_VITALITY = 3244,
    ENCHANT_BOOTS_ICEWALKER = 3826,
    ENCHANT_BOOTS_GREATER_FORTITUDE = 1075,
    ENCHANT_BOOTS_NITRO_BOOTS = 3606,
    ENCHANT_BOOTS_PYRO_ROCKET = 3603,
    ENCHANT_BOOTS_ARMOR_WEBBING = 3860,

    ENCHANT_RING_ASSULT = 3839,
    ENCHANT_RING_GREATER_SP = 3840,
    ENCHANT_RING_STAMINA = 3791,
};

// Expansion tiers used to gate enchants by player level when progression mode is enabled.
// Vanilla  = enchants from original WoW (default available at levels 1-60)
// TBC      = enchants introduced in The Burning Crusade (default available at level 61+)
// Wrath    = enchants introduced in Wrath of the Lich King (default available at level 71+)
enum ExpansionTier
{
    EXPANSION_VANILLA = 0,
    EXPANSION_TBC     = 1,
    EXPANSION_WRATH   = 2,
};

uint32 roll;
bool EnchanterEnableModule;
bool EnchanterAnnounceModule;
uint32 EnchanterNumPhrases;
uint32 EnchanterMessageTimer;
uint32 EnchanterEmoteSpell;
uint32 EnchanterEmoteCommand;
bool EnchanterProgressionEnable;
uint32 EnchanterTBCLevel;
uint32 EnchanterWrathLevel;

// Returns true if the player's level qualifies them to see enchants of the given expansion tier.
// When progression mode is disabled this always returns true.
static bool CanSeeExpansion(Player* player, ExpansionTier tier)
{
    if (!EnchanterProgressionEnable)
        return true;

    uint32 level = player->GetLevel();
    switch (tier)
    {
        case EXPANSION_VANILLA: return true;
        case EXPANSION_TBC:     return level >= EnchanterTBCLevel;
        case EXPANSION_WRATH:   return level >= EnchanterWrathLevel;
    }
    return true;
}

// Adds a gossip item only if the player's level meets the tier threshold.
static void AddEnchantGossip(Player* player, ExpansionTier tier, const char* text, uint32 action)
{
    if (CanSeeExpansion(player, tier))
        AddGossipItemFor(player, 1, text, GOSSIP_SENDER_MAIN, action);
}

class EnchanterConfig : public WorldScript
{
public:
    EnchanterConfig() : WorldScript("EnchanterConfig_conf", {
        WORLDHOOK_ON_BEFORE_CONFIG_LOAD
    }) { }

    void OnBeforeConfigLoad(bool reload) override
    {
        if (!reload) {
            EnchanterEnableModule = sConfigMgr->GetOption<bool>("Enchanter.Enable", 1);
            EnchanterAnnounceModule = sConfigMgr->GetOption<bool>("Enchanter.Announce", 1);
            EnchanterNumPhrases = sConfigMgr->GetOption<uint32>("Enchanter.NumPhrases", 3);
            EnchanterMessageTimer = sConfigMgr->GetOption<uint32>("Enchanter.MessageTimer", 60000);
            EnchanterEmoteSpell = sConfigMgr->GetOption<uint32>("Enchanter.EmoteSpell", 44940);
            EnchanterEmoteCommand = sConfigMgr->GetOption<uint32>("Enchanter.EmoteCommand", 3);
            EnchanterProgressionEnable = sConfigMgr->GetOption<bool>("Enchanter.Progression.Enable", false);
            EnchanterTBCLevel = sConfigMgr->GetOption<uint32>("Enchanter.Progression.TBCLevel", 61);
            EnchanterWrathLevel = sConfigMgr->GetOption<uint32>("Enchanter.Progression.WrathLevel", 71);

            // Enforce Min/Max Time
            if (EnchanterMessageTimer != 0)
                if (EnchanterMessageTimer < 60000 || EnchanterMessageTimer > 300000)
                    EnchanterMessageTimer = 60000;
        }
    }
};

class EnchanterAnnounce : public PlayerScript
{

public:

    EnchanterAnnounce() : PlayerScript("EnchanterAnnounce", {
        PLAYERHOOK_ON_LOGIN
    }) {}

    void OnPlayerLogin(Player* player)
    {
        // Announce Module
        if (EnchanterAnnounceModule)
            ChatHandler(player->GetSession()).SendSysMessage("This server is running the |cff4CFF00EnchanterNPC |rmodule.");
    }
};

class npc_enchantment : public CreatureScript
{

public:

    npc_enchantment() : CreatureScript("npc_enchantment") { }

    // Pick Phrase
    static std::string PickPhrase()
    {
        // Choose and speak a random phrase to the player
        // Phrases are stored in the config file
        std::string phrase = "";
        uint32 PhraseNum = urand(1, EnchanterNumPhrases); // How many phrases does the NPC speak?
        phrase = "EC.P" + std::to_string(PhraseNum);

        // Sanitize
        if (phrase == "")
            phrase = "ERROR! NPC Emote Text Not Found! Check the npc_enchanter.conf!";

        std::string randMsg = sConfigMgr->GetOption<std::string>(phrase.c_str(), "");
        return randMsg.c_str();
    }

    // Builds the top-level enchant category menu. Called by OnGossipHello and the Back action.
    static void BuildMainMenu(Player* player, Creature* creature)
    {
        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Enchant Main Weapon]", GOSSIP_SENDER_MAIN, 1);
        if (player->HasSpell(674))
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_mace_116:24:24:-18|t[Enchant Offhand Weapon]", GOSSIP_SENDER_MAIN, 13);

        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_axe_113:24:24:-18|t[Enchant 2H Weapon]", GOSSIP_SENDER_MAIN, 2);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_shield_71:24:24:-18|t[Enchant Shield]", GOSSIP_SENDER_MAIN, 3);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_helmet_29:24:24:-18|t[Enchant Head]", GOSSIP_SENDER_MAIN, 4);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_shoulder_23:24:24:-18|t[Enchant Shoulders]", GOSSIP_SENDER_MAIN, 5);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_misc_cape_18:24:24:-18|t[Enchant Cloak]", GOSSIP_SENDER_MAIN, 6);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_chest_cloth_04:24:24:-18|t[Enchant Chest]", GOSSIP_SENDER_MAIN, 7);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_bracer_14:24:24:-18|t[Enchant Bracers]", GOSSIP_SENDER_MAIN, 8);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_gauntlets_06:24:24:-18|t[Enchant Gloves]", GOSSIP_SENDER_MAIN, 9);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_pants_11:24:24:-18|t[Enchant Legs]", GOSSIP_SENDER_MAIN, 10);
        AddGossipItemFor(player, 1, "|TInterface/ICONS/inv_boots_05:24:24:-18|t[Enchant Boots]", GOSSIP_SENDER_MAIN, 11);

        // Ring enchants are Wrath-era and require max Enchanting — hide both skill and level gates together.
        if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450 && CanSeeExpansion(player, EXPANSION_WRATH))
            AddGossipItemFor(player, 1, "|TInterface/ICONS/Inv_jewelry_ring_85:24:24:-18|t[Enchant Rings]", GOSSIP_SENDER_MAIN, 12);

        player->PlayerTalkClass->SendGossipMenu(601015, creature->GetGUID());
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (!EnchanterEnableModule)
            return false;

        BuildMainMenu(player, creature);
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        if (!EnchanterEnableModule)
            return false;

        Item * item;
        player->PlayerTalkClass->ClearMenus();

        switch (action)
        {

        case 1: // Enchant Main Hand Weapon
            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Blade Ward", 102);
                AddEnchantGossip(player, EXPANSION_WRATH, "Blood Draining", 103);
            }
            AddEnchantGossip(player, EXPANSION_VANILLA, "26 Agility", 100);
            AddEnchantGossip(player, EXPANSION_WRATH,   "45 Spirit", 101);
            AddEnchantGossip(player, EXPANSION_WRATH,   "Berserking", 104);
            AddEnchantGossip(player, EXPANSION_WRATH,   "25 Hit Rating + 25 Critical", 105);
            AddEnchantGossip(player, EXPANSION_WRATH,   "Black Magic", 106);
            AddEnchantGossip(player, EXPANSION_TBC,     "Battlemaster", 107);
            AddEnchantGossip(player, EXPANSION_WRATH,   "Icebreaker", 108);
            AddEnchantGossip(player, EXPANSION_WRATH,   "Lifeward", 109);
            AddEnchantGossip(player, EXPANSION_WRATH,   "50 Stamina", 110);
            AddEnchantGossip(player, EXPANSION_WRATH,   "65 Attack Power", 111);
            AddEnchantGossip(player, EXPANSION_WRATH,   "63 Spell Power", 112);
            AddEnchantGossip(player, EXPANSION_TBC,     "Mongoose", 113);
            AddEnchantGossip(player, EXPANSION_TBC,     "Executioner", 114);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
            return true;
            break;

        case 2: // Enchant 2H Weapon
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);
            if (!item)
            {
                creature->Whisper("This enchant requires a 2H weapon to be equipped.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Berserking", 104);
                AddEnchantGossip(player, EXPANSION_TBC,   "Mongoose", 113);
                AddEnchantGossip(player, EXPANSION_TBC,   "Executioner", 114);
                AddEnchantGossip(player, EXPANSION_WRATH, "81 Spell Power", 115);
                AddEnchantGossip(player, EXPANSION_TBC,   "35 Agility", 116);
                AddEnchantGossip(player, EXPANSION_WRATH, "110 Attack Power", 117);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper("This enchant requires a 2H weapon to be equipped.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
            }
            player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
            return true;
            break;

        case 3: // Enchant Shield
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item)
            {
                creature->Whisper("This enchant requires a shield to be equipped.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
            {
                AddEnchantGossip(player, EXPANSION_TBC,     "20 Defense", 118);
                AddEnchantGossip(player, EXPANSION_VANILLA, "25 Intellect", 119);
                AddEnchantGossip(player, EXPANSION_WRATH,   "12 Resilience", 120);
                AddEnchantGossip(player, EXPANSION_WRATH,   "36 Block", 121);
                AddEnchantGossip(player, EXPANSION_VANILLA, "18 Stamina", 122);
                AddEnchantGossip(player, EXPANSION_TBC,     "81 Block + 50% Less Disarm", 123);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper("This enchant requires a shield to be equipped.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());

            }
            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
            return true;
            break;

        case 4: // Enchant Head
            AddEnchantGossip(player, EXPANSION_WRATH, "30 Spell Power + 10 Mp5", 124);
            AddEnchantGossip(player, EXPANSION_WRATH, "30 Spell Power + 20 Crit", 125);
            AddEnchantGossip(player, EXPANSION_WRATH, "29 Spell Power + 20 Resilience", 126);
            AddEnchantGossip(player, EXPANSION_WRATH, "30 Stamina + 25 Resilience", 127);
            AddEnchantGossip(player, EXPANSION_WRATH, "37 Stamina + 20 Defense", 128);
            AddEnchantGossip(player, EXPANSION_WRATH, "50 Attack Power + 20 Crit", 129);
            AddEnchantGossip(player, EXPANSION_WRATH, "50 Attack Power + 20 Resilience", 130);
            AddEnchantGossip(player, EXPANSION_WRATH, "Arcanum of Eclipsed Moon", 131);
            AddEnchantGossip(player, EXPANSION_WRATH, "Arcanum of the Flame's Soul", 132);
            AddEnchantGossip(player, EXPANSION_WRATH, "Arcanum of the Fleeing Shadow", 133);
            AddEnchantGossip(player, EXPANSION_WRATH, "Arcanum of the Frosty Soul", 134);
            AddEnchantGossip(player, EXPANSION_WRATH, "Arcanum of Toxic Warding", 135);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100005, creature->GetGUID());
            return true;
            break;

        case 5: // Enchant Shoulders
            if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) == 450)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "120 Attack Power + 15 Crit", 136);
                AddEnchantGossip(player, EXPANSION_WRATH, "70 Spell Power + 8 Mp5", 137);
                AddEnchantGossip(player, EXPANSION_WRATH, "60 Dodge + 15 Defense", 138);
                AddEnchantGossip(player, EXPANSION_WRATH, "70 Spell Power + 15 Crit", 139);
            }
            AddEnchantGossip(player, EXPANSION_WRATH, "40 Attack Power + 15 Crit", 140);
            AddEnchantGossip(player, EXPANSION_WRATH, "24 Spell Power + 8 Mp5", 141);
            AddEnchantGossip(player, EXPANSION_WRATH, "30 Stamina + 15 Resilience", 142);
            AddEnchantGossip(player, EXPANSION_WRATH, "20 Dodge + 15 Defense", 143);
            AddEnchantGossip(player, EXPANSION_WRATH, "24 Spell Power + 15 Crit", 144);
            AddEnchantGossip(player, EXPANSION_WRATH, "23 Spell Power + 15 Resilience", 145);
            AddEnchantGossip(player, EXPANSION_WRATH, "40 Attack Power + 15 Resilience", 146);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100006, creature->GetGUID());
            return true;
            break;

        case 6: // Enchant Cloak
            if (player->HasSkill(SKILL_TAILORING) && player->GetSkillValue(SKILL_TAILORING) == 450)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Darkglow Embroidery", 149);
                AddEnchantGossip(player, EXPANSION_WRATH, "Lightweave Embroidery", 150);
                AddEnchantGossip(player, EXPANSION_WRATH, "Swordguard Embroidery", 151);
            }
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Parachute", 147);
            }
            AddEnchantGossip(player, EXPANSION_TBC,   "Shadow Armor", 148);
            AddEnchantGossip(player, EXPANSION_WRATH, "10 Spirit + 2% Reduced Threat", 152);
            AddEnchantGossip(player, EXPANSION_TBC,   "16 Defense", 153);
            AddEnchantGossip(player, EXPANSION_WRATH, "35 Spell Penetration", 154);
            AddEnchantGossip(player, EXPANSION_WRATH, "225 Armor", 155);
            AddEnchantGossip(player, EXPANSION_TBC,   "22 Agility", 156);
            AddEnchantGossip(player, EXPANSION_WRATH, "23 Haste", 157);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
            return true;
            break;

        case 7: // Enchant Chest
            AddEnchantGossip(player, EXPANSION_WRATH, "+10 All Stats", 158);
            AddEnchantGossip(player, EXPANSION_WRATH, "225 Health", 159);
            AddEnchantGossip(player, EXPANSION_TBC,   "10 Mp5", 160);
            AddEnchantGossip(player, EXPANSION_WRATH, "20 Resilience", 161);
            AddEnchantGossip(player, EXPANSION_TBC,   "22 Defense", 162);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
            return true;
            break;

        case 8: // Enchant Bracers
            AddEnchantGossip(player, EXPANSION_WRATH,   "40 Stamina", 163);
            AddEnchantGossip(player, EXPANSION_TBC,     "30 Spell Power", 164);
            AddEnchantGossip(player, EXPANSION_WRATH,   "50 Attack Power", 165);
            AddEnchantGossip(player, EXPANSION_VANILLA, "18 Spirit", 166);
            AddEnchantGossip(player, EXPANSION_WRATH,   "15 Expertise", 167);
            AddEnchantGossip(player, EXPANSION_TBC,     "+6 All Stats", 168);
            AddEnchantGossip(player, EXPANSION_VANILLA, "16 Intellect", 169);
            if (player->HasSkill(SKILL_LEATHERWORKING) && player->GetSkillValue(SKILL_LEATHERWORKING) == 450)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Arcane Resist", 170);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Fire Resist", 171);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Frost Resist", 172);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Nature Resist", 173);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Shadow Resist", 174);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Attack Power", 175);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Stamina", 176);
                AddEnchantGossip(player, EXPANSION_WRATH, "Fur Lining - Spellpower", 177);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
            return true;
            break;

        case 9: // Enchant Gloves
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 400)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Hyperspeed Accelerators", 200);
            }
            AddEnchantGossip(player, EXPANSION_WRATH,   "16 Critical Strike", 178);
            AddEnchantGossip(player, EXPANSION_WRATH,   "2% Threat + 10 Parry", 179);
            AddEnchantGossip(player, EXPANSION_VANILLA, "44 Attack Power", 180);
            AddEnchantGossip(player, EXPANSION_WRATH,   "20 Agility", 181);
            AddEnchantGossip(player, EXPANSION_WRATH,   "20 Hit Rating", 182);
            AddEnchantGossip(player, EXPANSION_WRATH,   "15 Expertise", 183);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
            return true;
            break;

        case 10: // Enchant Legs
            AddEnchantGossip(player, EXPANSION_WRATH, "40 Resilience + 28 Stamina", 184);
            AddEnchantGossip(player, EXPANSION_WRATH, "55 Stamina + 22 Agility", 185);
            AddEnchantGossip(player, EXPANSION_WRATH, "75 Attack Power + 22 Critical", 186);
            AddEnchantGossip(player, EXPANSION_WRATH, "50 Spell Power + 22 Spirit", 187);
            AddEnchantGossip(player, EXPANSION_WRATH, "50 Spell Power + 30 Stamina", 188);
            AddEnchantGossip(player, EXPANSION_WRATH, "72 Stamina + 35 Agility", 189);
            AddEnchantGossip(player, EXPANSION_WRATH, "100 Attack Power + 36 Critical", 190);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100011, creature->GetGUID());
            return true;
            break;

        case 11: // Enchant Boots
            AddEnchantGossip(player, EXPANSION_VANILLA, "32 Attack Power", 191);
            AddEnchantGossip(player, EXPANSION_WRATH,   "15 Stamina + Minor Speed Increase", 192);
            AddEnchantGossip(player, EXPANSION_VANILLA, "16 Agility", 193);
            AddEnchantGossip(player, EXPANSION_VANILLA, "18 Spirit", 194);
            AddEnchantGossip(player, EXPANSION_WRATH,   "Restore 7 Health + Mp5", 195);
            AddEnchantGossip(player, EXPANSION_WRATH,   "12 Hit Rating + 12 Critical", 196);
            AddEnchantGossip(player, EXPANSION_VANILLA, "22 Stamina", 197);
            if (player->HasSkill(SKILL_ENGINEERING) && player->GetSkillValue(SKILL_ENGINEERING) == 450)
            {
                AddEnchantGossip(player, EXPANSION_WRATH, "Nitro Boots", 198);
                AddEnchantGossip(player, EXPANSION_WRATH, "Hand-Mounted Pyro Rocket", 199);
                AddEnchantGossip(player, EXPANSION_WRATH, "Reticulated Armor Webbing", 201);
            }
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
            return true;
            break;

        case 12: // Enchant Rings
            AddEnchantGossip(player, EXPANSION_WRATH, "40 Attack Power", 202);
            AddEnchantGossip(player, EXPANSION_WRATH, "23 Spell Power", 203);
            AddEnchantGossip(player, EXPANSION_WRATH, "30 Stamina", 204);
            AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
            return true;
            break;

        case 13: // Enchant Offhand Weapon
            item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);
            if (!item)
            {
                creature->Whisper("This enchant requires a weapon to be equipped in offhand.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendCloseGossip();
                return false;
            }
            if (item->GetTemplate()->InventoryType == INVTYPE_WEAPON)
            {
                if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) == 450)
                {
                    AddEnchantGossip(player, EXPANSION_WRATH, "Blade Ward", 207);
                    AddEnchantGossip(player, EXPANSION_WRATH, "Blood Draining", 208);
                }
                AddEnchantGossip(player, EXPANSION_VANILLA, "26 Agility", 205);
                AddEnchantGossip(player, EXPANSION_WRATH,   "45 Spirit", 206);
                AddEnchantGossip(player, EXPANSION_WRATH,   "Berserking", 209);
                AddEnchantGossip(player, EXPANSION_WRATH,   "25 Hit Rating + 25 Critical", 210);
                AddEnchantGossip(player, EXPANSION_WRATH,   "Black Magic", 211);
                AddEnchantGossip(player, EXPANSION_TBC,     "Battlemaster", 212);
                AddEnchantGossip(player, EXPANSION_WRATH,   "Icebreaker", 213);
                AddEnchantGossip(player, EXPANSION_WRATH,   "Lifeward", 214);
                AddEnchantGossip(player, EXPANSION_WRATH,   "50 Stamina", 215);
                AddEnchantGossip(player, EXPANSION_WRATH,   "65 Attack Power", 216);
                AddEnchantGossip(player, EXPANSION_WRATH,   "63 Spell Power", 217);
                AddEnchantGossip(player, EXPANSION_TBC,     "Mongoose", 218);
                AddEnchantGossip(player, EXPANSION_TBC,     "Executioner", 219);
                AddGossipItemFor(player, GOSSIP_ICON_TALK, "Back", GOSSIP_SENDER_MAIN, 300);
            }
            else
            {
                creature->Whisper("This enchant requires a weapon to be equipped in offhand.", LANG_UNIVERSAL, player);
                player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());

            }
            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
            return true;
            break;

        case 100:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY_1H);
            break;

        case 101:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
            break;

        case 102:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLADE_WARD);
            break;

        case 103:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLOOD_DRAINING);
            break;

        case 104:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BERSERKING);
            break;

        case 105:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ACCURACY);
            break;

        case 106:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLACK_MAGIC);
            break;

        case 107:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BATTLEMASTER);
            break;

        case 108:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEBREAKER);
            break;

        case 109:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFEWARD);
            break;

        case 110:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_TITANGUARD);
            break;

        case 111:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POTENCY);
            break;

        case 112:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
            break;

        case 113:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MONGOOSE);
            break;

        case 114:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_EXECUTIONER);
            break;

        case 115:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_GREATER_SPELL_POWER);
            break;

        case 116:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
            break;

        case 117:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MASSACRE);
            break;

        case 118:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_DEFENSE);
            break;

        case 119:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_INTELLECT);
            break;

        case 120:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_RESILIENCE);
            break;

        case 121:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM_PLATING);
            break;

        case 122:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAMINA);
            break;

        case 123:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TOUGHSHIELD);
            break;

        case 124:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BLISSFUL_MENDING);
            break;

        case 125:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BURNING_MYSTERIES);
            break;

        case 126:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_DOMINANCE);
            break;

        case 127:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_SAVAGE_GLADIATOR);
            break;

        case 128:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_STALWART_PROTECTOR);
            break;

        case 129:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TORMENT);
            break;

        case 130:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TRIUMPH);
            break;

        case 131:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_ECLIPSED_MOON);
            break;

        case 132:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLAME_SOUL);
            break;

        case 133:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLEEING_SHADOW);
            break;

        case 134:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FROSTY_SOUL);
            break;

        case 135:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TOXIC_WARDING);
            break;

        case 136:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_AXE);
            break;

        case 137:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_CRAG);
            break;

        case 138:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_PINNACLE);
            break;

        case 139:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_STORM);
            break;

        case 140:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_AXE);
            break;

        case 141:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_CRAG);
            break;

        case 142:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_GLADIATOR);
            break;

        case 143:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_PINNACLE);
            break;

        case 144:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_STORM);
            break;

        case 145:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_DOMINANCE);
            break;

        case 146:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_TRIUMPH);
            break;

        case 147:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
            break;

        case 148:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SHADOW_ARMOR);
            break;

        case 149:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
            break;

        case 150:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
            break;

        case 151:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
            break;

        case 152:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_WISDOM);
            break;

        case 153:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_TITANWEAVE);
            break;

        case 154:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPELL_PIERCING);
            break;

        case 155:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MIGHTY_ARMOR);
            break;

        case 156:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MAJOR_AGILITY);
            break;

        case 157:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPEED);
            break;

        case 158:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_POWERFUL_STATS);
            break;

        case 159:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_SUPER_HEALTH);
            break;

        case 160:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_MAINA_REST);
            break;

        case 161:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_RESIL);
            break;

        case 162:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_DEFENSE);
            break;

        case 163:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STAMINA);
            break;

        case 164:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_SP);
            break;

        case 165:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_ASSUALT);
            break;

        case 166:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_SPIRT);
            break;

        case 167:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXPERTISE);
            break;

        case 168:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_STATS);
            break;

        case 169:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
            break;

        case 170:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ARCANE);
            break;

        case 171:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FIRE);
            break;

        case 172:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FROST);
            break;

        case 173:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_NATURE);
            break;

        case 174:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SHADOW);
            break;

        case 175:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ATTACK);
            break;

        case 176:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_STAMINA);
            break;

        case 177:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SPELLPOWER);
            break;

        case 178:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_BLASTING);
            break;

        case 179:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_ARMSMAN);
            break;

        case 180:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_CRUSHER);
            break;

        case 181:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
            break;

        case 182:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_PRECISION);
            break;

        case 183:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_EXPERTISE);
            break;

        case 184:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_EARTHEN);
            break;

        case 185:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_FROSTHIDE);
            break;

        case 186:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_ICESCALE);
            break;

        case 187:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_BRILLIANT_SPELLTHREAD);
            break;

        case 188:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_SAPPHIRE_SPELLTHREAD);
            break;

        case 189:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_DRAGONSCALE);
            break;

        case 190:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_WYRMSCALE);
            break;

        case 191:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_ASSULT);
            break;

        case 192:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_TUSKARS_VITLIATY);
            break;

        case 193:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SUPERIOR_AGILITY);
            break;

        case 194:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_SPIRIT);
            break;

        case 195:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_VITALITY);
            break;

        case 196:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ICEWALKER);
            break;

        case 197:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_FORTITUDE);
            break;

        case 198:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_NITRO_BOOTS);
            break;

        case 199:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_PYRO_ROCKET);
            break;

        case 200:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_HYPERSPEED);
            break;

        case 201:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ARMOR_WEBBING);
            break;

        case 202:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_ASSULT);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_ASSULT);
            break;

        case 203:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_SP);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_SP);
            break;

        case 204:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STAMINA);
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STAMINA);
            break;

        case 205:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY_1H);
            break;

        case 206:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_SPIRIT);
            break;

        case 207:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLADE_WARD);
            break;

        case 208:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLOOD_DRAINING);
            break;

        case 209:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BERSERKING);
            break;

        case 210:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ACCURACY);
            break;

        case 211:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLACK_MAGIC);
            break;

        case 212:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BATTLEMASTER);
            break;

        case 213:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEBREAKER);
            break;

        case 214:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LIFEWARD);
            break;

        case 215:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_TITANGUARD);
            break;

        case 216:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POTENCY);
            break;

        case 217:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
            break;

        case 218:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_2WEP_MONGOOSE);
            break;

        case 219:
            Enchant(player, creature, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_EXECUTIONER);
            break;

        case 300:
            BuildMainMenu(player, creature);
            return true;
            break;
        }

        player->PlayerTalkClass->SendCloseGossip();
        return true;
    }

    void Enchant(Player* player, Creature* creature, Item* item, uint32 enchantid)
    {
        if (!item)
        {
            creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
            creature->Whisper("Please equip the item you would like to enchant!", LANG_UNIVERSAL, player);
            player->PlayerTalkClass->SendCloseGossip();
            return;
        }

        if (!enchantid)
        {
        	ChatHandler(player->GetSession()).SendNotification("Something went wrong in the code. It has been logged for developers and will be looked into, sorry for the inconvenience.");
            player->PlayerTalkClass->SendCloseGossip();
            creature->HandleEmoteCommand(EMOTE_ONESHOT_LAUGH);
            return;
        }

        // Roll the dice
        roll = urand(1, 100);

        item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
        item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);

        // Random enchantment notification
        if (roll > 0 && roll < 33)
        {
            ChatHandler(player->GetSession()).SendNotification("|cff00ff00Beauregard's bony finger crackles with energy when he touches |cffDA70D6%s|cff00ff00!", item->GetTemplate()->Name1.c_str());
            // creature->HandleEmoteCommand(EMOTE_ONESHOT_POINT);
        }
        else if (roll > 33 && roll < 75)
        {
            ChatHandler(player->GetSession()).SendNotification("|cff00ff00Beauregard holds |cffDA70D6%s |cff00ff00up in the air and utters a strange incantation!", item->GetTemplate()->Name1.c_str());
            // creature->HandleEmoteCommand(EMOTE_ONESHOT_CHEER);
        }
        else
        {
            ChatHandler(player->GetSession()).SendNotification("|cff00ff00Beauregard concentrates deeply while waving his wand over |cffDA70D6%s|cff00ff00!", item->GetTemplate()->Name1.c_str());
            // creature->HandleEmoteCommand(EMOTE_ONESHOT_WAVE);
        }
        creature->CastSpell(player, 12512); // enchantment visual
        player->PlayerTalkClass->SendCloseGossip();
    }

    // Passive Emotes
    struct NPC_PassiveAI : public ScriptedAI
    {
        NPC_PassiveAI(Creature * creature) : ScriptedAI(creature) { }

        uint32 MessageTimer = 0;

        // Called once when client is loaded
        void Reset()
        {
            if (EnchanterMessageTimer != 0)
                MessageTimer = urand(EnchanterMessageTimer, 300000); // 1-5 minutes
        }

        // Called at World update tick
        void UpdateAI(const uint32 diff)
        {
            // If Enabled
            if (EnchanterEnableModule && EnchanterMessageTimer != 0)
            {
                if (MessageTimer <= diff)
                {
                    if (EnchanterNumPhrases > 0)
                    {
                        std::string Message = PickPhrase();
                        me->Say(Message.c_str(), LANG_UNIVERSAL);
                    }

                    // Use gesture?
                    if (EnchanterEmoteCommand != 0)
                        me->HandleEmoteCommand(EnchanterEmoteCommand);

                    // Alert players?
                    if (EnchanterEmoteSpell != 0)
                        me->CastSpell(me, EnchanterEmoteSpell);

                    MessageTimer = urand(EnchanterMessageTimer, 300000);
                }
                else { MessageTimer -= diff; }
            }
        }
    };

    // CREATURE AI
    CreatureAI * GetAI(Creature * creature) const
    {
        return new NPC_PassiveAI(creature);
    }
};

void AddNPCEnchanterScripts()
{
    new EnchanterConfig();
    new EnchanterAnnounce();
    new npc_enchantment();
}
