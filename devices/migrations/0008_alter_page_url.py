# Generated by Django 4.0.4 on 2022-06-21 22:33

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('devices', '0007_page_pageconfig'),
    ]

    operations = [
        migrations.AlterField(
            model_name='page',
            name='url',
            field=models.CharField(max_length=400, unique=True),
        ),
    ]
