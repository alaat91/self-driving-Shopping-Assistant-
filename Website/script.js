
/* global webkitSpeechRecognition */

// Allows our website to use speech to text. @Carl
function runSpeechRecognition() {
    // get output div reference
    var output = document.getElementById("output");
    // get action element reference
   var action = document.getElementById("action");
    // new speech recognition object
    var SpeechRecognition = SpeechRecognition || webkitSpeechRecognition;
    var recognition = new SpeechRecognition();

    // This runs when the speech recognition service starts
    recognition.onstart = function () {
        action.innerHTML = "<small>listening...</small>";
    };

    recognition.onspeechend = function () {
        //action.innerHTML = "<small>Voice result</small>";
        recognition.stop();
    }

    // This runs when the speech recognition service returns result
    recognition.onresult = function (event) {
        var transcript = event.results[0][0].transcript;
        output.innerHTML = "<b>You said:</b> " + transcript;
        output.classList.remove("hide");

        document.getElementById('searchbar').value = transcript;

        searchItem()

    };

    // start recognition
    recognition.start();
}

//---------------
//DummyData for items. @Carl
let itemJson = `[
  {"Item": "Tomato", "Quantity": "30", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Ice Cream", "Quantity": "40", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Salmon", "Quantity": "40", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Apple", "Quantity": "100", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Frozen Pizza", "Quantity": "14", "Price": "29 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Coca-Cola", "Quantity": "14", "Price": "19 SEK", "Aisle": "2", "Section": "B"},
  {"Item": "Coca-Cola", "Quantity": "14", "Price": "12 SEK", "Aisle": "2", "Section": "B"}
]`

let itemData = JSON.parse(itemJson)

//This function is used in both voice recognition and manual search
function searchItem() {
    let input = document.getElementById('searchbar').value
    input = input.toLowerCase();
    let x = document.querySelector('#result');
    x.innerHTML = ""

    var msg = new SpeechSynthesisUtterance();
    msg.lang = 'en-US';
    msg.rate = 0.75;

    for (let i = 0; i < itemData.length; i++) {
        let obj = itemData[i];

        if (obj.Item.toLowerCase().includes(input)) {
            const elem = document.createElement("li")
            elem.innerHTML = `${obj.Item} - Quantity: ${obj.Quantity} - Price: ${obj.Price} | Location:  Aisle: ${obj.Aisle} - Section: ${obj.Section} `
            x.appendChild(elem)
        }

        msg.text = `you can find the ${obj.Item} in Aisle ${obj.Aisle} Section ${obj.Section} at this moment we have ${obj.Quantity} ${obj.Item} in stock and the price is ${obj.Price}`
        window.speechSynthesis.speak(msg);

    }
}

function getInfo(){
    var username = document.getElementById("username").value;
    var password = document.getElementById("password").value;

    if((username === "admin") && password==="123"){
        window.location = "MovementControlWeb.html";
    }else {
        document.getElementById("info").innerHTML = "Your login credentials were incorrect! ";
    }

}