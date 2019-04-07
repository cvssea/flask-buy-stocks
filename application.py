import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd, update_portfolio, is_positive_int

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # get user's portfolio
    portfolios = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id", user_id=session["user_id"])

    # compute table info
    # available funds
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=session["user_id"])[0]["cash"]
    assets = 0
    for portfolio in portfolios:
        assets += lookup(portfolio["symbol"])["price"] * portfolio["shares"]
    total = usd(float(cash) + float(assets))

    # render
    return render_template("index.html", portfolios=portfolios, lookup=lookup, usd=usd, cash=usd(cash), total=total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    # Render template
    if request.method == "GET":
        return render_template("buy.html")

    # Validation
    symbol = request.form.get("symbol")
    if not symbol or not lookup(symbol):
        return apology("DUMBASS!", 400)

    # current user
    user_id = session['user_id']

    stock = lookup(symbol)
    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)[0]['cash']
    shares = request.form.get("shares")

    # check for positive integer in shares input
    if not is_positive_int(shares):
        return apology("only positive integers, fucker!", 400)
    shares = int(shares)

    # funds needed for transaction
    required_cash = float(stock['price']) * shares

    # check if funds available
    if required_cash > cash:
        return apology("poor fuck", 400)

    # add transaction to history
    db.execute("INSERT INTO transactions(user_id, symbol, name, shares, price) VALUES(:user_id, :symbol, :name, :shares, :price)",
               user_id=user_id, symbol=stock['symbol'], name=stock["name"], shares=shares, price=required_cash)

    # add transaction to portfolio
    portfolio = db.execute("SELECT shares FROM portfolios WHERE user_id = :user_id AND symbol = :symbol",
                           user_id=user_id, symbol=stock["symbol"])

    # update portfolio shares if it exists
    if portfolio:
        portfolio = update_portfolio(portfolio, int(shares))
        db.execute("UPDATE portfolios SET shares = :portfolio WHERE user_id = :user_id AND symbol = :symbol",
                   portfolio=portfolio, user_id=user_id, symbol=stock["symbol"])
    else:
        db.execute("INSERT INTO portfolios (user_id, symbol, name, shares) VALUES(:user_id, :symbol, :name, :shares)",
                   user_id=user_id, symbol=stock["symbol"], name=stock["name"], shares=shares)

    # update available funds
    db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash-required_cash, user_id=user_id)
    flash("Bought!")
    return redirect("/")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSON format"""
    username = request.args.get("username")
    exists = db.execute("SELECT id FROM users WHERE username = :username", username=username)
    if exists:
        return jsonify(False)
    return jsonify(True)


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    user_id = session["user_id"]
    history = db.execute("SELECT * FROM transactions WHERE user_id = :user_id ORDER BY date", user_id=user_id)
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

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
    if request.method == "POST":
        symbol = request.form.get("symbol")
        if not lookup(symbol):
            return apology("fuck off", 400)
        return render_template("quoted.html", data=lookup(symbol), usd=usd)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "GET":
        return render_template("register.html")
    else:
        # INPUT CHEKS

        # input username?
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # is username already taken?
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        if len(rows) == 1:
            return apology("username taken", 400)

        # input password?
        if not request.form.get("password"):
            return apology("please provide password", 400)

        # password not retyped or not matching?
        if not request.form.get("password") == request.form.get("confirmation"):
            return apology("passwords do not match", 400)

        # add new user to db
        username = request.form.get("username")
        pw_hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8)
        db.execute("INSERT INTO users (id, username, hash) VALUES(NULL, :username, :pw_hash)", username=username, pw_hash=pw_hash)

        # auto login the user after registration success
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=username)
        if len(rows) != 1:
            return apology("something went wrong", 500)
        session["user_id"] = rows[0]["id"]

        # redirect to home page
        flash("Registered!")
        return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    user_id = session["user_id"]

    # render form
    if request.method == "GET":
        portfolios = db.execute("SELECT * FROM portfolios WHERE user_id = :user_id", user_id=user_id)
        return render_template("sell.html", stocks=portfolios)

    # get form input
    symbol = request.form.get("symbol")
    if request.form.get("shares"):
        selling = int(request.form.get("shares"))
    else:
        return apology("enter no of shares motherfucker", 400)
    stock = lookup(symbol)

    # validate form input
    if not symbol:
        return apology("enter symbol", 400)

    cash = db.execute("SELECT cash FROM users WHERE id = :user_id", user_id=user_id)[0]["cash"]
    shares = db.execute("SELECT shares FROM portfolios WHERE symbol = :symbol AND user_id = :user_id",
                        symbol=symbol, user_id=user_id)[0]["shares"]

    # check if user wants too sell unowned no. of shares
    if selling > shares:
        return apology("cheater", 400)

    # update portfolios
    profit = stock["price"] * selling
    if shares == selling:
        db.execute("DELETE FROM portfolios WHERE symbol = :symbol AND user_id = :user_id", symbol=symbol, user_id=user_id)
    else:
        db.execute("UPDATE portfolios SET shares = :shares WHERE symbol = :symbol AND user_id = :user_id",
                   shares=shares-selling, symbol=symbol, user_id=user_id)

    # add history and update available funds
    db.execute("UPDATE users SET cash = :cash WHERE id = :user_id", cash=cash+profit, user_id=user_id)
    db.execute("INSERT INTO transactions(user_id, symbol, name, shares, price) VALUES(:user_id, :symbol, :name, :shares, :price)",
               user_id=user_id, symbol=stock['symbol'], name=stock["name"], shares=-selling, price=stock["price"]*selling)
    flash("Sold!")
    return redirect("/")


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)