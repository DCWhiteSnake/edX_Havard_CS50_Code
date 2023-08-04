
// TODO: Add code to check answers to questions
document.addEventListener("DOMContentLoaded", function () {

    const choices = ["Arsenal", "Watford", "Leicester", "Wolves", "Bournmouth", "Burnley"];
    const answers = ["Arsenal", "Watford", "Leicester", "Wolves", "Bournmouth"];
    let decision_txt = document.querySelector("#decision_txt");
    for (let i = 0; i < choices.length; i++) {
        let opt = document.createElement("button");
        opt.textContent = choices[i];
        opt.setAttribute("class", "btn btn-secondary mx-2");
        opt.id = "choice_" + i;
        opt.addEventListener("click", function (event) {
            for (let j = 0; j < answers.length; j++) {
                let tc = opt.textContent;
                let ans = answers[i];
                if (tc === ans) {
                    let question_bucket = document.querySelector("#question_bucket");
                    opt.classList.remove("btn-secondary");
                    opt.classList.add("btn-success");
                    break;
                }
            }
            if (opt.classList.contains("btn-success")) {
                decision_txt.textContent = "Correct";
            }
            else {
                opt.classList.add("btn-danger");
                decision_txt.textContent = "Wrong";
            }
        })
        let multichoice_holder = document.querySelector("#options_bucket");
        multichoice_holder.appendChild(opt);
    }

    document.querySelector('form').addEventListener('submit', function (e) {
        let super_secret_pass = "Secret123$";
        let p_input = document.querySelector('#password');
        let usr_input = p_input.value;
        let decision_txt_2 = document.querySelector("#decision_txt_2");

        // Change color
        if (usr_input === super_secret_pass) {
            decision_txt_2.textContent = "Correct!";
            p_input.classList.remove("text-red");
            p_input.classList.add("text-green");

        }
        else {
            decision_txt_2.textContent = "Wrong";
            p_input.classList.remove("text-green");
            p_input.classList.add("text-red");
        }
        e.preventDefault();
    });
});