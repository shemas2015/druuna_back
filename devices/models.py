from distutils.command.upload import upload
from email.mime import image
from django.db import models
from django.conf import settings

# Create your models here.


class Device( models.Model):
    name  = models.CharField(max_length=200,unique=True)
    image = models.ImageField(upload_to='devices/devices_img')

class UserDevice( models.Model ):
    mac   = models.CharField( max_length = 200 , unique=True )
    owner = models.ForeignKey(settings.AUTH_USER_MODEL,   on_delete = models.PROTECT  , default=None , null=True , related_name='owner')
    device = models.ForeignKey(Device,  on_delete=models.PROTECT, related_name="device" )


class Page( models.Model ):
    name = models.CharField(max_length=200,unique=True)
    url = models.CharField(max_length=400,unique=True)

class PageConfig( models.Model ):
    user_device = models.ForeignKey(UserDevice, on_delete=models.PROTECT , related_name="user_device" )
    page = models.ForeignKey(Page, on_delete=models.PROTECT )
    tokens = models.IntegerField()
    seconds = models.IntegerField()



