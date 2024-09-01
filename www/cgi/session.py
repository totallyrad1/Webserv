import os
import time
from datetime import datetime

def generate_session():
    new_session_value = time.time() * 1000
    current_time = datetime.now()
    formatted_time = current_time.strftime("[%Y-%m-%d %H:%M:%S]")
    try:
        with open(".sessions.txt", 'a') as file:
            file.write(f"{new_session_value}|{formatted_time}\n")
            body = '<head><title>NewSession</title></head><style> body {margin: 0px;padding: 0px;}.all{background-color: black;width: 100%;height: 100vh;display: flex;align-items: center;justify-content: center;overflow: hidden;}.inside p{padding: 0px;margin: 0px;}.inside {gap: 1px;flex-direction: column;font-style: italic;display: flex;color: rgb(255, 0, 200);align-items: center;justify-content: center;border-radius: 10px;width: 500px;border-style: solid;border-color: white;border-width: 2px;box-shadow: 0px 0px 10px white;height: 100px;}</style><body><div class="all"><div class="inside"><p> Hello NewComer</p><p> This is the Generated Session '
            body += f"{new_session_value}</p></div></div></body></html>"
            print("Content-type: text/html\r")
            print(f"Set-Cookie: session={new_session_value}\r")
            print(f"Content-Length: {len(body)}\r")
            print("\r")
            print(body)
    except Exception as e:
        exit(1)

def session_value_found(session_value):
    try:
        with open(".sessions.txt", 'r') as file:
            content = file.read()
            values = content.split('\n')
            for value in values:
                val = value.split("|")
                if(val[0] == session_value):
                    head = "<head><title>HomePage</title></head>"
                    style = "<style>body {color: rgb(255, 0, 191);margin: 0;background-color: black;height: 100vh;display: flex;flex-direction: column;justify-content: center;align-items: center;}div {display: flex;flex-direction: column;justify-content: center;align-items: center;gap: 3rem;}h1 {font-size: 3rem;font-family: 'Courier New', Courier, monospace;letter-spacing: 1rem;text-align: center;padding: 0;margin: 0;margin-bottom: 8rem;}pre {font-family: 'Courier New', Courier, monospace;font-weight: 100;}a, label, button {box-shadow: 0px 0px 9px 1px rgb(255, 0, 191);text-decoration: none;color: rgb(255, 0, 191);font-size: 2rem;font-family: 'Courier New', Courier, monospace;letter-spacing: 0.5rem;text-align: center;border: 1px solid rgb(255, 0, 191);border-radius: 5px;padding: 0.5rem 1rem;margin: 0 1rem;background-color: black;}a:hover, label:hover, button:hover {background-color: rgb(255, 0, 191);color: black;}form {display: flex;flex-direction: column;justify-content: center;align-items: center;margin-top: 2rem;}section {display: flex;flex-direction: column;justify-content: center;align-items: center;margin-right: 1rem;gap: 1rem;margin: 1rem;} </style>"
                    body1 = f'<body><pre>ID : {val[0]}                    MEMBER SINCE : {val[1]}</pre> <h1>WELCOME A JEMMI</h1><div><form id="form" method="post" enctype="multipart/form-data"><input type="file" name="file" id="choose_file" hidden><section><label for="choose_file">CHOOSE FILE</label><span id="file-chosen">No file chosen</span></section><button type="submit" name="submit">UPLOAD</button></form><a id="game">Take Break!</a></div></body></html>'
                    script = "<script>const game = document.getElementById('game');game.href = window.location.href+\"/../../snake/snack.html\";const form = document.getElementById('form');form.action = window.location.href+\"/../..\";const actualBtn = document.getElementById('choose_file');const fileChosen = document.getElementById('file-chosen');actualBtn.addEventListener('change', function(){fileChosen.textContent = this.files[0].name})</script>"
                    body = head + style + body1 + script
                    print("Content-type: text/html\r")
                    print(f"Content-Length: {len(body)}\r")
                    print("\r") 
                    print(body)
                    return
            generate_session()
    except Exception as e:
        generate_session()

env_variable = os.getenv('HTTP_COOKIE', '')

key_value_pairs = {}

pairs = env_variable.split(";")

for pair in pairs:
    if "=" in pair:
        key, value = pair.split("=", 1)
        key = key.strip()
        value = value.strip()
        key_value_pairs[key] = value

session_value = key_value_pairs.get("session", "")

if session_value == "":
    generate_session()
else:
    session_value_found(session_value)