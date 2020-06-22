$(document).ready(function(){
  $('.header').height($(window).height());
})

$(function () {
  $('[data-toggle="tooltip"]').tooltip()
})

document.addEventListener('DOMContentLoaded', function() {
    confidentBox = document.querySelector(".confident");
    learningBox = document.querySelector(".learning");
    plannedBox = document.querySelector(".planned");
    confidentArrow = document.querySelector(".confident-arrow");
    learningArrow = document.querySelector(".learning-arrow");
    plannedArrow = document.querySelector(".planned-arrow");
    document.querySelector("#confident").onclick = function() {
        confidentBox.style.display = confidentArrow.style.display = "inline";
        learningBox.style.display = learningArrow.style.display = "none";
        plannedBox.style.display = plannedArrow.style.display = "none";
    };
    document.querySelector("#learning").onclick = function() {
        confidentBox.style.display = confidentArrow.style.display = "none";
        learningBox.style.display = learningArrow.style.display = "inline";
        plannedBox.style.display = plannedArrow.style.display = "none";
    };
    document.querySelector("#planned").onclick = function() {
        confidentBox.style.display = confidentArrow.style.display = "none";
        learningBox.style.display = learningArrow.style.display = "none";
        plannedBox.style.display = plannedArrow.style.display = "inline";
    }
});
document.addEventListener('DOMContentLoaded', function() {
    document.querySelector("#reload").onclick = function() {
        location.reload();
        return false;
    }
});