const eventLog = document.getElementById("event-log");
const objectDetails = document.getElementById("selected-object");
const timeMetric = document.getElementById("metrics-time");
const targetMetric = document.getElementById("metrics-targets");
const trackMetric = document.getElementById("metrics-tracks");
const interceptorMetric = document.getElementById("metrics-interceptors");
const alertBanner = document.querySelector(".alert-banner");
const alertMessage = document.querySelector(".alert-message");

let eventLogRows = 0;

function addToEventLog(message) {
    console.log(message);
    const newMessage = document.createElement('div');

    if(message.includes(':')) {
        const [label, ...rest] = message.split(':');
        const content = rest.join(':');

        const messageClass = getEventLabel(label);

        newMessage.innerHTML = `<span class="${messageClass}">${label}:</span>${content}`
    } else {
        newMessage.textContent = message;
    }

    newMessage.style.padding = "3px";
    eventLog.appendChild(newMessage);
    eventLog.scrollTop = eventLog.scrollHeight;
}

function clearEventLog() {
    eventLog.replaceChildren();     // removes all children if no arguments specified
}

function getEventLabel(type) {
    switch (type) {
        case "ERROR":
            return "label-red";
        case "WARNING":
            return "label-yellow";
        case "SUCCESS":
            return "label-green";
        default:
            return "label-gray";
    }
}

function showObjectDetails(piece) {
    /* TO DO: should accept json object from backend and display information
        - Command Center: mode, known tracks, commands issued, successful intercepts
        - Radar: range, detected missiles, uptime
        - Protected Target: health status, incoming threats
        - Interceptor: ammo, cooldown, range, assigned target, successful intercepts
        - Enemy Missile: position, speed, target, detected status, estimated impact time
        - Barrage: total missiles, active missiles, intercepted missiles, targets hit
        - Tree: placement
        - Lake: placement     
    */
   console.log(piece.id);
}

function updateTimeMetric(time) {timeMetric.textContent = time;}
function updateTargetMetric() {targetMetric.textContent = placedTarget;}
function updateTrackMetric(tracks) {trackMetric.textContent = tracks;}
function updateInterceptorMetric() {interceptorMetric.textContent = placedInterceptor;}

function resetMetrics() {
    targetMetric.textContent = 0;
    trackMetric.textContent = 0;
    interceptorMetric.textContent = 0;
    placedCommandCenter = 0;
    placedRadar = 0;
    placedTarget = 0;
    placedInterceptor = 0;
    placedEnemyMissile = 0;
    placedEnemyMissileBarrage = 0;
    placedTree = 0;
    placedLake = 0;
}

function updateDefcon(newDefcon) {
    defcon = newDefcon;

    alertBanner.classList.remove("defcon-1", "defcon-2", "defcon-3", "defcon-4");
    alertBanner.classList.add(`defcon-${defcon}`);
    alertMessage.textContent = getDefconMessage(defcon);
}

function updateDefconFromMissiles(missiles) {
    if (!simulationActive) {
        return;
    }

    const activeMissiles = missiles.filter(missile => !missile.blownUp);

    if (activeMissileBarrage && activeMissiles.length > 0) {
        updateDefcon(1);
        return;
    }

    activeMissileBarrage = false;

    if (activeMissiles.length > 0) {
        updateDefcon(2);
    } else {
        updateDefcon(3);
    }
}

function getDefconMessage(level) {
    switch (level) {
        case 1:
            return "DEFCON 1 - MISSILE BARRAGE INBOUND";
        case 2:
            return "DEFCON 2 - MISSILE INBOUND";
        case 3:
            return "DEFCON 3 - SIMULATION ACTIVE";
        case 4:
        default:
            return "DEFCON 4 - HEIGHTENED ALERT";
    }
}

updateDefcon(defcon);
