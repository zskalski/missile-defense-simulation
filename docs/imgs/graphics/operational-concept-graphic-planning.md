1. ###### **Operational Scenario and Scope**
* **Environment**: sky (missiles will be shot out of the sky of a 2-d landscape)
* **Players**: Radar stations, Command and Control Logic station (+ target tracking), Launcher/Interceptor system, hostile rockets/rocket launchers
* **Mission**: hostile missing tracking and interception



###### **2. Interactions and Data Flows**

* **Connections**:

  * Launcher/Interceptor -> Command and Control (state: remaining ammunition, remaining cooldown)
  * Command and Control -> Launcher/Interceptor (state: starting ammunition, starting cooldown, position, range)
  * Command and Control -> Launcher/Interceptor (where and when to fire, pass in track obj. launcher will calculate velocity/angle/etc.)
  * Radar Stations -> Command and Control (target detected, send state: radar ID, target ID, timestamp, measured position, confidence)
  * Command and Control -> Radar Stations (beginning state: position, range, scan interval)

