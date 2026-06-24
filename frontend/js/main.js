const mapCanvas = document.getElementById("mapCanvas");
const mapContext = mapCanvas.getContext("2d");
const mapBackground = new Image();
mapBackground.src = 'assets/imgs/map/grassland-background.png';
const mapArea = mapCanvas.parentElement;
const gridRows = 16;
const gridCols = 16;
const cellWidth = mapArea.clientWidth / gridCols;
const cellHeight = mapArea.clientHeight / gridRows;


function resizeCanvas() {
    mapCanvas.width = mapArea.clientWidth;
    mapCanvas.height = mapArea.clientHeight;

    // .complete tells if browser is finished loading image
    if (mapBackground.complete) {
        mapContext.drawImage(mapBackground, 0, 0, mapCanvas.width, mapCanvas.height);  // image, x, y, wdith, height
        drawGrid();
    }
}

function drawGrid() {
    mapContext.lineWidth = 1;

    console.log(`Cell width is: ${cellWidth}`);
    console.log(`Cell height is: ${cellHeight}`);

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

mapBackground.onload = resizeCanvas;
window.addEventListener("resize", resizeCanvas);