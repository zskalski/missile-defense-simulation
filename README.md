# Missile Defense Simulation

A C++ and JavaScript missile defense simulation focused on real-time system state, frontend/backend communication, and incremental delivery through a requirements-driven workflow.

This project is currently in progress. The present implementation establishes the interactive frontend, C++ backend service structure, WebSocket communication path, simulation timer, backend-owned world state, map placement validation, radar detection range behavior, and missile movement updates. Remaining simulation, testing, and documentation work is tracked in [`docs/requirements.md`](docs/requirements.md).

## Project Purpose

The simulator models a 2D operational area where users can place defense components and hostile missile threats, then observe how the system state changes over time. The long-term goal is to build a complete missile defense workflow:

- Simulate incoming targets and defended assets on a 2D map.
- Detect hostile missiles using configurable radar stations.
- Track detected targets and estimate future positions.
- Classify threats through command-and-control logic.
- Assign interceptors and simulate engagement outcomes.
- Display live world-state updates in a browser-based interface.

## Current Capabilities

- Browser-based operational display with a 2D canvas map.
- Drag-and-place support for command centers, radar stations, protected targets, interceptors, enemy missiles, missile barrages, terrain, and supporting sprites.
- Backend validation for placement rules, including single command center enforcement, missile target requirements, barrage size validation, and prevention of placement while the simulation is running.
- C++ backend world state for placed objects, active radars, missiles, detected targets, simulation metrics, and reset behavior.
- Simulation controls for start, pause, and reset.
- Backend-controlled simulation timer.
- Configurable simulation speed and radar visibility.
- Radar range checks against active missiles.
- Missile movement toward assigned targets with position updates sent to the frontend.
- JSON-based WebSocket message routing between the JavaScript frontend and C++ backend.
- Local HTTP server that serves the frontend from the backend executable.
- Thread-safe queues and protected console output for communication between backend threads.

## Technical Stack

- **Backend:** C++20
- **Networking:** Boost.Beast / Boost.Asio
- **Data format:** JSON with `nlohmann-json`
- **Frontend:** HTML, CSS, JavaScript, Canvas API
- **Build system:** CMake
- **Dependency management:** vcpkg
- **Planning and tracking:** Jira backlog, epics, stories, weekly summaries, and requirement IDs

## Architecture

The project is organized around a backend-owned simulation state. The frontend sends user commands over WebSocket, and the backend validates those commands before applying them to the simulation world.

Key backend components:

- `MissileDefenseSimulator` coordinates the HTTP server, WebSocket server, message handler, simulation options, and game loop.
- `SimulationWorld` owns the simulation timer, map state, component totals, radar objects, missile objects, and detected targets.
- `WebSocketServer` handles frontend/backend communication using structured JSON messages.
- `MessageHandler` routes incoming message types to simulator callbacks.
- `SimulationMap` validates map occupancy and placement behavior.
- `Radar`, `Missile`, and `DetectedTarget` model core simulation entities.

The backend currently runs separate threads for HTTP serving, WebSocket communication, message processing, and simulation updates. Thread-safe queues pass JSON messages between the WebSocket server and simulator logic.

## Software Engineering Approach

This project is being developed with a process modeled after professional software engineering practices:

- Requirements are written as traceable IDs in [`docs/requirements.md`](docs/requirements.md).
- Work is organized in Jira using epics, stories, and task-level status tracking.
- Weekly progress is documented in [`docs/weekly-summary.md`](docs/weekly-summary.md), including completed work, Jira items, design decisions, and blockers.
- The implementation is being built incrementally, starting with the frontend prototype, then moving authoritative state and validation into the backend.
- The codebase is split into focused modules for simulation state, networking, message routing, frontend display logic, and reusable components.
- Design decisions are documented as the system evolves, including the decision to make the backend the source of truth for simulation state.

## Skills Demonstrated

- **C++ systems programming:** object-oriented simulation components, C++20 project structure, RAII-oriented ownership, STL containers, atomics, mutexes, and multithreading.
- **Networked application design:** local HTTP serving, WebSocket communication, request/response message flow, and JSON serialization.
- **Concurrent programming:** simulation loop, server threads, message-processing thread, thread-safe queues, and protected output streams.
- **Frontend development:** interactive canvas rendering, drag-and-drop controls, responsive UI sections, event log display, metrics display, and modular JavaScript.
- **Simulation modeling:** 2D map representation, entity placement, missile movement, radar range detection, configurable simulation timing, and world-state updates.
- **Requirements-driven development:** requirement IDs, feature status tracking, planned traceability, and a Jira-based workflow.
- **Build and dependency management:** CMake configuration, vcpkg dependencies, and a PowerShell run script for local execution.
- **Professional documentation habits:** requirements, weekly summaries, diagrams, design notes, and implementation roadmap.

## Planned Work

The next development stages will focus on completing the remaining requirements in [`docs/requirements.md`](docs/requirements.md), including:

- Target tracking with position, velocity, heading, stale-track handling, and future-position estimates.
- Threat classification for unknown, friendly, low-priority, and intercept-required targets.
- Command-and-control logic for manual approval and automatic engagement.
- Launcher and interceptor assignment, cooldowns, ammunition counts, intercept success checks, missed intercept handling, and target status updates.
- More complete world-state events for detections, classifications, launches, intercepts, misses, and impacts.
- Unit tests for timing, entity movement, radar detection, tracking, classification, launcher assignment, and interceptor behavior.
- Integration tests for WebSocket message handling.
- CI pipeline for backend build and automated test execution.
- Architecture documentation, test plan, and a traceability matrix mapping requirements to implementation and tests.

## Running Locally

### Prerequisites

- Windows
- CMake 3.21 or newer
- A C++20-capable compiler
- vcpkg with dependencies from `vcpkg.json`

### Build and Run

From the project root:

```powershell
.\run.ps1
```

The script configures the CMake build, builds the backend, starts the simulator, serves the frontend locally, and opens the browser.

Manual CMake commands:

```powershell
cmake -S . -B backend/build
cmake --build backend/build --config Debug
cmake --build backend/build --config Debug --target run
```

Default local ports:

- Frontend HTTP server: `http://127.0.0.1:8080`
- WebSocket server: `ws://127.0.0.1:8081`

## Repository Structure

```text
backend/
  include/              C++ headers for simulator, servers, world state, and components
  src/                  C++ implementation files
frontend/
  index.html            Browser UI
  css/                  Stylesheets
  js/                   Frontend behavior, map logic, WebSocket handling, and messages
  assets/               Sprites, images, and fonts
docs/
  requirements.md       System requirements and planned work
  weekly-summary.md     Jira-linked weekly progress notes
  imgs/                 Diagrams and Jira screenshots
planning/               Additional planning artifacts
```

## Status

This repository represents an active build of the simulator rather than a finished product. The current focus is establishing a reliable architecture and backend-owned simulation state before completing the remaining tracking, command, interceptor, testing, CI, and documentation requirements.
