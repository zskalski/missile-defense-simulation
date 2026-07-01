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

| Jira ID | Title | Status |
| ------- | ----- | ------ |
|    FD-001     |   The frontend shall display a 2D map of the operational area.    |    Completed    |
|    FD-009     |   The frontend shall display a menu with all configurable options of the simulation.    |    Completed    |
|    FD-005     |   The frontend shall provide controls to start, pause, resume, reset, and stop the simulation.    |    Completed    |
|    FD-002     |   The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors.    |    In-progress    |
|    FD-006     |   The frontend shall allow the user to manually spawn a target.    |    In-progress    |


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

| Jira ID | Title | Status |
| ------- | ----- | ------ |
|    FD-002     |   The frontend shall display the defended asset, radar stations, launchers, targets, and interceptors.    |    In-progress    |
|    FD-006     |    The frontend shall allow the user to manually spawn a target.   |    In-progress    |
|    FD-008     |    The frontend shall display an event log for detections, classifications, launches, intercepts, misses, and impacts.   |    In-progress    |


**Notes / Decisions**

- The page layout is too zoomed in for split screen, idealy, the page pixel sizes should be scaled downwards ~80%. However, the page is functional and looks good in full screen mode, which is acceptable. If time permits, all frontend UI should be scaled down.

**Blockers**

### Thursday

**Work Completed**
- Created event log, selected object details, system status, metrics, and alert banner sections for the informational display.
- Created centralized world-state data structure that houses: user options, placed objects, ect...

**Jira Items Worked On**

| Jira ID | Title | Status |
| ------- | ----- | ------ |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |

**Notes / Decisions**

**Blockers**

### Day

**Work Completed**

**Jira Items Worked On**

| Jira ID | Title | Status |
| ------- | ----- | ------ |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |
|         |       |        |

**Notes / Decisions**

**Blockers**

---

