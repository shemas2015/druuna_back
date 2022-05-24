from pickletools import read_long1
from rest_framework import serializers
from devices.models import Device,UserDevice




class DeviceModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = "__all__"


class DeviceUserModelSerializer(serializers.ModelSerializer):
    #device = DeviceModelSerializer(read_only =True)
    class Meta:
        model = UserDevice
        fields = ["id","device","mac"]
        depth = 1

    def __init__(self, *args, **kwargs):
        super(DeviceUserModelSerializer, self).__init__(*args, **kwargs)
        request = self.context.get('request')
        if request and request.method=='POST':
            self.Meta.depth = 0
        else:
            self.Meta.depth = 1