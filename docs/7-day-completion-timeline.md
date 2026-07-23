# 7-Day Completion Timeline for the Missile Defense Simulation

## Goal

Finish the project in the next 7 days by closing the gap between the requirements in [docs/requirements.md](docs/requirements.md) and the current implementation, while also introducing the process-based and UDP-based architecture you described.

## Current Code Reality to Respect

The current codebase already has a strong starting point, but it is still centered on threads rather than processes:

- [backend/src/MissileDefenseSimulator.cpp](backend/src/MissileDefenseSimulator.cpp) already creates separate threads for the HTTP server, WebSocket server, and message processing loop.
- [backend/include/MissileDefenseSimulator.hpp](backend/include/MissileDefenseSimulator.hpp) already contains commented placeholders for a process manager, component registry, and message router, which is a good fit for the architecture you want.
- [backend/src/WebSocketServer.cpp](backend/src/WebSocketServer.cpp) and [backend/src/MessageHandler.cpp](backend/src/MessageHandler.cpp) show that the project already has a clear control plane and message pipeline.
- [backend/include/ThreadSafeQueue.hpp](backend/include/ThreadSafeQueue.hpp) is useful for in-process communication, but it will need to remain only for local coordination while a new UDP transport layer handles inter-process messages.

This means the plan should not treat process management as an optional add-on; it should become the central architectural change.

## Revised Architecture Direction

### Core idea

The simulator should be organized as a supervisor process plus multiple child processes, where each major entity or subsystem can run independently and communicate over UDP.

### Suggested structure

- Supervisor process
  - Owns the frontend-facing WebSocket/HTTP interfaces.
  - Runs the main simulation coordinator.
  - Spawns and monitors child processes.
  - Maintains the global world state.

- Child processes
  - Radar process
  - Target process or target manager process
  - Tracking process
  - Launcher/interceptor process
  - Event/logging process
  - Optional per-object processes later for more isolation

### Communication model

- Use UDP for inter-process messaging between the supervisor and child processes.
- Keep WebSocket for frontend-to-backend communication.
- Keep the existing JSON schema as the message format so the frontend does not need a major rewrite.
- Use the current Boost.Asio stack already present in the project for the new UDP transport layer.

## 7-Day Plan

### Day 1 — Rebuild the architecture around a supervisor and process manager

Focus:
- Add a process manager layer inside [backend/src/MissileDefenseSimulator.cpp](backend/src/MissileDefenseSimulator.cpp) and [backend/include/MissileDefenseSimulator.hpp](backend/include/MissileDefenseSimulator.hpp).
- Replace the current thread-only orchestration with a supervisor model that can spawn child processes.
- Define the message envelope for process-to-process communication using JSON over UDP.
- Make the current WebSocket path feed into the supervisor process rather than directly into the old thread-based flow.

Deliverables:
- A supervisor process that can start, stop, and monitor child processes.
- A clear process registry and lifecycle model.
- A basic UDP message transport ready for child processes.

Priority:
- Highest priority because the rest of the architecture depends on it.

### Day 2 — Implement the first working child-process boundary

Focus:
- Build the first concrete child process for a simple subsystem, such as a radar manager or target manager.
- Use UDP to send commands and receive state updates from that process.
- Keep the current WebSocket frontend behavior working while the new process-based backend is introduced.

Deliverables:
- One component process can be launched from the supervisor.
- The supervisor can send a command to it and receive a JSON update back.
- The system is no longer purely thread-based for simulation logic.

Priority:
- This proves the new architecture is viable.

### Day 3 — Implement radar detection and target spawning in the new model

Focus:
- Move radar detection logic into a dedicated process or subsystem process.
- Add target creation from map edges or launch zones.
- Send detection results back to the supervisor and store them in the shared world-state model.
- Ensure destroyed, intercepted, or impacted targets are filtered out.

Deliverables:
- Radar processes can detect nearby targets and report detections.
- The supervisor can update the world state from child-process telemetry.
- The core simulation loop now works through component processes.

Priority:
- Critical for the radar and simulation requirements.

### Day 4 — Implement tracking, threat estimation, and launcher/interceptor coordination

Focus:
- Add a tracking process that receives radar detections and builds or updates tracks.
- Add a threat-classification step that uses trajectory and time-to-impact logic.
- Add a launcher/interceptor process that receives threat decisions and manages intercept activities.
- Keep the process manager responsible for lifecycle, restart, and health monitoring.

Deliverables:
- Tracks are created and updated by a child process.
- Threats can be classified and passed to the engagement layer.
- Interceptors can move and change the status of the target through the new process model.

Priority:
- This is the functional core of the simulator.

### Day 5 — Connect the frontend to the new world-state flow

Focus:
- Update the frontend message handling in [frontend/js/messages.js](frontend/js/messages.js) and [frontend/js/main.js](frontend/js/main.js) so the UI receives rich backend world-state updates.
- Render targets, tracks, radars, launchers, interceptors, and events from the supervisor’s state.
- Use the existing event log and metrics components to show real simulation activity.

Deliverables:
- The UI reflects actual backend simulation activity.
- The event log, object details, and metrics are driven by the process-managed backend state.
- The current frontend still works while receiving richer data.

Priority:
- Makes the whole system feel like a real simulator rather than a prototype.

### Day 6 — Add reliability, safety, and process recovery

Focus:
- Add heartbeat logic between the supervisor and child processes.
- Handle child-process crashes or hangs gracefully.
- Add validation for malformed UDP and WebSocket messages.
- Make reset, pause, and stop operations cleanly shut down or restart processes.

Deliverables:
- The simulator can recover from a failed child process or invalid message.
- The system is robust enough for repeated runs and demos.
- The major flows are stable and understandable.

Priority:
- Important because process-based systems need stronger fault handling than thread-based ones.

### Day 7 — Add tests, CI, and documentation

Focus:
- Add unit tests for timing, movement, radar range, tracking, threat classification, and launcher/interceptor behavior.
- Add integration tests for WebSocket and UDP messaging.
- Add a CI pipeline that builds the backend and runs the tests.
- Finish the architecture doc, test plan, traceability matrix, and workflow notes.

Deliverables:
- The project has automated verification and a repeatable build/test flow.
- The documentation reflects the new process-based architecture and the completed requirements.

Priority:
- Required to fully satisfy the requirements in [docs/requirements.md](docs/requirements.md).

## Implementation Notes for the Existing Files

The following files should be treated as the main implementation targets:

- [backend/include/MissileDefenseSimulator.hpp](backend/include/MissileDefenseSimulator.hpp)
- [backend/src/MissileDefenseSimulator.cpp](backend/src/MissileDefenseSimulator.cpp)
- [backend/src/main.cpp](backend/src/main.cpp)
- [backend/include/WebSocketServer.hpp](backend/include/WebSocketServer.hpp)
- [backend/src/WebSocketServer.cpp](backend/src/WebSocketServer.cpp)
- [backend/src/MessageHandler.cpp](backend/src/MessageHandler.cpp)
- [backend/src/SimulationWorld.cpp](backend/src/SimulationWorld.cpp)
- [backend/src/SimulationMap.cpp](backend/src/SimulationMap.cpp)
- [frontend/js/messages.js](frontend/js/messages.js)
- [frontend/js/main.js](frontend/js/main.js)

## Practical Advice

Because the current code already uses Windows-specific headers and Boost.Asio, the simplest path is:

1. Keep the existing WebSocket and HTTP layers as the frontend-facing control plane.
2. Introduce a supervisor process manager in the backend.
3. Start by moving one subsystem into a child process.
4. Expand to more subsystems once the communication contract is stable.

That approach will keep the project realistic and reduce the risk of a large rewrite.

## Definition of Done

The project is done when the major requirements in [docs/requirements.md](docs/requirements.md) are implemented, the process-based backend architecture is working, and the frontend can demonstrate the simulation through the new supervisor/child-process model.
