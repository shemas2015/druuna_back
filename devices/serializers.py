from pickletools import read_long1
from rest_framework import serializers
from devices.models import Device, Page, PageConfig,UserDevice




class DeviceModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = "__all__"
        

class PageModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Page
        fields = "__all__"
        
   
class ConfigModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = PageConfig
        fields = "__all__"

    def __init__(self, *args, **kwargs):
        super(ConfigModelSerializer, self).__init__(*args, **kwargs)
        request = self.context.get('request')

        if request and (request.method == 'POST' or request.method == 'PUT'):
            self.Meta.depth = 0
        else:
            self.Meta.depth = 1


        
   

class DeviceUserModelSerializer(serializers.ModelSerializer):
    
    class Meta:
        model = UserDevice
        fields = ("id","device","mac")

    
        depth = 1

    def __init__(self, *args, **kwargs):
        super(DeviceUserModelSerializer, self).__init__(*args, **kwargs)
        request = self.context.get('request')
        if request and request.method=='POST':
            self.Meta.depth = 0
        else:
            self.Meta.depth = 1
    