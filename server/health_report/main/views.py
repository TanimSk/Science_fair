from django.shortcuts import render
from .models import Health_report
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
import json

def mainPage(req):
    content = Health_report.objects.all()
    return render(req, "main/mainPage/index.html", {'content': content})

@csrf_exempt
def add_report(req):
    if req.method == 'POST':
        phone_no = req.POST.get('phone_no')
        temperature = req.POST.get('temperature')
        heart_rate = req.POST.get('heart_rate')
        spo2 = req.POST.get('spo2')
        eye_lens = req.POST.get('eye_lens')

        entry =  Health_report(phone_no=phone_no, temperature=temperature, heart_rate=heart_rate, spo2=spo2, eye_lens=eye_lens)
        entry.save()
        
        return HttpResponse(json.dumps({'done': True}), content_type='application/json')
    else:
        return HttpResponse("400")


