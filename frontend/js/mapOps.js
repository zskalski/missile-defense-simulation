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

// SPRITE TOOLTIP
const spriteToolTip = document.getElementById("sprite-tooltip");


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

    const sprites = mapArea.querySelectorAll('.sprite-placed');
    for(let i = 0; i < sprites.length; i++) {
        removePiece(sprites[i]);
    }
}

//mapCanvas coordinate-box


// RADAR VISIBILITY

function drawRadarVisibility(piece) {
    radars = mapArea.querySelectorAll('[data-type="radar"]');
    for(let i = 0; i < radars.length; i++) {
        let curRadar = radars[i];
        // console.log(curRadar);
        // console.log("curRadar X: " + curRadar.dataset.placed_x + ", Y: " + curRadar.dataset.placed_y);
        console.log("options: " + options.radarVis);
        mapContext.beginPath();
        mapContext.arc(Number(curRadar.dataset.placed_x), Number(curRadar.dataset.placed_y) - 40, options.radarVis, 0, (Math.PI * 2));
        mapContext.strokeStyle = "red";
        mapContext.lineWidth = 5;
        mapContext.stroke();

        // TO DO: make radar visibility area more transparent as if fades out (maybe also add logic where it gets more unlikely to detect missiles the further away they are from the radar)

        // BUG : radar circle x and y are not centered on the radar sprite, need to fix when x and y are assigned to be the middle of the sprite
    }
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
    if (!(findCell(newPiece, event))) {
        console.log("Cell is occupied!");
        return;
    } 
    
    // get the new piece id and update counters
    newPiece.id = getNewPieceID(newPiece);

    placePiece(newPiece);
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
        piece.dataset.row = row;
        piece.dataset.col = col;
        piece.dataset.placed_x = x;
        piece.dataset.placed_y = y;
        return true;
    } else {
        return false;
    }
}

function placePiece(piece) {
    
    const row = Number(piece.dataset.row);
    const col = Number(piece.dataset.col);

    // backend map
    mapGrid[row][col] = {
        id: piece.dataset.id,
        type: piece.dataset.type
    };

    // remove old draggeable class & set new placed class
    piece.classList.replace('sprite-draggable','sprite-placed');

    // frontend map
    piece.style.position = "absolute";
    piece.style.left = `${(col * cellWidth) - 3}px`;
    piece.style.top = `${(row * cellHeight) - 3}px`;

    console.log("piece classlist: " + piece.classList);

    // add click event listener to display information
    piece.addEventListener("click", () => showPieceTooltip(piece));
    piece.addEventListener("pointerleave", () => hidePieceTooltip(piece));

    mapArea.appendChild(piece);

    drawRadarVisibility();
}

function removePiece(piece) {
    
    const row = Number(piece.dataset.row);
    const col = Number(piece.dataset.col);

    // ensure it is removed from backend map
    if (row && col)
        mapGrid[row][col] = {
            id: null,
            type: null
        };

    mapArea.removeChild(piece);
}

// TO DO: put tool tip in the info box of the information display
//        instead of appearing on screen above the element
function showPieceTooltip(piece) {
    spriteToolTip.style.position = "absolute";
    spriteToolTip.style.left = `${(piece.dataset.col * cellWidth) + 13}px`;
    spriteToolTip.style.top = `${(piece.dataset.row * cellHeight) - 35}px`;
    spriteToolTip.style.display = "block";
    spriteToolTip.style.fontSize = "2rem";
    spriteToolTip.style.width = "200px";
    spriteToolTip.style.height = "200px";
    spriteToolTip.style.whiteSpace = "pre-wrap";
    spriteToolTip.textContent = "ID: interceptor-1\nMissiles Launched: 49\nSuccess Rate: 75%"; 
}

function hidePieceTooltip(piece) {
    spriteToolTip.style.display = "none";
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