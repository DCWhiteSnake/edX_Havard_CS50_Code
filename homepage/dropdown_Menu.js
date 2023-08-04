document.addEventListener("DOMContentLoaded",  make_sidebar_responsive);
document.addEventListener("DOMContentLoaded", () => {
    let xMenuButton = document.querySelector("#x_menu_btn");
    xMenuButton.addEventListener("click", () => {
    let xMenu = document.querySelector(".x_menu");
    xMenu.classList.toggle("hidden")});

})
window.addEventListener("resize", make_sidebar_responsive);

function make_sidebar_responsive() {
    let screen_width = window.screen.width;
    // Min width for large screen according to
    // https://learn.microsoft.com/en-us/windows/apps/design/layout/screen-sizes-and-breakpoints-for-responsive-design
    let MIN_LARGE_SCREEN_WIDTH = 1008;
    let xMenuButton = document.querySelector("#x_menu_btn");
    let xMenu = document.querySelector(".x_menu");
    if (screen_width < MIN_LARGE_SCREEN_WIDTH) {
        // then the vertical scrolls should be hidden;
        // the menu button should be displayed
        xMenu.classList.add("hidden");
        xMenuButton.display = "block";
    }
    else {
        xMenu.classList.remove("hidden");
    }


}