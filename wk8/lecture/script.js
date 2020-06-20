function greet()
{
    let name = document.querySelector('#name_input').value;
    if (name === "")
    {
        name = "world";
    }
    document.querySelector("#result").innerHTML = ("Hello, " + name + "!");
}
let counter = 0;
function increment()
{
    counter++;
    document.querySelector("#counter_result").innerHTML = counter;
}
function decrement()
{
    counter--;
    document.querySelector("#counter_result").innerHTML = counter;
}
function blink()
{
    let blinker = document.querySelector("#blinker");
    if (blinker.style.visibility === "hidden")
    {
        blinker.style.visibility = "visible";
    }
    else
    {
        blinker.style.visibility = "hidden"
    }
}
// Blink every 500ms
window.setInterval(blink, 500);
navigator.geolocation.getCurrentPosition(function(position) {
   let lat = position.coords.latitude;
   let long = position.coords.longitude;
   document.querySelector("#coords").innerHTML = lat + ", " + long;
});