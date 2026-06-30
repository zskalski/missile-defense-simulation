const mapCanvas = document.getElementById("mapCanvas");
const mapContext = mapCanvas.getContext("2d");
const mapBackground = new Image();
const mapArea = mapCanvas.parentElement;
const gridRows = 16;
const gridCols = 16;
const cellWidth = mapArea.clientWidth / gridCols;
const cellHeight = mapArea.clientHeight / gridRows;

// fetch background image
mapBackground.src = 'assets/imgs/map/grassland-background.png';

function resizeCanvas(options) {
    mapCanvas.width = mapArea.clientWidth;
    mapCanvas.height = mapArea.clientHeight;

    // .complete tells if browser is finished loading image
    if (mapBackground.complete) {
        
        mapContext.drawImage(mapBackground, 0, 0, mapCanvas.width, mapCanvas.height);  // image, x, y, wdith, height
        
        if(options.doGrid)
            drawGrid();
        if(options.doTrack)
            drawTrack();
    }
}

function drawGrid() {
    mapContext.lineWidth = 1;

    // draw horizonal rows
    for (let i = 0; i <= gridRows; i++) {
        mapContext.beginPath();
        mapContext.moveTo(0, i * cellHeight);
        mapContext.lineTo(mapCanvas.width, i * cellHeight);
        mapContext.stroke();
    }

    // draw vertical cols
    for (let i = 0; i <= gridCols; i++) {
        mapContext.beginPath();
        mapContext.moveTo(i * cellWidth, 0);
        mapContext.lineTo(i * cellWidth, mapCanvas.height);
        mapContext.stroke();
    }
}