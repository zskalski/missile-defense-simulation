# Weekly Project Summaries

## Week 1 - Project Planning and Requirements

**Dates:**
Start:  6/22/2026
End:  6/26/2026

### Weekly Goals

- Fill backlog with system requirements
- Design major system components (simulation core, radar, target tracking, command logic, launchers/interceptors, and frontend connections)
- Begin work on frontend display
- Connect the frontend display to the backend, enable parameter passing for the system (tickrate, radar range, etc.)

### Monday

**Work Completed**

- Created initial project design
- Documented Epics in Jira
- Created weekly-summary.md
- Began working on preliminary system design and layout

**Jira Items Worked On**

| Jira ID | Title                           | Status           |
| ------- | ------------------------------- | ---------------- |
| MDS-1   | Simulation Core                 | Added to backlog |
| MDS-2   | Radar Detected System           | Added to backlog |
| MDS-3   | Target Tracking System          | Added to backlog |
| MDS-4   | Command and Control Logic       | Added to backlog |
| MDS-5   | Launcher and Interceptor System | Added to backlog |
| MDS-6   | WebSocket API                   | Added to backlog |
| MDS-7   | Frontend Display                | Added to backlog |
| MDS-8   | Testing and CI                  | Added to backlog |
| MDS-9   | Documentation and Traceability  | Added to backlog |

**Notes / Decisions**

- Goals for tomorrow: create main system diagram, create major sub-system diagrams, add system requirements.

**Blockers**

- Struggling to make decisions on a system-wide scale, finding it difficult to plan out a large system with many components
- Jira's language (Epics, Stories, Tasks, etc.) was unfamiliar, and I found it difficult to navigate the many tools that Jira has

### Tuesday

**Work Completed**

- Completed DT-001, created requirements.md with all requirements the system must have
- Added all requirements as stories under each epic in Jira backlog
- Began drawing operational concept graphic.
- All planned diagrams:
  - Operational Concept Graphic
    - Show the scenario visually
  - SysML Block Definition Diagram
    - Show major components
    - Demonstrate architecture
  - Sequence Diagram
    - Show missile interception flow
  - State Machine Diagram of Target/Interceptor
  - Traceability Diagram/Matrix
    - requirement -> design -> test

**Jira Items Worked On**

| Jira ID | Title                                                                                | Status      |
| ------- | ------------------------------------------------------------------------------------ | ----------- |
| DT-001  | The project shall include a requirements.md file listing all project requirements.   | Completed   |
| DT-003  | The project shall include diagrams showing major system components and command flow. | In-progress |

**Notes / Decisions**

- Utilized ChatGPT to give a starting point for system requirements, then went through each to make sure that nothing was missing or illogical. However, I kept system design decisions (like class/componenet setup) as I want to practice designing a system on my own.
- Made major design decisions on system components, including what data they communicate.

### Wednesday

**Work Completed**

- Created preliminary front-end in strcuture in HTML, with CSS styling and JavaScript canvas drawing (to draw squares on the map area)
- Generated sprites for missile-to-ground impact, command center, radar tower, interceptor, and enemy missile.
- Generated a pixel-game style map for the user to place sprites.

**Jira Items Worked On**

| Jira ID | Title                                                                                                | Status      |
| ------- | ---------------------------------------------------------------------------------------------------- | ----------- |
| FD-001  | The frontend shall display a 2D map of the operational area.                                         | In-Progress |
| FD-002  | The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors. | In-Progress |
| FD-0009 | The frontend shall display a menu with all configurable options of the simulation.                   | In-Progress |

**Notes / Decisions**

- Changed SC-001 to not have the operational area (map area) configurable to the user. Instead, the map area remains fixed at a max width of 940 pixels. This decision simplies the frontend development, as the main focus of this project in on backend components and the communication between them.
- Decided major color themes, UI/UX formatting, and sprite icons.

### Thursday

**N/A**

### Friday

**N/A**

---

## Week 2 - Frontend Development

**Dates:**
Start:  6/30/2026
End:  7/03/2026

### Weekly Goals

- Complete frontend display
- Connect the frontend display to the backend, enable parameter passing for the system (tickrate, radar range, etc.)

### Monday

**Work Completed**

- Completed sidebar options menu
- Implemented custom toggle buttons, range sliders, control buttons (start/pause/reset), and sprite images to drag and place on the operational area.
- Seperated CSS code for better orangization and maintainability

**Jira Items Worked On**

| Jira ID | Title                                                                                                | Status      |
| ------- | ---------------------------------------------------------------------------------------------------- | ----------- |
| FD-001  | The frontend shall display a 2D map of the operational area.                                         | Completed   |
| FD-009  | The frontend shall display a menu with all configurable options of the simulation.                   | Completed   |
| FD-005  | The frontend shall provide controls to start, pause, resume, reset, and stop the simulation.         | Completed   |
| FD-002  | The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors. | In-progress |
| FD-006  | The frontend shall allow the user to manually spawn a target.                                        | In-progress |

**Notes / Decisions**

- CSS code files need to be refactored, there exists classnames which are not needed and some class selectors are sloppy.

**Blockers**

- I found it difficult to use CSS display options like grid and flexbox along with how aligment/placement commands work along-side them. For example, align-items is useless unless used in a CSS layout such as grid.

### Tuesday

**Work Completed**

- Added JavaScript to slider bars to display radar visibility range and tick rate
- Added listeners to all sidebar components

### Wednesday

**Work Completed**

- Refactored CSS files
- Added ON/OFF Display for toggleable options
- Added ability to place sprites on map area
- Created coordinate display for map
- Developed sprite id system for event log

**Jira Items Worked On**

| Jira ID | Title                                                                                                               | Status      |
| ------- | ------------------------------------------------------------------------------------------------------------------- | ----------- |
| FD-002  | The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors.                | In-progress |
| FD-006  | The frontend shall allow the user to manually spawn a target.                                                       | In-progress |
| FD-008  | The frontend shall display an event log for detections, classifications, launches, intercepts, misses, and impacts. | In-progress |

**Notes / Decisions**

- The page layout is too zoomed in for split screen, idealy, the page pixel sizes should be scaled downwards ~80%. However, the page is functional and looks good in full screen mode, which is acceptable. If time permits, all frontend UI should be scaled down.

**Blockers**

### Thursday

**Work Completed**

- Created frontend display for event log, selected object details, system status, metrics, and alert banner sections for the informational display.

**Jira Items Worked On**

| Jira ID | Title                                                                                                               | Status      |
| ------- | ------------------------------------------------------------------------------------------------------------------- | ----------- |
| FD-008  | The frontend shall display an event log for detections, classifications, launches, intercepts, misses, and impacts. | In-progress |

**Notes / Decisions**

**Blockers**

### Friday

*** N/A ***

---

## Week 3 - Backend Connection and Initial Structure

**Dates:**
Start:  7/6/2026
End:  7/10/2026

### Weekly Goals

- Connect the frontend display to the backend, enable parameter passing for the system (tickrate, radar range, etc.)
- Complete WebSocket Integration
- Finish remaing Frontend tasks (radar visibility, sprite tooltips)

### Monday

*** N/A ***

### Tuesday

*** N/A ***

### Wednesday

**Work Completed**

- Added map reset behavior that clears placed sprites and resets the internal grid state.
- Added initial sprite tooltip support for placed map objects.
- Added visual styling for placed sprites and tooltip display.

### Thursday

**Work Completed**

- Implemented visual radar coverage drawing around radar sprites and improved radar coverage rendering with centered, fading circular visibility areas.
- Added placed-sprite click behavior for tooltips and object-detail hooks.
- Added highlight bubbles for selected non-radar objects.
- Added a simulation clock with start, pause, and reset behavior.
- Added event log message support with colored labels for status types like error, warning, success, and default messages.
- Added metrics updates for simulation time, protected targets, tracks, and interceptors.
- Added a preliminary socket.js

**Jira Items Worked On**

| Jira ID | Title                                                                                                               | Status    |
| ------- | ------------------------------------------------------------------------------------------------------------------- | --------- |
| FD-002  | The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors.                | Completed |
| FD-008  | The frontend shall display an event log for detections, classifications, launches, intercepts, misses, and impacts. | Completed |
| FD-003  | The frontend shall display radar detection ranges                                                                   | Completed |
| FD-006  | The frontend shall allow the user to manually spawn a target.                                                       | Completed |

**Notes / Decisions**

- Timer, placement, and vertification logic must be moved to the backend. Frontend should be only visuals, backend should represent the true state.

**Blockers**

### Friday

**Work Completed**

- Stuided CMake build system, WebSockets, and the Boost.Beast library
- Designed communication system between frontend and backend
- Began initial WebSocket Server implementation

**Jira Items Worked On**

| Jira ID | Title                                                                             | Status      |
| ------- | --------------------------------------------------------------------------------- | ----------- |
| WS-001  | The backend shall provide a WebSocket connection for frontend communication.      | In-progress |
| WS-006  | The backend shall reject malformed or invalid WebSocket messages without crashing | In-progress |

**Notes / Decisions**

- The communication between frontend/backend will occur using the built-in JS websocket and the Boost.Beast C++ websocket lib
- The communication will have a synchronous, request-reply style architecture. This means that the frontend will send a request to the backend, wait for a response, apply updates, then send an additional request. Multiple requests will not be sent at one time.
- 
- The backend will run on two threads: a simulation thread and a websocketserver thread. The websocket server thread will own every data member inside of the WebSocketServer instance to avoid synchronization issues
- The frontend will send requests to the WebSocketServer which will put them into a custom ThreadSafeQueue for incoming messages. This queue will be shared in the backend so that the simulation thread can access it. The simulation thread will read the message in the incomingMessages ThreadSafeQueue, then add a message into another queue called outgoingMessages, where the webSocketServer will forward it to the frontend.
- Messages will be sent using a json format.

**Blockers**

- Boost.Beast library is vast and complex
- Orginally decided to have a stop() function in the WebSocketServer but this created many issues with the simulation thread being able to call stop() at any time. I tried to implement thread safe data, however this became too complex and I decided to simplify it by having one thread own the entire instance of the server.

---

## Week 4 - Complete Backend Connection

**Dates:**
Start:  7/13/2026
End:  7/17/2026

### Weekly Goals

- Connect the frontend display to the backend, enable parameter passing for the system (tickrate, radar range, etc.)
- Connect start, pause, reset buttons to backend
- Move frontend placement logic and timer to the backend
- Implement basic classes for each piece type (radar, command center, protected target, launcher, missile, etc.)
- Create basic simulation loop
- Connect event log and object details to backend

### Monday

**Work Completed**

- Completed WebSocketServer implementation
- Fixed design issues from enabling two threads' access to WebSocketServer data
- Completed basic network tests using json
- Added error checks using try/catch blocks for json parsing and network errors
- Created the first backend build structure and dependency setup
- Created the first simulator entry point so the backend could be run as a program
- Added queues for sending messages between backend threads
- Connected the frontend socket code to the backend connection
- Moved frontend message code into a seperate file so socket code only handles the connection
- Updated the weekly log and cleaned unneeded planning/editor files from the repo

**Jira Items Worked On**

| Jira ID | Title                                                                                     | Status      |
| ------- | ----------------------------------------------------------------------------------------- | ----------- |
| WS-001  | The backend shall provide a WebSocket connection for frontend communication.              | Completed   |
| WS-004  | The frontend shall send user commands to the backend using structured WebSocket messages. | In-progress |
| WS-006  | The backend shall reject malformed or invalid WebSocket messages without crashing         | Completed   |
| DT-006  | The project shall include weekly summaries describing completed work and remaining work.  | In-progress |

**Notes / Decisions**

- Removed local editor settings and old planning files from git so the repo only tracks project files.
- Split message code out of socket.js because the socket should only manage the connection, while message formatting and response handling should live somewhere else.

**Blockers**

### Tuesday

*** N/A ***

### Wednesday

*** N/A ***

### Thursday

*** N/A ***

### Friday

**Work Completed**

- Created the first backend options storage for user-changeable simulation settings
- Added startup code for the backend connection server from inside the simulator
- Updated the backend main function so it creates the simulator and starts the program through it
- Updated build files so the new backend files compile with the rest of the project
- Added an HTTP server so the frontend can be served from the backend
- Added a script to build and run the project
- Added protected output streams because the backend now prints from multiple threads
- Updated the simulator to start both the page server and the backend connection server
- Added browser launch behavior so running the backend opens the frontend page

**Jira Items Worked On**

| Jira ID | Title                                                                                         | Status      |
| ------- | --------------------------------------------------------------------------------------------- | ----------- |
| SC-003  | The system shall update the simulation using a tick rate toggleable by the user.              | In-progress |
| SC-005  | The system shall support starting, pausing, resuming, resetting, and stopping the simulation. | In-progress |
| WS-004  | The frontend shall send user commands to the backend using structured WebSocket messages.     | Completed   |

**Notes / Decisions**

- User options should live in the backend because the backend will be responsible for deciding how the simulation runs.
- The simulator should own and start the major backend systems instead of spreading setup logic across main.
- The frontend should be opened from the local backend instead of manually opening the html file, because the project is moving toward one run command for the full system.

**Blockers**

---

## Week 5 - Backend State and Simulation Communication

**Dates:**
Start:  7/20/2026
End:  7/24/2026

### Weekly Goals

- Implement backend message responses for frontend controls and user options
- Move timer and placement logic to the backend
- Move object totals and placement validation to the backend
- Add backend data for radars and missiles
- Send fuller world updates back to the frontend

### Monday

**Work Completed**

- Added backend message routing so incoming frontend messages call the correct backend response
- Added update request and update response messages between the frontend and backend
- Connected the frontend to repeated backend update requests after the connection is ready
- Updated the frontend clock display using backend response data
- Connected backend incoming and outgoing message queues into the backend message flow

**Jira Items Worked On**

| Jira ID | Title                                                                                                                    | Status |
| ------- | ------------------------------------------------------------------------------------------------------------------------ | ------ |
| WS-002  | The backend shall send world-state updates to connected frontend clients.                                                | Done   |
| WS-003  | World-state messages shall include simulation time, targets, tracks, radars, launchers, interceptors, and recent events. | Done   |
| WS-004  | The frontend shall send user commands to the backend using structured WebSocket messages.                                | Done   |

**Notes / Decisions**

- The frontend should request updates from the backend instead of creating its own simulation state.
- Message types should be mapped to handler functions so more request types can be added without filling the socket code with if-statements.

**Blockers**

### Tuesday

*** N/A ***

### Wednesday

**Work Completed**

- Connected start, pause, reset, automatic mode, radar visibility, and tick rate controls to backend messages
- Added backend timer behavior so the backend controls simulation time
- Created the first backend world state object to hold simulation state
- Moved placement decisions into the backend so the frontend asks to place a piece and waits for the backend response
- Added backend map storage and object storage for placed pieces
- Updated frontend placement code so it responds to backend placement results

**Jira Items Worked On**

| Jira ID | Title                                                                                                                       | Status      |
| ------- | --------------------------------------------------------------------------------------------------------------------------- | ----------- |
| SC-001  | The system shall simulate a 2D operational area.                                                                            | Done        |
| SC-002  | The system shall maintain a central world state containing all active targets, radars, launchers, interceptors, and events. | In-progress |
| SC-003  | The system shall update the simulation using a tick rate toggleable by the user.                                            | In-progress |
| SC-004  | The system shall track simulation time in milliseconds.                                                                     | Done        |
| SC-005  | The system shall support starting, pausing, resuming, resetting, and stopping the simulation.                               | Done        |
| SC-007  | Each simulated entity shall have a unique ID, position, status, and update behavior.                                        | Done        |
| WS-004  | The frontend shall send user commands to the backend using structured WebSocket messages.                                   | In-progress |
| WS-005  | The backend shall validate all incoming user commands before applying them.                                                 | In-progress |
| FD-004  | The frontend shall update entity positions using orld-state messages from the backend.                                      | Completed   |

**Notes / Decisions**

- Placement logic should not stay in the frontend because the backend must be the true source of what exists on the map.
- The frontend can still create the visual piece, but the backend should decide if the placement is valid.

**Blockers**

### Thursday

**Work Completed**

- Moved object totals into the backend world state
- Updated backend update messages to include piece totals and component data
- Updated frontend info and map code to use the new backend update format
- Added backend data structures for radar, missile, and detected target pieces
- Added missile placement options on the frontend so the user can select a target and speed before placing a missile
- Added placement validation for missiles without a target or speed
- Connected radar visibility changes to backend radar data

**Jira Items Worked On**

| Jira ID | Title                                                                                                                       | Status      |
| ------- | --------------------------------------------------------------------------------------------------------------------------- | ----------- |
| SC-002  | The system shall maintain a central world state containing all active targets, radars, launchers, interceptors, and events. | In-progress |
| RD-001  | The system shall support zero or more simulated radar stations.                                                             | Completed   |
| RD-002  | Each radar station shall have a configurable position, and detection range.                                                 | Completed   |
| RD-003  | A radar station shall only detect targets within its configuraed detection range.                                           | In-progress |
| DT-006  | The project shall include weekly summaries describing completed work and remaining work.                                    | In-progress |

**Notes / Decisions**

- Object count data should live in the backend with the world state, not in the frontend display code.
- Missiles need extra placement data, so a small placement popup is needed before the frontend sends the placement request.

**Blockers**

### Friday

*** N/A ***

## Week 6 - MVP Simulation Behavior

**Dates:**
Start:  7/27/2026
End:  7/31/2026

### Weekly Goals

*** N/A ***

### Monday

*** N/A ***

### Tuesday

*** N/A ***

### Wednesday

*** N/A ***

### Thursday

*** N/A ***

### Friday

*** N/A ***

---

## Week 7 - Final Implementation and Testing

**Dates:**
Start:  8/3/2026
End:  8/7/2026

### Weekly Goals

*** N/A ***

### Monday

*** N/A ***

### Tuesday

*** N/A ***

### Wednesday

*** N/A ***

### Thursday

*** N/A ***

### Friday

*** N/A ***

---

## Daily Summary Example

### Day

**Work Completed**

**Jira Items Worked On**

| Jira ID | Title | Status |
| ------- | ----- | ------ |
|         |       |        |
|         |       |        |
|         |       |        |

**Notes / Decisions**

**Blockers**
