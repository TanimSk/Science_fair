# Generated by Django 3.1.13 on 2021-12-15 18:57

from django.db import migrations, models


class Migration(migrations.Migration):

    initial = True

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Health_report',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('phone_no', models.CharField(max_length=20)),
                ('temperature', models.FloatField()),
                ('heart_rate', models.FloatField()),
                ('spo2', models.FloatField()),
                ('eye_lens', models.FloatField()),
            ],
        ),
    ]
