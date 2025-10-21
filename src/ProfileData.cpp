#include "ProfileData.hpp"
#include <sstream>

ProfileData::ProfileData(const std::string& json_file_path) {
    // Read file contents
    std::ifstream file(json_file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open profile file: " + json_file_path);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json_content = buffer.str();

    // Parse JSON
    jsom::JsonDocument doc;
    try {
        doc = jsom::parse_document(json_content);
    } catch (const std::exception& e) {
        throw std::runtime_error("Failed to parse JSON: " + std::string(e.what()));
    }

    // Load configuration
    if (doc.exists("/config")) {
        auto config = doc.at("/config");
        if (config.exists("/markov_order")) {
            markov_order_ = config.at("/markov_order").as<int>();
        }
        if (config.exists("/syllables_enabled")) {
            syllables_enabled_ = config.at("/syllables_enabled").as<bool>();
        }
        if (config.exists("/components_enabled")) {
            components_enabled_ = config.at("/components_enabled").as<bool>();
        }
    }

    // Load letter-level Markov chains
    if (doc.exists("/letter_analysis/markov_chains")) {
        auto markov = doc.at("/letter_analysis/markov_chains");
        if (markov.exists("/order_1")) {
            markov_order1_ = jsonObjectToMarkov(markov.at("/order_1"));
        }
        if (markov.exists("/order_2")) {
            markov_order2_ = jsonObjectToMarkov(markov.at("/order_2"));
        }
    }

    // Load positional n-grams
    if (doc.exists("/letter_analysis/positional_bigrams")) {
        auto pos_bigrams = doc.at("/letter_analysis/positional_bigrams");
        if (pos_bigrams.exists("/start")) {
            bigrams_start_ = jsonObjectToWeighted(pos_bigrams.at("/start"));
        }
        if (pos_bigrams.exists("/middle")) {
            bigrams_middle_ = jsonObjectToWeighted(pos_bigrams.at("/middle"));
        }
        if (pos_bigrams.exists("/end")) {
            bigrams_end_ = jsonObjectToWeighted(pos_bigrams.at("/end"));
        }
    }

    if (doc.exists("/letter_analysis/positional_trigrams")) {
        auto pos_trigrams = doc.at("/letter_analysis/positional_trigrams");
        if (pos_trigrams.exists("/start")) {
            trigrams_start_ = jsonObjectToWeighted(pos_trigrams.at("/start"));
        }
        if (pos_trigrams.exists("/middle")) {
            trigrams_middle_ = jsonObjectToWeighted(pos_trigrams.at("/middle"));
        }
        if (pos_trigrams.exists("/end")) {
            trigrams_end_ = jsonObjectToWeighted(pos_trigrams.at("/end"));
        }
    }

    // Load syllable data
    if (syllables_enabled_ && doc.exists("/syllable_analysis")) {
        auto syllable = doc.at("/syllable_analysis");

        if (syllable.exists("/positional_syllables")) {
            auto pos_syl = syllable.at("/positional_syllables");
            if (pos_syl.exists("/start")) {
                syllables_start_ = jsonObjectToWeighted(pos_syl.at("/start"));
            }
            if (pos_syl.exists("/middle")) {
                syllables_middle_ = jsonObjectToWeighted(pos_syl.at("/middle"));
            }
            if (pos_syl.exists("/end")) {
                syllables_end_ = jsonObjectToWeighted(pos_syl.at("/end"));
            }
        }

        if (syllable.exists("/syllable_markov")) {
            auto syl_markov = syllable.at("/syllable_markov");
            if (syl_markov.exists("/order_1")) {
                syllable_markov1_ = jsonObjectToMarkov(syl_markov.at("/order_1"));
            }
            if (syl_markov.exists("/order_2")) {
                syllable_markov2_ = jsonObjectToMarkov(syl_markov.at("/order_2"));
            }
        }
    }

    // Load component data
    if (components_enabled_ && doc.exists("/component_analysis")) {
        auto comp = doc.at("/component_analysis");

        // Load nuclei (all positions use same nuclei)
        if (comp.exists("/frequencies/nuclei")) {
            nuclei_ = jsonObjectToWeighted(comp.at("/frequencies/nuclei"));
        }

        // Load codas (general)
        if (comp.exists("/frequencies/codas")) {
            codas_ = jsonObjectToWeighted(comp.at("/frequencies/codas"));
        }

        // Load positional onsets
        if (comp.exists("/positional_onsets")) {
            auto pos_onsets = comp.at("/positional_onsets");
            if (pos_onsets.exists("/start")) {
                onsets_start_ = jsonObjectToWeighted(pos_onsets.at("/start"));
            }
            if (pos_onsets.exists("/middle")) {
                onsets_middle_ = jsonObjectToWeighted(pos_onsets.at("/middle"));
            }
            if (pos_onsets.exists("/end")) {
                onsets_end_ = jsonObjectToWeighted(pos_onsets.at("/end"));
            }
        }

        // Load positional codas
        if (comp.exists("/positional_codas")) {
            auto pos_codas = comp.at("/positional_codas");
            if (pos_codas.exists("/start")) {
                codas_start_ = jsonObjectToWeighted(pos_codas.at("/start"));
            }
            if (pos_codas.exists("/middle")) {
                codas_middle_ = jsonObjectToWeighted(pos_codas.at("/middle"));
            }
            if (pos_codas.exists("/end")) {
                codas_end_ = jsonObjectToWeighted(pos_codas.at("/end"));
            }
        }
    }
}

std::vector<ProfileData::WeightedItem> ProfileData::jsonObjectToWeighted(const jsom::JsonDocument& obj) {
    std::vector<WeightedItem> result;

    if (!obj.is_object()) {
        return result;
    }

    // JSOM doesn't expose object iteration directly, so we'll use the paths
    // For each key in the object, get the value
    // We need to access the object members through the variant
    const auto& obj_map = obj.as<std::map<std::string, jsom::JsonDocument>>();

    for (const auto& [key, value] : obj_map) {
        if (value.is_number()) {
            result.push_back({key, value.as<int>()});
        }
    }

    return result;
}

std::map<std::string, std::vector<ProfileData::WeightedItem>> ProfileData::jsonObjectToMarkov(const jsom::JsonDocument& obj) {
    std::map<std::string, std::vector<WeightedItem>> result;

    if (!obj.is_object()) {
        return result;
    }

    const auto& obj_map = obj.as<std::map<std::string, jsom::JsonDocument>>();

    for (const auto& [context, transitions] : obj_map) {
        if (transitions.is_object()) {
            result[context] = jsonObjectToWeighted(transitions);
        }
    }

    return result;
}
