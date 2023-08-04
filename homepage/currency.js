
document.addEventListener("DOMContentLoaded", () => {
    var countries = [];
    let count = 0;
    let idx = 0;
    let size = 10;
    fetch("https://restcountries.com/v2/all")
        .then(response => response.json())
        .then(data => {
            data.forEach(row => {
                let country_name = row.name;
                let currency = "";
                try {
                    currency = row.currencies.pop()["name"];
                }
                catch (error) {
                    currency = "N/A";
                }
                let new_entry = [country_name, currency];
                countries.push(new_entry);
            });
            let nextbutton = document.querySelector('#next_btn');
            let prevbutton = document.querySelector('#prev_btn');
            nextbutton.addEventListener('click', () => { idx += 10; populate(idx, size);});
            prevbutton.addEventListener('click', () =>{ idx-=10;populate(idx, size);});
            populate(idx, size);
        })

        .catch(error => {
            console.error("Error consuming payload");
        });
    // populate table with elements.
    function populate(start, size)
    {
        if (start < 0)
        {
            start = 0;
            idx = 0;
        }
        let t_body = document.querySelector("#country_t_body");
        for (let i = start; i < (size + start); i++)
        {
            if (countries[i] == null) {
                break;
                // Todo, delete all other table items.
            }
            let x_id = i%size;
            let row = document.querySelector("#row_id_" + x_id);
            if (row == null && i < size) {
                let x_row = document.createElement('tr');
                let id = document.createElement('th');
                id.scope = "row";
                id.textContent = i;
                let country = document.createElement('td');
                country.textContent = countries[i][0];
                let currency = document.createElement('td');
                currency.textContent = countries[i][1];

                x_row.appendChild(id);
                x_row.appendChild(country);
                x_row.appendChild(currency);
                x_row.id = "row_id_" + i;
                t_body.appendChild(x_row);
            }
            else if (row != null || i >= size) {

                let id = i % 10;
                let x_row = document.querySelector("#row_id_" + id);
                let row_kids = x_row.children;
                let country = row_kids[1]
                country.textContent = countries[i][0];
                let currency = row_kids[2]
                currency.textContent = countries[i][1];
            }
        }
    }
});

