#!/usr/bin/env python3

import time
from datetime import datetime

from mia_py.I1820.app import I1820App
from mia_py.I1820.domain.notif import I1820Notification
from light import Light
from lamp import Lamp

tenant_id = "danial-home"

app = I1820App(tenant_id, "127.0.0.1")

light_LDR = Light("light", "test1:1")

lamps = [
    Lamp(device_id="test1:222"),
    Lamp(device_id="test1:333"),
    Lamp(device_id="test1:444"),
    Lamp(device_id="test1:555")
]


@app.notification("lamp", "alarm", "smartLamp")
def lamp_notification(data):
    print(
        f'Notification data received: {data.__dict__}'
    )
    state = get_state(data)
    if state is None:
        print(f'State is None, Notification Invalid')


    lamp = get_lamp(data)
    if lamp is None:
        print(
            f'Notification device_id {data.device} is invalid!'
        )
        return False

    lamp.state = state
    print(
        f'we have a notification for {data.device} of {data.type}, {data.settings}'
    )
    return True

def get_lamp(data):
    device_id = data.device

    for lamp in lamps:
        if lamp.device_id == device_id:
            return lamp

    return None
    
def get_state(data):
    try:
        settings = data.settings
    except Exception as e:
        print(f"Notification data doesn't have any attribute settings. {e}")

    try:
        for setting in settings:
            if setting['name'].lower() == 'on':
                return "on" if setting['value'] else "off"
            if setting['name'].lower() == 'off':
                return "off" if setting['value'] else "on"
                
    except Exception as e:
        print(
            f'Exception: {e}'
        )
    
    return None


if __name__ == "__main__":

    app.add_thing(light_LDR.name, light_LDR.device_id)
    for lamp in lamps:
        app.add_thing(lamp.name, lamp.device_id)

    app.run()

    while True:
        app.log("light", "test1:1", [light_LDR.to_dict()])
        for lamp in lamps:
            app.log("lamp", lamp.device_id, [lamp.to_dict()])

        print(f'sending information to mia {datetime.now()}')
        time.sleep(10)
