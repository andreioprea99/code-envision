import influxdb_client
from influxdb_client.client.write_api import SYNCHRONOUS
import os
from datetime import datetime
from flask import Flask, request, jsonify

app = Flask(__name__)

client = None
bucket = os.environ.get("INFLUXDB_BUCKET", default="mybucket")
auth_bucket = os.environ.get("INFLUXDB_AUTH_BUCKET", default="myauthbucket")
org = os.environ.get("INFLUXDB_ORG", default="myorg")
token = os.environ.get("INFLUXDB_TOKEN", default="mytoken")
url = os.environ.get("INFLUXDB_URL", default="http://influxdb2:8086")

registered_devices = set()

@app.route("/register/<device_id>", methods=["POST"])
def create_device(device_id=None):
    if device_id is None:
        return  "bad request", 400

    registered_devices.add(device_id)
    return "ok", 200



@app.route("/data/<device_id>", methods=["POST"])
def post_temperature(device_id):
    if device_id not in registered_devices:
        return "unauthorized",403
    
    request_data = request.get_json()

    point = influxdb_client.Point("environment") \
        .tag("device", device_id) \
        .field("temperature", float(request_data["temperature"])) \
        .field("humidity", float(request_data["humidity"])) \
        .field("ppm", float(request_data["ppm"])) \
        .field("ppm_1", float(request_data["ppm_1"])) \
        .field("ppm_25", float(request_data["ppm_25"])) \
        .field("lat", float(request_data['latitude'])) \
        .field("lon", float(request_data['longitude'])) \
        .time(datetime.now())
    # print(f"Writing: {point.to_line_protocol()}")
    client_response = write_api.write(bucket=bucket, record=point)
    # write() returns None on success
    if client_response is None:
        # TODO Maybe also return the data that was written
        return "ok", 201
    # Return None on failure
    return None


@app.route("/", methods=["GET"])
def get_latest_data():
    return "<p>Hello, World!</p>"


def init_influxdb_client():
    global client, write_api
    client = influxdb_client.InfluxDBClient(
        url=url,
        token=token,
        org=org
    )
    write_api = client.write_api(write_options=SYNCHRONOUS)

if __name__ == "__main__":
    port = int(os.environ.get('PORT', 8443))
    init_influxdb_client()
    app.run(debug=True, host='0.0.0.0', port=port)
