# Generated by Django 4.0.4 on 2022-05-24 03:44

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('devices', '0004_rename_mac_device_name_remove_device_owner_and_more'),
    ]

    operations = [
        migrations.AddField(
            model_name='userdevice',
            name='device',
            field=models.ForeignKey(default=None, on_delete=django.db.models.deletion.PROTECT, related_name='device', to='devices.device'),
            preserve_default=False,
        ),
    ]
