const intRegex = /^(\d+)px$/;
let forward_fast = true;
let forward_slow = true;
let parent_width = parseInt(document.querySelector("#fast_box").parentNode.offsetWidth) - 70;
document.addEventListener("DOMContentLoaded", () => {
    let fastBox = document.querySelector("#fast_box");
    let slowBox = document.querySelector("#slow_box");
    let startBtn = document.querySelector("#start_btn");
    let stopBtn = document.querySelector("#stop_btn");
    let resetBtn = document.querySelector("#reset_btn");
    let slowBoxInterval;
    let fastBoxInterval;

    stopBtn.addEventListener("click", stopMoving);
    startBtn.addEventListener("click", () => startMoving(fastBox, slowBox));
    resetBtn.addEventListener("click", () => {
        fastBox.style.left = "0px";
        slowBox.style.left = "0px";
    });

});
/**
 * Since the intention is for both blocks to start moving at the same time
 * this function will do just that, but the difference is in the size of the
 * intervals.
 */
function startMoving(fastBox, slowBox) {
    slowBoxInterval = setInterval(() => move_slow(slowBox), 5);
    fastBoxInterval = setInterval(() => move_fast(fastBox), 20);
}

/**
 * Move box fast: If the block is close to the offsetWidth of the parent container,
 * then switch off forwar_fast ie., change its value to false, which then allows
 * the block to drift in reverse.
 */
function move_fast(box) {
    let x = null;

    try {
        x = parseInt(box.style.left.match(intRegex)[1]);
    } catch (error) {
        x = 0;
    }
    if (x < parent_width && forward_fast == true) {
        x += 1;
        box.style.left = x + "px";
        if (x >= parent_width) {
            forward_fast = false;
        }
    } else {
        x -= 1
        box.style.left = x + "px";
        if (x <= 0) {
            forward_fast = true;
        }
    }
}
/**
 * Move box slow: If the block is close to the offsetWidth of the parent container,
 * then switch off forwar_fast ie., change its value to false, which then allows
 * the block to drift in reverse.
 */
function move_slow(box) {
    let x = null;

    try {
        x = parseInt(box.style.left.match(intRegex)[1]);
    } catch (error) {
        x = 0;
    }
    if (x < parent_width && forward_slow == true) {
        x += 1;
        box.style.left = x + "px";
        if (x >= parent_width) {
            forward_slow = false;
        }
    } else {
        x -= 1;
        box.style.left = x + "px";
        if (x <= 0) {
            forward_slow = true;
        }
    }
}

/**
 * When the stop button is clicked, stop the infinite motion of the blocks
 */
function stopMoving() {
    clearInterval(slowBoxInterval);
    clearInterval(fastBoxInterval);
}