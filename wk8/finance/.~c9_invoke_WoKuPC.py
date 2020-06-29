import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import time

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

#Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    cash = db.execute("SELECT cash FROM users WHERE id=(:userid)", userid=session["user_id"])[0]["cash"]
    stocks = db.execute("SELECT symbol, shares FROM stocks WHERE userid=(:userid)", userid=session["user_id"])
    total_assets = 0
    for stock in stocks:
        api_stock = lookup(stock["symbol"])
        stock["price"] = api_stock["price"]
        stock["name"] = api_stock["name"]
        total_assets += stock["shares"] * stock["price"]
    return render_template("index.html", stocks=stocks, cash=cash, total_assets=total_assets)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html", render_card=False)
    elif request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        amount = request.form.get("amount")
        if not stock:
            return render_template("buy.html", render_card=False, show_error="No stock found with that symbol")
        try:
            amount = int(amount)
        except:
            return render_template("quote.html", render_card=False, show_error="Please enter an integer stock amount to purchase")
        else:
            if amount < 1:
                return render_template("buy.html", render_card=False, show_error="Please specify a positive integer")
            purchase_price = float(stock["price"]) * amount
            cash_available = db.execute("SELECT cash FROM users WHERE id=(:userid)", userid=session["user_id"])[0]["cash"]
            if purchase_price <= cash_available:
                new_cash = cash_available - purchase_price
                db.execute("INSERT INTO history (userid, symbol, amount, purchaseprice, datetime) VALUES (:userid, :symbol, :amount, :purchaseprice, :time)", userid=session["user_id"], symbol=stock["symbol"], amount=amount, purchaseprice=stock["price"], time=time.strftime('%Y-%m-%d %H:%M:%S'))
                db.execute("UPDATE users SET cash=(:new_cash) WHERE id=(:userid)", new_cash=new_cash, userid=session["user_id"])
                users_stocks = db.execute("SELECT * FROM stocks WHERE userid=(:userid)", userid=session["user_id"])
                if not any(d["symbol"] == stock["symbol"] for d in users_stocks):
                    db.execute("INSERT INTO stocks (userid, symbol, shares) VALUES (:userid, :symbol, :amount)", userid=session["user_id"], symbol=stock["symbol"], amount=amount)
                else:
                    for i in users_stocks:
                        if i["symbol"] == stock["symbol"]:
                            new_shares = i["shares"] + amount
                    db.execute("UPDATE stocks SET shares=(:new_shares) WHERE userid=(:userid) AND symbol=(:symbol)", userid=session["user_id"], new_shares=new_shares, symbol=stock["symbol"])
                return render_template("buy.html", render_card=True, purchase_total=purchase_price, amount=amount, stock_name=stock["name"], stock_symbol=stock["symbol"], stock_price=stock["price"], account_total=new_cash)
            else:
                return render_template("buy.html", render_card=False, show_error="Insufficient funds for purchase")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    return jsonify("TODO")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT symbol, amount, purchaseprice, datetime FROM history WHERE userid=(:userid) ORDER BY transid DESC", userid=session["user_id"])
    for row in history:
        row["transaction"] = "BUY" if row["amount"]>0 else "SELL"
    return render_template("history.html", table=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return render_template("login.html", show_error="Please provide a username")

        # Ensure password was submitted
        elif not request.form.get("password"):
            return render_template("login.html", show_error="Please provide a password")

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return render_template("login.html", show_error="Invalid username or password")

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html", render_card=False)
    elif request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return render_template("quote.html", render_card=False, show_error="No stock found with that symbol")
        else:
            return render_template("quote.html", render_card=True, stock_name=stock["name"], stock_symbol=stock["symbol"], stock_price=stock["price"])


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    elif request.method == "POST":
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")
        if password != confirmation:
            return render_template("register.html", show_error="Passwords must match")
        if not username or not password:
             return render_template("register.html", show_error="Enter a username and a password")
        db.execute("INSERT INTO users (username, hash, cash) VALUES (:username, :passhash, 10000)", username=username, passhash=generate_password_hash(password))
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "GET":
        stocks = db.execute("SELECT symbol, shares FROM stocks WHERE userid=(:userid)", userid=session["user_id"])
        print(stocks)
        return render_template("sell.html", stocks=stocks)
    elif request.method == "POST":
        stock = lookup(request.form.get("symbol"))
        amount = request.form.get("amount")
        if not stock:
            return render_template("sell.html", render_card=False, show_error="No stock found with that symbol")
        try:
            amount = int(amount)
        except:
            return render_template("sell.html", render_card=False, show_error="Please enter an integer stock amount to purchase")
        else:
            if amount < 1:
                return render_template("sell.html", render_card=False, show_error="Please specify a positive integer")
            try:
                stocks_available = db.execute("SELECT shares FROM stocks WHERE userid=(:userid) AND symbol=(:symbol)", userid=session["user_id"], symbol=stock["symbol"])[0]["shares"]
            except:
                return render_template("sell.html", render_card=False, show_error="Insufficient " + stock["symbol"] + " stocks")
            else:
                if stocks_available >= amount:
                    new_stocks = stocks_available - amount
                    cash_available = db.execute("SELECT cash FROM users WHERE id=(:userid)", userid=session["user_id"])[0]["cash"]
                    sale_total = float(stock["price"]) * amount
                    new_cash = cash_available + sale_total
                    db.execute("UPDATE users SET cash=(:new_cash) WHERE id=(:userid)", new_cash=new_cash, userid=session["user_id"])
                    db.execute("UPDATE stocks SET shares=(:new_stocks) WHERE userid=(:userid) AND symbol=(:symbol)", new_stocks=new_stocks, userid=session["user_id"], symbol=stock["symbol"])
                    db.execute("INSERT INTO history (userid, symbol, amount, purchaseprice, datetime) VALUES (:userid, :symbol, :amount, :purchaseprice, :time)", userid=session["user_id"], symbol=stock["symbol"], amount=(-1*amount), purchaseprice=stock["price"], time=time.strftime('%Y-%m-%d %H:%M:%S'))
                    return render_template("sell.html", render_card=True, purchase_total=sale_total, amount=amount, stock_name=stock["name"], stock_symbol=stock["symbol"], stock_price=stock["price"], account_total=new_cash)
                else:
                    return render_template("sell.html", render_card=False, show_error="Insufficient " + stock["symbol"] + " stocks")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
