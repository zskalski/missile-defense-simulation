const mapCanvas = document.getElementById("mapCanvas");
const mapContext = mapCanvas.getContext("2d");
const mapBackground = new Image();
const mapArea = mapCanvas.parentElement;
mapBackground.src = 'assets/imgs/map/grassland-background.png';     // fetch background image
const coordinateBox = document.getElementById("coordinate-box");

// GRID  LOGIC
const gridRows = 16;
const gridCols = 16;
const cellWidth = mapArea.clientWidth / gridCols;
const cellHeight = mapArea.clientHeight / gridRows;
let mapGrid = [];

// SPRITE PLACEMENT LOGIC
let placedCommandCenter = 1;
let placedRadar = 1;
let placedTarget = 1;
let placedInterceptor = 1;
let placedEnemyMissile = 1;
let placedEnemyMissileBarrage = 1;
let placedTree = 1;
let placedLake = 1;



// REFRESH MAP

function redrawCanvas(options) {
    mapCanvas.width = mapArea.clientWidth;
    mapCanvas.height = mapArea.clientHeight;

    // .complete tells if browser is finished loading image
    if (mapBackground.complete) {
        
        mapContext.drawImage(mapBackground, 0, 0, mapCanvas.width, mapCanvas.height);  // image, x, y, wdith, height
        
        if(options.doGrid)
            drawGrid();
        if(options.doTrack)
            drawTrack();

        drawRadarVisibility();
    }
}



// GRID

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

function clearGrid() {
    for(let r = 0; r < gridRows; r++) {
        mapGrid[r] = [];
        for(let c = 0; c < gridCols; c++) {
            mapGrid[r][c] = null;
        }
    }
}



// RADAR VISIBILITY

function drawRadarVisibility() {
    // TO DO: draw circle around each placed radar to indicate visibile zone
}



// SPRITE PLACEMENT LOGIC

mapArea.addEventListener("dragover", event => {
    event.preventDefault();
});

mapArea.addEventListener("drop", event => {
    event.preventDefault();

    const pieceId = event.dataTransfer.getData("text/plain");
    // console.log("Dropped pieceId:", pieceId);

    const originalPiece = document.getElementById(pieceId);
    // console.log("Original piece:", originalPiece);

    const newPiece = originalPiece.cloneNode(true);

    // check to see if it can snap to map square location
    let chosenCell = findCell(newPiece, event)
    if (chosenCell == null) {
        console.log("Cell is occupied!");
        return;
    } 
    
    // get the new piece id and update counters
    newPiece.id = getNewPieceID(newPiece);
    // console.log("New piece id:", newPiece.id);

    placePiece(chosenCell, newPiece);
});

function getNewPieceID(piece) {
    let pieceNumber = 1;
    let dataType = piece.dataset.type;
    switch(dataType) {
        case "command-center":
            pieceNumber = placedCommandCenter;
            placedCommandCenter++;
            break;
        case "radar":
            pieceNumber = placedRadar;
            placedRadar++;
            break;
        case "protected-target":
            pieceNumber = placedTarget;
            placedTarget++;
            break;
        case "interceptor":
            pieceNumber = placedInterceptor;
            placedInterceptor++;
            break;
        case "enemy-missile":
            pieceNumber = placedEnemyMissile;
            placedEnemyMissile++;
            break;
        case "enemy-missile-barrage":
            pieceNumber = placedEnemyMissileBarrage;
            placedEnemyMissileBarrage++;
            break;
        case "tree":
            pieceNumber = placedTree;
            placedTree++;
            break;
        case "lake":
            pieceNumber = placedLake;
            placedLake++;
            break;
    } return `${dataType}-${pieceNumber}`;
}

function findCell(piece, event) {
    
    //console.log("Finding Cell: ");

    //console.log("Map Width is: " + mapArea.clientWidth);
    //console.log("Map Heigh is: " + mapArea.clientHeight);

    const mapRect = mapArea.getBoundingClientRect();

    //console.log("Dropped at X: " + event.clientX);
    //console.log("Dropped at Y: " + event.clientY);

    const x = event.clientX - mapRect.left;
    const y = event.clientY - mapRect.top;

    //console.log("X location is: " + x);
    //console.log("Y location is " + y);

    const row = Math.floor(y / cellHeight);
    const col = Math.floor(x / cellWidth);

    //console.log("Selecting row " + row + " and column " + col);

    if(mapGrid[row][col] == null) {
        return {
            row: row,
            col: col
        };
    } else {
        return null;
    }
}

function placePiece(cell, piece) {
    
    // backend map
    mapGrid[cell.row][cell.col] = {
        id: piece.id,
        type: piece.type
    };

    // frontend map
    piece.style.position = "absolute";
    piece.style.left = `${(cell.col * cellWidth) - 3}px`;
    piece.style.top = `${(cell.row * cellHeight) - 3}px`;

    mapArea.appendChild(piece);
}



// MAP COORDINATES

mapCanvas.addEventListener("mouseover", (event) => {
    const mapRect = mapCanvas.getBoundingClientRect();
    let mouseX = Math.floor(event.clientX - mapRect.left);
    let mouseY = Math.floor(event.clientY - mapRect.top);

    coordinateBox.textContent = `X: ${mouseX} Y: ${mouseY}`;
});

mapCanvas.addEventListener("mousemove", (event) => {
    const mapRect = mapCanvas.getBoundingClientRect();
    let mouseX = Math.floor(event.clientX - mapRect.left);
    let mouseY = Math.floor(event.clientY - mapRect.top);

    coordinateBox.textContent = `X: ${mouseX} Y: ${mouseY}`;
});

mapCanvas.addEventListener("mouseout", () => {
    coordinateBox.textContent = "X: - Y: -";
});