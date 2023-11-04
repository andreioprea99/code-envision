from influxdb_client import Authorization, InfluxDBClient, Permission, PermissionResource, Point, WriteOptions
import os
import json
from flask import Flask

app = Flask(__name__)

@app.route("/")
def hello_world():
    return "<p>Hello, World!</p>"

if __name__ == "__main__":
    port = int(os.environ.get('PORT', 443))
    app.run(debug=True, host='0.0.0.0', port=port)




