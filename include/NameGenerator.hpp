#ifndef NAME_GENERATOR_HPP
#define NAME_GENERATOR_HPP

#include <string>
#include <vector>
#include <random>

struct NameWithPattern {
    std::string name;
    std::string pattern;
};

class NameGenerator {
public:
    NameGenerator();

    // Generate a single name
    std::string generate();

    // Generate a single name with pattern information
    NameWithPattern generateWithPattern();

    // Generate multiple names
    std::vector<std::string> generate(size_t count);

    // Generate multiple names with pattern information
    std::vector<NameWithPattern> generateWithPattern(size_t count);

    // Seed the random number generator
    void seed(unsigned int seed);

private:
    std::mt19937 rng_;

    // ===== PHONETIC CHARACTER SETS =====
    // Consonants are organized by their phonetic properties (how they're produced)
    // This creates more natural-sounding consonant clusters

    // PLOSIVES/STOPS: Air is completely blocked, then released in a burst
    // These have a "percussive" quality: b, d, g, k, p, t
    // (c and q included for spelling variety, though c=k and q=kw phonetically)
    static constexpr std::string_view plosives_ = "bcdgkpqt";

    // FRICATIVES: Air is forced through a narrow opening, creating friction
    // These have a "breathy" or "hissy" quality: f, h, s, v, x, z
    static constexpr std::string_view fricatives_ = "fhsvxz";

    // NASALS: Air flows through the nose
    // These have a "humming" quality: m, n
    static constexpr std::string_view nasals_ = "mn";

    // LIQUIDS: Air flows around the tongue (lateral or rhotic)
    // These have a "flowing" quality: l, r
    static constexpr std::string_view liquids_ = "lr";

    // GLIDES/APPROXIMANTS: Smooth transition, like a vowel but shorter
    // These have a "sliding" quality: w, y (represented as j in many languages)
    static constexpr std::string_view glides_ = "wj";

    // All consonants combined (for backward compatibility)
    static constexpr std::string_view consonants_ = "bcdfghjklmnpqrstvwxyz";

    // Vowels: a, e, i, o, u
    static constexpr std::string_view vowels_ = "aeiou";

    // Special endings that give names a "tech" or "modern" feel
    static constexpr std::string_view special_endings_ = "xzk";

    // Consonant blends (pre-defined natural-sounding pairs)
    std::vector<std::string> consonant_blends_;

    // Pattern-based generation rules
    // Pattern codes:
    //   C = any Consonant           P = Plosive (b,d,g,k,p,t)
    //   V = Vowel                   F = Fricative (f,h,s,v,x,z)
    //   B = Blend (sh, tr, etc.)    N = Nasal (m,n)
    //   D = Double letter           L = Liquid (l,r)
    //   S = Special ending (x,z,k)  W = glide/With flow (w,j)
    //   Q = Quality pair (smart consonant pair from different categories)
    std::vector<std::string> patterns_;

    // Helper functions for random selection
    char randomConsonant();          // Any consonant
    char randomPlosive();            // Plosive/stop only
    char randomFricative();          // Fricative only
    char randomNasal();              // Nasal only
    char randomLiquid();             // Liquid only
    char randomGlide();              // Glide only
    char randomVowel();              // Any vowel
    char randomSpecial();            // Special ending
    std::string randomBlend();       // Pre-defined blend
    std::string randomQualityPair(); // Smart consonant pair

    std::string generateFromPattern(const std::string& pattern);
    std::string capitalize(std::string str);

    // Pattern element generators (called by generateFromPattern)
    std::string generateC();  // Any Consonant
    std::string generateP();  // Plosive
    std::string generateF();  // Fricative
    std::string generateN();  // Nasal
    std::string generateL();  // Liquid
    std::string generateW();  // Glide (W = "with flow")
    std::string generateV();  // Vowel
    std::string generateB();  // Blend
    std::string generateD();  // Double letter
    std::string generateS();  // Special ending
    std::string generateQ();  // Quality pair (smart consonant pairing)
};

#endif // NAME_GENERATOR_HPP
