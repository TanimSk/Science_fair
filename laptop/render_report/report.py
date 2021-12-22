
from datetime import date
from jinja2 import Template
import os


def get_precription(temperature, has_cough):
    return "Paracitamol - napa extra"


def print_report(phone_no, age, temperature, heart_rate, spo2, eye_lens, has_cough):
    
    if temperature == "":
        temperature = "Not Tested"
    elif float(temperature) > 100.0:
        temperature += " *"
    
    if spo2 == "":
        spo2 = "Not Tested"
    elif float(spo2) < 95.0:
        spo2 += " *"
    
    if heart_rate == "":
        heart_rate += " *"
    elif not (60.0<float(heart_rate)<100.0):
        heart_rate += " *"


    data = {
        "AGE" : age,
        "PHONE_NUMBER" : phone_no,
        "BODY_TEMP" : temperature,
        "O2_LEVEL" : spo2,
        "HEART_RATE" : heart_rate,
        "EYE_LENS": eye_lens,
        "PRESCRIPTION" : get_precription(temperature, has_cough),
        "now_date" : str(date.today())
    }

    f = open("./base.html", 'r')
    html = f.read()
    f.close()

    html = Template(html).render(data)

    print("writing...")
    
    with open("rendered.html", "w") as f:
        f.write(html)

    print("ok, printing")
    os.system("htmlprint.exe rendered.html")
    print("Done")


# print_report(
#     temperature="",
#     phone_no="65165156",
#     age="51",
#     heart_rate="65",
#     eye_lens="0.4",
#     spo2="94",
#     has_cough=False
# )