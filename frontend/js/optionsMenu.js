
// START / PAUSE / RESET
const play = document.getElementById("start-button");
const pause = document.getElementById("pause-button");
const reset = document.getElementById("reset-button");

const playImg = play.querySelector("img");
const pauseImg = pause.querySelector("img");

// AUTO MODE / TRACK VISIBILITY / GRID LINES
const autoCheckbox = document.getElementById("automatic-switch");
const trackCheckbox = document.getElementById("track-switch");
const gridCheckbox = document.getElementById("grid-switch");
const autoTooltip = document.getElementById("auto-tooltip");
const trackTooltip = document.getElementById("track-tooltip");
const gridTooltip = document.getElementById("grid-tooltip");

// RADAR VISIBILITY
const radarSlider = document.getElementById("radar-slider");
const radarTooltip = document.getElementById("radar-tooltip");

// SIMULATION SPEED
const simulationSlider = document.getElementById("simulation-slider");
const simulationTooltip = document.getElementById("simulation-tooltip");

// DRAGGABLE SPRITES
const draggableSprites = document.querySelectorAll(".sprite-draggable")



// STATE CONTROL (START/PAUSE/RESET)

play.addEventListener('click', () => {
    options.play = true;
    options.pause = false;
    options.reset = false;

    pressedButton(playImg);
    releasedButton(pauseImg);

    console.log("play:"+ options.play);
    console.log("pause:"+ options.pause);
    console.log("reset:"+ options.reset);
});

pause.addEventListener('click', () => {
    options.play = false;
    options.pause = true;
    options.reset = false;

    pressedButton(pauseImg);
    releasedButton(playImg);

    console.log("play:"+ options.play);
    console.log("pause:"+ options.pause);
    console.log("reset:"+ options.reset);
});

reset.addEventListener('click', () => {
    options.play = false;
    options.pause = false;
    options.reset = true;

    releasedButton(playImg);
    releasedButton(pauseImg);
    clearGrid();
    redrawCanvas(options);
    
    console.log("play:"+ options.play);
    console.log("pause:"+ options.pause);
    console.log("reset:"+ options.reset);
});

function pressedButton(img) {
    img.style.transform = "scale(0.95)";
    img.style.filter = "brightness(0.8)";
    img.style.boxShadow = "inset 0 0 5px black";
}

function releasedButton(img) {
    img.style.transform = "scale(1)";
    img.style.filter = "brightness(1)";
    img.style.boxShadow = "none";
}




// TOGGLEABLES (AUTOMATIC MODE / TRACK VISIBILITY / GRID LINES)

autoCheckbox.addEventListener('change', function(event) {
    if(this.checked) {
        options.doAuto = true;
        autoTooltip.textContent = "ON";
        autoTooltip.style.left = `52px`;
    } else {
        options.doAuto = false;
        autoTooltip.textContent = "OFF";
        autoTooltip.style.left = `0px`;
    }
    console.log("doAuto:" + options.doAuto);
});

trackCheckbox.addEventListener('change', function(event) {
    if(this.checked) {
        options.doTrack = true;
        trackTooltip.textContent = "ON";
        trackTooltip.style.left = `52px`;
    } else {
        options.doTrack = false;
        trackTooltip.textContent = "OFF";
        trackTooltip.style.left = `0px`;
    }
    redrawCanvas(options);
    console.log("doTrack:" + options.doTrack);
});

gridCheckbox.addEventListener('change', function(event) {
    if(this.checked) {
        options.doGrid = true;
        gridTooltip.textContent = "ON";
        gridTooltip.style.left = `52px`;
    } else {
        options.doGrid = false;
        gridTooltip.textContent = "OFF";
        gridTooltip.style.left = `0px`;
    }
    redrawCanvas(options);
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
    resizeCanvas(options)
    console.log("radar vis:" + options.radarVis);
});

simulationSlider.addEventListener('pointerup', () => {
    simulationTooltip.style.display = "none";             
    options.simRate = simulationSlider.value;
    console.log("sim rate:" + options.simRate);
});

updateSliderTooltip(radarSlider, radarTooltip);
updateSliderTooltip(simulationSlider, simulationTooltip);




// DRAGGABLE SPRITES

// add event listeners to each sprite
for (let i = 0; i < draggableSprites.length; i++) {
    draggableSprites[i].addEventListener("dragstart", event => {
        event.dataTransfer.setData("text/plain", draggableSprites[i].id);
    });
}
