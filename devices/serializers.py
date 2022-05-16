from rest_framework import serializers
from devices.models import Device



class DeviceModelSerializer(serializers.ModelSerializer):
    class Meta:
        model = Device
        fields = "__all__"

