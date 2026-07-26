let missilePopUp = null;
let missileTargetSelect = null;
let missileSpeedSlider = null;
let missileSpeedTooltip = null;
let missileBarrageCountSlider = null;
let missileBarrageCountTooltip = null;
let missilePendingPiece = null;
let missileSelectedTargetID = 0;
let missileSelectedSpeed = 0;
let missileSelectedBarrageCount = 2;
let missilePopUpDragging = false;
let missilePopUpDragOffsetX = 0;
let missilePopUpDragOffsetY = 0;

function createMissilePopUp() {
    missilePopUp = document.createElement("div");
    missilePopUp.id = "missile-popup";
    missilePopUp.style.display = "none";
    missilePopUp.style.position = "fixed";
    missilePopUp.style.top = "50%";
    missilePopUp.style.left = "50%";
    missilePopUp.style.transform = "translate(-50%, -50%)";
    missilePopUp.style.width = "420px";
    missilePopUp.style.backgroundColor = "var(--metal)";
    missilePopUp.style.border = "4px solid var(--outline)";
    missilePopUp.style.borderRadius = "10px";
    missilePopUp.style.padding = "20px";
    missilePopUp.style.zIndex = "10000";
    missilePopUp.style.fontFamily = "BlackOps";
    missilePopUp.style.color = "var(--text)";

    const closeButton = document.createElement("button");
    closeButton.textContent = "X";
    closeButton.style.position = "absolute";
    closeButton.style.top = "8px";
    closeButton.style.right = "8px";
    closeButton.style.cursor = "pointer";
    closeButton.addEventListener("click", closeMissilePopUp);

    const title = document.createElement("h2");
    title.textContent = "Missile Options";
    title.id = "missile-popup-title";
    title.style.marginTop = "0px";
    title.style.cursor = "move";
    title.style.userSelect = "none";
    title.addEventListener("pointerdown", startMissilePopUpDrag);

    const targetLabel = document.createElement("p");
    targetLabel.textContent = "Target";
    targetLabel.id = "missile-target-label";

    missileTargetSelect = document.createElement("select");
    missileTargetSelect.id = "missile-target-select";
    missileTargetSelect.style.width = "100%";
    missileTargetSelect.style.marginBottom = "25px";

    const speedLabel = document.createElement("p");
    speedLabel.textContent = "Missile Speed";
    speedLabel.id = "missile-speed-label";

    const sliderContainer = document.createElement("div");
    sliderContainer.classList.add("range-slider-container");
    sliderContainer.id = "missile-speed-slider-container";
    sliderContainer.style.marginBottom = "25px";

    missileSpeedTooltip = document.createElement("div");
    missileSpeedTooltip.classList.add("range-tooltip");
    missileSpeedTooltip.id = "missile-speed-tooltip";
    missileSpeedTooltip.textContent = "10";

    missileSpeedSlider = document.createElement("input");
    missileSpeedSlider.type = "range";
    missileSpeedSlider.min = "10";
    missileSpeedSlider.max = "100";
    missileSpeedSlider.value = "10";
    missileSpeedSlider.classList.add("range-slider");
    missileSpeedSlider.id = "missile-speed-slider";

    sliderContainer.appendChild(missileSpeedTooltip);
    sliderContainer.appendChild(missileSpeedSlider);

    const barrageCountLabel = document.createElement("p");
    barrageCountLabel.textContent = "Missile Count";
    barrageCountLabel.id = "missile-barrage-count-label";

    const barrageCountSliderContainer = document.createElement("div");
    barrageCountSliderContainer.classList.add("range-slider-container");
    barrageCountSliderContainer.id = "missile-barrage-count-slider-container";
    barrageCountSliderContainer.style.marginBottom = "25px";

    missileBarrageCountTooltip = document.createElement("div");
    missileBarrageCountTooltip.classList.add("range-tooltip");
    missileBarrageCountTooltip.id = "missile-barrage-count-tooltip";
    missileBarrageCountTooltip.textContent = "2";

    missileBarrageCountSlider = document.createElement("input");
    missileBarrageCountSlider.type = "range";
    missileBarrageCountSlider.min = "2";
    missileBarrageCountSlider.max = "100";
    missileBarrageCountSlider.value = "2";
    missileBarrageCountSlider.classList.add("range-slider");
    missileBarrageCountSlider.id = "missile-barrage-count-slider";

    barrageCountSliderContainer.appendChild(missileBarrageCountTooltip);
    barrageCountSliderContainer.appendChild(missileBarrageCountSlider);

    const submitButton = document.createElement("button");
    submitButton.textContent = "Place Missile";
    submitButton.id = "missile-popup-submit";
    submitButton.style.cursor = "pointer";
    submitButton.addEventListener("click", submitMissilePopUp);

    missilePopUp.appendChild(closeButton);
    missilePopUp.appendChild(title);
    missilePopUp.appendChild(targetLabel);
    missilePopUp.appendChild(missileTargetSelect);
    missilePopUp.appendChild(speedLabel);
    missilePopUp.appendChild(sliderContainer);
    missilePopUp.appendChild(barrageCountLabel);
    missilePopUp.appendChild(barrageCountSliderContainer);
    missilePopUp.appendChild(submitButton);

    document.body.appendChild(missilePopUp);

    addSliderEventListeners(missileSpeedSlider, missileSpeedTooltip);
    missileSpeedSlider.addEventListener('pointerup', () => {
        missileSpeedTooltip.style.display = "none";
    });
    updateSliderTooltip(missileSpeedSlider, missileSpeedTooltip);

    addSliderEventListeners(missileBarrageCountSlider, missileBarrageCountTooltip);
    missileBarrageCountSlider.addEventListener('pointerup', () => {
        missileBarrageCountTooltip.style.display = "none";
    });
    updateSliderTooltip(missileBarrageCountSlider, missileBarrageCountTooltip);
}

function startMissilePopUpDrag(event) {
    missilePopUpDragging = true;

    const popupRect = missilePopUp.getBoundingClientRect();
    missilePopUpDragOffsetX = event.clientX - popupRect.left;
    missilePopUpDragOffsetY = event.clientY - popupRect.top;

    missilePopUp.style.transform = "none";
    missilePopUp.setPointerCapture(event.pointerId);
    missilePopUp.addEventListener("pointermove", dragMissilePopUp);
    missilePopUp.addEventListener("pointerup", stopMissilePopUpDrag);
    missilePopUp.addEventListener("pointercancel", stopMissilePopUpDrag);
}

function dragMissilePopUp(event) {
    if (!missilePopUpDragging) {
        return;
    }

    missilePopUp.style.left = `${event.clientX - missilePopUpDragOffsetX}px`;
    missilePopUp.style.top = `${event.clientY - missilePopUpDragOffsetY}px`;
}

function stopMissilePopUpDrag(event) {
    missilePopUpDragging = false;

    missilePopUp.releasePointerCapture(event.pointerId);
    missilePopUp.removeEventListener("pointermove", dragMissilePopUp);
    missilePopUp.removeEventListener("pointerup", stopMissilePopUpDrag);
    missilePopUp.removeEventListener("pointercancel", stopMissilePopUpDrag);
}

function openMissilePopUp(piece) {
    if (piece.dataset.type == "enemy-missile-barrage") {
        openMissileBarragePopUp(piece);
        return;
    }

    const protectedTargets = mapArea.querySelectorAll('[data-type="protected-target"]');

    if (protectedTargets.length == 0) {
        piece.dataset.target_id = 0;
        piece.dataset.speed = 0;
        piece.dataset.x_dest = 0;
        piece.dataset.y_dest = 0;
        sendSimulationPlacementRequest(piece);
        return;
    }

    missilePendingPiece = piece;
    missileSelectedTargetID = 0;
    missileSelectedSpeed = 0;
    missileSelectedBarrageCount = 2;

    setMissilePopUpMode("missile");
    populateMissileTargetList();
    missileSpeedSlider.value = "10";
    updateSliderTooltip(missileSpeedSlider, missileSpeedTooltip);

    if (missilePopUp.style.left == "50%") {
        missilePopUp.style.transform = "translate(-50%, -50%)";
    }

    missilePopUp.style.display = "block";
}

function openMissileBarragePopUp(piece) {
    missilePendingPiece = piece;
    missileSelectedTargetID = 0;
    missileSelectedSpeed = 0;
    missileSelectedBarrageCount = 2;

    setMissilePopUpMode("barrage");
    missileBarrageCountSlider.value = "2";
    updateSliderTooltip(missileBarrageCountSlider, missileBarrageCountTooltip);

    if (missilePopUp.style.left == "50%") {
        missilePopUp.style.transform = "translate(-50%, -50%)";
    }

    missilePopUp.style.display = "block";
}

function setMissilePopUpMode(mode) {
    const isBarrage = mode == "barrage";

    document.getElementById("missile-popup-title").textContent = isBarrage ? "Missile Barrage" : "Missile Options";
    document.getElementById("missile-popup-submit").textContent = isBarrage ? "Place Barrage" : "Place Missile";
    document.getElementById("missile-target-label").style.display = isBarrage ? "none" : "block";
    missileTargetSelect.style.display = isBarrage ? "none" : "block";
    document.getElementById("missile-speed-label").style.display = isBarrage ? "none" : "block";
    document.getElementById("missile-speed-slider-container").style.display = isBarrage ? "none" : "block";
    document.getElementById("missile-barrage-count-label").style.display = isBarrage ? "block" : "none";
    document.getElementById("missile-barrage-count-slider-container").style.display = isBarrage ? "block" : "none";
}

function populateMissileTargetList() {
    missileTargetSelect.innerHTML = "";

    const emptyOption = document.createElement("option");
    emptyOption.value = "0";
    emptyOption.textContent = "Select target";
    missileTargetSelect.appendChild(emptyOption);

    const protectedTargets = mapArea.querySelectorAll('[data-type="protected-target"]');

    for (let i = 0; i < protectedTargets.length; i++) {
        const targetOption = document.createElement("option");
        targetOption.value = protectedTargets[i].id;
        targetOption.textContent = protectedTargets[i].id;
        missileTargetSelect.appendChild(targetOption);
    }
}

function submitMissilePopUp() {
    if (missilePendingPiece?.dataset.type == "enemy-missile-barrage") {
        missileSelectedBarrageCount = Number(missileBarrageCountSlider.value);
    } else {
        missileSelectedTargetID = missileTargetSelect.value;
        missileSelectedSpeed = Number(missileSpeedSlider.value);
    }

    sendMissilePopUpRequest();
}

function closeMissilePopUp() {
    if (!missilePendingPiece) {
        missilePopUp.style.display = "none";
        return;
    }

    missilePendingPiece = null;
    missilePopUp.style.display = "none";
    missileSelectedTargetID = 0;
    missileSelectedSpeed = 0;
    missileSelectedBarrageCount = 2;
}

function sendMissilePopUpRequest() {
    if (!missilePendingPiece) {
        missilePopUp.style.display = "none";
        return;
    }

    missilePendingPiece.dataset.target_id = missileSelectedTargetID;
    missilePendingPiece.dataset.speed = missileSelectedSpeed;
    missilePendingPiece.dataset.missile_count = missileSelectedBarrageCount;

    const target = document.getElementById(missileSelectedTargetID);
    if (target) {
        const row = Number(target.dataset.row);
        const col = Number(target.dataset.col);
        missilePendingPiece.dataset.x_dest = Math.floor((col * cellWidth) + (0.5 * cellWidth));
        missilePendingPiece.dataset.y_dest = Math.floor((row * cellHeight) + (0.5 * cellHeight));
    } else {
        missilePendingPiece.dataset.x_dest = 0;
        missilePendingPiece.dataset.y_dest = 0;
    }

    missilePopUp.style.display = "none";
    sendSimulationPlacementRequest(missilePendingPiece);
    missilePendingPiece = null;
}

createMissilePopUp();
