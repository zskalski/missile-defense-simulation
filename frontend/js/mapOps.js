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
    redrawCanvas();

    radars = mapArea.querySelectorAll('[data-type="radar"]');
    for(let i = 0; i < radars.length; i++) {
        drawSingleRadarVisibility(radars[i]);
    }
}

function clearRadarVisibilityPreview() {
    redrawCanvas();
    canDrawRadars = true;
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

    if (newPiece.dataset.type == "enemy-missile" || newPiece.dataset.type == "enemy-missile-barrage") {
        openMissilePopUp(newPiece);
    } else {
        sendSimulationPlacementRequest(newPiece);
    }

    clearRadarVisibilityPreview();
});

function getNewPieceID(piece) {
    let pieceNumber = 1;
    let dataType = piece.dataset.type;
    switch(dataType) {
        case "command-center":
            pieceNumber = placedCommandCenter + 1;
            break;
        case "radar":
            pieceNumber = placedRadar + 1;
            break;
        case "protected-target":
            pieceNumber = placedTarget + 1;
            break;
        case "interceptor":
            pieceNumber = placedInterceptor + 1;
            break;
        case "enemy-missile":
            pieceNumber = placedEnemyMissile + 1;
            break;
        case "enemy-missile-barrage":
            pieceNumber = placedEnemyMissileBarrage + 1;
            break;
        case "tree":
            pieceNumber = placedTree + 1;
            break;
        case "lake":
            pieceNumber = placedLake + 1;
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


// missile movement updates
function updateMissileLocation(missiles) {
    // missiles is an array of missiles
    const spriteCenterOffset = 32;

    for (const missile of missiles) {
        let missileElement = document.getElementById(missile.id);

        const id = missile.id;
        const x = missile.position.x;
        const y = missile.position.y;
        const direction = missile.direction?.degrees ?? 45;
        const blownUp = missile.blownUp;

        if (!missileElement) {
            missileElement = createMissileElementFromUpdate(missile);
        }

        missileElement.style.left = `${x - spriteCenterOffset}px`;
        missileElement.style.top = `${y - spriteCenterOffset}px`;
        missileElement.dataset.placed_x = x;
        missileElement.dataset.placed_y = y;
        missileElement.dataset.direction = direction;

        const missileImage = missileElement.querySelector("img");
        if (missileImage) {
            // ensure that the img is rotated around its center
            missileImage.style.transformOrigin = "center center";
            missileImage.style.transform = `rotate(${direction - 315}deg)`;
            //missileImage.style.transform = `rotate(${direction - 45}deg)`;
        }

        if (blownUp) {
            applyMissileImpact(missile, missileElement);
        }
    }
}

function createMissileElementFromUpdate(missile) {
    const missileTemplate = document.getElementById("enemy-missile-template");
    const missileElement = missileTemplate.cloneNode(true);
    const x = missile.position.x;
    const y = missile.position.y;

    missileElement.id = missile.id;
    missileElement.dataset.type = "enemy-missile";
    missileElement.dataset.row = Math.floor(y / cellHeight);
    missileElement.dataset.col = Math.floor(x / cellWidth);
    missileElement.dataset.placed_x = x;
    missileElement.dataset.placed_y = y;
    missileElement.classList.replace("sprite-draggable", "sprite-placed");
    missileElement.style.position = "absolute";

    missileElement.addEventListener("click", () => {
        showPieceTooltip(missileElement);
        showObjectDetails(missileElement);
    });
    missileElement.addEventListener("pointerleave", () => hidePieceTooltip(missileElement));

    mapArea.appendChild(missileElement);

    return missileElement;
}

function applyMissileImpact(missile, missileElement) {
    if (missileElement.dataset.blown_up == "true") {
        return;
    }

    missileElement.dataset.blown_up = "true";
    missileElement.dataset.type = "explosion";

    const missileImage = missileElement.querySelector("img");
    if (missileImage) {
        missileImage.src = "assets/imgs/sprites/explosion.png";
        missileImage.alt = "Explosion";
        missileImage.title = "Explosion";
        missileImage.style.transform = "none";
        missileElement.style.left = `${Number(missileElement.dataset.placed_x) - 32}px`;
        missileElement.style.top = `${Number(missileElement.dataset.placed_y) - 37}px`;
    }

    const targetElement = document.getElementById(missile.target_id);
    const targetImage = targetElement?.querySelector("img");

    if (targetImage) {
        targetElement.dataset.destroyed = "true";
        targetImage.src = "assets/imgs/sprites/protected-target-destroyed.png";
        targetImage.alt = "Destroyed Protected Target";
        targetImage.title = "Destroyed Protected Target";
    }

    addToEventLog(`${missile.id}: hit ${missile.target_id}`);

    setTimeout(() => {
        missileElement.remove();
    }, 1000);
}
