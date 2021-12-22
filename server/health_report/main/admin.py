from django.contrib import admin
from .models import Health_report

class HealthReportAdmin(admin.ModelAdmin):
    list_display = ("phone_no", "temperature", "heart_rate", "spo2", "eye_lens")

admin.site.register(Health_report, HealthReportAdmin)
