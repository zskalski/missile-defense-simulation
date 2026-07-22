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

// SPRITE PLACEMENT LOGIC
let draggedSprite = null;

// SPRITE TOOLTIP
const spriteToolTip = document.getElementById("sprite-tooltip");


// EVENT LISTENER FLAGS
let canDrawRadars = true;
let canDrawSingleRadar = true;
let canDrawHighlight = true;


// REFRESH MAP

function redrawCanvas() {
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
    const sprites = mapArea.querySelectorAll('.sprite-placed');
    for(let i = 0; i < sprites.length; i++) {
        removePiece(sprites[i]);
    }
}

//mapCanvas coordinate-box


// RADAR VISIBILITY

function drawRadarVisibility() {
    radars = mapArea.querySelectorAll('[data-type="radar"]');
    for(let i = 0; i < radars.length; i++) {
        drawSingleRadarVisibility(radars[i]);
    }
}

function drawSingleRadarVisibility(piece) {
    let curRadar = piece;
    let centerX = curRadar.dataset.col * cellWidth + (0.5 * cellWidth);
    let centerY = curRadar.dataset.row * cellHeight + (0.5 * cellHeight);
        
    // fading arc logic
    let maxRadius = Number(options.radarVis) * 5 + 60;
    let curRadius = 0;
    while (curRadius < maxRadius) {
        let opacity = 0.5 - (curRadius / maxRadius);
        opacity = Math.max(0 , opacity);

        mapContext.strokeStyle = `rgba(8, 94, 6, ${opacity})`;
        mapContext.lineWidth = 5;
        mapContext.beginPath();
        mapContext.arc(centerX, centerY, curRadius, 0, (Math.PI * 2));       
        mapContext.stroke();

        curRadius += 1;
    }

    // TO DO: make radar visibility area more transparent as if fades out (maybe also add logic where it gets more unlikely to detect missiles the further away they are from the radar)
}


// SPRITE PLACEMENT LOGIC

mapArea.addEventListener("dragover", event => {
    
    if (!draggedSprite) {
        return;
    }
    
    event.preventDefault();
    
    if(!canDrawRadars)
        return;
    
    drawRadarVisibility();
    canDrawRadars = false;
});

mapArea.addEventListener("drop", event => {
    event.preventDefault();

    const pieceId = event.dataTransfer.getData("text/plain");
    console.log("Dropped pieceId:", pieceId);

    const originalPiece = document.getElementById(pieceId);
    // console.log("Original piece:", originalPiece);

    const newPiece = originalPiece.cloneNode(true);

    findCell(newPiece, event);
    
    newPiece.id = getNewPieceID(newPiece);

    sendSimulationPlacementRequest(newPiece);

    canDrawRadars = true;
});

function getNewPieceID(piece) {
    let pieceNumber = 1;
    let dataType = piece.dataset.type;
    switch(dataType) {
        case "command-center":
            placedCommandCenter++;
            pieceNumber = placedCommandCenter;
            break;
        case "radar":
            placedRadar++;
            pieceNumber = placedRadar;
            break;
        case "protected-target":
            placedTarget++;
            pieceNumber = placedTarget;
            break;
        case "interceptor":
            placedInterceptor++;
            pieceNumber = placedInterceptor;
            break;
        case "enemy-missile":
            placedEnemyMissile++;
            pieceNumber = placedEnemyMissile;
            break;
        case "enemy-missile-barrage":
            placedEnemyMissileBarrage++;
            pieceNumber = placedEnemyMissileBarrage;
            break;
        case "tree":
            placedTree++;
            pieceNumber = placedTree;
            break;
        case "lake":
            placedLake++;
            pieceNumber = placedLake;
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

    piece.dataset.row = row;
    piece.dataset.col = col;
    piece.dataset.placed_x = x;
    piece.dataset.placed_y = y;

    return true;
}

function placePiece(piece) {
    
    const row = Number(piece.dataset.row);
    const col = Number(piece.dataset.col);

    // remove old draggeable class & set new placed class
    piece.classList.replace('sprite-draggable','sprite-placed');

    // frontend map
    piece.style.position = "absolute";
    piece.style.left = `${(col * cellWidth) - 3}px`;
    piece.style.top = `${(row * cellHeight) - 3}px`;

    // console.log("piece classlist: " + piece.classList);

    // add click event listener to display information
    piece.addEventListener("click", () => {
        showPieceTooltip(piece);
        showObjectDetails(piece);
    });
    piece.addEventListener("pointerleave", () => hidePieceTooltip(piece));

    mapArea.appendChild(piece);

    addToEventLog(String(piece.id + ": placed at (x: " + Math.floor((col * cellWidth) + (0.5 * cellWidth)) + ", y: " + Math.floor((row * cellHeight) + (0.5 * cellHeight)) + ")"));
}

function removePiece(piece) {
    
    const row = Number(piece.dataset.row);
    const col = Number(piece.dataset.col);

    mapArea.removeChild(piece);
}


function showPieceTooltip(piece) {
    
    // tool tip text
    spriteToolTip.style.position = "absolute";
    // console.log("piece.dataset.col: " + piece.dataset.col);
    // console.log("cellWidth: " + cellWidth);
    // console.log("piece.dataset.row: " + piece.dataset.row);
    // console.log("cellHeight: " + cellHeight);
    spriteToolTip.style.left = `${(piece.dataset.col * cellWidth + (0.5 * cellWidth))}px`;
    spriteToolTip.style.top = `${(piece.dataset.row * cellHeight)}px`;
    spriteToolTip.style.transform = "translate(-50%, -90%)";
    spriteToolTip.style.display = "block";
    spriteToolTip.style.fontSize = "2rem";
    spriteToolTip.style.whiteSpace = "nowrap";
    spriteToolTip.style.width = "fit-content";
    spriteToolTip.style.height = "fit-content";
    spriteToolTip.style.padding = "10px";
    spriteToolTip.textContent = piece.id;
    spriteToolTip.style.zIndex = "9999";

    // console.log(piece.dataset.type);

    if(piece.dataset.type == "radar") {

        if(!canDrawSingleRadar)
            return;

        drawSingleRadarVisibility(piece);
        canDrawSingleRadar = false;
        return;
    }

    if(!canDrawHighlight)
        return;

    // piece highlight bubble
    let centerX = piece.dataset.col * cellWidth + (0.5 * cellWidth);
    let centerY = piece.dataset.row * cellHeight + (0.5 * cellHeight);
    mapContext.beginPath();
    mapContext.arc(centerX, centerY, cellHeight * 0.6, 0, (Math.PI * 2));
    mapContext.fillStyle = "rgba(250, 247, 94, 0.5)";
    mapContext.fill();

    canDrawHighlight = false;
}

function hidePieceTooltip(piece) {
    spriteToolTip.style.display = "none";
    canDrawSingleRadar = true;
    canDrawHighlight = true;
    redrawCanvas();
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