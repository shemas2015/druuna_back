# Generated by Django 4.0.4 on 2022-06-21 23:06

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('devices', '0008_alter_page_url'),
    ]

    operations = [
        migrations.AlterUniqueTogether(
            name='pageconfig',
            unique_together={('user_device', 'page')},
        ),
    ]