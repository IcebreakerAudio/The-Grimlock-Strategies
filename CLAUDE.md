# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is "The Grimlock Strategies" - a JUCE audio plugin that displays inspirational quotes from Grimlock, the Dinobot leader from the original Transformers series. The plugin serves as a quote viewer with themed UI and provides no audio processing functionality.

**Current Version:** 0.9.0

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
The plugin builds as: Standalone, AU, VST3, and LV2 formats (configured in CMakeLists.txt line 28).
Additional CLAP support is provided via clap-juce-extensions library.

## Architecture Overview

### Core Components
- **PluginProcessor** (`source/PluginProcessor.h/.cpp`): Standard JUCE AudioProcessor implementation, handles quote index state persistence
- **PluginEditor** (`source/PluginEditor.h/.cpp`): Main GUI component, manages display and user interaction
- **QuoteGenerator** (`source/QuoteGenerator.hpp`): Manages quote data parsing and random quote generation from JSON
- **ScreenDisplay** (`source/ScreenDisplay.hpp/.cpp`): Custom component for rendering quotes with associated screenshots
- **Utils** (`source/utils/GinStackBlur.hpp`): Utility functions for visual effects

### Data Management
- Quote data stored in `assets/data.json` containing episode information, quotes, and metadata
- Screenshots stored in `assets/` folder with naming convention: `{episode}_{index}.png`
- Binary data embedded via CMake Assets system using `cmake/Assets.cmake`
- Raw screenshots available in `raw_screenshots/` folder for reference

### Key Features
- Random quote generation from Transformers episodes/movie (114 quotes from 18 episodes + 1 movie)
- Episode screenshots display with themed UI reminiscent of Teletraan I
- Hyperlinks to episode videos on YouTube
- State persistence for current quote index
- Resizable UI
- Debug reroll button (debug builds only)
- Jersey10 font integration for authentic retro look

### Development Notes
- Plugin is configured as non-synth, no MIDI input required (IS_SYNTH=FALSE, NEEDS_MIDI_INPUT=FALSE)
- Company: "Icebreaker Audio"
- Bundle ID: "com.icebreakeraudio.grimlockstrats"
- Plugin codes: Manufacturer=IceB, Plugin=IAGS
- VST3 Category: "Fx Tools"
- Uses JUCE modules: core, audio_utils, gui_basics, gui_extra
- CLAP features: "audio-effect"
- Quote data follows episodic structure with nested quote arrays indexed by number keys
- Uses C++23 standard with fast math optimizations
- Project structure based on simplified pamplejuce template

### Dependencies
- JUCE Framework (included as submodule)
- clap-juce-extensions (included as submodule at `libs/clap-juce-extensions/`)
- Jersey10 font (OFL license) at `assets/Jersey10-Regular.ttf`

### File Structure
- `source/`: Main plugin source code
- `assets/`: Quote data, screenshots, fonts, and background SVG
- `cmake/`: Build system configuration files
- `build/`: Generated build directory (not in repo)
- `screenshots/`: Plugin UI screenshots for documentation
- `raw_screenshots/`: Original unprocessed episode screenshots