#include "NameGenerator.hpp"
#include <algorithm>
#include <cctype>

NameGenerator::NameGenerator() : rng_(std::random_device{}()) {
    // Initialize consonant blends
    consonant_blends_ = {
        "bl", "br", "ch", "cl", "cr", "dr", "fl", "fr", "gl", "gr",
        "pl", "pr", "sc", "sh", "sk", "sl", "sm", "sn", "sp", "st",
        "sw", "th", "tr", "tw", "wh", "wr", "qu", "scr", "spr", "str"
    };

    // Initialize patterns (50+ diverse patterns)
    patterns_ = {
        // Short punchy names (2-4 chars)
        "CVC",    // Git, Fax, Mod
        "VCC",    // Axe, Ork
        "CCV",    // Sky, Pro
        "VC",     // At, Ex
        "CV",     // Go, Do

        // Classic 4-letter patterns
        "CVCC",   // Jolt, Link, Mark
        "CCVC",   // Snap, Clap, Trim
        "CVCV",   // Java, Kona, Zara, Jira
        "VCVC",   // Ajax, Uber, Opus
        "CVVC",   // Neat, Zoom, Teal

        // 5-letter patterns
        "CVCVC",  // Radar, Civic, Rapid
        "CVCCV",  // Joomla, Trello
        "CCVCV",  // Promo, Blaze
        "VCVCV",  // Aviva, Opera
        "CVCCC",  // Craft, Burst
        "CCCVC",  // Script, Sprint

        // 6-letter patterns
        "CVCVCV", // Banana, Canada
        "CVCCVC", // Perfect, Syntax
        "CCVCVC", // Prefix, Proton
        "CVCVCC", // Basket, Magnet
        "VCVCVC", // Amoeba, Oracle

        // Blend-based patterns (B = consonant blend)
        "BVC",    // Bro, Sky, Fly
        "BVV",    // Bloo, Tree
        "BVCC",   // Brisk, Flash, Clamp
        "BVC" ,   // Slack, Prism, Glint
        "BVCV",   // Bravo, Cloak, Primo
        "CVBV",   // Cobra, Fedra
        "BVCVC",  // Plasma, Trauma, Chrome
        "CVBVC",  // Contra, Mantra

        // Double letter patterns (D = double)
        "CVDV",   // Mood, Google, Zorro
        "VDVC",   // Eerie, Aaron
        "CVDVC",  // Pepper, Bitter
        "CVVCV",  // Cooler, Keeper

        // Special ending patterns (S = x, z, k)
        "CVS",    // Fax, Box, Pix
        "CVCS",   // Linux, Kodak, Redux
        "CVCVS",  // Forex, Xerox, Fedex
        "BVCS",   // Brinx, Clorox

        // Mixed creative patterns
        "VCCV",   // Akka, Ikea
        "VCCVC",  // Aspen, Ember
        "CCVVC",  // Sleek, Groot
        "CVCVVC", // Devour, Random
        "BVVCV",  // Skype, Troop

        // Longer dramatic names
        "CVCVCVC", // Velocity, Mimetic
        "CCVCVCV", // Prophecy, Strategy
        "BVCVCVC", // Chromatic, Strategic

        // Quirky patterns
        "VCV",    // Ava, Ida, Eli
        "VCVV",   // Audi, Oleo
        "CVVCC",  // Boost, Cloud
        "CCVCC",  // Trunk, Plank

        // === NEW PHONETIC PATTERNS (using P, F, N, L, W, Q codes) ===

        // Plosive-based patterns (percussive)
        "PVP",    // Percussive: bag, dot, kit
        "PVPV",   // Tiki, Boba, Pupa

        // Fricative-based patterns (breathy)
        "FVF",    // Breathy: fox, sax, haze
        "FVFV",   // Viva, Sasa, Fifi

        // Liquid-based patterns (flowing)
        "LVL",    // Flowing: lol, rar, lil
        "LVLV",   // Lara, Riri, Lola

        // Mixed phonetic patterns (combining categories)
        "PVL",    // Plosive-Liquid: pal, tel, bar
        "PVLV",   // Pala, Tara, Boli
        "FVL",    // Fricative-Liquid: sol, far, vil
        "FVLV",   // Solo, Fara, Velo
        "NVL",    // Nasal-Liquid: mal, nir, mel
        "NVLV",   // Mala, Nira, Melo

        // Quality pair patterns (smart auto-pairing)
        "QVC",    // Quality pair start
        "QVCV",   // Quality pair + simple end
        "QVQV",   // Multiple quality pairs
        "QVCVC",  // Quality pair with longer tail
        "VQVC",   // Quality pair in middle
    };
}

void NameGenerator::seed(unsigned int seed) {
    rng_.seed(seed);
}

std::string NameGenerator::generate() {
    // Pick a random pattern
    std::uniform_int_distribution<size_t> dist(0, patterns_.size() - 1);
    const auto& pattern = patterns_[dist(rng_)];

    return generateFromPattern(pattern);
}

NameWithPattern NameGenerator::generateWithPattern() {
    // Pick a random pattern
    std::uniform_int_distribution<size_t> dist(0, patterns_.size() - 1);
    const auto& pattern = patterns_[dist(rng_)];

    return {generateFromPattern(pattern), pattern};
}

std::vector<std::string> NameGenerator::generate(size_t count) {
    std::vector<std::string> names;
    names.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        names.push_back(generate());
    }

    return names;
}

std::vector<NameWithPattern> NameGenerator::generateWithPattern(size_t count) {
    std::vector<NameWithPattern> results;
    results.reserve(count);

    for (size_t i = 0; i < count; ++i) {
        results.push_back(generateWithPattern());
    }

    return results;
}

// ===== RANDOM CHARACTER SELECTION FUNCTIONS =====
// These functions randomly select characters from the phonetic categories

char NameGenerator::randomConsonant() {
    // Select any consonant (all phonetic categories combined)
    std::uniform_int_distribution<size_t> dist(0, consonants_.size() - 1);
    return consonants_[dist(rng_)];
}

char NameGenerator::randomPlosive() {
    // Select a plosive/stop consonant: b, c, d, g, k, p, q, t
    // These are "percussive" - air is blocked then released
    std::uniform_int_distribution<size_t> dist(0, plosives_.size() - 1);
    return plosives_[dist(rng_)];
}

char NameGenerator::randomFricative() {
    // Select a fricative consonant: f, h, s, v, x, z
    // These are "breathy" - air is forced through a narrow gap
    std::uniform_int_distribution<size_t> dist(0, fricatives_.size() - 1);
    return fricatives_[dist(rng_)];
}

char NameGenerator::randomNasal() {
    // Select a nasal consonant: m, n
    // These have a "humming" quality - air flows through the nose
    std::uniform_int_distribution<size_t> dist(0, nasals_.size() - 1);
    return nasals_[dist(rng_)];
}

char NameGenerator::randomLiquid() {
    // Select a liquid consonant: l, r
    // These have a "flowing" quality
    std::uniform_int_distribution<size_t> dist(0, liquids_.size() - 1);
    return liquids_[dist(rng_)];
}

char NameGenerator::randomGlide() {
    // Select a glide/approximant: w, j (y)
    // These have a "sliding" quality, like short vowels
    std::uniform_int_distribution<size_t> dist(0, glides_.size() - 1);
    return glides_[dist(rng_)];
}

char NameGenerator::randomVowel() {
    // Select any vowel: a, e, i, o, u
    std::uniform_int_distribution<size_t> dist(0, vowels_.size() - 1);
    return vowels_[dist(rng_)];
}

char NameGenerator::randomSpecial() {
    // Select a special ending character: x, z, k
    // These give names a modern/tech feel
    std::uniform_int_distribution<size_t> dist(0, special_endings_.size() - 1);
    return special_endings_[dist(rng_)];
}

std::string NameGenerator::randomBlend() {
    // Select a pre-defined consonant blend: sh, tr, bl, etc.
    // These are known natural-sounding consonant pairs
    std::uniform_int_distribution<size_t> dist(0, consonant_blends_.size() - 1);
    return consonant_blends_[dist(rng_)];
}

std::string NameGenerator::randomQualityPair() {
    // Generate a "quality pair" - two consonants from different phonetic categories
    // This avoids problematic clusters like "kt", "pb", "mg" (plosive + plosive)
    // and creates natural-sounding combinations like "sl", "fr", "mn"

    // Define valid pairing strategies (consonant categories that sound good together)
    enum class PairType {
        PlosiveLiquid,     // p+l: pl, tr, kr, bl, dr, gl
        PlosiveFricative,  // p+f: ps, kf, ts
        FricativeLiquid,   // f+l: fl, sl, fr, sr
        FricativeNasal,    // f+n: fn, sn, zm
        LiquidPlosive,     // l+p: lk, rt, ld
        LiquidFricative,   // l+f: lf, ls, rv, rz
        NasalPlosive,      // n+p: mp, nt, nd, nk
        NasalFricative,    // n+f: nf, ns, nz
        GlidePlosive,      // w+p: wt, wd (rare but valid)
        GlideFricative     // w+f: wh-like, ws
    };

    // Randomly select a pairing strategy
    std::uniform_int_distribution<int> strategy_dist(0, 9);
    PairType strategy = static_cast<PairType>(strategy_dist(rng_));

    std::string pair;

    switch (strategy) {
        case PairType::PlosiveLiquid:
            // Plosive + Liquid: very common, sounds great (black, tree, play, grow)
            pair += randomPlosive();
            pair += randomLiquid();
            break;

        case PairType::PlosiveFricative:
            // Plosive + Fricative: less common but valid (pseudo, gnostic)
            pair += randomPlosive();
            pair += randomFricative();
            break;

        case PairType::FricativeLiquid:
            // Fricative + Liquid: very natural (flow, slide, shred)
            pair += randomFricative();
            pair += randomLiquid();
            break;

        case PairType::FricativeNasal:
            // Fricative + Nasal: less common (snack, smack via blends)
            pair += randomFricative();
            pair += randomNasal();
            break;

        case PairType::LiquidPlosive:
            // Liquid + Plosive: natural (old, art, help)
            pair += randomLiquid();
            pair += randomPlosive();
            break;

        case PairType::LiquidFricative:
            // Liquid + Fricative: common (also, mars, elf)
            pair += randomLiquid();
            pair += randomFricative();
            break;

        case PairType::NasalPlosive:
            // Nasal + Plosive: very natural (lamp, hand, link)
            pair += randomNasal();
            pair += randomPlosive();
            break;

        case PairType::NasalFricative:
            // Nasal + Fricative: works well (ounce, tense)
            pair += randomNasal();
            pair += randomFricative();
            break;

        case PairType::GlidePlosive:
            // Glide + Plosive: less common but pronounceable
            pair += randomGlide();
            pair += randomPlosive();
            break;

        case PairType::GlideFricative:
            // Glide + Fricative: natural (wish, yes)
            pair += randomGlide();
            pair += randomFricative();
            break;
    }

    return pair;
}

// ===== PATTERN ELEMENT GENERATORS =====
// These functions generate pattern elements based on the pattern code
// They are called by generateFromPattern() for each character in the pattern

std::string NameGenerator::generateC() {
    // C = any Consonant
    return std::string(1, randomConsonant());
}

std::string NameGenerator::generateP() {
    // P = Plosive/stop consonant (b, c, d, g, k, p, q, t)
    // Percussive sounds - air is blocked then released
    return std::string(1, randomPlosive());
}

std::string NameGenerator::generateF() {
    // F = Fricative consonant (f, h, s, v, x, z)
    // Breathy sounds - air forced through narrow gap
    return std::string(1, randomFricative());
}

std::string NameGenerator::generateN() {
    // N = Nasal consonant (m, n)
    // Humming sounds - air flows through nose
    return std::string(1, randomNasal());
}

std::string NameGenerator::generateL() {
    // L = Liquid consonant (l, r)
    // Flowing sounds
    return std::string(1, randomLiquid());
}

std::string NameGenerator::generateW() {
    // W = Glide (w, j/y)
    // Sliding sounds, like short vowels
    return std::string(1, randomGlide());
}

std::string NameGenerator::generateV() {
    // V = Vowel (a, e, i, o, u)
    return std::string(1, randomVowel());
}

std::string NameGenerator::generateB() {
    // B = pre-defined Blend (sh, tr, bl, etc.)
    // These are known natural-sounding consonant combinations
    return randomBlend();
}

std::string NameGenerator::generateD() {
    // D = Double letter (same letter repeated)
    // Can be either a vowel or consonant
    std::uniform_int_distribution<int> dist(0, 1);
    char c = dist(rng_) ? randomVowel() : randomConsonant();
    return std::string(2, c);
}

std::string NameGenerator::generateS() {
    // S = Special ending (x, z, k)
    // Gives names a modern/tech feel
    return std::string(1, randomSpecial());
}

std::string NameGenerator::generateQ() {
    // Q = Quality pair - smart consonant pairing from different phonetic categories
    // Avoids bad clusters like "kt", "pb" and creates natural pairs like "sl", "fr"
    return randomQualityPair();
}

std::string NameGenerator::generateFromPattern(const std::string& pattern) {
    // Interpret a pattern string and generate a name
    // Each character in the pattern is a code that generates specific sounds
    //
    // Pattern codes (organized by category):
    //   BASIC:
    //     C = any Consonant
    //     V = Vowel
    //
    //   PHONETIC CONSONANTS (organized by how they're pronounced):
    //     P = Plosive/stop (b,d,g,k,p,t)     - percussive
    //     F = Fricative (f,h,s,v,x,z)        - breathy
    //     N = Nasal (m,n)                    - humming
    //     L = Liquid (l,r)                   - flowing
    //     W = Glide (w,j)                    - sliding
    //
    //   SPECIAL:
    //     B = pre-defined Blend (sh,tr,bl)   - known good pairs
    //     Q = Quality pair                   - smart auto-pairing
    //     D = Double letter                  - repeated character
    //     S = Special ending (x,z,k)         - modern/tech feel

    std::string result;

    for (char code : pattern) {
        switch (code) {
            // Basic codes
            case 'C':
                result += generateC();  // Any consonant
                break;
            case 'V':
                result += generateV();  // Vowel
                break;

            // Phonetic consonant categories
            case 'P':
                result += generateP();  // Plosive
                break;
            case 'F':
                result += generateF();  // Fricative
                break;
            case 'N':
                result += generateN();  // Nasal
                break;
            case 'L':
                result += generateL();  // Liquid
                break;
            case 'W':
                result += generateW();  // Glide
                break;

            // Special codes
            case 'B':
                result += generateB();  // Pre-defined blend
                break;
            case 'Q':
                result += generateQ();  // Smart quality pair
                break;
            case 'D':
                result += generateD();  // Double letter
                break;
            case 'S':
                result += generateS();  // Special ending
                break;

            default:
                // Unknown pattern code - skip silently
                // This allows for future extension
                break;
        }
    }

    return capitalize(result);
}

std::string NameGenerator::capitalize(std::string str) {
    if (!str.empty()) {
        str[0] = std::toupper(static_cast<unsigned char>(str[0]));
    }
    return str;
}
