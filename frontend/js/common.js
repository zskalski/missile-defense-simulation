// GLOBAL VARIABLES

const options = {
    play: null,
    stop: null,
    reset: null,
    doAuto: false,
    doTrack: false,
    doGrid: false,
    radarVis: 50,
    simRate: 1
};

let totalSeconds = 0;
let timerInterval = null;

let placedCommandCenter = 0;
let placedRadar = 0;
let placedTarget = 0;
let placedInterceptor = 0;
let placedEnemyMissile = 0;
let placedEnemyMissileBarrage = 0;
let placedTree = 0;
let placedLake = 0;

function startTimer() {
    if (timerInterval !== null)
        return;

        timerInterval = setInterval(() => {
        totalSeconds++;

        // Calculate hours, minutes, and remaining seconds
        const hours = Math.floor(totalSeconds / 3600);
        const minutes = Math.floor((totalSeconds % 3600) / 60);
        const seconds = totalSeconds % 60;

        // Format numbers to always have two digits
        const displayHours = String(hours).padStart(2, '0');
        const displayMinutes = String(minutes).padStart(2, '0');
        const displaySeconds = String(seconds).padStart(2, '0');

        // Update
        updateTimeMetric(`${displayHours}:${displayMinutes}:${displaySeconds}`);
            
    }, 1000); // 1000 milliseconds = 1 second
}

function pauseTimer() {
    clearInterval(timerInterval);
    timerInterval = null;
}

function resetTimer() {
    clearInterval(timerInterval);
    timerInterval = null;
    totalSeconds = 0;

    updateTimeMetric("00:00:00");
}