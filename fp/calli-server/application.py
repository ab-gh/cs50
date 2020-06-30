import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import time
import datetime as dt
import json

app = Flask(__name__)

db = SQL("sqlite:///calli.db")

@app.route("/")
def root():
    return jsonify("hello world")

@app.route("/api/today", methods=["GET"])
def today():
    if request.method == "GET":
        today_calendar = db.execute("SELECT * FROM (:userid) WHERE date=(:today)", userid='1', today=dt.date.today().strftime("%Y-%m-%d"))
        return jsonify(today_calendar)

@app.route("/api/new/event", methods=["POST"])
def new_event():
    if request.method == "POST":
        content = request.json
        db.execute("INSERT INTO '1' (type, name, date, start, end, location) VALUES (:etype, :name, :date, :start, :end, :location)", etype="E", name=content['name'], date=content['date'], start=content['start'], end=content['end'], location=content['location'])
        return "success"
