# Generated by Django 4.0.4 on 2022-05-28 22:38

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('devices', '0005_userdevice_device'),
    ]

    operations = [
        migrations.AlterField(
            model_name='device',
            name='image',
            field=models.ImageField(upload_to='devices/devices_img'),
        ),
    ]