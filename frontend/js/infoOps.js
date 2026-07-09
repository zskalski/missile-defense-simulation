const eventLog = document.getElementById("event-log");
const objectDetails = document.getElementById("selected-object");
const timeMetric = document.getElementById("metrics-time");
const targetMetric = document.getElementById("metrics-targets");
const trackMetric = document.getElementById("metrics-tracks");
const interceptorMetric = document.getElementById("metrics-interceptors");

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
    resetTimer();
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