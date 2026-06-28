# System Requirements For Each Major Component

## Simulation Core

- **SC-001**	The system shall simulate a 2D operational area.
- **SC-002**	The system shall maintain a central world state containing all active targets, radars, launchers, interceptors, and events.
- **SC-003**	The system shall update the simulation using a tick rate toggleable by the user.
- **SC-004**	The system shall track simulation time in milliseconds.
- **SC-005**	The system shall support starting, pausing, resuming, resetting, and stopping the simulation.
- **SC-006**	The system shall spawn incoming targets from configurable map edges or launch zones.
- **SC-007**	Each simulated entity shall have a unique ID, position, status, and update behavior.

## Radar Detection System

- **RD-001**	The system shall support zero or more simulated radar stations.
- **RD-002**	Each radar station shall have a configurable position, detection range, and scan interval.
- **RD-003**	A radar station shall only detect targets within its configured detection range.
- **RD-004**	A radar station shall generate detection reports at its configured scan interval.
- **RD-005**	Each detection report shall include radar ID, target ID, timestamp, measured position, and confidence value.
- **RD-006**	The radar system shall apply a configurable random error to detected target positions to simulate noisy signals.
- **RD-007**	The radar system shall ignore destroyed, intercepted, or impacted targets.

## Target Tracking System

- **TT-001**	The tracking system shall create and store a track when it receives a valid radar detection for an untracked target.
- **TT-002**	The tracking system shall update existing tracks when new detections are received.
- **TT-003**	Each track shall store estimated position, velocity, heading, status, and last update time.
- **TT-004**	The tracking system shall estimate each target’s future position based on current velocity.
- **TT-005**	The tracking system shall estimate time to impact for targets moving toward the defended asset or hostile missiles for interceptors.
- **TT-006**	The tracking system shall mark tracks as stale if no detection is received within a configurable timeout.

## Command and Control Logic

- **CC-001**	The command system shall classify tracked targets by threat level.
- **CC-002**	The command system shall support the classifications UNKNOWN, FRIENDLY, LOW_PRIORITY, and INTERCEPT_REQUIRED.
- **CC-003**	The command system shall classify targets using distance, trajectory, and estimated time to impact.
- **CC-004**	The command system shall identify whether a target is projected to threaten the defended asset.
- **CC-005**	The system shall support manual user approval before interceptor launch.
- **CC-006**	The system shall support an automatic engagement mode.
- **CC-007**	The command system shall reject engagement decisions for invalid, destroyed, or already impacted targets.

## Launcher and Interceptor System

- **LI-001**	The system shall support one or more simulated launcher units.
- **LI-002**	Each launcher shall have a configurable position, range, ammunition count, and cooldown time.
- **LI-003**	The engagement manager shall assign available launchers to eligible threats.
- **LI-004**	The system shall prevent a launcher from firing when it has no ammunition available.
- **LI-005**	The system shall prevent a launcher from firing while it is in cooldown.
- **LI-006**	The system shall simulate interceptor movement toward an assigned target.
- **LI-007**	The system shall determine intercept success when an interceptor reaches a configurable distance from its target.
- **LI-008**	The system shall update target status after intercept, missed intercept, or impact.
- **LI-009**      The system shall, when possible, fire additional interceptors upon a missed intercept.

## WebSocket API

- **WS-001**	The backend shall provide a WebSocket connection for frontend communication.
- **WS-002**	The backend shall send world-state updates to connected frontend clients.
- **WS-003**	World-state messages shall include simulation time, targets, tracks, radars, launchers, interceptors, and recent events.
- **WS-004**	The frontend shall send user commands to the backend using structured WebSocket messages.
- **WS-005**	The backend shall validate all incoming user commands before applying them.
- **WS-006**	The backend shall reject malformed or invalid WebSocket messages without crashing.

## Frontend Display

- **FD-001**	The frontend shall display a 2D map of the operational area.
- **FD-002**	The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors.
- **FD-003**	The frontend shall display radar detection ranges.
- **FD-004**	The frontend shall update entity positions using world-state messages from the backend.
- **FD-005**	The frontend shall provide controls to start, pause, resume, reset, and stop the simulation.
- **FD-006**	The frontend shall allow the user to manually spawn a target.
- **FD-007**	The frontend shall allow the user to approve or deny simulated interceptor launches.
- **FD-008**	The frontend shall display an event log for detections, classifications, launches, intercepts, misses, and impacts.
- **FD-0009** The frontend shall display a menu with all configurable options of the simulation.

## Testing and CI

- **TC-001**	The project shall include unit tests for simulation timing and entity movement.
- **TC-002**	The project shall include unit tests for radar detection range logic.
- **TC-003**	The project shall include unit tests for target tracking updates.
- **TC-004**	The project shall include unit tests for threat classification logic.
- **TC-005**	The project shall include unit tests for launcher assignment and interceptor behavior.
- **TC-006**	The project shall include integration tests for backend WebSocket message handling.
- **TC-007**	The project shall include a CI pipeline that builds the backend and runs automated tests.
- **TC-008**	The project shall document how to run all tests locally.

## Documentation and Traceability

- **DT-001**	The project shall include a requirements.md file listing all project requirements.
- **DT-002**	The project shall include an architecture document describing backend, frontend, and communication design.
- **DT-003**	The project shall include diagrams showing major system components and command flow.
- **DT-004**	The project shall include a test plan describing unit, integration, and system testing.
- **DT-005**	The project shall include a traceability matrix mapping requirements to tests.
- **DT-006**	The project shall include weekly summaries describing completed work and remaining work.
- **DT-007**	The project shall document the Jira/GitHub workflow used to manage development.
