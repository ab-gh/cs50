from cs50 import SQL
from flask import Flask, redirect, render_template, request

app = Flask(__name__)

db = SQL("sqlite:///lecture.db")

@app.route("/")
def index():
    rows = db.execute("SELECT * FROM registrants")
    return render_template("index.html", rows=rows)

@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        name = request.form.get("name")
        email = request.form.get("email")
        if not name:
            return render_template("error.html", message="You must provide a name")
        if not email:
            return render_template("error.html", message="You must provide an email address")
        db.execute("INSERT INTO registrants (name, email) VALUES (:name, :email)", name=name, email=email)
        return redirect("/")