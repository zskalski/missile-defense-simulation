
// START / PAUSE / RESET
const play = document.getElementById("start-button");
const pause = document.getElementById("pause-button");
const reset = document.getElementById("reset-button");

// AUTO MODE / TRACK VISIBILITY / GRID LINES
const auto = document.getElementById("automatic-switch");
const track = document.getElementById("track-switch");
const grid = document.getElementById("grid-switch");

// RADAR VISIBILITY
const radarSlider = document.getElementById("radar-slider");
const radarTooltip = document.getElementById("radar-tooltip");

// SIMULATION SPEED
const simulationSlider = document.getElementById("simulation-slider");
const simulationTooltip = document.getElementById("simulation-tooltip");





// STATE CONTROL (START/PAUSE/RESET)

play.addEventListener('click', () => {
    options.play = true;
    options.pause = false;
    options.reset = false;
    console.log("play:"+ options.play);
    console.log("pause:"+ options.pause);
    console.log("reset:"+ options.reset);
});

pause.addEventListener('click', () => {
    options.play = false;
    options.pause = true;
    options.reset = false;
    console.log("play:"+ options.play);
    console.log("pause:"+ options.pause);
    console.log("reset:"+ options.reset);
});

reset.addEventListener('click', () => {
    options.play = false;
    options.pause = false;
    options.reset = true;
    console.log("play:"+ options.play);
    console.log("pause:"+ options.pause);
    console.log("reset:"+ options.reset);
});





// TOGGLEABLES (AUTOMATIC MODE / TRACK VISIBILITY / GRID LINES)

auto.addEventListener('change', function(event) {
    if(this.checked) {
        options.doAuto = true;
    } else {
        options.doAuto = false;
    }
    console.log("doAuto:" + options.doAuto);
});

track.addEventListener('change', function(event) {
    if(this.checked) {
        options.doTrack = true;
    } else {
        options.doTrack = false;
    }
    console.log("doTrack:" + options.doTrack);
});

grid.addEventListener('change', function(event) {
    if(this.checked) {
        options.doGrid = true;
    } else {
        options.doGrid = false;
    }
    console.log("doGrid:" + options.doGrid);
});





// SLIDERS (RADAR VISIBILITY / SIM SPEED)

function updateSliderTooltip(slider, tooltip) {
    
    const val = slider.value;
    const min = slider.min;
    const max = slider.max;

    // get width of slider element (pixels)
    const sliderWidth = slider.getBoundingClientRect().width;

    // width set in CSS file
    const thumbWidth = 25;

    const usableTrackWidth = sliderWidth - thumbWidth;

    // how many pixels thumb has traveled
    const percent = (val - min) / (max - min);
    const ballPixelLocation = (percent * usableTrackWidth) + (thumbWidth / 2);

    tooltip.style.left = `${ballPixelLocation}px`;
    tooltip.textContent = val;
}

function addSliderEventListeners(slider, tooltip) {

    slider.addEventListener('pointerdown', () => {
        tooltip.style.display = "block";    // make the tooltip appear
        updateSliderTooltip(slider, tooltip);
    });

    slider.addEventListener('input', () => {
        updateSliderTooltip(slider, tooltip);
    });


    slider.addEventListener('pointerleave', () => {
        tooltip.style.display = "none";
    });
}

addSliderEventListeners(radarSlider, radarTooltip);
addSliderEventListeners(simulationSlider, simulationTooltip);

// when mouse / pointer is released
radarSlider.addEventListener('pointerup', () => {
    radarTooltip.style.display = "none";         // make the tooltip disappear
    options.radarVis = radarSlider.value;        // update value in options data struct
    console.log("radar vis:" + options.radarVis);
});

simulationSlider.addEventListener('pointerup', () => {
    simulationTooltip.style.display = "none";             
    options.simRate = simulationSlider.value;
    console.log("sim rate:" + options.simRate);
});

updateSliderTooltip(radarSlider, radarTooltip);
updateSliderTooltip(simulationSlider, simulationTooltip);