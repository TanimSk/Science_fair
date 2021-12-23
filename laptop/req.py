import requests

def send_data(phone_no, age, temperature, heart_rate, spo2, eye_lens, stat) -> None:
        
        if float(temperature) == 0.0:
                temperature = "Not Tested"
        elif float(temperature) > 100.0:
                temperature += " *"
        
        if float(spo2) == 0.0:
                spo2 = "Not Tested"
        elif float(spo2) < 95.0:
                spo2 += " *"
        
        if float(heart_rate) == 0.0:
                heart_rate += "Not Tested"
        elif not (60.0<float(heart_rate)<100.0):
                heart_rate += " *"


        payload = {
                'phone_no': phone_no,
                'age': age,
                'temperature': temperature,
                'heart_rate': heart_rate,
                'spo2': spo2,
                'eye_lens': eye_lens,
                'stat': stat
                }
                
        print("sending post request")
        requests.post('http://medirobo.pythonanywhere.com/add_report', data=payload)
        print("Done")


if __name__ == "__main__":
        phone_no = input("Enter phone no: ")
        temp = input("temperature: ")
        heart_rate = input("heart rate: ")
        spo2 = input("SpO2: ")
        eye_lens = input("Eye: ")

        send_data(
                phone_no=phone_no,
                age="18",
                temperature=temp,
                heart_rate=heart_rate,
                spo2=spo2,
                eye_lens=eye_lens,
                stat=True
                )