from django.db import models

class Health_report(models.Model):
    phone_no = models.CharField(max_length=20)
    temperature = models.FloatField()
    heart_rate = models.FloatField()
    spo2 = models.FloatField()
    eye_lens = models.FloatField()

    def __str__(self):
        return self.phone_no