document.addEventListener("DOMContentLoaded", () => {
    // Do magic stuff here.
    let inbox = document.querySelector("#input_box");
    let outbox = document.querySelector("#output_box");
    let num_btns = document.querySelectorAll(".n_btn");
    let op_btns = document.querySelectorAll(".op_btn");
    let eq_btn = document.querySelector("#eq_btn");
    let cancel_btn = document.querySelector("#cancel_btn");
    let delete_btn = document.querySelector("#delete_btn");
    let dot_btn = document.querySelector("#dot_btn");
    let n_1 = 0;
    let n_2 = 0;
    let op = "";
    let pattern = /\d+(\.\d+)?[-+*/%](\d+(\.\d+)?)/;


    num_btns.forEach((btn) => {
        btn.addEventListener("click", function(){
            inbox.value += btn.textContent;
            console.log(inbox.value);
        });
    });

    op_btns.forEach((btn) => {
        btn.addEventListener("click", function(){
            n_1 = parseFloat(inbox.value);
            inbox.value += btn.textContent;
            op = btn.textContent;
        });
    });

    eq_btn.addEventListener("click", function(){
        try{
            n_2 = parseFloat(inbox.value.match(pattern)[2]);
        }
        catch{
            n_2 = 0;
        }

        switch (op) {
            case '-':
                outbox.value = minus(n_1, n_2);
                break;
            case '/':
                outbox.value = divide(n_1, n_2);
                break;
            case '%':
                outbox.value = modulo(n_1, n_2);
                break;
            case 'X':
                outbox.value = multiply(n_1, n_2);
                break;
            case '+':
            default:
                n_1 = parseFloat(inbox.value);
                outbox.value = add(n_1, n_2);
                console.log(`Invalid operation`);
        }
    });

    dot_btn.addEventListener("click", () => {
        inbox.value += ".";
    });

    delete_btn.addEventListener("click", () => {
        inbox.value = inbox.value.slice(0,-1);
    });
    cancel_btn.addEventListener("click", () => {
        inbox.value = "";
        outbox.value = "";
        n_1 = null;
        n_2 = null;
    });

});

function multiply(x, y) {
    return x * y;
}

function divide(x, y) {
    return x / y;
}

function modulo(x, y) {
    return x % y;
}

function add(x, y) {
    return x + y;
}

function minus(x, y) {
    return x - y;
}