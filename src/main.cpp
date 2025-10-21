#include "NameGenerator.hpp"
#include <iostream>
#include <string>
#include <cstdlib>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [count] [options]\n"
              << "\n"
              << "Arguments:\n"
              << "  count                   Number of names to generate (default: 10)\n"
              << "\n"
              << "Options:\n"
              << "  --profile <file>        Load NameAnalyzer JSON profile\n"
              << "  --strategy <name>       Generation strategy (default: markov2)\n"
              << "                          Strategies: markov1, markov2, syllable,\n"
              << "                                     component, ngram, random, legacy\n"
              << "  --min-length <n>        Minimum name length (default: unbounded)\n"
              << "  --max-length <n>        Maximum name length (default: unbounded)\n"
              << "  --debug, -d             Show strategy/pattern used for each name\n"
              << "  --help, -h              Show this help message\n"
              << "\n"
              << "Examples:\n"
              << "  " << programName << "                                    # Legacy mode, 10 names\n"
              << "  " << programName << " 20                                  # Legacy mode, 20 names\n"
              << "  " << programName << " 15 --profile greek.json              # Use Greek profile\n"
              << "  " << programName << " 10 --profile greek.json --strategy syllable\n"
              << "  " << programName << " 20 --profile norse.json --min-length 5 --max-length 10\n"
              << "  " << programName << " 10 --profile greek.json --strategy random --debug\n";
}

int main(int argc, char* argv[]) {
    size_t count = 10;
    bool debug = false;
    std::string profile_path;
    GenerationStrategy strategy = GenerationStrategy::Markov2;
    size_t min_length = 0;
    size_t max_length = 0;

    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--debug" || arg == "-d") {
            debug = true;
        } else if (arg == "--profile") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --profile requires a file path\n";
                return 1;
            }
            profile_path = argv[++i];
        } else if (arg == "--strategy") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --strategy requires a strategy name\n";
                return 1;
            }
            std::string strategy_name = argv[++i];
            if (strategy_name == "markov1") {
                strategy = GenerationStrategy::Markov1;
            } else if (strategy_name == "markov2") {
                strategy = GenerationStrategy::Markov2;
            } else if (strategy_name == "syllable") {
                strategy = GenerationStrategy::Syllable;
            } else if (strategy_name == "component") {
                strategy = GenerationStrategy::Component;
            } else if (strategy_name == "ngram") {
                strategy = GenerationStrategy::NGram;
            } else if (strategy_name == "random") {
                strategy = GenerationStrategy::Random;
            } else if (strategy_name == "legacy") {
                strategy = GenerationStrategy::Legacy;
            } else {
                std::cerr << "Error: Unknown strategy '" << strategy_name << "'\n";
                std::cerr << "Valid strategies: markov1, markov2, syllable, component, ngram, random, legacy\n";
                return 1;
            }
        } else if (arg == "--min-length") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --min-length requires a number\n";
                return 1;
            }
            try {
                min_length = std::stoull(argv[++i]);
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid min-length value\n";
                return 1;
            }
        } else if (arg == "--max-length") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --max-length requires a number\n";
                return 1;
            }
            try {
                max_length = std::stoull(argv[++i]);
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid max-length value\n";
                return 1;
            }
        } else {
            // Try to parse as count
            try {
                count = std::stoull(arg);
                if (count == 0) {
                    std::cerr << "Error: Count must be greater than 0\n";
                    return 1;
                }
                if (count > 10000) {
                    std::cerr << "Error: Count is too large (max 10000)\n";
                    return 1;
                }
            } catch (const std::exception&) {
                std::cerr << "Error: Invalid argument '" << arg << "'\n";
                printUsage(argv[0]);
                return 1;
            }
        }
    }

    // Create generator
    NameGenerator generator;

    // Load profile if specified
    if (!profile_path.empty()) {
        try {
            generator.loadProfile(profile_path);
            generator.setStrategy(strategy);
            generator.setMinLength(min_length);
            generator.setMaxLength(max_length);
        } catch (const std::exception& e) {
            std::cerr << "Error loading profile: " << e.what() << '\n';
            return 1;
        }
    }

    // Generate names
    if (debug) {
        // Generate with pattern/strategy information
        auto results = generator.generateWithPattern(count);

        // Print names with patterns/strategies
        for (const auto& result : results) {
            std::cout << result.name << " [" << result.pattern << "]\n";
        }
    } else {
        // Generate names only
        auto names = generator.generate(count);

        // Print all generated names
        for (const auto& name : names) {
            std::cout << name << '\n';
        }
    }

    return 0;
}
