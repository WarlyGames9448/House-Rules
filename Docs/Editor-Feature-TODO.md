# Fuze Editor --- Feature TODO Roadmap

**Project:** Fuze\
**Purpose:** Document potential editor workspaces and tools for the 2D
pixel-art metroidvania.\
**Status:** Planning backlog --- all items are proposals, not
implemented features.\
**UI framework:** Dear ImGui\
**Priority rule:** Priorities reflect dependencies and practical value
for building, testing, and debugging the game. They do not prescribe
where a tool must appear in the UI.

## Scope and design principles

-   These entries describe **editor capabilities**, not necessarily 52
    separate tabs. A capability may be a dockable window, a mode within
    another window, a contextual panel, or a command.
-   Build tools around engine capabilities that actually exist. Avoid
    designing elaborate editor panels before their underlying runtime
    systems are available.
-   Prefer a small, usable core editor first, then expand toward level
    authoring, combat, puzzles, narrative, and production workflows.
-   Keep the editor useful for a 2D pixel-art metroidvania:
    interconnected rooms, exploration gates, puzzles, challenging
    bosses, animation, and past/present versions of the world.
-   Revisit priorities as the engine architecture and game requirements
    become clearer.

# TODO Backlog

## Editor foundation

### \[ \] 01. Scene View --- Scene Editor

**Purpose:** Display and edit the current 2D scene.

**Technical utility:** - Render the scene inside an editor viewport. -
Support object selection, transform gizmos, camera navigation, and scene
overlays. - Provide the main workspace for placing and inspecting world
content. - Expose editor-only visualization without changing the game's
runtime rendering.

### \[ \] 02. Hierarchy --- Scene Hierarchy

**Purpose:** Show the objects/entities in the current scene as a
navigable tree.

**Technical utility:** - List entities and their parent-child
relationships. - Support selection, renaming, creation, deletion, and
reparenting. - Keep the selected entity synchronized with the Inspector
and Scene View. - Help organize complex rooms and object groups.

### \[ \] 03. Inspector --- Property Inspector

**Purpose:** Edit the selected entity's properties and components.

**Technical utility:** - Display editable component fields such as
transforms, sprites, colliders, scripts, and gameplay parameters. -
Validate values and expose appropriate controls for each property
type. - Provide a common UI for inspecting runtime and serialized
data. - Reduce the need to edit scene data manually.

### \[ \] 04. Game View --- Runtime Preview

**Purpose:** Show the game from the player's camera perspective.

**Technical utility:** - Preview the actual camera output at a chosen
resolution or aspect ratio. - Support play, pause, and stop workflows
when runtime integration is available. - Verify framing, HUD placement,
pixel scaling, and gameplay readability. - Distinguish the runtime view
from the editor's Scene View.

### \[ \] 05. Asset Browser --- Project Asset Browser

**Purpose:** Browse and open files belonging to the game project.

**Technical utility:** - Navigate project directories and identify
supported asset types. - Provide search, filtering, thumbnails, and
basic previews. - Open assets in suitable editors or select them for
assignment to components. - Make it easier to manage sprites, tilesets,
audio, fonts, scenes, and scripts.

### \[ \] 06. Console --- Output Console

**Purpose:** Display engine, editor, script, and runtime messages.

**Technical utility:** - Show logs, warnings, and errors with severity
and timestamps where useful. - Support filtering, clearing, and copying
messages. - Link messages to source locations when such metadata is
available. - Make runtime failures and content issues visible during
development.

### \[ \] 07. Project Settings --- Project Configuration

**Purpose:** Configure project-wide settings.

**Technical utility:** - Store settings such as game resolution, target
frame rate, input configuration, rendering options, and asset paths. -
Validate configuration and serialize it to project files. - Separate
project-wide configuration from per-scene or per-entity properties. -
Expose only settings supported by the engine.

## Core 2D authoring and debugging

### \[ \] 08. Tilemap Editor --- Tilemap Authoring

**Purpose:** Build rooms and terrain from reusable tiles.

**Technical utility:** - Paint, erase, fill, sample, and select tiles on
a grid. - Support multiple tile layers and grid configuration. - Store
tilemap data in a format the runtime can load efficiently. - Provide the
main workflow for constructing pixel-art environments.

### \[ \] 09. Tileset Editor --- Tileset Management

**Purpose:** Import, slice, organize, and configure tiles.

**Technical utility:** - Define tile dimensions and regions within
source textures. - Associate tile metadata such as collision, terrain
type, or material. - Support tile palettes and, if implemented,
autotiling rules. - Keep source texture coordinates and tile identifiers
consistent.

### \[ \] 10. Collision Editor --- Collision Authoring

**Purpose:** Configure collision geometry and interaction volumes.

**Technical utility:** - Create and edit solid colliders, trigger
volumes, hitboxes, and hurtboxes. - Visualize collision shapes over the
scene. - Configure collision layers, masks, and relevant physics
properties. - Make collision data inspectable without relying on sprite
boundaries.

### \[ \] 11. Level Design Tools --- Level Placement Tools

**Purpose:** Place and configure common gameplay objects in a room.

**Technical utility:** - Create spawn points, checkpoints, doors,
hazards, moving platforms, and interactables. - Provide reusable
placement tools and scene templates. - Validate required references and
common setup mistakes. - Reduce repetitive manual entity setup.

### \[ \] 12. Camera Editor --- Camera Configuration

**Purpose:** Configure how the game camera frames and follows the
player.

**Technical utility:** - Set camera bounds, follow targets, dead zones,
smoothing, and room limits where supported. - Preview camera framing and
transitions. - Prevent the camera from exposing areas outside a room. -
Support deliberate framing for exploration and boss arenas.

### \[ \] 13. Debug Visualization --- Debug Overlays

**Purpose:** Visualize internal game data directly in the scene.

**Technical utility:** - Draw collision shapes, hitboxes, hurtboxes,
trigger regions, camera bounds, vectors, and entity origins. - Toggle
individual overlay categories. - Help diagnose movement, physics,
combat, and spatial issues. - Keep debug rendering separate from final
game visuals.

### \[ \] 14. Sprite & Texture Editor --- Sprite and Texture Import

**Purpose:** Inspect and configure image assets used by the game.

**Technical utility:** - Preview textures and sprite regions. -
Configure slicing, pivots, filtering, wrapping, and import settings
where supported. - Verify pixel-art assets remain crisp at intended
scales. - Provide consistent texture metadata to renderers and animation
tools.

### \[ \] 15. Animation Editor --- Sprite Animation

**Purpose:** Create and inspect frame-based sprite animations.

**Technical utility:** - Define animation clips, frame sequences, frame
durations, and playback settings. - Preview idle, run, jump, attack,
damage, and death animations. - Support frame events if the animation
system provides them. - Ensure animation data can be serialized and
consumed by runtime entities.

### \[ \] 16. Character Controller Debugger --- Movement Debugging

**Purpose:** Inspect the player and other character controllers.

**Technical utility:** - Display velocity, grounded state, movement
mode, jump state, and collision results. - Help tune acceleration,
gravity, jump height, and air control. - Reveal issues such as missed
ground checks or unexpected collision responses. - Depend on diagnostic
data exposed by the controller implementation.

### \[ \] 17. Import Settings --- Asset Import Configuration

**Purpose:** Configure how source assets are processed.

**Technical utility:** - Store per-asset import options for textures,
audio, fonts, and other supported formats. - Expose compression,
filtering, slicing, and processing options where available. - Show
import errors and allow assets to be reprocessed. - Keep source files
distinct from generated or cached resources.

### \[ \] 18. Interaction Editor --- Interaction Configuration

**Purpose:** Configure how the player interacts with world objects.

**Technical utility:** - Define interaction prompts, range, activation
conditions, and resulting actions. - Connect objects such as doors,
signs, levers, and NPCs to supported interaction logic. - Validate
references to events or scripts. - Provide reusable interaction
configurations rather than one-off code for every object.

### \[ \] 19. Trigger & Event Editor --- Trigger and Event Setup

**Purpose:** Configure events activated by gameplay conditions.

**Technical utility:** - Associate trigger volumes or event sources with
actions. - Support conditions such as entering a region, interacting
with an object, or defeating an enemy. - Inspect event links and missing
references. - Provide a foundation for scripted encounters, secrets,
checkpoints, and room transitions.

## Metroidvania gameplay systems

### \[ \] 20. World Map Editor --- Room Connectivity Map

**Purpose:** Visualize the game's rooms, regions, and connections.

**Technical utility:** - Represent rooms as nodes and transitions as
edges. - Associate exits and entrances with their target rooms. -
Identify disconnected rooms, missing destinations, and navigation dead
ends. - Help manage the interconnected world structure typical of a
metroidvania.

### \[ \] 21. Progression Editor --- Ability Gates and Progression

**Purpose:** Model the abilities, keys, and conditions that gate
exploration.

**Technical utility:** - Associate access requirements with doors,
routes, or regions. - Represent dependencies between abilities, items,
events, and locations. - Check whether required progression paths are
reachable. - Help prevent accidental sequence breaks or softlocks, while
preserving intended optional routes.

### \[ \] 22. Combat Editor --- Combat Parameters

**Purpose:** Configure attacks and combat interactions.

**Technical utility:** - Edit damage, range, knockback, cooldowns,
invulnerability windows, and recovery timing. - Configure attack
hitboxes and target hurtboxes. - Preview combat geometry and timing
where supported. - Centralize balance parameters that would otherwise be
scattered across code.

### \[ \] 23. AI Editor --- Enemy Behavior

**Purpose:** Configure enemy decision-making and behavior.

**Technical utility:** - Inspect or author state machines, behavior
trees, or other AI representations supported by the engine. - Configure
patrol, detection, pursuit, attack, retreat, and recovery behavior. -
Visualize current AI state during runtime. - Keep the editor
representation aligned with the actual runtime AI implementation.

### \[ \] 24. Puzzle Editor --- Puzzle Authoring

**Purpose:** Configure environmental puzzles and mechanisms.

**Technical utility:** - Author switches, pressure plates, movable
blocks, doors, timed mechanisms, and puzzle conditions. - Connect puzzle
inputs to state changes and outputs. - Preview puzzle state and reset
behavior. - Validate missing connections and impossible configurations
where feasible.

### \[ \] 25. Logic Graph --- Visual Logic Graph

**Purpose:** Connect events, conditions, and actions using a node graph.

**Technical utility:** - Represent supported logic as nodes and typed
connections. - Validate graph connections, missing inputs, and invalid
node configurations. - Serialize graphs and execute them through a
defined runtime system. - Potentially let designers create simple
mechanics without writing a dedicated script for every object.

### \[ \] 26. Gameplay Data Editor --- Gameplay Data

**Purpose:** Edit reusable gameplay parameters for entities and systems.

**Technical utility:** - Edit health, damage, movement values,
resistances, rewards, and similar data. - Store reusable definitions
separately from individual scene instances. - Validate ranges and
references. - Support balancing many enemies or objects consistently.

### \[ \] 27. Item & Ability Editor --- Items and Abilities

**Purpose:** Define collectible items, keys, equipment, and exploration
abilities.

**Technical utility:** - Store item identifiers, icons, descriptions,
effects, and unlock conditions. - Define ability parameters and links to
progression requirements. - Support data-driven item definitions where
the engine architecture allows it. - Keep item identity separate from
the logic that consumes or grants it.

### \[ \] 28. Animation State Machine --- Animation State Graph

**Purpose:** Define transitions between animation states.

**Technical utility:** - Configure states, transitions, conditions, and
parameters. - Represent transitions such as idle-to-run, run-to-attack,
and damage-to-recovery. - Validate unreachable states and invalid
transitions. - Integrate with the runtime animation controller rather
than duplicating its behavior.

### \[ \] 29. Boss Pattern Editor --- Boss Attack Patterns

**Purpose:** Author multi-phase boss encounters and attack sequences.

**Technical utility:** - Define attack patterns, phase conditions,
projectile sequences, cooldowns, and recovery windows. - Configure
transitions based on health, elapsed time, or encounter state. - Preview
attack timing and arena coverage. - Support synchronized or rhythm-based
attacks if the runtime systems expose timing events.

### \[ \] 30. Element & Status Editor --- Elements and Status Effects

**Purpose:** Configure elemental properties and gameplay status effects.

**Technical utility:** - Define resistances, weaknesses, damage types,
and status effect parameters. - Specify how effects interact with
enemies, objects, and abilities. - Keep elemental rules in reusable data
definitions. - Support the game's themes such as nature, shadow, light,
water, metal, and war if those mechanics are adopted.

### \[ \] 31. HUD Editor --- In-Game HUD

**Purpose:** Design persistent gameplay indicators.

**Technical utility:** - Place health, energy, ability, resource, and
status indicators. - Bind UI elements to runtime data sources. - Preview
layout at target resolutions and aspect ratios. - Keep player-facing HUD
authoring separate from the editor's own UI.

### \[ \] 32. UI Layout Editor --- In-Game UI Layout

**Purpose:** Design menus and other game-facing interfaces.

**Technical utility:** - Arrange menus, inventory screens, maps, pause
screens, dialogue panels, and settings. - Configure anchors, layout
rules, and navigation behavior if supported. - Preview scaling across
target resolutions. - Reuse UI assets and styles across screens.

### \[ \] 33. Audio Event Editor --- Audio Events

**Purpose:** Associate sound effects with gameplay events.

**Technical utility:** - Map events such as footsteps, attacks, impacts,
doors, and interactions to sound assets. - Configure variation, volume,
pitch, and playback conditions where supported. - Inspect missing audio
references. - Keep event-to-sound mapping data-driven when appropriate.

### \[ \] 34. Audio Mixer --- Audio Routing and Mixing

**Purpose:** Manage music and sound-effect levels.

**Technical utility:** - Organize audio buses or groups and configure
volume controls. - Support routing and effects if the audio backend
provides them. - Preview level balance and transitions. - Separate
music, ambience, UI, and combat audio categories.

### \[ \] 35. Music Timeline --- Music and Beat Timeline

**Purpose:** Inspect music timing and coordinate gameplay events with a
soundtrack.

**Technical utility:** - Display waveform or timeline data and author
beat/event markers. - Associate markers with boss attacks or other timed
events. - Support deterministic timing against the music playback
clock. - Require a runtime audio timing interface; a visual timeline
alone cannot guarantee synchronization.

### \[ \] 36. World State Editor --- World State Variants

**Purpose:** Author different states or versions of a location.

**Technical utility:** - Define state-dependent objects, tiles, events,
and room variants. - Associate transitions with game events or
progression conditions. - Compare alternate states of a room, such as
the ruined kingdom and its prosperous past. - Validate that state
changes preserve required room and object references.

### \[ \] 37. Background & Parallax Editor --- Background Layers

**Purpose:** Configure multi-layer backgrounds.

**Technical utility:** - Arrange background layers and set parallax
factors, offsets, and repetition. - Preview camera movement against
layered scenery. - Support depth and atmosphere in 2D environments. -
Keep background rendering configuration distinct from collision-bearing
world geometry.

### \[ \] 38. VFX Editor --- Visual Effects

**Purpose:** Configure particles and other visual effects.

**Technical utility:** - Author supported particle emitters, lifetimes,
velocities, colors, and spawn rates. - Preview effects such as
corruption, dust, magic, impacts, and environmental ambience. - Expose
reusable effect presets to gameplay entities. - Depend on the engine's
available rendering and particle systems.

## Narrative tools and visual polish

### \[ \] 39. Lighting Editor --- 2D Lighting

**Purpose:** Configure lights, shadows, and illumination effects.

**Technical utility:** - Place lights and adjust supported parameters
such as color, radius, intensity, and falloff. - Preview lighting in the
scene. - Support mood changes between corrupted and prosperous
environments. - Only expose features implemented by the renderer;
lighting is not a prerequisite for a functional pixel-art game.

### \[ \] 40. Story & Timeline Editor --- Narrative Timeline

**Purpose:** Organize story events and their causal relationships.

**Technical utility:** - Record major events, dependencies, and the
order in which information is revealed. - Track relationships between
past and present events. - Link narrative milestones to rooms,
encounters, or world states where appropriate. - Serve primarily as an
authoring/reference tool unless connected to runtime story data.

### \[ \] 41. Dialogue Editor --- Dialogue Authoring

**Purpose:** Create conversations and conditional dialogue.

**Technical utility:** - Author lines, speakers, choices, and
conditional branches. - Reference localization keys and game-state
conditions. - Validate missing lines, invalid branches, and broken
references. - Export dialogue into a runtime-readable format.

### \[ \] 42. Lore Database --- World Reference Database

**Purpose:** Maintain consistent world-building information.

**Technical utility:** - Catalog characters, factions, creatures,
regions, artifacts, and world rules. - Link entries to related story
events and game assets. - Help track continuity across the ruined and
past versions of the kingdom. - Remain a production/reference tool
unless game runtime systems need its data.

### \[ \] 43. Palette Manager --- Color Palette Management

**Purpose:** Manage consistent color palettes for pixel-art assets.

**Technical utility:** - Store named palettes and color ramps. - Preview
colors and compare regional or temporal variants. - Support asset-art
workflows and optional palette validation. - Avoid imposing palette
restrictions unless the art pipeline requires them.

### \[ \] 44. Localization Editor --- Localization Management

**Purpose:** Manage translated game text.

**Technical utility:** - Maintain stable string identifiers and
translations by locale. - Detect missing translations and duplicate or
unused keys. - Preview text in UI layouts and identify overflow. -
Export data to the runtime localization format.

### \[ \] 45. Script Editor --- Script Authoring

**Purpose:** Edit game scripts from within the editor or open them in an
external tool.

**Technical utility:** - Navigate scripts associated with entities and
assets. - Provide syntax highlighting and diagnostics if a language
integration exists. - Integrate with an external code editor when a full
embedded editor is not justified. - Depend on the engine's actual
scripting model; C++ source editing may be better handled by a dedicated
IDE.

### \[ \] 46. Modules & Plugins --- Module and Extension Management

**Purpose:** Inspect optional engine modules and extensions.

**Technical utility:** - List available modules, their versions, and
enabled states. - Validate dependencies and incompatible
configurations. - Provide extension points for future editor tools. -
Requires a defined module/plugin architecture before implementation.

## Diagnostics, testing, and release workflow

### \[ \] 47. Profiler --- Performance Profiler

**Purpose:** Measure runtime performance and identify bottlenecks.

**Technical utility:** - Display frame time and available CPU,
rendering, physics, and memory metrics. - Show timing history and
identify expensive systems or functions where instrumentation exists. -
Compare performance before and after changes. - Requires reliable
runtime instrumentation; avoid reporting metrics the engine does not
measure.

### \[ \] 48. Debugger --- Code Debugger Integration

**Purpose:** Support source-level debugging.

**Technical utility:** - Integrate with a debugger for breakpoints, call
stacks, and variable inspection. - Link errors or stack frames to source
locations where possible. - Control runtime debugging sessions if
supported by the toolchain. - Prefer integration with an existing C++
debugger over reimplementing one.

### \[ \] 49. Test Runner --- Automated Test Interface

**Purpose:** Run and inspect automated tests.

**Technical utility:** - Discover and execute available unit,
integration, or content-validation tests. - Show pass/fail results and
diagnostic output. - Support repeatable regression checks. - Add
game-specific tests for progression, puzzles, and room connectivity as
systems mature.

### \[ \] 50. Build & Export --- Build and Packaging

**Purpose:** Configure and run builds for distribution.

**Technical utility:** - Select build configuration and target platform
where supported. - Configure output paths and included assets. - Display
build logs and errors. - Package the game reproducibly without mixing
generated outputs with source assets.

### \[ \] 51. Version Control --- Source Control Integration

**Purpose:** Inspect source-control state from the editor.

**Technical utility:** - Display changed, added, deleted, and untracked
files. - Show diffs and history and expose selected Git operations if
integration is implemented. - Help track changes to scenes, assets, and
project configuration. - Avoid replacing a dedicated Git client unless
the editor integration provides clear value.

### \[ \] 52. Runtime Diagnostics and Advanced Validation

**Purpose:** Provide broader validation of project and runtime behavior
beyond the basic Console and Debug Visualization tools.

**Technical utility:** - Check broken asset references, invalid scene
links, missing components, and malformed data. - Report progression or
connectivity issues when validation rules are available. - Aggregate
diagnostics from import, build, and runtime systems. - Keep validation
rules testable and separate from presentation code.

------------------------------------------------------------------------
