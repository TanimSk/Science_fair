import req
import transfer_data
from pygame import mixer
from .render_report import report

mixer.init()
mixer.music.set_volume(1)

phone_number = ""
age = ""

body_temp = ""
heart_rate = ""
spo2 = ""
eye_lens = ""
has_cough = False


def check_stat(temperature, heart_rate_, spo2_, eye_lens_):
    if temperature != "" and float(temperature) > 100.0:
        return False

    if heart_rate_ != "" and float(heart_rate_) < 60.0:
        return False

    if spo2_ != "" and float(spo2_) < 95.0:
        return False

    if eye_lens_ != "" and float(eye_lens_) > 0:
        return False
    


while True:
    data = transfer_data.get_data()

    if data[0] == "data":
        phone_number = data[1]
        age = data[2]
        body_temp = data[3]
        heart_rate = data[4]
        spo2 = data[5]
        has_cough = data[6]
        eye_lens = data[7]

        print(body_temp, phone_number)


    elif data[0] == "audio":
        path = data[1]
        print(f"now playing audio: {data[1]}")
        mixer.music.load(path)
        channel = mixer.music.play()
        while mixer.music.get_busy() == True:
            continue


    elif data[0] == "render":
        report.print_report(
            phone_no=phone_number, age=age, temperature=body_temp,
            heart_rate=heart_rate, spo2=spo2, eye_lens=eye_lens, has_cough=has_cough
        )
        
        req.send_data(
            phone_no=phone_number, age=age,
            temperature=body_temp, heart_rate=heart_rate,
            spo2=spo2, eye_lens=eye_lens,

            stat=check_stat(temperature=body_temp, heart_rate_=heart_rate,
                            spo2_=spo2, eye_lens_=eye_lens)
        )
