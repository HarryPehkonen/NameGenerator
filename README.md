# NameGenerator

A flexible C++20 name generator with dual modes:
- **Profile Mode**: Data-driven generation using statistical analysis from [NameAnalyzer](https://github.com/HarryPehkonen/NameAnalyzer)
- **Legacy Mode**: Pattern-based generation using phonetic rules

Generate names with different flavors (Greek, Norse, Fantasy, etc.) by analyzing real word lists and using the statistical patterns to create authentic-sounding new names.

## Features

- **Multiple generation strategies**: Markov chains, syllable assembly, component-based (onset/nucleus/coda), n-gram sampling
- **Data-driven profiles**: Load JSON profiles created by NameAnalyzer for themed name generation
- **Profile blending**: Combine two profiles to create hybrid names (e.g., Norse + Japanese, Greek + Egyptian)
- **Flexible constraints**: Set min/max length limits
- **Zero external dependencies** (except JSOM, auto-fetched by CMake)
- **Backward compatible**: Legacy pattern-based mode still works

## Building

### Requirements
- C++20 compatible compiler (GCC 10+, Clang 10+, MSVC 2019+)
- CMake 3.15+

### Quick Build
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The executable will be at `./build/namegen`

## Usage

### Basic Syntax
```bash
./build/namegen [count] [options]
```

### Options
- `count` - Number of names to generate (default: 10)
- `--profile <file>` - Load NameAnalyzer JSON profile
- `--profile2 <file>` - Load second profile for blending (optional)
- `--strategy <name>` - Generation strategy (default: markov2)
  - Strategies: `markov1`, `markov2`, `syllable`, `component`, `ngram`, `random`, `legacy`
- `--min-length <n>` - Minimum name length (default: unbounded)
- `--max-length <n>` - Maximum name length (default: unbounded)
- `--debug`, `-d` - Show strategy/pattern used for each name
- `--help`, `-h` - Show help message

## Quick Start Examples

### Legacy Mode (Pattern-Based)
Generate names without a profile using built-in phonetic patterns:

```bash
# Generate 10 names using legacy patterns
./build/namegen

# Generate 20 names
./build/namegen 20

# Show the pattern used for each name
./build/namegen 10 --debug
```

**Example output:**
```
Keooo
Rol
Qora
Iceo
Kumti
```

### Profile Mode (Data-Driven)

First, create a profile using NameAnalyzer, then generate names from it:

```bash
# Generate 15 names from Greek mythology profile
./build/namegen 15 --profile greek.json

# Try different generation strategies
./build/namegen 10 --profile greek.json --strategy syllable
./build/namegen 10 --profile greek.json --strategy component
./build/namegen 10 --profile greek.json --strategy markov1

# Random strategy (picks different strategy each time)
./build/namegen 20 --profile greek.json --strategy random

# Apply length constraints
./build/namegen 20 --profile norse.json --min-length 5 --max-length 10

# Debug mode shows which strategy was used
./build/namegen 10 --profile fantasy.json --strategy random --debug
```

## Generation Strategies Explained

When using a profile, you can choose different generation strategies:

### markov2 (Default - Recommended)
Second-order Markov chains - looks at the previous 2 letters to predict the next letter. Produces balanced, natural-looking names.

```bash
./build/namegen 10 --profile NameAnalyzerTestOutput.json --strategy markov2
```

**Example output:**
```
Dragon
Apollo
Stream
Poseidong
Artemis
```

### markov1
First-order Markov chains - looks at only the previous 1 letter. More chaotic and creative, less constrained by the original corpus.

```bash
./build/namegen 10 --profile NameAnalyzerTestOutput.json --strategy markov1
```

**Example output:**
```
Theus
Agong
Thereus
Seideme
```

### syllable
Chains syllables together using syllable-level Markov chains. Tends to produce shorter names with clear syllable boundaries.

```bash
./build/namegen 10 --profile NameAnalyzerTestOutput.json --strategy syllable
```

**Example output:**
```
Strong
Thun
Dra
Thro
```

### component
Assembles syllables from onset (initial consonants) + nucleus (vowel) + coda (final consonants). Linguistically sound approach.

```bash
./build/namegen 10 --profile NameAnalyzerTestOutput.json --strategy component
```

**Example output:**
```
Streursahe
Threutgen
Strength
Eungth
```

### ngram
Samples positional n-grams (bigrams and trigrams) from start/middle/end positions. Can produce longer, more complex names.

```bash
./build/namegen 10 --profile NameAnalyzerTestOutput.json --strategy ngram
```

**Example output:**
```
Posrcrinoneeus
Athomgth
Thuragthoseing
```

### random
Randomly picks a different strategy for each name. Great for experimentation!

```bash
./build/namegen 15 --profile NameAnalyzerTestOutput.json --strategy random --debug
```

**Example output:**
```
Amtehan [random]
Strometheus [random]
String [random]
Apoeidenraus [random]
```

## Profile Blending

**NEW!** You can now blend two profiles to create hybrid names that combine characteristics from different cultures or themes. The first 1-2 syllables (randomly chosen) come from the first profile, and the rest comes from the second profile.

### How Blending Works

Different strategies blend profiles in different ways:

- **markov1/markov2**: Switch from first profile's Markov chains to second profile's chains after 3-5 characters
- **syllable**: Use 1-2 syllables from first profile, remaining syllables from second profile
- **component**: Use 1-2 onset/nucleus/coda components from first profile, rest from second
- **ngram**: Use starting n-grams from first profile, middle/ending n-grams from second
- **legacy**: Doesn't support blending (shows warning, uses first profile only)

### Blending Examples

```bash
# Norse beginnings + Japanese endings
./build/namegen 20 --profile norse.json --profile2 japanese.json

# Greek beginnings + Egyptian endings
./build/namegen 15 --profile greek.json --profile2 egyptian.json --strategy syllable

# Celtic + Fantasy word blending
./build/namegen 20 --profile celtic.json --profile2 fantasy.json --strategy component

# Random blending with debug output
./build/namegen 10 --profile norse.json --profile2 japanese.json --strategy random --debug
```

### Expected Blended Results

When blending profiles, you get unique hybrid names:

**Norse + Japanese** (syllable strategy):
```
Thorami
Lokitsuki
Odinara
Freyako
Heimdatsumi
```

**Greek + Egyptian** (markov2 strategy):
```
Zeushotep
Athenutet
Poseidra
Heraset
Apollothoth
```

**Celtic + Fantasy** (component strategy):
```
Brighflame
Lughshadow
Morriastorm
Cernofrost
Dagdacrystal
```

### Use Cases for Blending

- **Cross-cultural names**: Combine mythologies (Norse-Japanese, Greek-Celtic, etc.)
- **Fantasy world-building**: Mix themes (dragon names with Celtic roots, Norse-inspired magic terms)
- **Brand names**: Blend familiar + exotic (English-Japanese, Latin-Fantasy)
- **Character names**: Create unique identities by mixing cultural influences
- **Project names**: Combine technical + mythological themes

### Tips for Blending

1. **Complementary sounds**: Profiles with similar phonetic patterns blend more smoothly
2. **Contrast for creativity**: Very different profiles (Greek + Japanese) create more unique results
3. **Try different strategies**: Syllable blending is most natural, component is most creative
4. **Use constraints**: Add `--min-length` and `--max-length` to control output
5. **Experiment freely**: There are no wrong combinations - unexpected blends often produce the best results!

## Creating Profile Files with NameAnalyzer

To use profile-based generation, you first need to create a statistical profile using NameAnalyzer. See the [NameAnalyzer documentation](NameAnalyzerREADME.md) for full details.

### Step 1: Create a Word List

Create a text file with one word per line. Words should represent the "flavor" you want your names to have.

**Example: `greek_names.txt`**
```
zeus
athena
apollo
artemis
poseidon
prometheus
hercules
```

**Example: `fantasy_words.txt`**
```
dragon
shadow
thunder
storm
flame
crystal
mystic
```

**Example: `norse_names.txt`**
```
thor
odin
loki
freya
tyr
heimdall
```

### Step 2: Analyze with NameAnalyzer

Use NameAnalyzer to extract statistical patterns from your word list:

```bash
# Basic analysis (letter-level only)
./build/nameanalyzer greek_names.txt -o greek.json

# Full analysis with syllables and components
./build/nameanalyzer greek_names.txt -o greek_profile.json \
  --markov-order 2 \
  --enable-syllables \
  --enable-components \
  -v

# Analyze fantasy words with higher-order Markov chains
./build/nameanalyzer fantasy_words.txt -o fantasy.json \
  --markov-order 3 \
  --enable-syllables \
  --enable-components

# Norse names with syllable analysis
./build/nameanalyzer norse_names.txt -o norse.json \
  --markov-order 2 \
  --enable-syllables
```

### Step 3: Generate Names from Profile

```bash
# Use the profile you just created
./build/namegen 20 --profile greek_profile.json

# Try different strategies
./build/namegen 15 --profile fantasy.json --strategy component
./build/namegen 15 --profile norse.json --strategy syllable

# Apply constraints
./build/namegen 20 --profile greek_profile.json --min-length 6 --max-length 12
```

## Complete Workflow Example

Here's a complete example creating a Greek mythology name generator:

```bash
# 1. Create a word list (or download one)
cat > greek_gods.txt << EOF
zeus
hera
poseidon
demeter
athena
apollo
artemis
ares
hephaestus
aphrodite
hermes
dionysus
hades
persephone
prometheus
atlas
hercules
perseus
theseus
odysseus
EOF

# 2. Analyze with NameAnalyzer to create a profile
./build/nameanalyzer greek_gods.txt -o profiles/greek.json \
  --markov-order 2 \
  --enable-syllables \
  --enable-components \
  -v

# 3. Generate Greek-themed names!
./build/namegen 20 --profile profiles/greek.json

# 4. Experiment with different strategies
./build/namegen 15 --profile profiles/greek.json --strategy syllable
./build/namegen 15 --profile profiles/greek.json --strategy component
./build/namegen 15 --profile profiles/greek.json --strategy random

# 5. Apply length constraints for shorter names
./build/namegen 20 --profile profiles/greek.json --min-length 4 --max-length 8
```

## Tips for Best Results

### Creating Good Word Lists

1. **Larger is better**: 50+ words minimum, 500+ ideal
2. **Thematic consistency**: Keep words related to achieve consistent style
3. **Mix lengths**: Include short, medium, and long words for variety
4. **Combine sources**: Merge related word lists for richer patterns

```bash
# Combine multiple Greek sources
cat greek_gods.txt greek_heroes.txt greek_places.txt > all_greek.txt
./build/nameanalyzer all_greek.txt -o greek_complete.json \
  --markov-order 2 --enable-syllables --enable-components
```

### Choosing Markov Order

- **Order 1**: More random, creative, less faithful to source
- **Order 2**: Balanced (recommended for most uses)
- **Order 3**: More faithful to source, less variation

### Strategy Selection Guide

- **markov2**: Best all-around choice, natural-sounding
- **syllable**: Good for shorter, punchier names
- **component**: Good for creating pronounceable novel combinations
- **ngram**: Good for longer, complex names
- **markov1**: Good for maximum creativity/chaos
- **random**: Good for exploration and experimentation

### Length Constraints

Use `--min-length` and `--max-length` to control output:

```bash
# Short punchy names (4-6 letters)
./build/namegen 20 --profile fantasy.json --min-length 4 --max-length 6

# Medium names (6-10 letters)
./build/namegen 20 --profile greek.json --min-length 6 --max-length 10

# Longer dramatic names (10-15 letters)
./build/namegen 20 --profile norse.json --min-length 10 --max-length 15
```

## Finding Word Lists

Good sources for themed word lists:

- **Mythology databases**: Greek, Norse, Egyptian, Celtic, Japanese
- **Fantasy literature**: Character names from fantasy novels
- **Place names**: Cities, mountains, rivers, regions
- **Dictionary filtering**: Filter words by theme or pattern
- **Historical databases**: Historical figures, ancient names
- **Online name databases**: Behind the Name, Fantasy Name Generators

## Example: Creating Multiple Themed Profiles

```bash
# Create a profiles directory
mkdir -p profiles

# Greek mythology
./build/nameanalyzer data/greek_names.txt -o profiles/greek.json \
  --markov-order 2 --enable-syllables --enable-components

# Norse mythology
./build/nameanalyzer data/norse_names.txt -o profiles/norse.json \
  --markov-order 2 --enable-syllables --enable-components

# Fantasy words
./build/nameanalyzer data/fantasy_words.txt -o profiles/fantasy.json \
  --markov-order 3 --enable-syllables --enable-components

# Celtic names
./build/nameanalyzer data/celtic_names.txt -o profiles/celtic.json \
  --markov-order 2 --enable-syllables --enable-components

# Test them all!
echo "=== Greek ===" && ./build/namegen 5 --profile profiles/greek.json
echo "=== Norse ===" && ./build/namegen 5 --profile profiles/norse.json
echo "=== Fantasy ===" && ./build/namegen 5 --profile profiles/fantasy.json
echo "=== Celtic ===" && ./build/namegen 5 --profile profiles/celtic.json
```

## Understanding the Output

### Without Debug Mode
```bash
./build/namegen 5 --profile greek.json
```
```
Poseidon
Athemon
Zeusara
Heradon
Apollonis
```

### With Debug Mode
```bash
./build/namegen 5 --profile greek.json --debug
```
```
Poseidon [markov2]
Athemon [markov2]
Zeusara [markov2]
Heradon [markov2]
Apollonis [markov2]
```

Debug mode shows which strategy was used to generate each name. Useful when using `--strategy random`.

## Troubleshooting

**Names seem random/unpronounceable**
- Your word list might be too small (< 20 words)
- Try a different strategy (`syllable` or `component` are more structured)
- Apply length constraints to filter out extreme outliers
- Increase your corpus size for richer patterns

**All names look similar to source words**
- Try `markov1` for more creativity
- Use `--strategy random` for variety
- Lower the Markov order when creating the profile
- Mix multiple source corpora together

**Names are too short/too long**
- Use `--min-length` and `--max-length` constraints
- Adjust the Markov order (higher = longer names)
- Try different strategies (syllable = shorter, ngram = longer)

**Error loading profile**
- Check the file path is correct
- Verify JSON is valid: `python3 -m json.tool profile.json`
- Ensure profile was created with NameAnalyzer
- Re-generate profile if it's from an older version

## Advanced Usage

### Batch Generation with Shell Scripts

```bash
#!/bin/bash
# generate_many.sh - Generate multiple sets of names

for strategy in markov1 markov2 syllable component ngram; do
    echo "=== $strategy ==="
    ./build/namegen 10 --profile profiles/greek.json --strategy "$strategy"
    echo ""
done
```

### Filtering Output

```bash
# Only names between 5-8 characters
./build/namegen 100 --profile greek.json --min-length 5 --max-length 8 > names.txt

# Remove duplicates
./build/namegen 200 --profile greek.json | sort -u > unique_names.txt

# Filter for names starting with specific letter
./build/namegen 100 --profile greek.json | grep '^A'
```

### Combining Profiles

While NameGenerator doesn't merge profiles directly, you can merge source word lists:

```bash
# Combine multiple themed word lists
cat greek_names.txt norse_names.txt celtic_names.txt > combined.txt

# Create a hybrid profile
./build/nameanalyzer combined.txt -o profiles/mythology_mix.json \
  --markov-order 2 --enable-syllables --enable-components

# Generate hybrid mythological names
./build/namegen 20 --profile profiles/mythology_mix.json
```

## See Also

- **NameAnalyzer** - Companion tool for creating statistical profiles from word lists
- [NameAnalyzer Documentation](NameAnalyzerREADME.md) - Full guide to creating profiles

## License

See LICENSE file for details.

## Contributing

When creating interesting profiles or discovering useful word lists:
- Share your findings!
- Document which strategies work best for your use case
- Contribute word lists that produce good results
