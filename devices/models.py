from django.db import models

# Create your models here.


class Device( models.Model ):
    mac = models.CharField( max_length = 200 )
