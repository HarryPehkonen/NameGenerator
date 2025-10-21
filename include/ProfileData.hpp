#ifndef PROFILE_DATA_HPP
#define PROFILE_DATA_HPP

#include <string>
#include <vector>
#include <map>
#include <optional>
#include <fstream>
#include <stdexcept>
#include <jsom/jsom.hpp>

// Stores data loaded from NameAnalyzer JSON output
class ProfileData {
public:
    // Weighted item for random selection
    struct WeightedItem {
        std::string value;
        int weight;
    };

    // Load profile from NameAnalyzer JSON file
    explicit ProfileData(const std::string& json_file_path);

    // Markov chain data access
    const std::map<std::string, std::vector<WeightedItem>>& getMarkovOrder1() const { return markov_order1_; }
    const std::map<std::string, std::vector<WeightedItem>>& getMarkovOrder2() const { return markov_order2_; }

    // Syllable data access
    const std::vector<WeightedItem>& getSyllablesStart() const { return syllables_start_; }
    const std::vector<WeightedItem>& getSyllablesMiddle() const { return syllables_middle_; }
    const std::vector<WeightedItem>& getSyllablesEnd() const { return syllables_end_; }
    const std::map<std::string, std::vector<WeightedItem>>& getSyllableMarkov1() const { return syllable_markov1_; }
    const std::map<std::string, std::vector<WeightedItem>>& getSyllableMarkov2() const { return syllable_markov2_; }

    // Component data access
    const std::vector<WeightedItem>& getOnsetsStart() const { return onsets_start_; }
    const std::vector<WeightedItem>& getOnsetsMiddle() const { return onsets_middle_; }
    const std::vector<WeightedItem>& getOnsetsEnd() const { return onsets_end_; }
    const std::vector<WeightedItem>& getNuclei() const { return nuclei_; }
    const std::vector<WeightedItem>& getCodas() const { return codas_; }
    const std::vector<WeightedItem>& getCodasStart() const { return codas_start_; }
    const std::vector<WeightedItem>& getCodasMiddle() const { return codas_middle_; }
    const std::vector<WeightedItem>& getCodasEnd() const { return codas_end_; }

    // N-gram data access
    const std::vector<WeightedItem>& getBigramsStart() const { return bigrams_start_; }
    const std::vector<WeightedItem>& getBigramsMiddle() const { return bigrams_middle_; }
    const std::vector<WeightedItem>& getBigramsEnd() const { return bigrams_end_; }
    const std::vector<WeightedItem>& getTrigramsStart() const { return trigrams_start_; }
    const std::vector<WeightedItem>& getTrigramsMiddle() const { return trigrams_middle_; }
    const std::vector<WeightedItem>& getTrigramsEnd() const { return trigrams_end_; }

    // Configuration metadata
    int getMarkovOrder() const { return markov_order_; }
    bool hasSyllables() const { return syllables_enabled_; }
    bool hasComponents() const { return components_enabled_; }

private:
    // Helper to convert JSON object {key: count} to vector of WeightedItems
    static std::vector<WeightedItem> jsonObjectToWeighted(const jsom::JsonDocument& obj);

    // Helper to convert JSON object {context: {next: count}} to markov map
    static std::map<std::string, std::vector<WeightedItem>> jsonObjectToMarkov(const jsom::JsonDocument& obj);

    // Markov chain data (letter-level)
    std::map<std::string, std::vector<WeightedItem>> markov_order1_;
    std::map<std::string, std::vector<WeightedItem>> markov_order2_;

    // Syllable data
    std::vector<WeightedItem> syllables_start_;
    std::vector<WeightedItem> syllables_middle_;
    std::vector<WeightedItem> syllables_end_;
    std::map<std::string, std::vector<WeightedItem>> syllable_markov1_;
    std::map<std::string, std::vector<WeightedItem>> syllable_markov2_;

    // Component data
    std::vector<WeightedItem> onsets_start_;
    std::vector<WeightedItem> onsets_middle_;
    std::vector<WeightedItem> onsets_end_;
    std::vector<WeightedItem> nuclei_;
    std::vector<WeightedItem> codas_;
    std::vector<WeightedItem> codas_start_;
    std::vector<WeightedItem> codas_middle_;
    std::vector<WeightedItem> codas_end_;

    // N-gram data
    std::vector<WeightedItem> bigrams_start_;
    std::vector<WeightedItem> bigrams_middle_;
    std::vector<WeightedItem> bigrams_end_;
    std::vector<WeightedItem> trigrams_start_;
    std::vector<WeightedItem> trigrams_middle_;
    std::vector<WeightedItem> trigrams_end_;

    // Configuration
    int markov_order_ = 2;
    bool syllables_enabled_ = false;
    bool components_enabled_ = false;
};

#endif // PROFILE_DATA_HPP
