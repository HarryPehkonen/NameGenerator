#ifndef NAME_GENERATOR_HPP
#define NAME_GENERATOR_HPP

#include <string>
#include <vector>
#include <random>
#include <memory>
#include "ProfileData.hpp"

struct NameWithPattern {
    std::string name;
    std::string pattern;
};

enum class GenerationStrategy {
    Legacy,      // Original pattern-based generation
    Markov1,     // First-order Markov chains
    Markov2,     // Second-order Markov chains (default)
    Syllable,    // Syllable-based generation
    Component,   // Onset + nucleus + coda assembly
    NGram,       // Positional n-gram sampling
    Random       // Random strategy each time
};

class NameGenerator {
public:
    NameGenerator();

    // Load a profile for data-driven generation
    void loadProfile(const std::string& profile_path);

    // Load a second profile for blending (optional)
    void loadSecondProfile(const std::string& profile_path);

    // Set generation strategy (only applies when profile is loaded)
    void setStrategy(GenerationStrategy strategy);

    // Set min/max length constraints (0 = unbounded)
    void setMinLength(size_t min);
    void setMaxLength(size_t max);

    // Generate a single name
    std::string generate();

    // Generate a single name with pattern/strategy information
    NameWithPattern generateWithPattern();

    // Generate multiple names
    std::vector<std::string> generate(size_t count);

    // Generate multiple names with pattern/strategy information
    std::vector<NameWithPattern> generateWithPattern(size_t count);

    // Seed the random number generator
    void seed(unsigned int seed);

private:
    std::mt19937 rng_;

    // Profile-based generation
    std::unique_ptr<ProfileData> profile_;
    std::unique_ptr<ProfileData> profile2_;  // Optional second profile for blending
    GenerationStrategy strategy_ = GenerationStrategy::Markov2;
    size_t min_length_ = 0;
    size_t max_length_ = 0;

    // Profile-based generation methods
    std::string generateFromProfile();
    std::string generateMarkov1();
    std::string generateMarkov2();
    std::string generateSyllable();
    std::string generateComponent();
    std::string generateNGram();

    // Helper: weighted random selection
    std::string selectWeighted(const std::vector<ProfileData::WeightedItem>& items);

    // Helper: get random blend point (1 or 2)
    int getBlendPoint();

    // ===== LEGACY PATTERN-BASED GENERATION =====

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
