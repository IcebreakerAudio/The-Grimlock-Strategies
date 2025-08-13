# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is "The Grimlock Strategies" - a JUCE audio plugin that displays inspirational quotes from Grimlock, the Dinobot leader from the original Transformers series. The plugin serves as a quote viewer with themed UI and provides no audio processing functionality.

## Build System

This project uses CMake with JUCE framework:

### Build Commands
```bash
# Configure build (from project root)
cmake -B build

# Build the project
cmake --build build

# Build specific configurations
cmake --build build --config Debug
cmake --build build --config Release
```

### Plugin Formats
The plugin builds as: Standalone, AU, VST3, and LV2 formats (configured in CMakeLists.txt line 21).

## Architecture Overview

### Core Components
- **PluginProcessor** (`source/PluginProcessor.h/.cpp`): Standard JUCE AudioProcessor implementation, handles quote index state persistence
- **PluginEditor** (`source/PluginEditor.h/.cpp`): Main GUI component, manages display and user interaction
- **QuoteGenerator** (`source/QuoteGenerator.hpp`): Manages quote data parsing and random quote generation from JSON
- **ScreenDisplay** (`source/ScreenDisplay.hpp/.cpp`): Custom component for rendering quotes with associated screenshots

### Data Management
- Quote data stored in `assets/data.json` containing episode information, quotes, and metadata
- Screenshots stored in `assets/` folder with naming convention: `{episode}_{index}.png`
- Binary data embedded via CMake Assets system

### Key Features
- Random quote generation from Transformers episodes/movie
- Episode screenshots display
- Hyperlinks to episode videos
- State persistence for current quote index
- Debug reroll button (debug builds only)

### Development Notes
- Plugin is configured as non-synth, no MIDI input required
- Company: "Icebreaker Audio"
- Bundle ID: "com.icebreakeraudio.grimlockstrats"
- Uses JUCE modules: core, audio_utils, audio_processors, dsp, gui_basics, gui_extra
- Quote data follows episodic structure with nested quote arrays indexed by number keys